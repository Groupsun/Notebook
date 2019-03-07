# A Byte of Python reading notes

Python简明教程 的阅读笔记，面向有编程经验背景的程序员阅读。

## 基础

### 注释

以'#'开头的任何文本都是注释：

```python
# print function
print('hello world')

print('hello world') # print function
```

### 字面常量

字面常量的例子有：数字如5，1.23，字符串如"This is a string"或者'This is a string'。

之所以被称为常量，是因为字面常量2永远代表数字2，它不能改变。

### 数字

数字有两种类型：整型和浮点型。

整型的例子如字符常量2。

浮点型的例子如3.23和52.3E-4。

注意：Python中没有long类型，int类型可以是任意大小的。

### 字符串

字符串由一个字符序列组成。

#### 单引号

可以使用单引号来声明字符串：'Quote me on this'。

引号内的所有空间，如空格和制表符，都会得到保留。

#### 双引号

在双引号内声明的字符串的工作机理和在单引号中的一模一样："What's your name?"。

#### 三引号

可以使用三引号来声明多行的字符串。在三引号当中，你可以随意使用单引号或者双引号：

```python
'''This is a multi-line string. This is the first line.
This is the second line.
"What's your name?," I asked.
He said "Bond, James Bond."
'''
```

#### 字符串是一成不变的

这意味着当你创建一个字符串后，你就不能够改变它。这看起来像是一件坏事，其实不然。在后面的程序例程当中我们将会发现这并不是一个糟糕的限制。

#### 格式化方法

有时候我们想要用其他的信息来对字符串进行构造。此时 format() 方法十分有用：

```python
name = "SunnyChen"
age  = 22

print("{0} is {1} years old.".format(name, age))
print("{0} is studying python".format(name))
```

输出：

```
SunnyChen is 22 years old.
SunnyChen is studying python
```

格式化中的括号是可选的，你也可以这样来使用：

```python
name = "SunnyChen"
age  = 22

print("{} is {} years old.".format(name, age))
print("{} is studying python".format(name))
```

这样得到的结果是一样的。当然，此时括号按照format中参数的顺序进行匹配。

此外你还可以在括号中命名参数：

```python
name = "SunnyChen"
age  = 22

print("{name} is {age} years old.".format(name=name, age=age))
print("{name} is studying python".format(name=name))
```

在Python 3.6中引入了更为简短的方法来命名参数，叫"f-strings"方法：

```python
name = "SunnyChen"
age  = 22

print(f"{name} is {age} years old.")
print(f"{name} is studying python")
```

Python在 format 方法当中所做的事情，就是将每个参数替换到格式所在的位置，下面展示更多的细节方法：

```python
# 浮点数保留小数点后3位
print("{0:.3f}".format(1.0/3))
# 使用'_'来将"test"扩充到13个字符长
print("{0:_^13}".format("test"))
# 显式指定format中的参数
print("{name} reading {str}".format(name=name, str="books"))
```

输出：

```
0.333
____test_____
SunnyChen reading books
```

更多具体的格式参见：https://docs.python.org/3/library/string.html#formatspec

print 方法输出的字符串，隐式使用'\n'结尾，因此 print 方法输出的字符串都会自动换行。如果要让 print 方法的不以'\n'结尾，需要显式声明：

```python
print('a', end='')
print('b', ehd='')
```

#### 转义序列

带来这个问题的是在字符串中使用符号'的问题，在Python中有两种解决的方法，一种是使用转义字符\'，另一个方法就是使用双引号来声明字符串：

```python
print("test'test'") # 输出：test'test'
```

当然，使用双引号声明的字符串，在字符串中使用双引号同样要使用转义字符。

其他转义字符的使用和一般高级语言的用法一致，这里不再赘述。

另外可以说明的一点是，在字符串声明中'\'的用途是在一行字符串的结尾中使用，表明不会新开一行，如下：

```python
str  = "one line. \
       another line."
print(str)
```

输出：

```
one line.        another line.
```

#### 原始字符串

如果需要指定未经过特殊处理的字符串如转义序列，可以在字符串之前用r或者R来声明：

```python
r"Newlines are indicated by \n"
```

值得注意的是，在处理正则表达式的时候要全程使用原始字符串。

### 变量

#### 标识符命名

Python的标识符命名规则：

- 标识符的第一个字符必须是大小写的字母或者下划线。
- 标识符剩下的部分可以包括大小写字母或者Unicode的字符或者下划线。
- 标识符是大小写敏感的。

#### 数据类型

基本的数据类型包括数字和字符串。在后面的部分介绍使用类（classes）来创建自己的数据类型。

#### 对象

