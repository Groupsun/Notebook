# RISC-V Privileged指令集中对CSR的各类定义

## 总览

对CSRs寄存器进行定义的地址空间一共有**12**位`csr[11:0]`，也就是可以定义最大**4096**个CSR寄存器。同时，指令集中还有部分通用的细节定义如下：

- 高4位`csr[11:8]`定义存取CSR的行为
  - 高2位`csr[11:10]`定义读写（`00`、`01`或`10`）或者只读`11`。
  - 接下来的2位`csr[9:8]`定义最低的特权级是否可以访问该CSR。该两位实际上就是定义该CSR使用的特权等级：`00`、`01`、`10`、`11`分别代表了用户级、特权级、Hypervisor级以及机器级。

## CSR指令

使用两条写CSR的指令，即`CSRRW`以及`CSRRWI`。指令的格式为：

```
 31							20 19			15 14		 12	11		 7 6					 0
|				csr				|		rs1		| funct3 |	 rd 	|  	opcode  	|
		 csr addr		  source/uimm					  dest
```

可以发现，还需要一个源寄存器`rs2`的定义，这部分考虑从CSRs地址范围当中抽取。

## CSR地址范围定义

- 用户级CSRs：`XX00XXXXXXXX`
- 特权级CSRs：`XX01XXXXXXXX`
- Hypervisor CSRs：`XX10XXXXXXXX`
- 机器级CSRs：`XX11XXXXXXXX`

在该remapping的方案中，选择使用用户级CSRs的地址进行remapping。已知当前已经使用了的用户级CSRs地址有23个：

- `0x000`~`0x005`：6个，用于Trap Setup以及浮点CSRs
- `0x040`~`0x044`：5个，用于Trap Handling
- `0xC00`~`0xC04`、`0xC1F`~`0xC84`、`0xC9F`：12个，用户计数器等

由于重构方案中，需要写CSRs指令，因此还需要排除只读的CSRs地址，共256个。

因此，在用户级的CSRs地址中，剩余的可以使用的空余地址有：745个。

通过分析，可以发现，CSR地址中高4位为`0x4`以及`0x8`的地址空间完全没有被使用。这里提供了8位的空间给我们使用，此时如果把5位的`rs2`也匀进来的话，那么就只能定义16个`ex_opcode`了。

但进一步来说，需求上应该不需要实现Hypervisor，因此还可以新增高4位为`0x2`、`0x6`、`0xA`、`0xD`的地址空间进行编码。

最终敲定的方案中，可供新增编码的`ex_op`数量为96个。

## Remapping方案

将所有信息集中在单条指令中完成：

```
-------------------------------------------------------------
 31							20 19			15 14		 12	11		 7 6					 0
|				csr				|		rs1		| funct3 |	 rd 	|  	opcode  	|
		 ex_opcode		source/uimm					  dest
-------------------------------------------------------------
其中，csr具体的域为：
-------------------------------------------------------------
 31       28 27			23 22 20
|  rm_funct | 	rs2		|ex_op|
-------------------------------------------------------------
其中，rm_funct={ 0x2, 0x4, 0x6, 0x8, 0xA, 0xD }
```

此时`rs1`、`rs2`、`rd`均可以在单条指令中完整表示，且可以新增编码96条新的指令。

# CMSIS的DSP API

## API简要介绍

### 数据类型表

函数名后缀用于API名称后缀使用，表示该API操纵的数据类型，即：`<func_name>_<ts>`。如，向量绝对值：`<func_name> = arm_abs`，`<ts> = { f32, q15, q31, q7 } `。

数据类型名称用于表示参数的类型。

|   数据类型   | 简称/函数名后缀(ts) | 数据类型名称(tn) |
| :----------: | :-----------------: | :--------------: |
| 单精度浮点数 |         f32         |   `float32_t`    |
|    Q1.15     |         q15         |     `q15_t`      |
|    Q1.31     |         q31         |     `q31_t`      |
|     Q1.7     |         q7          |      `q7_t`      |
|    Q1.63     |         q63         |     `q63_t`      |
|  无符号半字  |         u16         |    `uint16_t`    |
|   无符号字   |         u32         |    `uint32_t`    |
|  无符号字节  |         u8          |    `uint8_t`     |

#### Q格式

一种定点的二进制小数表示法。CMSIS中的`q15_t`、`q31_t`以及`q7_t`都是这种类型的定点小数。Q格式一般会指定小数位的位数，也就是Q后的数字。有些亦会指定整数位的个数，如Q1.15表示1位整数15位小数的表示方式。CMSIS中均采用后者，也就是说其支持的表示方式为：Q1.15、Q1.31以及Q1.7。

