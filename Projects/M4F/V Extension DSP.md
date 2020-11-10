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

- strided方式，存取操纵从基本有效地址开始存取，然后由`rs2`寄存器（某个x寄存器）提供的偏移量不断递增地址来进行存取。[^1]

  [^1]: （待定）strided方式在什么场景下会用到？

- indexed的方式，存取操作的元素的地址由基本有效地址与`vs2`中制定的偏移量相加得到。数据向量寄存器组中有EEW=SEW，EMUL=LMUL。偏移向量寄存器组EEW则直接在指令中编码，且EMUL=(EEW/SEW)*LMUL。[^2]

  [^2]: （待定）indexed方式在什么场景下会用到？

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

### 向量Unit-Stride类访存指令

- [x] `vle{EEW}.v`：Unit-Stride加载。
- [x] `vse{EEW}.v`：Unit-Stride存储。

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

### Stride类访存指令

- [x] `vlse{EEW}.v`：Stride加载。
- [x] `vsse{EEW}.v`：Stride存储。

#### 简记

```
# Vector strided loads and stores

# vd destination, rs1 base address, rs2 byte stride
vlse8.v    vd, (rs1), rs2, vm  #    8-bit strided load
vlse16.v   vd, (rs1), rs2, vm  #   16-bit strided load
vlse32.v   vd, (rs1), rs2, vm  #   32-bit strided load
vlse64.v   vd, (rs1), rs2, vm  #   64-bit strided load
vlse128.v  vd, (rs1), rs2, vm  #  128-bit strided load
vlse256.v  vd, (rs1), rs2, vm  #  256-bit strided load
vlse512.v  vd, (rs1), rs2, vm  #  512-bit strided load
vlse1024.v vd, (rs1), rs2, vm  # 1024-bit strided load

# vs3 store data, rs1 base address, rs2 byte stride
vsse8.v    vs3, (rs1), rs2, vm  #    8-bit strided store
vsse16.v   vs3, (rs1), rs2, vm  #   16-bit strided store
vsse32.v   vs3, (rs1), rs2, vm  #   32-bit strided store
vsse64.v   vs3, (rs1), rs2, vm  #   64-bit strided store
vsse128.v  vs3, (rs1), rs2, vm  #  128-bit strided store
vsse256.v  vs3, (rs1), rs2, vm  #  256-bit strided store
vsse512.v  vs3, (rs1), rs2, vm  #  512-bit strided store
vsse1024.v vs3, (rs1), rs2, vm  # 1024-bit strided store
```

### Indexed类访存指令

- [ ] `vlxei{SEW}.v`：indexed加载。
- [ ] `vsxei{SEW}.v`：indexed有序存储。
- [ ] `vsuxei{SEW}.v`：indexed无序存储。

#### 简记

```
# Vector indexed loads and stores

# vd destination, rs1 base address, vs2 indices
vlxei8.v    vd, (rs1), vs2, vm  #    8-bit indexed load of SEW data
vlxei16.v   vd, (rs1), vs2, vm  #   16-bit indexed load of SEW data
vlxei32.v   vd, (rs1), vs2, vm  #   32-bit indexed load of SEW data
vlxei64.v   vd, (rs1), vs2, vm  #   64-bit indexed load of SEW data

# Vector ordered indexed store instructions
# vs3 store data, rs1 base address, vs2 indices
vsxei8.v    vs3, (rs1), vs2, vm  # ordered  8-bit indexed store of SEW data
vsxei16.v   vs3, (rs1), vs2, vm  # ordered 16-bit indexed store of SEW data
vsxei32.v   vs3, (rs1), vs2, vm  # ordered 32-bit indexed store of SEW data
vsxei64.v   vs3, (rs1), vs2, vm  # ordered 64-bit indexed store of SEW data

# Vector unordered-indexed store instructions
vsuxei8.v   vs3, (rs1), vs2, vm # unordered  8-bit indexed store of SEW data
vsuxei16.v  vs3, (rs1), vs2, vm # unordered 16-bit indexed store of SEW data
vsuxei32.v  vs3, (rs1), vs2, vm # unordered 32-bit indexed store of SEW data
vsuxei64.v  vs3, (rs1), vs2, vm # unordered 64-bit indexed store of SEW data
```

### Unit-stride仅故障优先（Fault-Only-First）加载

该指令和一般的加载指令没有区别除了其仅仅会在元素0中会允许执行trap。如果在>0的元素的执行过程中发生了异常，则后续元素的执行将会停止，并且将`vl`更改为已经执行了的元素个数。