Python将在程序中的任何内容统称为对象，这是一种一般的说法。对于使用面向对象编程的用户来说：Python是强面向对象的，一切都是对象，包括数字、字符串和函数。

对于静态语言程序员来说，要注意的是：Python中的变量使用它们只需要给它们赋值即可，不需要声明它们的数据类型。

### 逻辑行和物理行

一个物理行（physical line）是指你在程序中所看到的写的内容。一个逻辑行（logical line）是python所看到的单个语句。Python会假定每一个物理行对应一个逻辑行。

Python不像C++等语言，每一行都需要以';'结尾，但是如果你想在一个物理行上写多个逻辑行，那么就需要使用';'来进行分隔：

```python
i = 5
print(i)
```

相当于：

```python
i = 5;
print(i);
```

也相当于：

```python
i = 5; print(i);
```

然而，极力不推荐你使用最后一种方式来书写Python程序。一般来说，另外一个有关的比较有用的用法是使用反斜杠'\'来分隔一条很长的逻辑行：

```python
s = 'This is a string. \
This continues the string.'
print(s)
```

当然，有时候可以不用显式使用反斜杠来标明逻辑行的分隔，比如在括号开头的逻辑行当中，这被称为隐式行连接（implicit line joining）。

### 缩进（Indentation）

Python中每一行开始的空白是很重要的（缩进）。在逻辑行开头留下空白区（使用空格或者制表符）可以定义逻辑行的缩进级别，从而定义逻辑行的语句分组。

这意味着放在一起的语句必须拥有相同的缩进，这样的一组语句称为块（block）。

下面的例子就是错误的缩进示范：

```python
i = 5
# 下面的逻辑行是错误的，自行缩进了一个空格
 print('Value is', i)
print('I repeat, the value is', i)
```

当运行这个程序就会报错：

```
File "whitespace.py", line 3
print('Value is', i)
^
IndentationError: unexpected indent
```

上面的例子告诉我们，不能够自己任意的新开一组表达式的块（除非是默认的main主块）。关于新开块的细节将会在之后说明，比如控制流（条件语句块）。

Python官方的缩进建议：4个空格，可以将制表符设定为这个值来缩进。

对静态语言程序员需要注意的是：Python不会使用大括号来表示块，只能使用缩进的形式来表示。

## 运算符和表达式

### 运算符

Python中一些常用的运算符：+ - * **（乘方） / //（整除） % << >> & | ^ ~ < > <= >= == != not（布尔非） and or 等等

详细的运算符信息参考：https://docs.python.org/3/library/operator.html#module-operator

### 数值运算与赋值的快捷方式

变量 = 变量 运算符 表达式 => 变量 运算符 = 表达式

### 求值顺序

详细的求值顺序参见：https://docs.python.org/3/reference/expressions.html?highlight=evaluation%20order#evaluation-order

下面给出一个大致的顺序，从最低优先级到最高优先级：

![avatar](./images/python-evaluation-order-1.png)

![avatar](./images/python-evaluation-order-2.png)

### 改变求值的顺序

使用括号：2 + 3 * 4 => (2 + 3) * 4

### 结合性（Associativity）

运算符的结合性是从左到右，意味着相同优先级的运算符从左到右的方式依次求值。比如，2 + 3 + 4 等同于 (2 + 3) + 4。

### 表达式

示例：

```python
length = 5
breadth = 2

area = length * breadth
print('Area is', area)
print('Perimeter is', 2 * (length + breadth))
```

输出：

```
Area is 10
Perimeter is 14
```

可以注意到的是，Python是如何漂亮的进行输出的。我们在'Area is'之后没有声明一个空格，但是在打印的时候却自动加上了一个空格使得输出更容易阅读。

## 控制流

### if语句

[这里]: ./关于input函数.md

Python中if语句块的格式：

```
if [conditions]:
    # statements
(elif [conditions]:)
    (# statements)
(else:)
    (# statements)
```

例子：

```python
number = 23
guess = int(input('Enter an integer : '))

if guess == number:
    # New block starts here
    print('Congratulations, you guessed it.')
    print('(but you do not win any prizes!)')
    # New block ends here
elif guess < number:
    # Another block
    print('No, it is a little higher than that')
    # You can do whatever you want in a block ...
else:
    print('No, it is a little lower than that')
    # you must have guessed > number to reach here

print('Done')
# This last statement is always executed,
# after the if statement is executed.
```

当然，使用if语句块可以是if-else、if-elif-else的形式，或者只是用if语句。例子中使用的 input() 方法，可以在[这里]参考。

实际上，在Python当中，int是一个类。当然目前我们可以只把它看作是一个将字符串转换成整型的方法。

