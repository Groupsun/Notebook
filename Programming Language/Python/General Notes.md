# Python字符串相等的判断

判断字符串值是否相同使用`==`，判断两个字符串变量引用是否相同使用`is`。

例子：

```shell
>>> a = 'test'
>>> b = ''.join(['t','e','s','t'])
>>> a == b
True
>>> a is b
False
```

# 去除字符串空格

## 去除左右空格

删除左边空格：`lstrip()`

```shell
>>> str = '    test'
>>> str.lstrip()
'test'
```

删除右边空格：`rstrip()`

```shell
>>> str = 'test    '
>>> str.rstrip()
'test'
```

## 删除中间空格

1. `replace()`函数

```shell
>>> str = "hello world"
>>> str.replace(' ','')
'helloworld'
```

2. `split()`函数

```shell
>>> str = "hello world"
>>> str = ''.join(str.split())
>>> str
'helloworld'
```

# syntax suger

syntax suger（语法糖）。比如`[]`语法，实际上就是创建一个列表，其在底层调用`list`的构造函数。

```
>>> [] == list()
True
```

# 函数定义中的list以及dic默认参数问题

函数定义中的list以及dic默认参数，在函数定义第一次执行默认参数的时候会初始化，之后将会不再清空：

```python
def foo(a, b=[]):
    b.append(a)
    return b

def main():
    print(foo(1))
    print(foo(2))
    print(foo(3))
```

输出：

```
[1]
[1, 2]
[1, 2, 3]
```

正确写法：

```python
def foo(a, b=None):
    if b is None:
        b = []
    b.append(a)
    return b
```
