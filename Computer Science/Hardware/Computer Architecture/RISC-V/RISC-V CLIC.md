# RISC-V Core-Local Interrupt Controller (CLIC)

基于文档：SiFive Proposal for a RISC-V Core-Local Interrupt Controller (CLIC), https://github.com/sifive/clic-spec/blob/master/clic.adoc#background-and-motivation 的简要翻译。

CLIC，Core-Local Interrupt Controller，即内核本地的中断控制器。

- 机器模式：M-mode
- 特权模式：S-mode
- 用户模式：U-mode
- WARL：Write Any Read Legal

# 背景以及动机

CLIC设计的目的是用来给RISC-V系统提供低延迟、向量式、抢占式的中断。当激活CLIC之后，CLIC会替代现存的RISC-V本地中断控制模式（CLINT）。CLIC的设计含有一个基本的设计模式，具有最小硬件的需求，但是同样支持额外的扩展来提供硬件加速的特性。CLIC设计的目标是为多种不同的软件ABI以及中断模式提供支持，而不需要复杂的硬件结构给高性能的处理器实现带来冲击。

## 现有的RISC-V中断

现有的RISC-V中断系统已经支持了抢占，但是只有在特权级以及更高的特权等级下才可用。在任何一个时刻，一个RISC-V的hart（hardware thread）都运行在一个特定的特权等级下。全局中断使能位：MIE/SIE/UIE，分别被包含在mstatus/sstatus/ustatus寄存器当中，用于控制中断是否能在当前或者更高的的特权模式下触发。任何更高特权级别的中断都会停止当前特权级别的程序执行，而进入更高的特权模式下的中断处理程序。每个特权级别都有它自己的中断状态寄存器（机器模式下的mepc/mcause，特权模式下的sepc/scause，用户模式下的uepc/ucause）来支持抢占，或者直接使用机器模式下的mepc。被更高特权等级的中断抢占还会将当前的特权模式以及中断使能状态压入在mstatus的mpp以及mpie的栈当中。

mtvec寄存器用来指示中断的模式以及中断向量表的基地址。mtvec的WARL最低2位用于指示当前处于哪种中断模式。在CLINT的模型当中，该两位用于表示中断是向量还是非向量（01 or 00），而高位的则是4字节地址对齐的基地址。

## CLIC与PLIC

标准的RISC平台级别中断控制器（Platform-level interrupt controller，PLIC）提供一种集中统一的中断优先级以及不同平台中断共同仲裁的功能。并且最后只会将仲裁后得到的单一的外部中断信号发送给每个hart的每个特权级别的指定位（meip/seip/ueip）。

## CLIC与CLINT

CLINT是一个相对来说比较小的逻辑单元，提供在早期的设计当中提供本地的中断控制，并且管理软件、计时器以及外部的中断信号（在mip寄存器中的msip/mtip/meip信号）。CLINT同时允许额外自定义的高速中断信号，通过在mip的搞16位中进行添加。

新mtvec模式的设置将在下文进行描述，是CLIC用于代替原有CLINT的方案。平台的配置可能同时需要支持原来的CLINT以及CLIC中断模式。
