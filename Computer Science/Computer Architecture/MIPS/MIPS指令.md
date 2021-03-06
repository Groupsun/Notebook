# MIPS指令集

MIPS指令集属于精简指令集（RISC）、定长指令集（32位），指令格式简单（x86不定长指令集）。

MIPS有32个通用寄存器REG。

## MIPS指令格式

### R格式

<table>
    <tr>
        <th>6</th>
        <th>5</th>
        <th>5</th>
        <th>5</th>
        <th>5</th>
        <th>6</th>
    </tr>
    <tr>
        <th>op</th>
        <th>rs</th>
        <th>rt</th>
        <th>rd</th>
        <th>shamt</th>
        <th>funct</th>
    </tr>
</table>

寄存器-寄存器ALU操作、读写专用寄存器。

### I格式

<table>
    <tr>
        <th>6</th>
        <th>5</th>
        <th>5</th>
        <th>16</th>
    </tr>
    <tr>
        <th>op</th>
        <th>rs</th>
        <th>rt</th>
        <th>Imm</th>
    </tr>
</table>

立即数操作，加载/存储字节、半字、字、双字。
条件分支，跳转，跳转并链接寄存器。

### J格式

<table>
    <tr>
        <th>6</th>
        <th>26</th>
    </tr>
    <tr>
        <th>op</th>
        <th>target addr</th>
    </tr>
</table>

跳转、跳转并链接。从陷阱和异常中返回。

各字段含义： 
- op:     指令基本操作，称为操作码。 
- rs:     第一个源操作数寄存器。 
- rt:     第二个源操作数寄存器。 
- rd:     存放操作结果的目的操作数。 
- shamt:  位移量； 
- funct:  函数，这个字段选择op操作的某个特定变体。

## MIPS常用指令集

- lb/lh/lw: 从存储器中读取一个byte/half word/word的数据到寄存器中.如 lb 1,0(1,0(2) 
- sb/sh/sw: 把一个byte/half word/word的数据从寄存器存储到存储器中.如 sb 1,0(1,0(2) 
- add/addu:把两个定点寄存器的内容相加add 1,1,2,3(3(1=2+2+3);u为不带符号加。 - addi/addiu:把一个寄存器的内容加上一个立即数add 1,1,2,#3(1=1=2+3);u为不带符号加。 
- sub/subu：把两个定点寄存器的内容相减。 
- div/divu:两个定点寄存器的内容相除。 
- mul/mulu:两个定点寄存器的内容相乘。 
- and/andi:与运算，两个寄存器中的内容相与and 1,1,2,3(3(1=2 &2 &3);i为立即数 
- or/ori:或运算。 
- xor/xori:异或运算。 
- beq/beqz/benz/bne:条件转移eq相等，z零，ne不等。 
- j/jr/jal/jalr:j直接跳转；jr使用寄存器跳转； 
- lui:把一个16位的立即数填入到寄存器的高16位，低16位补零。 
- sll/srl:逻辑左移/右移sll 1,1,2,#2。 
- slt/slti/sltui:如果2的值小于2的值小于3，那么设置1的值为1，否则设置1的值为1，否则设置1的值为0。slt 1,1,2,$3。 
- mov/movz/movn:复制，n为负，z为零。mov 1,1,2; movz 1,1,2,3(3(3为零则复制2到2到1)。 
- trap:根据地址向量转入管态。 
- eret:从异常中返回到用户态。

## 32个通用寄存器

- 0($zero): 永远返回值为0 
- 1($at): 用做汇编器的暂时变量 
- 2-3(v0−v0−v1): 子函数调用返回结果 
- 4-7(a0−a0−a3): 子函数调用的参数 
- 8-15(t0−t0−t7)： 暂时变量，子函数使用时不需要保存与恢复 
- 16-23(s0−s0−s7)： 子函数寄存器变量。子函数必须保存和恢复使用过的变量在函数返回之前，从而调用函数知道这些寄存器的值没有变化。 
- 24-25(t8−t8−t9)： 暂时变量，子函数使用时不需要保存与恢复 
- 26-27(k0−k0−k1)： 通常被中断或异常处理程序使用作为保存一些系统参数 
- 28($gp)： 全局指针。一些运行系统维护这个指针来更方便的存取“static“和”extern”变量。 
- 29($sp)： 堆栈指针 
- 30(s8/s8/fp)： 第9个寄存器变量。子函数可以用来做桢指针 
- 31($ra)： 子函数的返回地 
- cp0：协处理器0，MIPS对CPU的控制用cp0完成
