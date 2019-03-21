# RISC-V Core-Local Interrupt Controller (CLIC)

基于文档：SiFive Proposal for a RISC-V Core-Local Interrupt Controller (CLIC), https://github.com/sifive/clic-spec/blob/master/clic.adoc#background-and-motivation 的简要翻译。

CLIC，Core-Local Interrupt Controller，即内核本地的中断控制器。

- 机器模式：M-mode
- 特权模式：S-mode
- 用户模式：U-mode
- WARL：Write Any Read Legal

# 1. 背景以及动机

CLIC设计的目的是用来给RISC-V系统提供低延迟、向量式、抢占式的中断。当激活CLIC之后，CLIC会替代现存的RISC-V本地中断控制模式（CLINT）。CLIC的设计含有一个基本的设计模式，具有最小硬件的需求，但是同样支持额外的扩展来提供硬件加速的特性。CLIC设计的目标是为多种不同的软件ABI以及中断模式提供支持，而不需要复杂的硬件结构给高性能的处理器实现带来冲击。

## 1.1 现有的RISC-V中断

现有的RISC-V中断系统已经支持了抢占，但是只有在特权级以及更高的特权等级下才可用。在任何一个时刻，一个RISC-V的hart（hardware thread）都运行在一个特定的特权等级下。全局中断使能位：MIE/SIE/UIE，分别被包含在mstatus/sstatus/ustatus寄存器当中，用于控制中断是否能在当前或者更高的的特权模式下触发。任何更高特权级别的中断都会停止当前特权级别的程序执行，而进入更高的特权模式下的中断处理程序。每个特权级别都有它自己的中断状态寄存器（机器模式下的mepc/mcause，特权模式下的sepc/scause，用户模式下的uepc/ucause）来支持抢占，或者直接使用机器模式下的mepc。被更高特权等级的中断抢占还会将当前的特权模式以及中断使能状态压入在mstatus的mpp以及mpie的栈当中。

mtvec寄存器用来指示中断的模式以及中断向量表的基地址。mtvec的WARL最低2位用于指示当前处于哪种中断模式。在CLINT的模型当中，该两位用于表示中断是向量还是非向量（01 or 00），而高位的则是4字节地址对齐的基地址。

## 1.2 CLIC与PLIC

标准的RISC平台级别中断控制器（Platform-level interrupt controller，PLIC）提供一种集中统一的中断优先级以及不同平台中断共同仲裁的功能。并且最后只会将仲裁后得到的单一的外部中断信号发送给每个hart的每个特权级别的指定位（meip/seip/ueip）。

## 1.3 CLIC与CLINT

CLINT是一个相对来说比较小的逻辑单元，提供在早期的设计当中提供本地的中断控制，并且管理软件、计时器以及外部的中断信号（在mip寄存器中的msip/mtip/meip信号）。CLINT同时允许额外自定义的高速中断信号，通过在mip的搞16位中进行添加。

新mtvec模式的设置将在下文进行描述，是CLIC用于代替原有CLINT的方案。平台的配置可能同时需要支持原来的CLINT以及CLIC中断模式。

# 2. CLIC设计

这一节描述CLIC的设计，包括其接收中断信号以及如何使处理器触发中断执行。

## 2.1 CLIC的中断级别

CLIC扩展了中断抢占的模式，支持每个特权级别高达256种中断级别，级别数越大，优先级越高，能够抢占低优先级中断的执行。0级中断代表中断处理程序外正常程序的执行。1 ～ 255级的中断对应中断处理的级别。平台的配置会决定需要支持多少中断的级别。

在相同的特权级别下，更高级别的到来的中断可以抢占当前正在执行的低级别中断处理程序，前提是此时该特权级别下全局中断是开放的。

## 2.2 CLIC中断输入控制(clicintctl)

CLIC设计中支持每个hart高达1024个中断输入，对于每个中断输入i来说，都有一个8位的存储映射的WARL控制寄存器：clicintctl[i]。前16个中断用于保留给CLINT的中断使用，该16位保留在mip以及mie寄存器中使用，余下的1008个本地的扩展中断可以添加进CLIC当中。

