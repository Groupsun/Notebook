# Fundamentals of Quantitative Design and Analysis

这类文章是我阅读 Computer Architecture: A Quantitative Approach 6th edition 的笔记。

## ISA的7个方面定义的维度（7种主要的特征）

1. ISA的种类（Class of ISA）：寄存器-内存ISA（register-memory ISAs），加载-存储ISA（load-store ISAs）。
2. 内存寻址（Memory addressing）：内存地址是否允许不对齐。
3. 寻址模式（Addressing modes）：寄存器寻址、立即数寻址、移位寻址等等。
4. 操作数的类型以及大小（Types and sizes of operands）：8位（ASCII字符）、16位（Unicode字符）、32位（整型或字）、64位（双字或者长整数）以及IEEE 754标准的浮点数，包括32位（单精度）以及64位（双精度），80x86还有80位的浮点数（扩展双精度）。
5. 指令类型（Operations）：数据传送（data transfer），算术逻辑（arithmetic logical），控制（control）以及浮点数操作（floating point）。
6. 控制流指令（Control flow instructions）：包括条件跳转、非条件跳转以及过程调用和返回。条件跳转可以是测试寄存器数值（RISC-V）或者测试条件码（ARMv8和80x86）。
7. ISA的编码方式（Encoding an ISA）：可以是定长的指令（fixed length）或者是变长的指令（variable length）。通常来说同一个程序，变长指令的代码大小要比定长指令的代码大小要小，但是定长指令一般会有压缩的指令集（Thumb-2和RISC-V的“C”扩展指令集）。

## 5种主要影响计算机架构的实现技术

架构师在设计ISA的过程中，需要考虑下列5种实现技术的后续发展：

1. 集成电路技术（Integrated circuit logic technology）。摩尔定律的失效。
2. 半导体DRAM（Semiconductor DRAM）。
3. 半导体闪存（Semiconductor Flash）。
4. 磁碟技术（Magnetic disk technoloy。基本很难再发展，唯一的出路是用HAMR（Heat Assisted Magnetic Recording）。
5. 网络技术（Network technology）。

## 微处理器中的能量以及功率

在CMOS电路中，消耗能量主要来自于开关晶体管，这称为动态能量（dynamic energy）。动态能量与电容负载以及电压的平方之间的乘积呈正相关：

$$ Energy_{dynamic} \sim Capacitive Load \times Voltage^2 $$

上面的式子是在1->0->1或者0->1->0脉冲产生的能量，如果要考虑的是1->0或者0->1脉冲产生的能量则：

$$ Energy_{dynamic} \sim \frac{1}{2} \times Capacitive Load \times Voltage^2 $$

功率则还要乘以开关晶体管的频率：

$$ Power_{dynamic} \sim \frac{1}{2} \times Capacitive Load \times Voltage^2 \times Frequency switched $$

因此，虽然现在处理器相比以前来说要电压要低，但是频率要高出许多，因此带来的就是功率的提高。单位时间内产生的热量也随着升高，从2003年开始，处理器的频率增长的速度变得十分平缓（大概每年4%左右）。因此，现在处理器为了解决热量耗散、能量消耗、工作电压以及功率的问题，采取以下的方法：

1. 什么都不做（Do nothing well）。当没有浮点运算的时候，停止浮点运算的时钟。当某个核处于空闲状态时，也会停止这个核的时钟。
2. 动态电压-频率缩放（Dynamic Voltage-Frequency Scaling，DVFS）。现代的处理器可以在不需要高负载的时候，降低主频以及电压来换取更低的能量消耗以及功率。
3. 为典型的情况做出设计（Design for the typical case）。现代的便携设备以及笔记本电脑的DRAM会在空闲的时候进入节能模式来减少能量的耗散，在硬盘上也有同样类似的方案提出（空闲的时候降低硬盘的旋转速度）。同样，PC上的微处理器也会在温度高的时候，利用片上的温度传感器来自动降低主频（高温降频）。
4. 超频（Overclocking）。Intel在2008年提出了这项技术。Intel的处理器在允许的情况下可以短时间内运行的主频超过额定的主频，也可以在某些单核运行的程序运行时，关闭其他核，只留一个核超频运转一段时间，来加速性能。

当然，能量消耗还有一部分来自于漏电流，称为静态能量。漏电流除非切断电源供给，否则将会一直存在：

$$ Energy_{static} \sim Current_{static} \times Voltage $$

一般来说，静态能量占据系统全部能量的25%，然而，一些服务器甚至高达50%以上。这是因为SRAM要维持其内部数据稳定所致。

因此，在这种大前提下，为了提高能量的效率，架构师使用了暗硅（dark silicon）的手段。也就是说，在任意时刻，处理器中很多的晶体管是没有工作的（也可以理解为，多核处理器中，一个核在很多时候是处于空闲状态，时钟停止）。
