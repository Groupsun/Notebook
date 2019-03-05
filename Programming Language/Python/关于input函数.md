# 关于input函数

对于刚刚接触python的我来说，首先遇到的是用户在终端输入的方法：使用 input() 方法。

需要注意的是， input() 方法从终端得到的用户的输入是字符串，以换行符结束。Python2里得到的是带有双引号的字符串，在Python3中 input() 方法调用默认使用 eval() 方法把双引号去掉了：

https://docs.python.org/3/library/2to3.html?highlight=input#2to3fixer-input

如果要输入的是整数，其实方法也很简单，做强制类型转换即可：

```python
i = int(input())
```

input() 方法里面可以放输入的时候想要的提示：

```python
i = int(input("i = "))
```

运行时就会显示 i = ，并在后面输入。