可以注意到，在if语句块中的缩进能够高速Python哪些表达式属于哪些语句块。

if语句块中条件的说明以冒号':'结尾，表示判断条件的结束。

当然，你也可以在if语句块中嵌套if语句块。同时，elif和else语句块部分是可选的。

python中的条件判断使用and、or、not运算符来组合成一个表达式：

```python
if i < 0 or i > 10:
    #statement
```

C/C++程序员需要注意的是：Python没有switch表达式。可以使用if..elif..else语句块来达成同样的效果。

### while语句

Python中while语句块的格式：

```
while [conditions]:
    #statements
(else:)
    (#statements)
```

例程：

```python
number = 23
running = True

while running:
    guess = int(input('Enter an integer : '))

    if guess == number:
        print('Congratulations, you guessed it.')
        # this causes the while loop to stop
        running = False
    elif guess < number:
        print('No, it is a little higher than that.')
    else:
        print('No, it is a little lower than that.')

else:
    print('The while loop is over.')
    # Do anything else you want to do here

print('Done')
```

可以注意到，running的类型是布尔型，在作为条件时可以直接使用（作为布尔值）。

对于C/C++程序员来说，值得注意的是：在Python中while语句可以拥有else语句块，else语句块是可选的。

### for语句

例子：

```python
for i in range(1, 5):
    print(i)
else:
    print('The for loop is over')
```

输出：

```
1
2
3
4
```

for的循环条件是一组序列，在上述的例子中，我们使用了 range 方法来生成这样的序列。通过在传递两个参数给 range 方法，返回一组数字的序列，注意：不包括最后一个数字，也就是说声明序列的两个参数表示的范围是左闭右开的。比如， range(1, 5) 得到的序列是[1, 2, 3, 4]。 range 默认的以1的长度步增。range的第三个参数可以声明步增的长度。比如， range(1, 5, 2)得到[1, 3]。

注意到 range() 每次只会产生一个数字，如果你想要完整的数字序列，可以在 list() 方法中调用 range() 。比如， list(range(5)) 可以得到 [0, 1, 2, 3, 4]：

```python
l = list(range(1, 5))
print(l)
```

输出：

```
[1, 2, 3, 4]
```

Lists会在后面的数据结构章节中详细阐述。

上述的例子中，for i in range(1, 5)和for i in [1, 2, 3, 4]的效果相同。这个语句每次将每个数字（或者对象）赋给i，然后对于每个i的值执行一次语句块中的表达式。

与while语句一样，else语句块是可选项。

对于C/C++/Java/C#程序员来说需要注意的是：Python的for循环和C/C++中的for循环有着根本性的不同。C#程序员会发现Python中的for循环和C#中的foreach循环很像。Java程序员则会发现和Java 1.5开始引入的for (int i : IntArray)语句很像。在C/C++中，for (int i = 0; i < 5; i++)语句，在Python中只需要写for i in range(0, 5)。Python中的for循环更简洁、更具有表达性且更难出错。

### break语句

Python中的break语句与其他高级语言的作用一致，在这里不再进行赘述。不过要注意的一点是，break语句执行后不会执行for或者while循环中的else语句块。

### continue语句

Python中的continue语句与其他高级语言的作用一致，在这里不再进行赘述。

注：原文例子中的 len() 将作为参数的字符串的长度返回。

## 函数

函数使用关键字 def 来定义。在这个关键字之后是函数的标识符，然后是一对括号，括号当中是一些变量的名字（参数），最后是一个冒号，标明函数所要执行的语句块开始。

例子：

```python
def say_hello():
    # block belonging to the function
    print("hello world")
# End of function

say_hello() # call the function
say_hello() # call the function again
```

### 函数参数

函数参数在函数声明中的括号中定义，使用逗号来进行分隔。当我们调用该函数时，就要提供函数参数。定义函数时的参数称为形参，调用函数时传递的参数称为实参。

例子：

```python
def print_max(a, b):
    if a > b:
        print(a, 'is maximum')
    elif a == b:
        print(a, 'is equal to', b)
    else:
        print(b, 'is maximum')

# directly pass literal values
print_max(3, 4)

x = 5
y = 7

# pass variables as arguments
print_max(x, y)
```

### 局部变量

在函数中定义的变量不能在函数以外的地方引用。也就是说在函数内定义的变量是局部的。举例如下：

```python
x = 50

def print_x(x):
    x = 2
    print(x)

print_x(x)
print(x)
```

输出：

```
2
50
```

### global语句

如果你想在函数里将值赋给顶层（不在任何函数和类定义中的语句块）的某个变量，且这个变量在函数块中有同名的变量，则可以使用global语句来标明所引用的是全局的变量。