### Basic Math Functions

| 名称                        | 函数名称     | 参数                                                         | 支持数据类型      |
| --------------------------- | ------------ | ------------------------------------------------------------ | ----------------- |
| Vector Absolute Value       | arm_abs      | 源地址`pSrc`、目的地址`pDst`、向量长度`blockSize`[^1]        | f32, q15, q31, q7 |
| Vector Addition             | arm_add      | 源地址`pSrcA和pSrcB`、目的地址`pDst`、向量长度`blockSize`    | f32, q15, q31, q7 |
| Vector bitwise AND          | arm_and      | 源地址`pSrcA和pSrcB`、目的地址`pDst`、向量长度`blockSize`    | u16, u32, u8      |
| Vector Dot Product          | arm_dot_prod | 源地址`pSrcA和pSrcB`、目的地址`result`[^2]、向量长度`blockSize` | f32, q15, q31, q7 |
| Vector Multiplication       | arm_mult     | 源地址`pSrcA和pSrcB`、目的地址`pDst`、向量长度`blockSize`    | f32, q15, q31, q7 |
| Vector Negate               | arm_negate   | 源地址`pSrc`、目的地址`pDst`、向量长度`blockSize`            | f32, q15, q31, q7 |
| Vector bitwise NOT          | arm_not      | 源地址`pSrc`、目的地址`pDst`、向量长度`blockSize`            | u16, u32, u8      |
| Vector Offset               | arm_offset   | 源地址`pSrc`、目的地址`pDst`、向量长度`blockSize`            | f32, q15, q31, q7 |
| Vector bitwise inclusive OR | arm_or       | 源地址`pSrcA和pSrcB`、目的地址`pDst`、向量长度`blockSize`    | u16, u32, u8      |
| Vector Scale                | arm_scale    | 源地址`pSrc`、目的地址`pDst`、向量长度`blockSize`；若数据类型为f32，则还有一个参数，类型为`float32_t`的缩放因子`scale`；若数据类型为q7、q31或q15，则还有两个参数：相同数据类型的缩放因子`scaleFract`以及类型为`int8_t`的移位位数`shift`[^3]。 | f32, q15, q31, q7 |
| Vector Shift                | arm_shift    | 源地址`pSrcA和pSrcB`、目的地址`pDst`、向量长度`blockSize`、类型为`int8_t`的移位位数`shiftBits`[^3]。 | q15, q31, q7      |
| Vector Subtraction          | arm_sub      | 源地址`pSrcA和pSrcB`、目的地址`pDst`、向量长度`blockSize`    | f32, q15, q31, q7 |
| Vector bitwise inclusive OR | arm_xor      | 源地址`pSrcA和pSrcB`、目的地址`pDst`、向量长度`blockSize`    | u16, u32, u8      |

[^1]: 向量长度blockSize的类型为`uint32_t`
[^2]: 若操纵的数据类型为q31、q15以及q7，则目的地址的指针类型为q63、q63以及q31。
[^3]: 若移位位数为正，则源向量中的元素左移。否则就右移。

#### 行为描述

下面是对每个基础数学函数的API行为的描述：

```
Vector Absolute Value			 : pDst[n] =  abs(pSrc[n]),				  					 0 <= n < blockSize
Vector Addition			 			 : pDst[n] =  pSrcA[n] + pSrcB[n],						 0 <= n < blockSize
Vector bitwise AND	 			 : pDst[n] =  pSrcA[n] & pSrcB[n], 						 0 <= n < blockSize
Vector Dot Product	 			 : result	 =  pSrcA[0]*pSrcB[0] + pSrcA[1]*pSrcB[1] + ... + pSrcA[blockSize-1]*pSrcB[blockSize-1]
Vector Multiplication			 : pDst[n] =  pSrcA[n] * pSrcB[n],						 0 <= n < blockSize
Vector Negate				 			 : pDst[n] = -pSrc[n],						  					 0 <= n < blockSize
Vector bitwise NOT	 			 : pDst[n] = ~pSrc[n],						  					 0 <= n < blockSize
Vector Offset				 			 : pDst[n] =  pSrc[n] + offset,		  					 0 <= n < blockSize
Vector bitwise inclusive OR: pDst[n] =  pSrcA[n] | pSrcB[n],						 0 <= n < blockSize
Vector Scale							 : pDst[n] =  pSrc[n] * scale, 								 0 <= n < blockSize, if ts = f16
														 pDst[n] =  (pSrc[n] * scaleFract) << shift, 0 <= n < blockSize, if ts = {q15|q31|q7}, shift >= 0;
														 pDst[n] =  (pSrc[n] * scaleFract) >> shift, 0 <= n < blockSize, if ts = {q15|q31|q7}, shift < 0.
Vector Shift							 : pDst[n] =  pSrc[n] << shiftBits,						 0 <= n < blockSize, if shiftBits >= 0;
														 pDst[n] =  pSrc[n] >> shiftBits,						 0 <= n < blockSize, if shiftBits < 0;
Vector Subtraction				 : pDst[n] =  pSrcA[n] - pSrcB[n],						 0 <= n < blockSize
Vector bitwise exclusive OR: pDst[n] =  pSrcA[n] ^ pSrcB[n],						 0 <= n < blockSize
```

