# SystemVerilog bind的使用

在调试RTL代码的时候，会遇到有存储器的代码。在RTL源码中插入assertion不是一个很好的选择，因此可以使用SV的bind特性来完成这一点。

实际上bind就是一种间接的实例化手段，对于RTL的设计者来说，其设计的模块是否被bind是不清楚的，除非验证的工程师打开生成的波形文件，才能够在被bind的模块中看到其下层的bind模块。

要使用bind特性，通过声明一个新的模块，这个模块的输入和输出和所要bind的模块的输入和输出一致。然后在模块当中书写所需要的测试模块的语句，比如readmemh等。

bind语句的格式：

```Verilog
bind <DUT模块名> <测试模块名> <实例化的模块名> ([connect语句，和模块实例化一致]);
```

## readmemh/readmemb

$readmemh("hex_memory_file.mem", memory_array, [start_address], [end_address])

$readmemb("bin_memory_file.mem", memory_array, [start_address], [end_address])

其中`memory_array`可以是Verilog中的`reg[n:0]`等存储器类型。