CLIC的一个固定的参数（CLICINTCTLBITS）用来表示clicintctl中总共有多少位，其中有：2<=CLICINTCTLBITS<=8。实现了的位在clicintctl[i]寄存器当中保持左对齐（以最高位作为基准），而低位没有使用的位则硬编码为1。这些配置相关作为模式、等级以及优先级的位取决于cliccfg寄存器当中的设置，将会在下文描述。

每个中断的输入还有一个中断使能位：clicintie[i]，以及一个中断等待位：clicintip[i]，两者都是存储映射的。对于对中断级别敏感的硬件中断，中断等待位是只读的。

CLIC将clicintctl的值作为无符号整数来看待，根据当前所有正在等待且开放的中断的优先级，从中选出最高优先级的中断来在内核中执行（即clicintctl中的值）。cliccfg寄存器中的设定用来决定如何将最大的clincintctl值分离表示位特权模式（M/S/U）、中断等级（0 ～ 255）以及中断优先级（0 ～ 255）。

### 2.2.1 中断输入原因代码

CLIC中的1024种中断向量给予独一无二的mcause exccode值。原生的CLINT中断保持它们原生的cause值，新的中断号码则从16开始。

## 2.3 CLIC配置寄存器(cliccfg)

CLIC有一个内存映射的全局配置寄存器：cliccfg，它用来定义clicintctl[i]如何细分来表示中断的模式、级别以及优先级域。它们所保存的顺序从clicintctl的最高位到最低位。clicintctl[i]的最低位还可以用来控制中断i是否使用硬件向量的方式来中断。

cliccfg寄存器有3个WARL域，一个2位的nmbits域，一个4位的nlbits域，以及1个nvbits域，再加上一个保留的WARL域硬编码位0的保留位：

<table>
    <tr>
        <th>bits</th>
        <th>field</th>
    </tr>
    <tr>
        <th>7</th>
        <th>reserved (WARL 0)</th>
    </tr>
    <tr>
        <th>6:5</th>
        <th>nmbits[1:0]</th>
    </tr>
    <tr>
        <th>4:1</th>
        <th>nlbits[3:0]</th>
    </tr>
    <tr>
        <th>0</th>
        <th>nvbits</th>
    </tr>
</table>

cliccfg接收到reset信号时会重置为0（所有的中断都位于机器模式下，且级别为255）。

### 2.3.1 指定中断的特权模式

2位WARL的cliccfg.nmbits域用来表示一个clicintctl[i]寄存器中有多少位用来保存中断输入i的特权模式的信息。

只支持机器模式的系统，cliccfg.nmbits的值为0（不支持其他的特权模式）。

支持机器模式和用户模式的系统，cliccfg.nmbits的值可以是0或者1。如果cliccfg.nmbits = 0，则所有的中断视为机器模式的中断。如果cliccfg.nmbits = 1，则clicintctl[i]最高位为1时，表示该中断在机器模式中触发，当最高位为0时，表示该中断在用户模式下触发。

支持机器模式、特权模式以及用户模式的系统，cliccfg.nmbits的值可以是0,1或者2。如果cliccfg.nmbits = 0，则所有的中断视为机器模式的中断。如果cliccfg.nmbits = 1，则表示clicintctl[i]的最高位表示该中断触发的特权级别（机器模式为1，特权模式为0）。如果cliccfg.nmbits = 2，表示clicintctl[i]中最高的两位编码代表中断触发的特权级别（与架构默认的特权模式编码一致）。下面给出的是nmbits设定的概括：