例子：

```python
x = 50

def print_x():
    global x
    x = 2
    print(x)

print_x()
print(x)
```

输出：

```
2
2
```

global语句可以声明多个变量，如：global x, y, z。

### 默认参数值

某些情况下，你可能想让你定义的函数中的参数是可选的。此时可以将参数声明默认的参数值来实现。通过在函数定义中在参数后使用赋值运算符赋予默认值即可。

参数的默认值必须是常数，并且是一成不变的。例子如下：

```python
def say(message, times=1):
    print(message * times)

say("test")
say("ha", 5)
```

输出：

```
test
hahahahaha
```

与C/C++一样，所有默认参数必须位于非默认参数的后面。也就是说所有的默认参数都必须位于参数声明的最后面。def fun(a, b=5)是正确的，但def fun(b=5, a)是错误的。

### 关键字参数

如果你所定义的函数参数比较多，那么你在调用的时候可能会想使用名字来指代它们而非顺序。

例子：

```python
def fun(a, b=2, c=3):
    print('a =', a, 'b =', b, 'c =', c)


fun(1, 2, 3)
fun(1, c=2)
fun(b=3, a=1, c=2)
```

输出

```
a = 1 b = 2 c = 3
a = 1 b = 2 c = 2
a = 1 b = 3 c = 2
```

### 可变参数

有时候你可能会想定义一个接受任意数量参数的函数，也就是可变参数的函数。可以使用星号来进行指定。

例子：

```python
def total(a=5, *numbers, **phonebook):
    print('a', a)

    #iterate through all the items in tuple
    for single_item in numbers:
        print('single_item', single_item)
    
    #iterate through all the items in dictionary
    for first_part, second_part in phonebook.items():
        print(first_part,second_part)

total(10,1,2,3,Jack=1123,John=2231,Inge=1560)
```

输出：

```
a 10
single_item 1
single_item 2
single_item 3
Inge 1560
John 2231
Jack 1123
```

当我们声明一个带有星号的参数如：*param时，从这个位置起到最后的参数都会被搜集并汇集成一个称为param的元组。

同样，当我们声明一个带有两个星号的参数如：**param时，从这个位置起到最后的参数都会被搜集并汇集成一个称为param的字典。

有关元组和字典的详细阐述在后面会提到。

### return语句

return语句可以在函数中返回值，或者从函数中跳出。

可以在函数中单单使用一个return来从函数中跳出，相当于return None。None在Python中是一个特别的类型，表示什么都没有。

所有的函数都会在最后隐式的包含一条return None语句，除非你显式的写你自己的return语句。你可以运行print(some_function())，some_function中没有return语句，比如：

```python
def some_function()
    pass
```

此时print输出的结果是"None"。Python中的pass语句表示一个空的语句块。

### DocStrings

DocString是一种重要的工具，可以帮助你更好的记录程序使其更容易理解。DocString甚至可以在程序运行的时候通过一个文档来获取信息。

例子：

```python
def print_max(x, y):
    '''This Function prints the max_number between x and y.

    :param x: Integer x
    :param y: Integer y
    :return: Max integer number between x and y
    '''

    x = int(x)
    y = int(y)

    if x > y:
        return x
    else:
        return y

print(print_max.__doc__)
print(print_max(3, 5))
```

输出：

```
This Function prints the max_number between x and y.

    :param x: Integer x
    :param y: Integer y
    :return: Max integer number between x and y
    
5
```

函数的语句块中第一行逻辑行就是该函数的docstring。DocString在模块（modules）以及类（classes）当中也存在。

docstring接下来是多行的字符串。docstring的一般个格式是，第一行是一个大写字母开头的句子，描述函数的作用，以句号结尾。第二行空一行，从第三行开始详细描述函数的功能，包括参数意义、具体功能等等。

我们可以通过使用 print_max 的__doc__属性来获得该函数的docstring。需要谨记，在Python中所有东西都是对象，自然包括函数。在后面类的章节中会更详细的了解对象。

当你在Python中使用 help() 函数时，实际上就是获取函数的__doc__属性并将其打印出来。

自动化的工具可以用这种方式来检索你程序中的文档，因此我强烈建议你给你重要的程序中加入文档。你的Python发行版中的 pydoc 的作用和 help() 函数的作用类似。

## 模块

模块（Module）可以让你在别的程序中重用一些函数。有多种使用module的方法，但最简单的方法是创建一个以.py扩展结尾的文件，并包含所需要的函数和变量。

另一种方法是使用撰写Python解释器本身的本地语言来编写模块。比如，你可以使用C语言来编写模块，在编译后，通过标准的Python解释器在Python代码中使用它。

