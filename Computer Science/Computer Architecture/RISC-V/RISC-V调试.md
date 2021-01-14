# RISC-V调试



## 概述

下图给出了RISC-V标准文档所定义的Debug系统全貌，其中虚线的块表示可选：

![RISC-V Debug系统全貌](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20210113144906368.png)

用户与调试主机（如笔记本）进行交互，在主机上运行着调试器（如gdb）。调试器通过与一个调试中继（Debug Translator，如OpenOCD，其中可能包含有一个硬件驱动）交互来与调试传输硬件（Debug Transport Hardware，如Olimex USB-JTAG适配器）交互。而调试传输硬件将调试主机与硬件平台的调试传输模块（DTM，Debug Transport Module）连接起来。DTM利用调试模块接口（DMI，Debug Module Interface）提供对一个或多个调试模块（DMs，Debug Modules）的访问。

DMs提供对应harts（hardware thread）的运行控制权，抽象指令（abstract commands）则提供了对通用寄存器的访问权限。额外的寄存器则可以通过抽象指令或者往可选的程序缓存中写入程序来访问。

程序缓存允许调试器在hart中执行任意的指令。这个机制同样可以用于访问内存。一个额外的系统总线访问块允许不使用RISC-V hart完成对内存的访问。

> 调试模式（D-mode）可以视作一种特殊的特权模式，并且比机器模式（M-mode）的权限要更大。调试模式保留了少量的CSR地址，且这些地址只能在D-mode下访问。同时，D-mode还会保留一定的物理地址空间。

由于RISC-V官方目前支持的调试方式是JTAG，因此我们先从JTAG讲起。



## JTAG简介

JTAG（Joint Test Action Group）是一种国际标准的调试方式（IEEE1149.1）。一个含有JTAG Debug接口模块的CPU，只要时钟正常，就可以通过JTAG接口访问CPU内部的寄存器（如通用寄存器）、挂在CPU总线上的设备以及内置模块的寄存器。JTAG的一个实现基础原理是利用了边界扫描技术。

> **边界扫描（Boundary Scan）**
>
> 边界扫描技术的基本思想是在靠近芯片的输入/输出引脚上增加一个移位寄存器单元，也就是边界扫描寄存器（Boundary Scan Regsiter，BSR）。通过边界扫描单元可以实现对输出/输出信号的观察和控制，且对芯片来说完全透明。并且BSR之间可以进行互连，形成一个边界扫描链。

标准的JTAG协议采用四线的方式，分别是TCK、TMS、TDI和TDO，还有一个可选的TRST引脚：

- **TCK**：测试时钟输入，为TAP的操作提供一个独立基本的时钟信号。
- **TMS**：测试模式选择，用于控制TAP状态机的切换。
- **TDI**：测试数据输入。
- **TDO**：测试数据输出。

其中复位信号**TRST**可以用来对TAP Controller进行复位，但此信号不强制要求实现，因为通过TMS也可以对TAP Controller进行复位。

调试时使用的工具如JLink或者CMSIS-DAP称为JTAG主机，而嵌入在芯片内部的JTAG成为JTAG从机。上述信号的方向都是对于JTAG从机来说的，下文若无特别说明，则JTAG都是指JTAG从机。

一个JTAG主机可以同时对多个JTAG从机进行调试，这通过JTAG扫描链完成：

![JTAG扫描链](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20210113161555367.png)

### TAP状态机

TAP（Test Access Port）是一个通用的端口，通过TAP可以访问芯片提供的所有数据寄存器（DR）和指令寄存器（IR）。对整个TAP的控制是通过TAP控制器（TAP Controller，实际上内部就是一个状态机）来完成的。可以通过TCK和TMS信号来改变状态机的状态。这个状态机的核心是两路SCAN，分别是IR SCAN和DR SCAN：

![TAP状态机](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20210113162752692.png)

TAP状态机的转移取决于TMS信号的电平（也就是箭头上的0/1）。JTAG在每一个TCK信号的上升沿采样TMS信号和TDI信号，决定状态机的状态是否发生变化，并在每一个TCK信号的下降沿输出TDO信号。可以发现，无论TAP目前处于哪一个状态，只要TMS保持高电平并持续5个TCK时钟，则TAP一定会回到**Test-Logic-Reset**状态。

JTAG内部有一个IR寄存器和多个DR寄存器，IR寄存器决定好要访问的是哪一个DR寄存器。DR寄存器有IDCODE、BYPASS等等。在**Test-Logic-Reset**状态下IR寄存器默认选择的是IDCODE这个DR寄存器。

