# Classes of Parallelism and Parallel Architecutures

在不同层次上的并行计算是现今计算机设计的主流驱动力。并行计算可以分为基本的两类：

1. 数据级并行计算，Data-Level Parallelism(DLP)
2. 任务级并行计算，Task-Level Parallelism(TLP)

在计算机硬件当中，这两种基本的并行计算类别可以细分为4类：

1. 指令级并行计算（Instruction-Level Parallelism，ILP）。在编译器的帮助之下，在较低的层次上实现数据级并行计算如流水线。在中等层次上实现数据级并行计算如投机执行（如分支预测）。
2. 向量架构（Vector Architectures）和图像处理单元（Graphic Processor Units，GPU）通过将单条指令在一个数据集合中执行来实现数据级并行计算。
3. 线程级并行计算（Thread-Level Parallelism）通过在一个紧密耦合的硬件架构上，允许并行线程间的交互来实现数据级以及任务级的并行计算。
4. 请求级并行计算（Request-Level Parallelism）通过大型的操作系统或者程序员定义的解耦任务并行执行来实现并行计算。

Michael J. Flynn在60年代提出的将所有的计算机体系结构划分为4种类型：

1. Single instruction stream, single data stream（SISD），单指令流单数据流结构。也就是单处理器结构，在程序员的可见的范围内，它是标准的顺序执行处理器，但是它可以通过使用超标量（superscalar）或者投机执行（speculative）模型来实现ILP。
2. Single instruction stream, multiple data streams（SIMD），单指令流多数据流结构。在GPU以及向量结构中使用，在这种体系结构的模型下，有多个处理器，每个处理器有自己独立的存储单元，但是只有一个指令存储器以及一个控制处理器，用于取指以及译码。译码的指令发送到每个处理器当中对多个数据流同时执行，实现了DLP。
3. Multiple instruction streams, singal data stream（MISD），多指令流单数据流结构。这种结构从未在商业上实现过，但是存在这种体系结构的概念。
4. Multiple instruction streams, multiple data streams（MIMD），多指令流多数据流结构。在这种体系结构的模型下，有多个处理器，每个处理器都会进行独立的取指译码执行等操作。一般来说MIMD要比SIMD更灵活，但是MIMD要比SIMD成本高，并且开销更大。紧密耦合的MIMD架构实现线程级并行计算，这种架构中多个合作的线程并行执行。松弛耦合的MIMD架构实现请求级并行计算，如集群、仓库级计算机，它们中会执行很多独立的任务，并且这些任务之间很少有交流或者同步。

来源：Computer Architecture: A Quantitative Approach, 1.2 Classes of Computers.