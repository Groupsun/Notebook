# gcc 生成.i以及.s文件

```bash
gcc -E test.c -o test.i
```
生成预编译处理后的文本文件test.i，它是将头文件的内容插入到源程序中。

```bash
gcc -S test.i -o test.s
```
生成编译后的汇编程序文本文件test.s。