JTAG主机通过IR SCAN设置IR寄存器的值，然后通过DR SCAN来读写对应的DR寄存器。



## Debug Transport Module（DTM）

DTM提供给予外部JTAG或USB访问DM的渠道。RISC-V Debug标准文档所提供的DTM定义基于标准的JTAG协议。

### JTAG DTM

文档中所定义的DTM基于标准的JTAG TAP，JTAG TAP允许访问任意的JTAG寄存器。通过使用JTAG IR进行选择，然后通过JTAG DR进行访问。

#### JTAG DTM 寄存器

JTAG TAP所使用的DTM中必须包含一个至少5位的IR。当TAP重置后，IR必须重置为默认值`00001`，选择IDCODE寄存器。下面给出JTAG寄存器列表：

![JTAG寄存器列表](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20210114155028593.png)

若IR的位宽比5位要更大，那么上面列表中的寄存器地址需要进行零扩展，而BYPASS的0x1f地址编码除外，它进行的是符号扩展，也就是高位填充1。通常的调试器所要用到的JTAG寄存器只有BYPASS以及IDCODE，但标准中还提供了冗余的编码地址空间给其他标准的JTAG指令。

#### IDCODE（位于0x01）

该寄存器在TAP状态机重置时被选择，且该寄存器是只读的。IDCODE寄存器各个域的含义如下：

<img src="https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20210114155956799.png" alt="IDCODE寄存器" style="zoom:50%;" />

| 域             | 描述                                           | 访问权限 | 重置值     |
| -------------- | ---------------------------------------------- | -------- | ---------- |
| **Version**    | 标记部件版本号                                 | R[^1]    | Preset[^2] |
| **PartNumber** | 标记部件设计者序列号                           | R[^1]    | Preset[^2] |
| **Manufld**    | 厂商号，遵循JEP106标准分配，实际中可以为任意值 | R[^1]    | Preset[^2] |

[^1]: 只读。
[^2]: 表示重置后的值由设计者决定。

#### DTM Control and Status（`dtmcs`，位于0x10）

DTM控制与状态寄存器。`dtmcs`各个域的含义如下：

![dtmcs寄存器](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20210114161145255.png)

| 域               | 描述                                                         | 访问权限 | 重置值     |
| ---------------- | ------------------------------------------------------------ | -------- | ---------- |
| **dmihardreset** | **DTM模块硬复位**<br>往该位写1将DTM进行硬复位，将会导致DTM丢弃任何正在进行的DMI事务，且将所有寄存器和内部的状态置为重置值。一般来说，这种情况只有在调试器有理由相信当前正在进行的DMI事务将永远不会完成时才会发生。 | W1[^3]   | -          |
| **dmireset**     | **清除错误状态**<br>往该位写1会清除错误状态，但是不会影响当前正在执行的DMI事务。 | W1[^3]   | -          |
| **idle**         | 提示调试器可以停留在**Run-Test-Idle**的周期数，用于避免在每次DMI扫描后返回'busy'。当必要时，调试器还需要检查`dmistat`。<br>0：没有进入**Run-Test-Idle**状态的必要。<br>1：进入**Run-Test-Idle**状态可以立刻离开。<br>2：进入**Run-Test-Idle**状态后需要保持1个周期后才能离开<br>以此类推 | R[^1]    | Preset[^2] |
| **dmistat**      | **上一次操作的状态**<br>0：无错。<br>1：保留。含义与2相同。<br>2：操作出错。<br>3：操作未完成。 | R[^1]    | 0          |
| **abits**        | `dmi`寄存器中地址域（`address`）的宽度。                     | R[^1]    | Preset     |
| **version**      | 实现所对应的Debug标准文档的版本。<br>0：v0.11。<br>1：v0.13和v1.0。<br>15：非标。 | R[^1]    | 1          |

[^3]: 只允许写，且只有写1时有效。尝试读时永远返回0。



## 参考资料

- [深入浅出RISC-V调试](https://liangkangnan.gitee.io/2020/03/21/深入浅出RISC-V调试/)
- [浅谈RISC-V的DEBUG系统及其仿真](https://zhuanlan.zhihu.com/p/125145986)
- [RISC-V Debug Support Version 1.0.0-STABLE](https://github.com/riscv/riscv-debug-spec/blob/master/riscv-debug-stable.pdf)
- [The RISC-V Instruction Set Manual Volume II: Privileged Architecture Document Version 1.12-draft](https://riscv.org/technical/specifications/)

- [JTAG原理](https://blog.csdn.net/orange_os/article/details/7544032)