一个模块可以在其他的程序中import来使用其功能（函数和变量）。同样，我们也可以在程序中导入Python的标准库。

例子：

```python
import sys

for i in sys.argv:
    print(i)

print(sys.path)
```

输出（因人而异）：

```
modules.py
['/home/sunnychen/Documents/python/learning', '/home/sunnychen/Software/miniconda3/lib/python37.zip', '/home/sunnychen/Software/miniconda3/lib/python3.7', '/home/sunnychen/Software/miniconda3/lib/python3.7/lib-dynload', '/home/sunnychen/Software/miniconda3/lib/python3.7/site-packages']
```

sys模块包含了与Python解释器相关的功能，也就是系统功能。如果它不是一个编译好的模块，也就是用Python写成的模块，那么Python的解释器会在它的sys.path提供的目录中搜索。如果找到了该模块，那么模块中的语句会开始运行并且能够为你所用。值得注意的是，初始化工作只在第一次导入模块的时候完成。

sys.argv变量是一个字符串的列表。特别的，sys.argv是一个包含命令行参数（command line arguments）的列表，也就是使用命令行运行你所写的程序时，传递到你程序当中的参数。如果你在命令行中使用如下的形式运行：

```bash
$ python3 modules.py a b c
```

其中modules.py为：

```python
import sys

for i in sys.argv:
    print(i)
```

则输出为：

```
modules.py
a
b
c
```

Python会将你在命令行中传递到程序中的参数存储在sys.argv当中。sys.argv中的列表，第一个元素永远是脚本的名称。

sys.path内包含了导入模块的字典名称列表。可以观察到当前程序的目录也是sys.path的一部分，与 PYTHONPATH 环境变量相同。也就是说，你可以导入当前目录下的所有模块。否则，你只能将你的模块放入sys.path中的目录当中。

因为当前目录就是你的程序运行的目录，可以运行import os; print(os.getcwd())来查看当前你运行程序所在的目录。

### 按字节码编译的.pyc文件

导入一个模块是的开销是很大的，因此Python使用了一些技巧使其变得更快。一种方法是创建按照字节码编译的.pyc文件，这个文件是Python将程序转换为中间形式的文件。这个.pyc文件当你在其它不同的程序再次导入该模块时十分有用。因为导入模块的一部分工作已经完成了，并且这些按字节编译的文件是平台独立的。

一般来说，这些.pyc文件通常会在同一个目录下创建。如果Python没有权限在这个目录写，则不会创建.pyc文件。（Python3.7中，会在同一个目录下创建一个目录名为__pycache__，将.pyc文件放进这个目录当中。）

### from..import语句

如果你想直接导入变量 argv 进你的程序当中，则可以使用 from sys import argv 语句。

警告：一般来说，尽量不要使用 from..import 语句，使用 import 语句。这样程序可以避免名字上的冲突问题，使其更具有可读性。

### 模块的名字（__name__）

所有的模块都有名字，并且都有方法在模块当中得到当前模块的名字。这对于一些特定的需求来说是比较有用的，比如查看当前的模块是独立运行的还是被导入进来运行的。如同之前所提到的，当一个模块第一次被导入的时候，模块的代码会被执行。我们可以利用这一特性来使模块的行为表现的不同——被它自己使用还是被其他模块导入使用。通过在程序中使用模块的__name__属性可以达到上述的需求：

```python
if __name__ == '__main__':
    print('This program is being run by itself')
else:
    print('I am being imported from another module')
```

输出：

```
$ python module_using_name.py
This program is being run by itself

$ python
>>> import module_using_name
I am being imported from another module
>>>
```

每个python模块都定义了__name__的属性。如果这个属性与__main__相等，则表明这个模块被用户独立的运行当中，此时我们就可以采取一些特别的行动。

### 创建你自己的模块

所有的Python程序都是一个模块，你只需要保证它是.py的扩展名即可。

例子：

```python
# save as mymodule.py

def say_hi():
    print('Hi, this is mymodule speaking.')

__version__ = '0.1'
```

需要注意的是，该模块需要放在你要调用该模块的程序的同一个目录下，或者在sys.path列举的目录下。

```python
# save as mymodule_demo.py

import mymodule

mymodule.say_hi()
print('Version', mymodule.__version__)
```

输出：

```
$ python mymodule_demo.py
Hi, this is mymodule speaking.
Version 0.1
```

你也可以使用 from..import 语句来完成：

```python
from mymodule import say_hi, __version__

say_hi()
print('Version', __version__)
```

