# Ubuntu 解压缩rar文件

安装压缩和解压缩的程序rar以及unrar：

```bash
$ sudo apt-get install rar unrar
```

解压的命令为：

```bash
# unrar使用rar亦可
$ unrar e file.rar  # 直接解压到当前目录，不会创建目录
$ unrar file file.rar # 解压到目录file中
```

压缩的命令为：

```bash
$ rar a FileName.rar DirName # 将文件夹DirName压缩至FileName.rar当中
```
