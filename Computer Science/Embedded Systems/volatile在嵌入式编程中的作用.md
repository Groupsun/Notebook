# volatile在嵌入式编程中的作用

volatile是C语言中的一个关键字，在嵌入式编程当中比较重要。

volatile关键字的作用是用来防止一些被编译器认为无法被代码改变的变量进行优化。volatile关键字用来提醒编译器它后面所定义的变量随时有可能改变，因此编译后的程序每次需要读写这个变量的时候，都会直接从变量地址中读取（内存地址、外设地址等等）。

考虑下列情景：一个IO口特征值，映射在内存地址中，用于表明该IO口是否已经准备完毕。如果没有将这个变量声明为volatile，那么编译器会认为这个变量在之后的代码中不会被改变（这是正确的，代码没有改变这个变量的逻辑，但是由于外设准备完毕后，就会写入新的值，变量的值被外部设备所改变了），那么就会把这个值复制到寄存器当中。这样的话程序永远也跑不起来了，因为在寄存器中的值将永远是“未准备”。而通过声明volatile，来促使编译器编译的结果，每次尝试读写这个值的时候都会从相应地址中读写，而不是单纯读写寄存器中的值。
