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



# RISC-V "V" Extension

## "V"扩展模型

新增了32个向量寄存器组，以及7个非特权级的CSRs：

![V-extension-CSRs](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201104145622321.png)

#### `vtype`

![v-extension-vtype](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201104145730553.png)

`vsew`编码以及实例：

![v-extension-vsew](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201104145820712.png)

`vlmul`设定以及相关参数的实例：

![v-extension-vlmul](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201104145913214.png)

`vta`以及`vma`：

![v-extension-vta-vma](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201104145955927.png)

#### `vl`、`vlenb`

`vl`表示当前指令所要更新的向量中的元素个数，`vlenb = vl/8`

#### `vstart`

表示向量指令在一个向量中开始执行的第一个元素的索引号。

#### `vxrm`、`vxsat`

分别表示服点向量运算的舍入以及饱和设置等。

#### `vcsr`

`vcsr[2:0] = CatBits(vxrm[1:0], vxsat)`



## 配置设定类指令

- [x] `vsetvli`：基于指令中设定的立即数参数以及`rd`、`rs1`的值设置`vtype`以及`vl`这两个CSRs，并将新的`vl`值写入到`rd`中。
- [x] `vsetvl`：基于指令中`rs2`、`rd`、`rs1`寄存器中的值设置`vtype`以及`vl`这两个CSRs，并将新的`vl`值写入到`rd`中。

#### 格式

![vsetvl(i)-format](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201104152431450.png)

#### 简记

```
vsetvli rd, rs1, vtypei # rd = new vl, rs1 = AVL, vtypei = new vtype setting 
vsetvl rd, rs1, rs2 		# rd = new vl, rs1 = AVL, rs2 = new vtype value
============================================================================
Suggested assembler names used for vsetvli immediate

e8    # SEW=8b
e16   # SEW=16b
e32   # SEW=32b
e64   # SEW=64b
e128  # SEW=128b
e256  # SEW=256b
e512  # SEW=512b
e1024 # SEW=1024b

mf8  # LMUL=1/8
mf4  # LMUL=1/4
mf2  # LMUL=1/2
m1   # LMUL=1, assumed if m setting absent
m2   # LMUL=2
m4   # LMUL=4
m8   # LMUL=8

Examples:
  vsetvli t0, a0, e8          # SEW= 8, LMUL=1
  vsetvli t0, a0, e8,m2       # SEW= 8, LMUL=2
  vsetvli t0, a0, e32,mf2     # SEW=32, LMUL=1/2
```

`vtype`的设置是基于立即数`vtypei`以及`rs2`中的值。而新的向量长度设定则要基于AVL（application vector length）的值：

![AVL-settings](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201104153457926.png)

- 若`rs1`不是`x0`，那么AVL则是`rs1`中的值，且新的`vl`将会写入到`rd`中。
- 若`rs1=x0`但`rd!=x0`，那么则将最大的无符号整数值（～0）作为AVL的值，且将VLMAX写入到`vl`以及`rd`中。
- 若`rs1=x0 && rd=x0`，那么当前的向量长度值（`vl`中的值）会作为AVL的值，且结果值仅写入到`vl`中。



## 向量存取指令

### 指令编码

基于标量浮点存取的编码格式（LOAD-FP/STORE-FP）：

![v-ldst-format](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201104170937684.png)