- [ ] `vle{EEW}ff.v`[^3]

[^3]: 在官方文档中，列举了在`strlen`中使用上述指令的例子。

#### 简记

```
# Vector unit-stride fault-only-first loads and stores

# vd destination, rs1 base address, vm is mask encoding (v0.t or <missing>)
vle8ff.v    vd, (rs1), vm  #    8-bit unit-stride fault-only-first load
vle16ff.v   vd, (rs1), vm  #   16-bit unit-stride fault-only-first load
vle32ff.v   vd, (rs1), vm  #   32-bit unit-stride fault-only-first load
vle64ff.v   vd, (rs1), vm  #   64-bit unit-stride fault-only-first load
vle128ff.v  vd, (rs1), vm  #  128-bit unit-stride fault-only-first load
vle256ff.v  vd, (rs1), vm  #  256-bit unit-stride fault-only-first load
vle512ff.v  vd, (rs1), vm  #  512-bit unit-stride fault-only-first load
vle1024ff.v vd, (rs1), vm  # 1024-bit unit-stride fault-only-first load
```

即使没有异常发生，该指令的实现允许执行少于`vl`的元素并减少`vl`的值。若`vstart=0`且`vl>0`则至少要执行一个元素。

### 向量加载/存储段指令（Zvlsseg）[^4][^5]

[^4]: （待定）该系列的指令还未纳入到基本的“V”扩展指令集当中，暂时不考虑纳入到DSP子集中。
[^5]: 此类型的指令给出的例子是用于读取内存中的像素RGB值或者复数值（packed structure data）。

### 向量加载/存储整个寄存器指令

此类的指令会加载以及存储整个向量寄存器（`VLEN`个位）。指令在`EEW=8`以及有效向量长度`evl=VLEN/8`的设置下执行，而不受寄存器`vtype`以及`vl`的影响。该指令同样适用于segment传输，由指令中的`nf`域制定segment中字段的个数。

- [ ] `vl{nf}r.v`
- [ ] `vs{nf}r.v`

#### 指令编码

![vector-whole-register](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201110151344521.png)

`rs1`表示基地址，指令传送的整个向量寄存器是加载的目的`vd`或者源寄存器`vs3`，指令的参数固定设定为：`EEW=8 and EMUL=1`。

#### 简记

```
# Format of whole register move instructions.
vl1r.v v3, (a0)		# Load v3 with VLEN/8 bytes held at address in a0
vs1r.v v3, (a1)		# # Store v3 to address in a1
```



## 向量算术指令格式

向量算术指令使用新的op码OP-V=$1010111_2$。`funct3`域用于定义不同的算术运算操作。格式一览：

![V-arithmatic-format](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201110154920411.png)

`funct3`用于定义操作数的类型以及来源：

![funct3](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/image-20201110155259859.png)

有关整数的运算使用无符号或者补码有符号整数的形式进行算术运算。对于向量-标量的运算来说，其中一个向量的源操作数来自`vs2`，另一个标量源操作数则可以是：

1. 对于整数运算来说，可以是在`rs1`域中编码的5位立即数。
2. 对于整数运算来说，可以是在`rs1`域中编码的通用寄存器号。若XLEN>SEW，则通用寄存器的低SEW位作为源操作数。若XLEN<SEW，则通用寄存器中的值将会做符号扩展到SEW位。

向量的算术指令运算的掩码行为受`vm`字段的控制。

#### 简记

```
# Assembly syntax pattern for vector binary arithmetic instructions

# Operations returning vector results, masked by vm (v0.t, <nothing>)
vop.vv  vd, vs2, vs1, vm  # integer vector-vector      vd[i] = vs2[i] op vs1[i]
vop.vx  vd, vs2, rs1, vm  # integer vector-scalar      vd[i] = vs2[i] op x[rs1]
vop.vi  vd, vs2, imm, vm  # integer vector-immediate   vd[i] = vs2[i] op imm
```

```
# Assembly syntax pattern for vector ternary arithmetic instructions (multiply-add)

# Integer operations overwriting sum input
vop.vv vd, vs1, vs2, vm  # vd[i] = vs1[i] * vs2[i] + vd[i]
vop.vx vd, rs1, vs2, vm  # vd[i] = x[rs1] * vs2[i] + vd[i]

# Integer operations overwriting product input
vop.vv vd, vs1, vs2, vm  # vd[i] = vs1[i] * vd[i] + vs2[i]
vop.vx vd, rs1, vs2, vm  # vd[i] = x[rs1] * vd[i] + vs2[i]
```

