这是将从RISC-V向量指令集“V“当中所提取的DSP类指令子集与RI5CY自定义扩展指令集中DSP相关的指令进行对比评估的记录文档。



## 概述

由于RI5CY已经停止了维护，因而本文档主要基于其后续的持续维护版本CV32E40P为基础。CV32E40P有大量的自定义指令，但是和DSP相关的只有两类，一类是Multiply-Accumulate，另一类则是SIMD。本文档着重对比评估“V”-DSP与这两类指令。



# Multiply-Accumulate

CV32E40P支持自定义的指令扩展用于乘累加（multiply-accumulate）以及支持可选乘后移位的半字乘法（half-word multiplications with an optional post-multiplication shift）。

## MAC Opeartions

### 32位x32位乘法运算

|                  RI5CY-ext                   |     V-DSP     |
| :------------------------------------------: | :-----------: |
| **cv.mac** rD, rs1, rs2：rD = rD + rs1 * rs2 | **vmacc.vv**  |
| **cv.msu** rD, rs1, rs2：rD = rD - rs1 * rs2 | **vnmsac.vv** |

### 16位x16位乘法运算

- 第一类：符号扩展/零扩展16位乘法（通用寄存器的高位与低位）：通过配置元素长度的向量单宽度乘法指令。

