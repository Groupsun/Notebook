# Scala访问修饰符

Scala访问修饰符基本和Java一样，分别有：private，protect，public。

如果没有指定访问修饰符，Scala的对象默认的访问级别都是public。

## private成员

用private关键字修饰，带有此标记的成员仅在包含了成员定义的类或对象内部可见，同样的规则还适用于内部类。

```Scala
class Outer{
    class Inner{
        private def f(){
            println("f")
        }
        class InnerMost{
            f()     // Correct
        }
    }
    (new Inner).f()     // Error
}
```

## protect成员

Scala中的protect成员只允许保护成员在定义了该成员的类的子类中被访问。

## public成员

Scala中的public成员在任何地方都可以被访问。