存在一部分会扩大运算结果宽度或者混合宽度的向量指令，其结果可以有：`EEW=2*SEW`，`EMUL=2*LMUL`。

```
# Assembly syntax pattern for vector widening arithmetic instructions

# Double-width result, two single-width sources: 2*SEW = SEW op SEW
vwop.vv  vd, vs2, vs1, vm  # integer vector-vector      vd[i] = vs2[i] op vs1[i]
vwop.vx  vd, vs2, rs1, vm  # integer vector-scalar      vd[i] = vs2[i] op x[rs1]

# Double-width result, first source double-width, second source single-width: 2*SEW = 2*SEW op SEW
vwop.wv  vd, vs2, vs1, vm  # integer vector-vector      vd[i] = vs2[i] op vs1[i]
vwop.wx  vd, vs2, rs1, vm  # integer vector-scalar      vd[i] = vs2[i] op x[rs1]
```

结果产生的向量寄存器组将会以两倍的SEW以及LMUL设置来组织，且EEW以及EMUL的设置必须是指令集所支持的。目标寄存器组对于EMUL必须是有效的，且不能由于不同的EEW设置而导致覆盖了源寄存器组。

无独有偶，同样存在一部分会缩小运算结果宽度的向量指令，其结果可以有使得：`EEW/EMUL=2*SEW/2*LMUL`。若EEW>ELEN或者EMUL>8，则会触发异常。如果指令中存在第二个向量源寄存器组（`vs1`），那么它的元素宽度与结果的元素宽度相同（`EEW=SEW`）。这种类型的指令会在简记的op码中使用`vn*`的前缀来在汇编器中与其他指令进行区分。双倍宽度的源寄存器组会使用`w`作为后缀（如`vnsra.wv`）。



## 向量整型算术指令

### 向量单宽度整型加法/减法

- [x] `vadd.{vv|vx|vi}`：向量-{向量|标量|立即数}加法。
- [x] `vsub.{vv|vx|vi}`：向量-{向量|标量|立即数}减法。
- [ ] `vrsub.{vv|vi}`：向量-{向量|立即数}倒转减法。

#### 简记

```
# Integer adds.
vadd.vv vd, vs2, vs1, vm   # Vector-vector
vadd.vx vd, vs2, rs1, vm   # vector-scalar
vadd.vi vd, vs2, imm, vm   # vector-immediate

# Integer subtract
vsub.vv vd, vs2, vs1, vm   # Vector-vector
vsub.vx vd, vs2, rs1, vm   # vector-scalar

# Integer reverse subtract
vrsub.vx vd, vs2, rs1, vm   # vd[i] = rs1 - vs2[i]
vrsub.vi vd, vs2, imm, vm   # vd[i] = imm - vs2[i]
```

### 向量宽度扩展整型加法/减法

宽度扩展的加法/减法指令提供有符号以及无符号的实现，取决于较小宽度的源操作数是零扩展还是符号扩展来进行运算。

- [ ] `vwaddu.{vv|vx}`：向量-{向量|标量}无符号整型加法，仅结果宽度扩展。
- [ ] `vwsubu.{vv|vx}`：向量-{向量|标量}无符号整型减法，仅结果宽度扩展。
- [ ] `vwadd.{vv|vx}`：向量-{向量|标量}有符号整型加法，仅结果宽度扩展。
- [ ] `vwsub.{vv|vx}`：向量-{向量|标量}有符号整型减法，仅结果宽度扩展。
- [ ] `vwaddu.{wv|wx}`：扩展向量-{向量|标量}无符号整型加法，扩展结果以及`vs2`的宽度。
- [ ] `vwsubu.{wv|wx}`：扩展向量-{向量|标量}无符号整型减法，扩展结果以及`vs2`的宽度。
- [ ] `vwadd.{wv|wx}`：扩展向量-{向量|标量}有符号整型加法，扩展结果以及`vs2`的宽度。
- [ ] `vwsub.{wv|wx}`：扩展向量-{向量|标量}有符号整型减法，扩展结果以及`vs2`的宽度。

### 向量整型扩展

该类指令将EEW小于SEW的源寄存器组中的元素零扩展或者符号扩展到SEW并存放到目的寄存器组中。源寄存器组的EEW可以是目的寄存器组的1/2、1/4或者1/8，且源寄存器组的EMUL为(EEW/SEW)*LMUL。

