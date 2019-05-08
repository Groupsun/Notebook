```
/*
 * ** : 可能的类实现选择
 * ()   : 可选项
 * [[]]...  : 可能有多个
 **/

circuit = circuit id : (info), [[*module*]]...
    module = module id : (info), [[*port*]]... [[*stmt*]]
        port = dir id : *type* (info)
            dir  = input | output
            type = UInt(<int>)      
                   SInt(<int>)
                   Clock
                   {[[*field*]]...}         /* <- Bundle */
                   type[int]                /* <- Vector */
                field = (flip) id : type
        stmt = wire id : type (info)
               reg id : type *exp* (with: {reset => (*exp*, *exp*)}) (info)
               mem id : (info) (
                   data-type => type
                   depth => int
                   read-latency => int
                   write-latency => int
                   read-under-write => ruw
                   [[reader => id]]...
                   [[writer => id]]...
                   [[readwriter => id]]...
               )
               inst id of id (info)         /* <- Module instance */
               node id = *exp* (info)
               *exp* <= exp (info)
               *exp* is invalid (info)
               when *exp* : (info) stmt (else : stmt)
               stop(*exp*, *exp*, int) (info)
               printf(*exp*, *exp*, string, [[*exp*]]...) (info)
               skip (info)
               ([[stmt]]...)
               ruw  = old | new | undefined
               info = @[string]
               exp  = UInt(<int>)(int)
                      UInt(<int>)(string)
                      SInt(<int>)(int)
                      SInt(<int>)(string)
                      id
                      exp.id
                      exp[int]
                      exp[exp]
                      mux(exp, exp, exp)
                      validif(exp, exp)
                      *primop*([[exp]]..., [[int]]...)
                      primop = add | sub | mul | div | mod | lt  | leq  | 
                               gt  | geq | eq  | neq | pad | asUInt     |
                               asSInt    | asClock   | shl | shr | dshl |
                               dshr      | cvt | neg | not | and | or   |
                               xor       | andr      | orr | xorr       |
                               cat | bits      | head      | tail       |
```

可能的需要实现成的类（后续会不断扩展、修改）：

circuit, module, port, stat, type, field, exp, primop。

两大类：**定义（声明）**与**语句**。

## 定义

- Circuit: 名字name(string)，源信息info(string)，一系列的模块modules(List[module])，内核编号id(instanceId)
- Module: 名字name(string)，源信息info(string)，一系列的端口ports(List[port])，语句块，内核编号id(instanceId)
- Port: 名字name(string)，源信息info(string)，方向direction(input|output)，类型(type)，内核编号id(instanceId)
- Field: 是否翻转is_flip(bool)，名字name(string)，类型(type)，内核编号id(instanceId)
- DefStat: 
    - DefWire: 名字name(string)，内核编号id(instanceId)，源信息info(string)
    - DefReg: 名字name(string)，内核编号id(instanceId)，源信息info(string)
    - DefRegInit: 名字name(string)，内核编号id(instanceId)，源信息info(string)，重置信号reset_signal(exp)，重置值reset_value(exp)
    - [DefMem: 名字name(string)，内核编号id(instanceId)，源信息info(string)，数据类型data-type(type)，大小depth(int)，读延迟read_latency(int)，写延迟write_latency(int)，RUW ruw(old|new|undefined)。]（FIRRTL文档没有即时更新，Chisel3会编译为cmem以及smem。）
    - DefNode: 名字name(string)，内核编号id(instanceId)，源信息info(string)，表达式node_exp(exp)
    - InstModule: 名字name(string)，内核编号id(instanceId)，源信息info(string)，实例化的模块内核编号id(instanceId)

关于chisel3编译成FIRRTL中的memory问题。chisel3有两种存储声明：Mem（异步/组合逻辑读）以及SyncReadMem（同步/时序逻辑读）。Mem在FIRRTL中会被编译为cmem，而SyncReadMem会被编译为smem。现在所观察得到的语法形式是：

```firrtl
cmem/smem id : type<width>[size]
```

Chisel中两个类都有read以及write方法来进行读写：

```firrtl
write mport v1 = memory[addr1], clock
read mport v2 = memory[addr2], clock
```

从这里观察得到的是，v1以及v2是对所要读写的存储位置的引用。但是write方法还有一个掩码写的重载方法。

*所有的定义都有名字字段，为了后续扩展方面，每个定义的类的对象都有其id*。

## 语句

- statcmd: 
    - Connect: 源信息info(string)，左表达式lexp(exp)，右表达式rexp(exp)。
    - IsInvalid: 源信息info(string)，表达式invad_exp(exp)。
    - WhenBegin: 源信息info(string)，条件表达式con_exp(exp)。
    - WhenEnd: 源信息info(string)，深度depth(int)，是否有else分支hasAlt(bool)。
    - AltBegin: 源信息info(string)。
    - OtherwiseEnd: 源信息info(string)，深度depth(int)。
    - Stop: 源信息info(string)，时钟信号clock_exp(exp)，停机条件信号con_exp(exp)，退出码exit_code(int)。
    - Printf: 源信息info(string)，时钟信号clock_exp(exp)，条件信号con_exp(exp)，输出的字符串str(string)，对应的表达式列表vars(List[exp])
    - Skip: 源信息info(string)

## Type、InstanceId、Exp、PrimOp

类型是比较特殊的，它另外归为一类：

- Type: 类型名字name(string)，聚合/简单agg_type(aggregate/element)，位宽width(int)，大小size(仅聚合类型，int)，字段fields(仅聚合类型， field)

除此之外还有每个声明的实例对象的id。id不是简单的序列，它还需要有标识父子关系的作用（该对象属于哪个上级对象？）。

- InstanceId: 内核编号_id(long)，父对象编号parent_id(instanceID)。

表达式类Exp有多种子类：

- LitInt: 字面整型类
    - LitUInt: 字面无符号整型类
    - LitSInt: 字面有符号整型类
- Ref:
    - RefId: 引用
    - RefSubfield: 引用子字段
    - RefSubindex: 引用下标
    - RefSubaccess: 引用访问
- Mux: 多路选择器
- validif: 条件有效运算
- Op: 基本运算

所有的表达式都有性别（gender）字段。

PrimOp类，就是基本运算的类。