### Fast Math Functions

这一类的函数提供了快速的正弦、余弦以及平方根运算的**近似**结果。同时，快速还体现在了其运算的对象可以不是向量，而是单个数值。

#### Square Root

```c++
arm_sqrt_<ts> (<tn> in, <tn> *pOut);
arm_vsqrt_<ts> (<tn> *pIn, <tn> *pOut, uint16_t len);
// 支持数据类型：f32, q31, q15
```

有两种类型的平方根运算，`arm_sqre`是对单个数值求平方根，而`arm_vsqrt`是对一个向量中的每个元素都求平方根。

计算一个数值的平方根。使用的算法为牛顿法（Newton-Raphson）:
$$
x_1 = x_0 - f(x_0)/f'(x_0)
$$
其中$x_1$是当前迭代的结果，$x_0$为上一次迭代的结果。对于求平方根来说，目标函数为：$f(x)=x^2-N$，其中$N$为所要求平方根的值。库函数中规定运算迭代为：
$$
x_0 = N/2, \quad [initial \; guess] \\
x_{n+1} = 1/2 * (x_n+N/x_n), \quad [each \; iteration]
$$

#### Cosine/Sine

```C++
arm_cos_<ts> (<tn> x);
// 支持数据类型：f32, q31, q15
```

对于定点数据类型q31以及q15来说，输入的参数`x`需要落在范围[0, +0.9999]之间，并映射到[0, 2*pi)的范围当中。

求余弦值/正弦值的函数的具体实现，使用的查表与线性插值的方法相结合来完成。查找的表是预先定义好的正弦值表，若计算的是余弦值，则通过将输入的参数+pi/2即可以转化为正弦值。计算余弦值/正弦值的步骤如下：

1. 计算最靠近`x`值的下标；
2. 计算`x`值与下标之差的小数部分`fract`；
3. 最终的结果等于：`(1.0f-fract)*a + fract*b`

其中：`a = Table[index]; b = Table[index+1]`

### Complex Math Functions

所有的复数运算相关的API都只支持f32、q15以及q31的数据类型。

复数向量中每个复数需要两个相邻的元素进行存储，一个实部一个虚部。也就是说，复数向量中一共有`2*numSamples`个元素。图示如下：

```
| real | imag | real | imag | ... | real | imag |			length    = 2*numSamples (数组实际大小)
|    cmplx1   |    cmplx2   | ... |    cmplxn   |			cmplx_num = numSamples
```

#### Complex Conjugate

```C++
arm_cmplx_conj_<ts> (const <tn> *pSrc, <tn> *pDst, uint32_t numSamples, <tn> realResult, <tn> imagResult);
```

计算一个复数向量中每个元素的共轭。求共轭的算法表示如下：

```C++
for (n = 0; n < numSamples; n++) {
    pDst[(2*n)  ] =  pSrc[(2*n)  ];    // 实部
    pDst[(2*n)+1] = -pSrc[(2*n)+1];    // 虚部
}
```

#### Complex Dot Product

```C++
arm_cmplx_dot_prod_<ts> (const <tn> *pSrcA, const <tn> *pSrcB, uint32_t numSamples, <tn> realResult, <tn> imagResult);
```

计算两个复数向量的点积（内积），即：
$$
\vec A \cdot \vec B = a_1b_1+a_2b_2+ \cdots + a_nb_n
$$
其中$a_n$以及$b_n$都为复数。

向量存储的方式与上述形式一致不再赘述。计算的实部结果`realResult`以及虚部结果`imagResult`分别存放。算法表示如下：