显然，上述的做法存在一种情况，就是当现在的模块中已经有一个名字是__version__的变量存在的时候，会造成名字上的冲突。这也是为什么我们不建议使用 from..import 语句的原因。你还可以这样使用：

```python
from mymodule import *
```

这样会将模块中所有公共的名称，如 say_hi 。但是不会导入__version__，因为它以双下划线开头。

### dir函数

dir() 函数是一个内置的函数，它返回一个对象中定义了的名称的列表。如果这个对象是一个模块，那么这个列表包括在这个模块中定义了的函数、类以及变量。

这个函数可以接受参数。如果参数是模块的名字，那么这个函数会返回指定模块定义的名称的列表。如果没有参数，则返回当前模块定义的名称的列表。

例子：

```
$ python
>>> import sys

# get names of attributes in sys module
>>> dir(sys)
['__displayhook__', '__doc__',
'argv', 'builtin_module_names',
'version', 'version_info']
# only few entries shown here

# get names of attributes for current module
>>> dir()
['__builtins__', '__doc__',
'__name__', '__package__', 'sys']

# create a new variable 'a'
>>> a = 5

>>> dir()
['__builtins__', '__doc__', '__name__', '__package__', 'sys', 'a']

# delete/remove a name
>>> del a

>>> dir()
['__builtins__', '__doc__', '__name__', '__package__', 'sys']
```

指的注意的几点是，当前模块中导入了其他模块后，在当前模块调用 dir() 函数也会把导入的模块的名字放在列表中。之后我们定义了一个新的变量 a ，再调用 dir() 函数，a也会出现在列表当中。接着我们使用 del 语句移除这个变量 a 。此时再调用 dir() 函数，变量 a 不会再出现在列表当中。

关于 del ：这个语句在执行之后就会删除所声明的变量/名称，且无法再访问它们。

### 包

现在，你应该开始组织你的程序的层次结构。变量通常位于函数当中，函数和全局变量通常位于模块当中。而组织模块的方法则是使用包。

包实际上就是一个包含模块的文件夹，并带有一个特殊的__init__.py文件，来提示Python这是一个包含Python模块的文件夹。

一个包组织的例子：

```
- <some folder present in the sys.path>/
    - world/
        - __init__.py
        - asia/
            - __init__.py
            - india/
                - __init__.py
                - foo.py
        - africa/
            - __init__.py
            - madagascar/
                - __init__.py
                - bar.py
```

## 数据结构

Python有四种内置的数据结构：list，tuple，dictionary以及set（列表、元组、字典以及集合）。

### 列表（List）

List 是存储一个序列的元素的数据结构。声明一个列表，其中的元素要使用方括号闭合，且使用逗号进行分隔。当你创建了一个列表之后，你可以在列表中增加、删除或者搜索元素。因此列表也是一个可变的数据类型。

#### 对象和类的简单介绍

一个列表可以作为对象以及类使用的例子。当我们使用一个变量i，并且将其赋值为5的时候，你可以认为我们创建了一个int类的对象i。实际上，你可以通过 help(int) 来作更多的了解。

一个类还可以拥有方法，也就是对这个类定义仅对它启用的函数。你可以只在拥有这个类的某个对象时使用它这些方法。举例来说，Python对于 list 类提供了一个 append 的方法，可以允许你在列表的末尾添加元素。如： mylist.append('an item') 会在 mylist 的末尾添加一个字符串。

一个类还可以拥有字段（fields），字段就是只在该类当中使用的变量定义的区域。你可以只在拥有这些类的对象时使用这些变量/名称。字段同样可以使用：mylist.field。

例子：

```python
strlist = ["amd", "intel", "arm", "x86", "risc-v", "mips"]

print("len of the strlist", len(strlist))

print("items: ", end = ' ')
print(strlist)

print("going to append")
strlist.append("sparc")

print(strlist)

print("going to sort the list")
strlist.sort()

print(strlist)

print("going to delete", strlist[0])

del strlist[0]

print(strlist)
```

输出：

```
len of the strlist 6
items:  ['amd', 'intel', 'arm', 'x86', 'risc-v', 'mips']
going to append
['amd', 'intel', 'arm', 'x86', 'risc-v', 'mips', 'sparc']
going to sort the list
['amd', 'arm', 'intel', 'mips', 'risc-v', 'sparc', 'x86']
going to delete amd
['arm', 'intel', 'mips', 'risc-v', 'sparc', 'x86']
```

列表是允许异构的，也就是说可以将不同类型的数据放在同一个列表当中，你甚至可以将另一个列表作为该列表的元素。但是并不推荐这样做，这样会给程序带来很多的隐患。请尽量使列表中的元素都是同类型的。

