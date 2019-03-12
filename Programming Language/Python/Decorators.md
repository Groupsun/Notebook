# Python装饰器

装饰器的本质还是一个Python函数，它用于在其他函数不需要做代码变动的情况下增加一些额外的功能，其返回的也是一个函数对象。简单来说，装饰器的作用就是**为已经存在的函数或者对象添加额外的功能**。

## 基本装饰器基础

最简单的装饰器的例子如下：

```python
# 装饰器名字为debug，本质上还是一个函数
def debug(func):
    def wrapper(*args, **kwargs):   # 包装的函数接受任何参数
        print("[DEBUG]: Enter {}()".format(func.__name__))
        func(*args, **kwargs)
    return wrapper  # 返回函数对象


@debug
def greetings(s):
    print(s)


greetings("hello")
```

输出：

```
[DEBUG]: Enter greetings()
hello
```
