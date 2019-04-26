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
