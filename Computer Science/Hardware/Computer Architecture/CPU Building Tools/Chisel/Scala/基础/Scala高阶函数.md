# Scala高阶函数

高阶函数是函数式编程里面一个非常重要的特色，所谓的高阶函数，就是以其他函数作为参数的函数。

## 概览

首先查看一段Scala代码：

```Scala
object HigherOrderFunction {
    def main(args: Array[String]): Unit = {
        println(sum1(1, 3))
        println(sum2(1, 3))
        println(sum3(1, 3))
    }

    def sum1(a: Int, b: Int): Int = 
        if(a > b)   0;
        else a + sum1(a + 1, b);

    def sum2(a: Int, b: Int): Int = 
        if(a > b)   0;
        else cube(a) + sum2(a + 1, b);

    def sum3(a: Int, b: Int): Int = 
        if(a > b)   0;
        else fac(a) + sum3(a + 1, b);

    def fac(a: Int): Int = 
        if(a == 0)  1;
        else a * fac(a - 1);

    def cube(a: Int): Int = a * a * a;
}
```

分析函数sum1、sum2以及sum3的代码，很容易发现这三个函数有着相似的模式，显然着三个函数的唯一区别在于else语句中对a的处理。在函数式编程里面，可以利用这部分相似的模式来精简代码：

```Scala
object HigherOrderFunction {
    def main(args: Array[String]): Unit = {
        println(sumInt(1, 3));
        println(sumCube(1, 3));
        println(sumFac(1, 3));
    }

    def sum(f: Int => Int, a: Int, b: Int): Int = 
        if(a > b)   0;
        else f(a) + sum(f, a + 1, b);

    def id(a: Int): Int = a;
    def cube(a: Int): Int = a * a * a;
    def fac(a: Int): Int = 
        if(a == 0)  1;
        else a * fac(a - 1);

    def sumInt(a: Int, b: Int): Int = sum(id, a, b);
    def sumCube(a: Int, b: Int): Int = sum(cube, a, b);
    def sumFac(a: Int, b: Int): Int = sum(fac, a, b);
}
```

sum函数的参数列表中，第一个参数f，类型是"Int => Int"。说明这个参数是一个函数，且这个函数的输入的是一个Int类型的对象，返回的是一个Int类型的对象。从而可以套用不同的对a的处理方式的函数，来达到精简代码的目的。
