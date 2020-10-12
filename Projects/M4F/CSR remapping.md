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
 31							20 19			15 14		 12	11		 7 6					 0
|				csr				|		rs1		| funct3 |	 rd 	|  	opcode  	|
		 ex_opcode		source/uimm					  dest
其中，csr具体的域为：
 31       28 27			23 22 20
|  rm_funct | 	rs2		|ex_op|
其中，rm_funct={ 0x2, 0x4, 0x6, 0x8, 0xA, 0xD }
```

此时`rs1`、`rs2`、`rd`均可以在单条指令中完整表示，且可以新增编码96条新的指令。



# CMSIS的DSP API

## API简要介绍

### 基础数学函数（Basic Math Functions）

#### 向量绝对值（Vector Absolute Value）