```
  特权模式  nmbits clicintctl[i]   描述
       M      0     xxxxxxxx     机器模式中断，中断级别+中断优先级=xxxxxxxx

     M/U      0     xxxxxxxx     机器模式中断，中断级别+中断优先级=xxxxxxxx
     M/U      1     0xxxxxxx     用户模式中断，中断级别+中断优先级=xxxxxxx
     M/U      1     1xxxxxxx     机器模式中断，中断级别+中断优先级=xxxxxxx

   M/S/U      0     xxxxxxxx     机器模式中断，中断级别+中断优先级=xxxxxxxx
   M/S/U      1     0xxxxxxx     特权模式中断，中断级别+中断优先级=xxxxxxx
   M/S/U      1     1xxxxxxx     机器模式中断，中断级别+中断优先级=xxxxxxx
   M/S/U      2     00xxxxxx     用户模式中断，中断级别+中断优先级=xxxxxx
   M/S/U      2     01xxxxxx     特权模式中断，中断级别+中断优先级=xxxxxx
   M/S/U      2     10xxxxxx     保留（或者为扩展的特权模式）
   M/S/U      2     11xxxxxx     机器模式中断，中断级别+中断优先级=xxxxxx

   M/S/U      3     xxxxxxxx     保留
```

### 2.3.2 指定中断的级别

4位WARL的cliccfg.nlbits域用来指示在clicintctl[i]中有多少位用于编码当前触发中断的级别，有效的值为0 ～ 8。

如果nmbits + nlbits > CLICINTCTLBITS，则中断级别编码超出范围的低位硬编码为1。如果nlbits<8，则中断级别编码超出范围的低位也编码为1，下面是例子：

```
 #nlbits值   clincintctl[i]编码                             中断等级
     1      l.......                                  127,                            255
     2      ll......                     63,          127,            191,            255
     3      lll.....               31,   63,   95,    127,    159,    191,    223,    255
     4      llll....            15,31,47,63,79,95,111,127,143,159,175,191,207,223,239,255

 "l" 位指在clicintctl[i]中用于编码中断级别的位
 "." 位指不能用于编码中断级别的位，假设为1
```

如果nlbits=0，则所有的中断都看作是255级中断。

cliccfg设置下的一些例子：

```
CLICINTCTLBITS nmbits nlbits clicintctl[i]  中断等级
      2         2       2      mm......     255
      3         2       2      mml.....     127,255
      4         2       2      mmll....     63,127,191,255
      5         2       3      mmlll...     31,63,95,127,159,191,223,255
      5         1       1      mlppp...     127,255
```

### 2.3.3 指定中断的优先级

clicintctl[i]余下的低位用来表示正在等待且开放的中断，在同一个特权模式以及中断等级下的优先级。在给定的特权模式以及中断等级下，最高优先级的中断将会首先触发。在同一个特权模式下，几个优先级相同的中断同时等待触发，则级别最高的中断首先触发。

和中断级别的编码方式相同，如果剩下只有1个位来编码优先级（p111_1111），则优先级只有两种：127和255（0111_1111和1111_1111），而如果有两位（pp11_1111），则优先级可有4种：63,127,191,255。

### 2.3.4 指定选中触发中断的中断模式

WARL的nvbits位用于表示中断的模式是向量还是非向量的。

如果nvbits=0，则选中的中断使用非向量模式，而所有的中断则会根据mtvec的低两位来决定是否使用向量中断。mtvec寄存器在CLIC中用来指定在非向量中断的CLIC模式下，使用的中断模式是什么。mtvt寄存器保存着中断向量表的入口地址，软件中断则使用mnxti寄存器。

如果nvbits=1，则选中的中断使用向量模式。clicintctlp[i]的低位控制中断i的向量中断行为。如果低位中的值为0，则中断的行为遵循mtvec中默认的设定。如果低位中的值为1，则中断使用硬件向量模式，独立与mtvec。这样允许一些中断全部跳转到mtvec中的基地址，而其它中断则根据mtvt中定义的中断向量表入口地址来寻找相应的中断向量。

## 2.4 CLIC与其它本地中断的交互

CLIC包含了mip以及mie中提供的16个快速的本地中断，因此在启动CLIC时，这16个中断将不再在mip/mie中可见。

现有的计时中断（mtip/stip/utip）、软件中断（msip/ssip/usip）以及外部中断（meip/seip/ueip）输入将被看作是额外的本地中断源，它们的特权模式、中断等级以及优先级可以使用存储映射的clicintctl[i]寄存器来定义：对于所有的meip/mtip/msip/seip/stip/ssip/ueip/utip/usip，都可以提供一个8位的寄存器，格式如上所述。
