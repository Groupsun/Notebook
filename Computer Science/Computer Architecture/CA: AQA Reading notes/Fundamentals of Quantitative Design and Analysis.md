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