遍历列表的方法可以使用 for..in 语句，也可以使用比较经典的下标访问形式。列表也是一种序列（sequences），有关序列的相关知识在后面会提到。

接下来是对列表几个常用的方法进行展示。打印列表可以使用 for..in 语句，但是直接使用 print() 方法会更为简单。 append() 方法上面已经提到过， sort() 方法对列表中的元素进行排序，按照列表中元素的类所定义的"<"运算符方法来进行排序。

### 元组（Tuple）

元组用于将多个对象保存在一起。你可以认为它和列表很相似，但是元组不能提供列表类提供给你的广泛功能。元组的一个主要的特性是它们和字符串一样是不可变的，你不能修改元组。

元组的定义，需要将对象使用括号闭合，并使用逗号来进行分隔。

元组通常用于保证一个语句或者一个用户自定义的函数所操作的一组数据是安全的，也就是不变的，还有就是在 for 语句中指定下标范围时， range() 函数所产生的元组。

例子：

```python
# I would recommend always using parentheses
# to indicate start and end of tuple
# even though parentheses are optional.
# Explicit is better than implicit.
zoo = ('python', 'elephant', 'penguin')
print('Number of animals in the zoo is', len(zoo))

new_zoo = 'monkey', 'camel', zoo # parentheses not required but are a good idea
print('Number of cages in the new zoo is', len(new_zoo))
print('All animals in new zoo are', new_zoo)
print('Animals brought from old zoo are', new_zoo[2])
print('Last animal brought from old zoo is', new_zoo[2][2])
print('Number of animals in the new zoo is',
len(new_zoo)-1+len(new_zoo[2]))
```

输出：

```
$ python ds_using_tuple.py
Number of animals in the zoo is 3
Number of cages in the new zoo is 3
All animals in new zoo are ('monkey', 'camel', ('python', 'elephant', 'penguin'))
Animals brought from old zoo are ('python', 'elephant', 'penguin')
Last animal brought from old zoo is penguin
Number of animals in the new zoo is 5
```

值得注意的是，当一个元组中的其中一个元素是一个元组的时候，在应用 len() 函数时，该元组也只会被记做1个元素。

如果想要声明一个空的元组，那么很简单只需要：myempty = ()。而声明一个只有1个元素的元组，则需要在唯一的元素后加上一个逗号：singleton = (2, )。

### 字典（Dictionary）

与其他高级语言中的字典一样，Python中的字典将关键字（key）和值（value）相对应。需要注意的是，关键字必须是唯一的。字典中的关键字必须是不可变的对象（如字符串），而值则可以是可变的，也可以是不可变的对象。也可以这样说：字典中的关键字尽量使用简单的对象。

在字典中声明一对关键字与值的方法是：d = {key1 : value1, key2 : value2}。需要注意的是，字典中的 关键字-值 对不会按任何特定的顺序排列。如果想要其有序，需要在使用前对其进行排序。

所有你使用到的字典对象都是属于 dict 类的。

例子：

```python
# 'ab' is short for 'a'ddress'b'ook

ab = {
'Swaroop': 'swaroop@swaroopch.com',
'Larry': 'larry@wall.org',
'Matsumoto': 'matz@ruby-lang.org',
'Spammer': 'spammer@hotmail.com'
}

print("Swaroop's address is", ab['Swaroop'])

# Deleting a key-value pair
del ab['Spammer']

print('\nThere are {} contacts in the address-book\n'.format(len(ab)))

for name, address in ab.items():
    print('Contact {} at {}'.format(name, address))

# Adding a key-value pair
ab['Guido'] = 'guido@python.org'

if 'Guido' in ab:
    print("\nGuido's address is", ab['Guido'])
```

输出：

```
$ python ds_using_dict.py
Swaroop's address is swaroop@swaroopch.com

There are 3 contacts in the address-book

Contact Swaroop at swaroop@swaroopch.com
Contact Matsumoto at matz@ruby-lang.org
Contact Larry at larry@wall.org

Guido's address is guido@python.org
```

如果你有C++标准库中map容器的使用经验，那么Python中的字典应该对你来说是很熟悉的。字典中的 item() 方法会返回一个元组，元组中含有字典中所有的元素对。for语句中可以分别表示关键字和值。加入元素对到字典的方法和C++中的非常类似，可以简单地通过使用索引运算符访问一个键值并为其分配与之相应的值。我们还可以使用 in 操作符来检查特定的元素对是否在字典当中。

函数中的关键字参数就是使用了字典。调用函数时，声明的参数名字实际上就是字典中的关键字。

### 序列（Sequence）

列表、元组以及字符串都是序列的例子。序列最主要的特性是资格测试（membership tests）和索引操作（indexing operations），允许我们直接从一个序列中获取一个指定的元素。

