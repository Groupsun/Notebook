# 使用PeekPokeTester调试Chisel程序

## 需要导入的包

```scala
import chisel3._
import chisel3.util._
import chisel3.iotesters._
import org.scalatest.{Matchers, FlatSpec}
```

## 需要测试的设备

在这一部分的讨论当中，我们将会专注于需要测试的设备（Device Under Test, DUT）电路的IO口。

```scala
object RealGCD2 {
    val num_width = 16
}

class RealGCD2Input extends Bundle {
    val a = Bits(width = RealGCD2.num_width)
    val b = Bits(width = RealGCD2.num_width)
}

class RealGCD2 extends Module {
    val io  = new Bundle {
        val in  = Decoupled(new RealGCD2Input()).flip()
        val out = Valid(UInt(width = RealGCD2.num_width))
}
```

## 测试所要使用的工具接口

测试工具允许4种方式来与DUT进行交互：

1. 设定DUT的输入端口值：**poke**
2. 获取DUT的输出端口值：**peek**
3. 测试其中一个DUT的输出端口值：**expect**
4. 使DUT运行n个时钟周期：**step**，step(1)即运行一个时钟周期。
