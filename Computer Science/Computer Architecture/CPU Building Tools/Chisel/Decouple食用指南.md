Chisel有一个很方便的`Decoupled()`接口可以快速创建用于`ready-valid`形式的IO接口。关于`ready-valid`形式的接口可以参见[这里](http://inst.eecs.berkeley.edu/~cs150/Documents/Interfaces.pdf)。

`Decoupled()`可以将任何Chisel类型转换成`ready-valid`形式的IO接口：

- `Decoupled(...)`创建一个输出端/生产者（Source）的`ready-valid`接口（`bits`是输出端）；
- `Flipped(Decoupled(...))`创建一个输入端/消费者（Sink）的`ready-valid`接口（`bits`是输入端）。

下面给出一个例子：

```Scala
import chisel3._
import chisel3.util.Decoupled

/**
  * Using Decoupled(...) creates a producer interface.
  * i.e. it has bits as an output.
  * This produces the following ports:
  *   input         io_readyValid_ready,
  *   output        io_readyValid_valid,
  *   output [31:0] io_readyValid_bits
  */
class ProducingData extends Module {
  val io = IO(new Bundle {
    val readyValid = Decoupled(UInt(32.W))
  })
  // do something with io.readyValid.ready
  io.readyValid.valid := true.B
  io.readyValid.bits := 5.U
}

/**
  * Using Flipped(Decoupled(...)) creates a consumer interface.
  * i.e. it has bits as an input.
  * This produces the following ports:
  *   output        io_readyValid_ready,
  *   input         io_readyValid_valid,
  *   input  [31:0] io_readyValid_bits
  */
class ConsumingData extends Module {
  val io = IO(new Bundle {
    val readyValid = Flipped(Decoupled(UInt(32.W)))
  })
  io.readyValid.ready := false.B
  // do something with io.readyValid.valid
  // do something with io.readyValid.bits
}
```

而对于复合类型的情况又是如何处理，再看下面的一段例子：

```scala
import Chisel.Decoupled
import chisel3._

class Test_BaseIO extends Bundle{
  val addr = UInt(32.W)
  val len  = UInt(64.W)
}

class Test_IO extends Bundle {
  val base_io = Decoupled(new Test_BaseIO)
}

/**
  * This produces the following ports:
  * input         clock,
  * input         reset,
  * input         io_base_io_ready,
  * output        io_base_io_valid,
  * output [31:0] io_base_io_bits_addr,
  * output [63:0] io_base_io_bits_len
  */
class Test extends Module {
  val io = IO(new Test_IO)

  io.base_io.valid := true.B
  io.base_io.bits.addr := 1.U
  io.base_io.bits.len := 2.U
}
```

`Decoupled()`在连接两个模块的时候作为一种握手协议的接口非常有用。