```C++
realResult = 0;
imagResult = 0;
for (n = 0; n < numSamples; n++) {
    realResult += pSrcA[(2*n)+0] * pSrcB[(2*n)+0] - pSrcA[(2*n)+1] * pSrcB[(2*n)+1];
    imagResult += pSrcA[(2*n)+0] * pSrcB[(2*n)+1] + pSrcA[(2*n)+1] * pSrcB[(2*n)+0];
}
```

计算的最终结果是一个**复数**，表示为：$realResult+imagResult \cdot i$。

#### Complex Magnitude/Complex Magnitude Squared

```C++
arm_cmplx_mag_<ts> (const <tn> *pSrc, <tn> *pDst, uint32_t numSamples);						// 模
arm_cmplx_mag_squared_<ts> (const <tn> *pSrc, <tn> *pDst, uint32_t numSamples);		// 模的平方
```

计算复数向量中每个复数的模/模的平方，并存储到结果向量中。算法表示如下：

```C++
// magnitude
for (n = 0; n < numSamples; n++) {
    pDst[n] = sqrt(pSrc[(2*n)+0]^2 + pSrc[(2*n)+1]^2);
}

// magnitude complex
for (n = 0; n < numSamples; n++) {
    pDst[n] = pSrc[(2*n)+0]^2 + pSrc[(2*n)+1]^2;
}
```

#### Complex-by-Complex Multiplication

```C++
arm_cmplx_multi_cmplx_<ts> (const <tn> *pSrcA, const <tn> *pSrcB, <tn> *pDst, uint32_t numSamples);
```

计算两个复数向量之间每个对应元素之间的乘积，结果是一个**等长的复数向量**，即：
$$
c_i=a_ib_i
$$
其中复数$c_i$是结果向量$\vec C$的元素。算法表示如下：

```C++
for (n = 0; n < numSamples; n++) {
    pDst[(2*n)+0] = pSrcA[(2*n)+0] * pSrcB[(2*n)+0] - pSrcA[(2*n)+1] * pSrcB[(2*n)+1];
    pDst[(2*n)+1] = pSrcA[(2*n)+0] * pSrcB[(2*n)+1] + pSrcA[(2*n)+1] * pSrcB[(2*n)+0];
}
```

#### Complex-by-Real Multiplication

```C++
arm_cmplx_mult_real_<ts> (const <tn> *pSrcCmplx, const <tn><tn> *pSrcReal, <tn> *pCmplxDst, uint32_t numSamples);
```

计算一个复数向量和一个实数向量每个对应元素之间的乘积，结果是一个**等长的复数向量**。算法表示如下：

```C++
for (n = 0; n < numSamples; n++) {
    pCmplxDst[(2*n)+0] = pSrcCmplx[(2*n)+0] * pSrcReal[n];
    pCmplxDst[(2*n)+1] = pSrcCmplx[(2*n)+1] * pSrcReal[n];
}
```

### Filtering Functions

#### High Precision Q31 Biquad Cascade Filter

```C++
void 	arm_biquad_cas_df1_32x64_init_q31 (arm_biquad_cas_df1_32x64_ins_q31 *S, uint8_t numStages, const q31_t *pCoeffs, q63_t *pState, uint8_t postShift);			// 初始化函数

void 	arm_biquad_cas_df1_32x64_q31 (const arm_biquad_cas_df1_32x64_ins_q31 *S, const q31_t *pSrc, q31_t *pDst, uint32_t blockSize);														 // 运算函数
```

该函数用于实现一个高精度的双二阶级联滤波器。滤波器的系数（coefficients）为Q1.31类型，而状态变数（state variables）则为Q1.63类型。滤波器的输入和输出分别是`pSrc`和`pDst`指针指向的Q31数组。

##### 实现算法

每个级联的双二阶滤波器使用以下的二阶微分方程来进行计算：
$$
y_n=b_0*x_n+b1*x_{n-1}+b2*x_{n-1}+a_1*y_{n-1}+a_2*y_{n-2}
$$
滤波器中每一级使用了5个系数以及4个状态变数，如图所示：

![Biquad Filter Form I](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/Biquad.gif)

由于系数`a1`以及`a2`都是负反馈的系数，因此在CMSIS DSP库当中要求其为负数，即：
$$
y_n=b_0*x_n+b1*x_{n-1}+b2*x_{n-1}-a_1*y_{n-1}-a_2*y_{n-2}
$$
更高阶的滤波器可以通过增加级联的双二阶滤波器的数目来实现。`numStages`用于指定级联的数目。举例来说，如果需要一个8阶的滤波器，则可以指定`numStages=4`。

