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

DTM提供给予外部JTAG或USB访问DM的渠道。



## 参考资料

- [深入浅出RISC-V调试](https://liangkangnan.gitee.io/2020/03/21/深入浅出RISC-V调试/)
- [浅谈RISC-V的DEBUG系统及其仿真](https://zhuanlan.zhihu.com/p/125145986)
- [RISC-V Debug Support Version 1.0.0-STABLE](https://github.com/riscv/riscv-debug-spec/blob/master/riscv-debug-stable.pdf)
- [The RISC-V Instruction Set Manual Volume II: Privileged Architecture Document Version 1.12-draft](https://riscv.org/technical/specifications/)

- [JTAG原理](https://blog.csdn.net/orange_os/article/details/7544032)