- [ ] `vzext.vf{2|4|8}`：将{ SEW/2 | SEW/4 | SEW/8 }的源寄存器组元素零扩展到SEW并存放到目的寄存器组中。
- [ ] `vsext.vf{2|4|8}`：将{ SEW/2 | SEW/4 | SEW/8 }的源寄存器组元素符号扩展到SEW并存放到目的寄存器组中。

#### 简记

```
vzext.vf2 vd, vs2, vm  # Zero-extend SEW/2 source to SEW destination
vsext.vf2 vd, vs2, vm  # Sign-extend SEW/2 source to SEW destination
vzext.vf4 vd, vs2, vm  # Zero-extend SEW/4 source to SEW destination
vsext.vf4 vd, vs2, vm  # Sign-extend SEW/4 source to SEW destination
vzext.vf8 vd, vs2, vm  # Zero-extend SEW/8 source to SEW destination
vsext.vf8 vd, vs2, vm  # Sign-extend SEW/8 source to SEW destination
```

### 向量整型带进位加法/带借位减法指令

这类指令的目的是为了实现多字长的整数算术指令，这些指令的实现将会提供一个进位/借位比特。对于每一个运算（加法或者减法），不但会提供SEW宽度的运算结果，还会生成进位/借位输出（单个布尔掩码比特）。进位/借位输入只能来自于`v0`寄存器，而进位/借位输出可以写入到任何一个合法的向量寄存器中。

`vadc`以及`vsbc`将源操作数带进位相加或带借位相减，并将结果写入到目的寄存器`vd`中。这两类指令的`vm`域编码为0。

`vmadc`以及`vmsbc`将源操作数可选带进位相加或者带借位相减，并将结果写入到目的寄存器`vd`中。若`vm=0`则带上进位/借位，若`vm=1`则不带上进位/借位。

- [ ] `vadc.{vvm|vxm|vim}`：向量-{向量|标量|立即数}带进位加法。
- [ ] `vmadc.{vvm|vxm|vim}`：可选模式下向量-{向量|标量|立即数}带进位加法。
- [ ] `vmadc.{vv|vx|vi}`：可选模式下向量-{向量|标量|立即数}带不进位加法。

#### 简记

```
# Produce sum with carry.

# vd[i] = vs2[i] + vs1[i] + v0.mask[i]
vadc.vvm   vd, vs2, vs1, v0  # Vector-vector

# vd[i] = vs2[i] + x[rs1] + v0.mask[i]
vadc.vxm   vd, vs2, rs1, v0  # Vector-scalar

# vd[i] = vs2[i] + imm + v0.mask[i]
vadc.vim   vd, vs2, imm, v0  # Vector-immediate

# Produce carry out in mask register format

# vd.mask[i] = carry_out(vs2[i] + vs1[i] + v0.mask[i])
vmadc.vvm   vd, vs2, vs1, v0  # Vector-vector

# vd.mask[i] = carry_out(vs2[i] + x[rs1] + v0.mask[i])
vmadc.vxm   vd, vs2, rs1, v0  # Vector-scalar

# vd.mask[i] = carry_out(vs2[i] + imm + v0.mask[i])
vmadc.vim   vd, vs2, imm, v0  # Vector-immediate

# vd.mask[i] = carry_out(vs2[i] + vs1[i])
vmadc.vv    vd, vs2, vs1      # Vector-vector, no carry-in

# vd.mask[i] = carry_out(vs2[i] + x[rs1])
vmadc.vx    vd, vs2, rs1      # Vector-scalar, no carry-in

# vd.mask[i] = carry_out(vs2[i] + imm)
vmadc.vi    vd, vs2, imm      # Vector-immediate, no carry-in
```





## 初步的提取

- [x] `vsetvli`
- [x] `vsetvl`

- [x] `vle{EEW}.v`
- [x] `vse{EEW}.v`

- [x] `vlse{EEW}.v`[^1]
- [x] `vsse{EEW}.v`[^1]
- [ ] `vlxei{EEW}.v`[^2]
- [ ] `vsxei{EEW}.v`[^2]
- [ ] `vsuxei{EEW}.v`[^2]
- [ ] `vle{SEW}ff.v`[^3]
- [ ] Vector Load/Store Segment[^4][^5]
- [ ] `vl{nf}r.v`
- [ ] `vs{nf}r.v`

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