`pState`指向的是状态变数数组，每个双二阶滤波器都有4个状态变数，在数组内的存储方式为：

```
{x[n-1], x[n-2], y[n-1], y[n-2]}
```

级联多个滤波器时，状态变数按照顺序依次存储。状态变数的数据类型为Q1.63。

初始化函数用于初始化`S`，其用于存储一些用于初始化滤波器的数据结构：

```
arm_biquad_cas_df1_32x64_ins_q31 S1 = {numStages, pState, pCoeffs, postShift};
```

`postShift`是用于将滤波器的系数进行缩放的参数。函数规定系数的字面值范围为：[-1, +1)。通过使用`postShift`可以将系数扩大到超过该范围。若`postShift=1`，最终的结果会左移1位，也就是将所有的系数放大2倍。

#### Biquad Cascade IIR Filters Using Direct Form I Structure

该系列的函数实现的是双二阶级联IIR滤波器的Direct Form I形式，支持数据类型为f32、q15以及q31。其中q15以及q31具有快速运算的变体函数。由于函数数量较多，故不在此处一一列举。

由于实现的同样是双二阶滤波器，因此与上述的实现形式相同，故也不再进行赘述。与上述高精度的实现不同，该系列函数的实现，滤波器系数以及状态变数的数据类型都与所要处理的数据类型相同（精度相同）。

对于f32类型来说，初始化滤波器的数据结构与定点数不同：

```C++
arm_biquad_casd_df1_inst_f32 S1 = {numStages, pState, pCoeffs};
```

快速运算的版本使用了一个32位的Q2.30类型累加器。这个累加器保留着所有中间乘法步骤的结果的所有精度，但只提供了一个警戒位（guard bit）。因此当累加器溢出时，会将结果进行舍入，对结果产生一定的误差。输入的数据范围必须限制在[-0.25, +0.25)的范围中。最后累加器中的数据会截取Q1.15的数据长度输出，也就是说会舍弃掉低16位。

对于非快速运算的版本，累加器的数据类型由函数所要操纵的数据类型决定。累加器固定是64位，如果操纵的数据是q31，那么累加器的数据类型为Q2.62。如果操纵的数据是q15，那么累加器的数据类型为Q34.40。

#### Biquad Cascade IIR Filters Using a Direct Form II Transposed Structure

该系列的函数实现的双二阶级联IIR滤波器的转置Direct Form II形式，支持的数据类型为f64以及f32。由于函数数量较多，故不在此处一一列举。转置的Direct Form II更适合于浮点数的处理。

Direct Form II的计算形式如下：

```
y[n] = b0 * x[n] + d1
d1 = b1 * x[n] + a1 * y[n] + d2
d2 = b2 * x[n] + a2 * y[n]
```

其中`d1`以及`d2`是两个状态变数。图示为：

![BiquadDF2Transposed.gif](https://upic-groupsun.oss-cn-shenzhen.aliyuncs.com/uPic/BiquadDF2Transposed.gif)

单个滤波器当中，前向系数为`b0`、`b1`以及`b2`，后向的系数为`a1`以及`a2`。在CMSIS DSP当中，规定`a1`以及`a2`必须是负数：

```
y[n] = b0 * x[n] + d1;
d1 = b1 * x[n] - a1 * y[n] + d2;
d2 = b2 * x[n] - a2 * y[n];
```

在Direct Form II形式当中，`pState`数组的存放方式与Form I有区别：

```
{d11, d12, d21, d22, ...}
```

由于Direct Form II只支持浮点数，因此初始化的数据结构为：

```C++
arm_biquad_cascade_df2T_instance_f64 S1 = {numStages, pState, pCoeffs};
arm_biquad_cascade_df2T_instance_f32 S1 = {numStages, pState, pCoeffs};
```

#### Convolution

CMSIS DSP库提供了支持数据类型q7、q15、q31以及f32的卷积运算函数。同时，库中还提供了q15以及q31数据类型快速运算以及优化的版本。由于函数数量太多，这里只列举一个模版的版本：

```C++
void 	arm_conv_q15 (const q15_t *pSrcA, uint32_t srcALen, const q15_t *pSrcB, uint32_t srcBLen, q15_t *pDst);		// q15模版版本
```

卷积算法的计算很简洁，`srcALen`以及`srcBLen`分别代表两个向量（或信号样本）`a[n]`以及`b[n]`。两个向量的卷积定义如下：
$$
c[n]=\sum_{k=0}^{srcALen} a[k]b[n-k]
$$








