# Scala条件与循环语句

Scala的条件语句：if/if-else/if-elseif-...-else，以及循环语句：while/do-while/for语句基本和Java/C/C++语法一致，唯一不同的只有for语句。

## Scala for循环

### 语法

```Scala
// Range是一个数字区间表示i to j或者i until j
// 左箭头 <- 用于为变量x赋值
var x = 0;
for(x <- Range) {
    statement(s);
}
```

使用for语句时，对条件约束的变量x的赋值必须使用'<-'，后跟的Range的格式是'i to j'或者'i until j'，表示范围为[i,j]以及[i,j)。下面给出一个实例：

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        var x : Int = 0;
        for(x <- 1 to 10) {
            println(x);
        }
    }
}
```

在for循环中可以使用分号来设置多个区间，它将迭代给定区间的所有可能值：

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        var a = 0;
        var b = 0;
        for(a <- 1 to 2; b <- 1 until 3)
            println("a = " + a +" b = " + b);
    }
}
```

如上述例子，会将a以及b的所有可能结果迭代输出（相当于外层a循环中内嵌一个b循环，输出结果为：

```
a = 1 b = 1
a = 1 b = 2
a = 2 b = 1
a = 2 b = 2
```

### for循环集合

```Scala
var x = 0;
for(x <- List) {
    statement(s);
}
```

在上述语法中，List变量是一个集合，for循环会迭代所有集合的元素。例子如下：

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        var x = 0;
        var numlist = List(1, 2, 3, 4, 5);
        for(x <- numlist)
            println("x = " + x);
    }
}
```

### for循环过滤

Scala可以使用一个或者多个if语句来过滤一些元素：

```Scala
var x = 0;
for(var x <- List;
    if condition1; if condition2 ...) {
        statement(s);
    }
```

可以使用分号来为表达式添加一个或多个的过滤条件。下面给出一个例子：

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        var x = 0;
        var numlist = List(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        for(x <- numlist; if x != 2; if x <= 9) {
                println(x);
        }
    }
}
```

### for使用yield

Scala允许将for循环的返回值作为一个变量存储：

```Scala
var retVal = for{
    x <- List; if condition1; if condition2 ...
}yield x
```

yield会把当前的元素记录下来，保存在集合当中，循环结束后将返回该集合。下面给出一个实例：

```Scala
object Test {
    def main(args: Array[String]): Unit = {
        var x = 0;
        var numList = List(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        var retVal = for {
            x <- numList; if x != 5; if x <= 7
        }yield x
        for(x <- retVal)
            println(x);
    }
}
```

## 无限循环

要实现无限循环，可以使用while语句来实现：

```Scala
while(true) {
    statements(s);
}
```
