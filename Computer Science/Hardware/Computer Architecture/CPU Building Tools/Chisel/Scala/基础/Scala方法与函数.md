# Scala方法与函数

方法与函数在Scala中语义上的区别很小。Scala方法是类的一部分，而函数是一个对象可以赋值给一个变量。在类中定义的函数即是方法。Scala中的方法跟Java类似，方法是组成类的一部分。而函数则是一个完整的对象，实际上Scala中的函数就是继承了Trait的类的对象。

Scala中使用 **val** 语句定义函数，使用 **def** 语句定义方法。

## 方法

### 方法声明

Scala方法声明格式如下：

```Scala
def functionName ([参数列表]) : [返回类型]
```

如果不写'='和方法主体，那么方法会被隐式声明为**抽象**的，包含它的类型也是一个抽象类型。

### 方法定义

Scala方法定义格式如下：

```Scala
def functionName ([参数列表]) : [返回类型] = {
    funtion body;
    return [expr];
}
```

参数列表中的参数使用逗号分隔。如果方法没有返回值，可以返回为**Unit**，类似于Java的**void**。

### 方法调用

```Scala
functionName(参数列表);
[instance.]funtionName(参数列表);
```

## 函数

### Scala函数传名调用

Scala的解释器在解析函数参数时有两种方式：
- 传值调用：先计算参数表达式的值，再应用到函数内部。
- 传名调用：将未计算的参数表达式直接应用到函数内部。

在函数的参数列表中，使用'=>'来设置传名调用，如下例所示：

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        delay(time());
    }

    def time(): Long = {
        println("Get time, the unit is nano");
        return System.nanoTime;
    }

    def delay(t: => Long): Unit = {
        println("Inside the delay()");
        println("parameter: " + t);
        t;
    }
}
```

传名调用使得进入delay函数内部的t也时time()，并在在delay内部进行运算。结果如下：

```
Inside the delay()
Get time, the unit is nano
parameter: 362455574258567
Get time, the unit is nano
```

### Scala指定函数参数名

一般情况下调用函数，就按照函数定义时的参数顺序一个个传递，但是也可以通过指定函数参数名，并且不需要按照顺序向函数传递参数（与Verilog相似），实例如下：

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        printInt(b = 100, a = 10);
    }

    def printInt(a: Int, b: Int): Unit = {
        println(a);
        println(b);
    }
}
```

### Scala函数 - 可变参数

Scala允许指明函数的**最后一个参数**是重复的，可以向函数传入可变长度参数列表。Scala通过在参数的类型之后放一个星号来设置可变参数。例子如下：

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        printStrings("Tom", "Clancy's", "The", "Division");
    }

    def printStrings(args: String*): Unit = {
        var i : Int = 0;
        for(arg <- args) {
            println("args[" + i + "] = " + arg);
            i = i + 1;
        }
    }
}
```

输出结果如下：

```
args[0] = Tom
args[1] = Clancy's
args[2] = The
args[3] = Division
```

### 默认函数值

与C/C++相似，Scala可以为函数参数指定默认函数值，使用了默认参数，可以在调用函数的过程中不需要传递参数，这时函数就会调用它的默认参数值，如果传递了参数，则传递值会取代默认值：

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        println(addNumbers());
    }

    def addNumbers(a: Int = 1, b: Int = 2): Int = {
        return a + b;
    }
}
```

### Scala函数嵌套

Scala允许函数内定义函数，定义在函数内的函数称之为局部函数。

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        println(factorial(5));
    }

    def factorial(i: Int): Int = {
        def fact(i: Int, acc: Int): Int = {
            if(i == 1)
                return acc;
            else return fact(i-1, i * acc);
        }
        fact(i, 1);
    }
}
```

### Scala高阶函数

高阶函数(Higher-Order Function)就是操作其他函数的函数。高阶函数可以使用其他函数作为参数，或者使用函数作为输出结果。

由于高阶函数比较复杂，因此以专门一个页面来进行说明。

### Scala匿名函数

Scala允许定义匿名函数，下面的表达式定义了一个接受Int类型输入参数的匿名函数：

```Scala
var inc = (x: Int) => x + 1;
```

### 偏应用函数

Scala偏应用函数是一种表达式，不需要提供函数需要的所有参数，只需要提供部分或不提供所需参数。如下例：

```Scala
import java.util.Date;

object Test {
    def main(args: Array[String]): Unit = {
        val date = new Date;
        val logWithDateBound = log(date, _: String);

        logWithDateBound("test1");
        Thread.sleep(1000);
        logWithDateBound("test2");
        Thread.sleep(1000);
        logWithDateBound("test3");
        Thread.sleep(1000);
    }

    def log(date: Date, message: String): Unit = {
        println(date + " --- " + message);
    }
}
```

### Scala函数柯里化（Currying）

柯里化指的是将原来接受两个参数的函数变成新的接受一个参数的函数的过程。新的函数返回一个以原有第二个参数为参数的函数。下面给出一个实例说明这个过程：

首先定义一个函数：

```Scala
def add(x: Int, y: Int) = x + y;
```

将函数变形：

```Scala
def add(x: Int)(y: Int) = x + y;
```

则在调用这个函数的时候，使用这样的调用方式：

```Scala
add(1)(2)
```

实际上，上式是依次调用了两个普通的函数，第一次调用使用一个参数x，返回第一个函数类型的值，第二次使用参数y调用这个函数类型的值。可以看作是演变成了这样的一种方法：

```Scala
def add(x: Int) = (y: Int) => x + y;
```

下面给出一个完整的实例：

```Scala
import java.util.Date;

object Test {
    def main(args: Array[String]): Unit = {
        println(strcat("Tom")(" Clancy's"));
    }

    def strcat(s1: String)(s2: String): String = {
        return s1 + s2;
    }
}
```
