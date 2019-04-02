# FIRRTL Specification

基于FIRRTL标准文档的相关笔记记录。https://github.com/freechipsproject/firrtl/blob/master/spec/spec.pdf

Firrtl是一种专门用于数字电路的高层次灵活的中间代码形式RTL（Flexible Internal Representation for RTL）。

Firrtl将数字电路表示成一个AST（abstract syntax tree）的形式。

# Circuit以及Modules

## Circuits

所有的FIRRTL的circuits都包含一个模块的列表。circuits是AST的根结点，所有的FIRRTL表示的数字电路都只有一个circuit结点。同时，circuit的名字一般来说就是最顶层模块的名字。

```
circuit MyTop : 
    module MyTop :
    ...
    module MyModule :
    ...
```

## Modules

每个模块都有一个给定的名字，一个端口的列表以及语句（块）来表示模块内的电路连接。端口带有方向、名字以及该端口的数据类型。下面这个简单的模块表示一个模块只有一个输入和输出端口，并且模块内直接将输入输出端口相连：

```
module MyModule :
    input foo : UInt
    output bar : UInt
    bar <= foo
```

端口声明的一般格式为：

<方向> <名字> : <类型>

需要注意的是，一个模块的定义并不代表这个模块一定会在最终的电路上物理生成。

## Externally Defined Modules

外部定义的模块必须包含一个名字以及一个端口的列表，这些端口的名字和类型需要和外部定义一致。

# Types

FIRRTL中所有的类型要不是基本类型或者是由基本类型聚合而成的组合类型。

## Ground Types

### Integer Types

无符号以及有符号的整数类型，都可以选择带有一个指定的位宽：

```
UInt<10>
SInt<32>
```

如果没有指定位宽，那么会在FIRRTL的位宽推断中自动生成：

```
UInt
SInt
```

这部分的细节将在后文阐述。

### Clock Type

时钟类型用来描述传递时钟信号的wires或者ports。时钟信号不适用于大多数的操作，并且时钟信号连接到已经声明为时钟类型的部件上。

时钟类型的声明如下：

```
Clock
```

## Vector Types

向量类型用来表示一个给定类型的元素序列。序列的长度必须是非负且声明已知的。

下面是一个声明为10个单位长度的16位无符号整数的序列：

```
UInt<16>[10]
```

下面的例子是一个声明为10个单位长度的，但没有指定位数的无符号整数的序列：

```
UInt[10]
```

当然，聚合的类型同样可以作为向量中的元素。下面表示的是一个长度为20个单位的向量，其中每个元素是一个长度为10个单位的16位无符号整数的序列。

```
UInt<16>[10][20]
```

## Bundle Types

一个bundle类型的数据用来表示一系列嵌套的以及命名了的类型。比如下面是一个用bundle描述一个复数的例子。它含有两个域：real以及imag，都是10位的有符号整数。

```
{real:SInt<10>, imag:SInt<10>}
```

此外，bundle中某些字段可以声明为 filp ：

```
{word:UInt<32>, valid:UInt<1>, filp ready:UInt<1>}
```

电路模块之间，如果连接的对象是bundle，那么bundle中声明为flip的端口可以反向传输数据，下面举一个例子来说明。

考虑一个模块的输出定义为：

```
output a: {word:UInt<32>, valid:UInt<1>, flip ready:UInt<1>}
```

如果a端口和另外一个模块的相同端口相连，那么word以及valid的数据流方向是从端口内传输到端口外，但是ready的数据流方向则相反，**从端口外传输到端口内**。如果是 filp 声明的域当中内嵌了另一个 flip 声明的域，那么直接做翻转运算即可。（类似负负得正）

## Passive Types

对于一些电路中的模块来说，声明一个类型的端口允许数据流双向流动，是不太合适的。直观上来说，一个被动类型是指一种类型的对象，其所有的数据流动都是单向的。如果一个对象是被动类型的，则其包含的所有基本类型的对象都是被动类型。如果向量中的元素是被动类型，则这个向量也是被动类型。
