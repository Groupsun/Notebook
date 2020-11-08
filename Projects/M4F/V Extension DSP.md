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



## 向量内存存取指令

### 指令编码

基于标量浮点存取的编码格式（LOAD-FP/STORE-FP）：

![v-ldst-format](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201104170937684.png)

值得注意的是，向量内存存取指令中直接对静态传输数据的EEW编码。

### 寻址模式

支持三种：unit-stride、strided（跨步）以及indexed（索引）。向量加载与存储指令的基址寄存器以及步幅都是从`GPRx`寄存器中提取的。而所有向量存取指令的基本有效地址（base effective address）是由`rs1`寄存器（某个x寄存器）提供的。

- unit-stride方式，存取操作从基本有效地址开始连续的进行存取。
- strided方式，存取操纵从基本有效地址开始存取，然后由`rs2`寄存器（某个x寄存器）提供的偏移量不断递增地址来进行存取。
- indexed的方式，存取操作的元素的地址由基本有效地址与`vs2`中制定的偏移量相加得到。数据向量寄存器组中有EEW=SEW，EMUL=LMUL。偏移向量寄存器组EEW则直接在指令中编码，且EMUL=(EEW/SEW)*LMUL

向量偏移量的操作数被看作是字节地址偏移量的向量。如果该向量中的元素比XLEN小，则XLEN会被零扩展到XLEN。如果比XLEN大，则会截取到XLEN的大小来计算。

寻址模式的编码在2位的`mop`域当中：

![mop-encoding](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201105204739129.png)

unit-stride的还有额外的寻址模式表示在5位的`lumop`以及`sumop`当中，分别对应加载以及存储的指令：

![lumop](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201105211254690.png)

![sumop](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201105211317380.png)

`nf[2:0]`用于编码每个段（segment）的字段（field）数。对于一般的向量存取指令来说，`nf=0`，表示单个元素在向量寄存器组以及内存对应的每个位置之间移动。更大的`nf`值用于在一个段中连续的访问多个字段。`nf[2:0]`还对单条向量寄存器存取指令的向量寄存器数量做编码。

### 宽度编码

向量存取的指令将EEW直接编码在指令当中，且EMUL=(EEW/SEW)*LMUL：

![vector-width-encoding](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201106205419552.png)

### 向量Uint-Stride类访存指令

- [x] vle{EEW}.v：Unit-Stride加载。
- [x] vse{EEW}.v：Unit-Stride存储。

#### 简记

```
# Vector unit-stride loads and stores

# vd destination, rs1 base address, vm is mask encoding (v0.t or <missing>)
vle8.v    vd, (rs1), vm  #    8-bit unit-stride load
vle16.v   vd, (rs1), vm  #   16-bit unit-stride load
vle32.v   vd, (rs1), vm  #   32-bit unit-stride load
vle64.v   vd, (rs1), vm  #   64-bit unit-stride load
vle128.v  vd, (rs1), vm  #  128-bit unit-stride load
vle256.v  vd, (rs1), vm  #  256-bit unit-stride load
vle512.v  vd, (rs1), vm  #  512-bit unit-stride load
vle1024.v vd, (rs1), vm  # 1024-bit unit-stride load

# vs3 store data, rs1 base address, vm is mask encoding (v0.t or <missing>)
vse8.v    vs3, (rs1), vm  #    8-bit unit-stride store
vse16.v   vs3, (rs1), vm  #   16-bit unit-stride store
vse32.v   vs3, (rs1), vm  #   32-bit unit-stride store
vse64.v   vs3, (rs1), vm  #   64-bit unit-stride store
vse128.v  vs3, (rs1), vm  #  128-bit unit-stride store
vse256.v  vs3, (rs1), vm  #  256-bit unit-stride store
vse512.v  vs3, (rs1), vm  #  512-bit unit-stride store
vse1024.v vs3, (rs1), vm  # 1024-bit unit-stride store
```



## 初步的提取

- [x] `vsetvli`
- [x] `vsetvl`

- [x] `vle{EEW}.v`
- [x] `vse{EEW}.v`

- [x] `vlse{EEW}.v`
- [x] `vsse{EEW}.v`
- [ ] `vlxei{EEW}.v`
- [ ] `vsxei{EEW}.v`
- [ ] `vsuxei{EEW}.v`
- [x] `vadd.{vv|vx|vi}`
- [x] `vsub.{vv|vx}`
- [ ] Widening Integer Add/Subtract
- [ ] Vector Integer Extension
- [ ] Vector Integer Add-with-Carry / Subtract-with-borrow
- [x] `vand.{vv|vx|vi}`
- [x] `vor.{vv|vx|vi}`
- [x] `vxor.{vv|vx|vi}`
- [x] `vsll.{vv|vx|vi}`
- [x] `vsrl.{vv|vx|vi}`
- [x] `vsra.{vv|vx|vi}`
- [ ] Vector Narrowing Integer Right Shift
- [x] Vector Integer Comparison
- [ ] Vector Integer Min/Max
- [x] Vector Single-Width Integer Multiply
- [x] Vector Integer Divide
- [ ] Vector Widening Integer Multiply
- [x] Vector Single-Width Integer Multiply-Add
- [ ] Vector Widening Integer Multiply-Add
- [ ] Vector Quad-Widening Integer Multiply-Add
- [x] `vmerge.{vvm|vxm|vim}`
- [x] `vmv.v.{v|x|i}`
- [x] Vector Single-Width Saturating Add and Subtract
- [x] Vector Single-Width Averaging Add and Subtract
- [ ] Vector Single-Width Fractional Multiply with Rounding and Saturation
- [ ] Vector Single-Width Scaling Shift
- [ ] Vector Narrowing Fixed-Point Clip
- [x] Vector Single-Width Integer Reduction
- [ ] Vector Widening Integer Reduction
- [x] Vector Mask-Register Logical
- [x] `vpopc.m`
- [x] `vfirst.m`
- [x] `vmsbf.m`
- [x] `vmsif.m`
- [x] `vmsof.m`
- [x] `viota.m`
- [x] `vid.v`
- [x] Integer Scalar Move
- [ ] Vector Slide
- [ ] Vector Register Gather
- [ ] Vector Compress
- [x] Whole Vector Register Move