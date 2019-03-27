# ARM Cortex-M处理器简介

## Cortex-M3和Cortex-M4处理器

Cortex-M3和Cortex-M4处理器使用32位架构，寄存器组中的内部寄存器、数据通路以及总线接口都是32位的。Cortex-M处理器使用的指令集架构（ISA）位thumb ISA，其基于Thumb-2技术并同时支持16位和32位指令。

Cortex-M3和Cortex-M4处理器具有以下特点：

- 三级流水线设计。
- 哈佛总线结构。
- 32位寻址，支持最大4GB的存储器空间。
- 具有NVIC（嵌套向量中断控制器），支持最多240个中断请求和8~256级中断优先级。
- 支持可选的MPU（存储器保护单元），提供了可编程存储器或访问权限控制等存储器保护特性。

Cortex-M3和Cortex-M4处理器提供的指令：

- 普通数据处理，包括硬件除法指令。
- 存储器访问指令，支持8位、16位、32位和64位数据，以及其它可以传输多个32位数据的指令。
- 位域处理指令。
- 乘累加（MAC）以及饱和指令。
- 用于跳转、条件跳转以及函数调用的指令。
- 用于系统控制、支持OS等的指令。

## Thumb ISA的架构版本

Thumb ISA是Cortex-M处理器使用的指令集架构。

ARMv4T指令集架构中引入了Thumb指令集。ARMv5E架构增加了增强DSP指令。ARMv6架构增加了SIMD指令，且增加了对存储器的支持。

Cortex-M使用的是ARMv7版本的指令集架构。它为了满足不同领域的不同需求划分为了3类：

- Cortex-A处理器：ARMv7-A架构
- Cortex-R处理器：ARMv7-R架构
- Cortex-M处理器：ARMv7-M架构

Cortex-M3和Cortex-M4基于ARMv7-M。

所有的Cortex-M处理器都支持Thumb-2技术以及Thumb ISA的不同子集。由于处理器支持Thumb-2指令集中的16位和32位指令，因此无须在Thumb状态和ARM状态间来回切换。