上面所提到的序列的三种形态，还有一个称为切片（slicing）的运算符，允许我们获取序列中的特定一部分。

例子：

```python
shoplist = ['apple', 'mango', 'carrot', 'banana']
name = 'swaroop'

# Indexing or 'Subscription' operation #
print('Item 0 is', shoplist[0])
print('Item 1 is', shoplist[1])
print('Item 2 is', shoplist[2])
print('Item 3 is', shoplist[3])
print('Item -1 is', shoplist[-1])
print('Item -2 is', shoplist[-2])
print('Character 0 is', name[0])

# Slicing on a list #
print('Item 1 to 3 is', shoplist[1:3])
print('Item 2 to end is', shoplist[2:])
print('Item 1 to -1 is', shoplist[1:-1])
print('Item start to end is', shoplist[:])

# Slicing on a string #
print('characters 1 to 3 is', name[1:3])
print('characters 2 to end is', name[2:])
print('characters 1 to -1 is', name[1:-1])
print('characters start to end is', name[:])
```

```
$ python ds_seq.py
Item 0 is apple
Item 1 is mango
Item 2 is carrot
Item 3 is banana
Item -1 is banana
Item -2 is carrot
Character 0 is s
Item 1 to 3 is ['mango', 'carrot']
Item 2 to end is ['carrot', 'banana']
Item 1 to -1 is ['mango', 'carrot']
Item start to end is ['apple', 'mango', 'carrot', 'banana']
characters 1 to 3 is wa
characters 2 to end is aroop
characters 1 to -1 is waroo
characters start to end is swaroop
```

Python中的下标可以是负数，此时位置会从序列的结尾开始算，也就是说，-1表示序列最后的元素，-2表示序列最后一个元素的前一个元素。

切片运算在下标访问时通过一个冒号来转化为访问序列中的一部分。如果开始部分的数字没有声明，那么python会从头开始，同理，如果结束部分的数字没有声明，那么python会一直切片到结尾。注意，切片结果包含开头，但是不包括结尾，也就是左闭右开的形式。

切片的范围同样可以使用负数，如 shoplist[:-1] 表示切片的范围从开头到倒数第二个元素。当然，你还可以提供第三个参数给切片，表示步长（默认为1）。步长可以为负数，表示从后往前数。

### 集合（Set）

集合是简单对象的无序集合。使用集合，你可以测试某些对象的资格，查看它是否是别的集合的自己，或者查看两个集合的交集等。

例子：

```
>>> bri = set(['brazil', 'russia', 'india'])
>>> 'india' in bri
True
>>> 'usa' in bri
False
>>> bric = bri.copy()
>>> bric.add('china')
>>> bric.issuperset(bri)
True
>>> bri.remove('russia')
>>> bri & bric # OR bri.intersection(bric)
{'brazil', 'india'}
```

### 引用

当你创建了一个对象，并且将其赋值给一个变量时，这个变量只是引用了这个对象并且不代表对象其本身。可以理解为，对象的名字指向了该对象在内存中的位置（地址）。这称为将名字绑定（binding）给对象。

一般来说在编程的时候不需要担心这个特性，但是这个特性会带来一些微妙的影响。

例子：

```python
print('Simple Assignment')
shoplist = ['apple', 'mango', 'carrot', 'banana']
# mylist is just another name pointing to the same object!
mylist = shoplist

# I purchased the first item, so I remove it from the list
del shoplist[0]

print('shoplist is', shoplist)
print('mylist is', mylist)
# Notice that both shoplist and mylist both print
# the same list without the 'apple' confirming that
# they point to the same object

print('Copy by making a full slice')
# Make a copy by doing a full slice
mylist = shoplist[:]
# Remove first item
del mylist[0]

print('shoplist is', shoplist)
print('mylist is', mylist)
# Notice that now the two lists are different
```

输出：

```
$ python ds_reference.py
Simple Assignment
shoplist is ['mango', 'carrot', 'banana']
mylist is ['mango', 'carrot', 'banana']
Copy by making a full slice
shoplist is ['mango', 'carrot', 'banana']
mylist is ['carrot', 'banana']
```

实际上就是老生常谈的引用问题，有编程经验的程序员已经熟稔于心，这里不再赘述。不过指的注意的是复制整个序列对象的时候，使用切片运算符从开始到结尾进行切片即可。

### 关于字符串更多的内容

所有的字符串都是对象，字符串有各种各样的方法，如之前使用过的 format() 。所有的字符串都是类 str 的对象。关于字符串更多的方法参见：https://docs.python.org/3/library/stdtypes.html#str
