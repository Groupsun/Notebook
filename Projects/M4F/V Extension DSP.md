# 参考：RISC-V "P" Extension Proposal

Gihub repo[地址](https://github.com/riscv/riscv-p-spec/blob/master/P-ext-proposal.adoc)。

## 概览

### SIMD数据处理指令

包含有8位以及16位的加减法、移位、比较、乘法、Misc（Miscellaneous，一些其它杂项相关的指令）以及unpacking的指令。

### Partial-SIMD数据处理指令

包含有：

- 16位的Packing指令；
- 最高有效字的“32x32”以及“32x16”的乘法&加法指令；
- 有符号的16位乘法与32位&64位加减法混合指令；
- Misc指令；
- 8位乘法与32位加法混合指令。

### 64位Profile指令

包含有64位加减法指令、32位乘法与64位加减法混合指令、有符号16位乘法与64位加减法混合指令。

### 非SIMD指令

包含有Q15与Q31饱和算术指令、32位算术运算指令、溢出/饱和状态操纵指令以及Misc指令。

### 仅RV64支持指令

包含有仅在RV64中支持的指令。