# 安装Eclipse

## ubuntu下安装eclipse IDE for C/C++ developers 

## 安装JDK
eclipse是使用Java语言开发的，一个Java应用程序，这意味着eclipse只能运行在Java虚拟机上。倘若没有安装JDK（Java Development Kit），即使在ubuntu上安装了eclipse，也不能运行，所以先执行下面的命令安装JDK。
```bash
#apt-get install openjdk-6-jre-headless
```

## 获得eclipse IDE for C/C++ developers的源码包
从http://www.eclipse.org/downloads/?osType=linux&release=undefined下载Eclipse IDE for C/C++ Developers ，文件eclipse-cpp-kepler-SR1-linux-gtk.tar.gz。需要注意的是，下载的是版本是linux，并且注意CPU的位数。

## 解压安装
```bash
#tar xzvf eclipse-cpp-kepler-SR1-linux-gtk.tar.gz -C /opt
```
把安装包解压到/opt目录下，默认的会在该目录下创建一个eclipse目录。从eclipse目录里，可以看到一个eclipse的可执行程序，可以尝试通过以下命令运行。
```bash
$cd /opt/eclipse
$./eclipse
```

## 设置环境变量
eclipse IDE for C/C++ developers已经安装成功，但是终端模式必须到这个目录下才能运行该程序。为了让在任何目录下都能运行该程序（像cd命令一样），进行如下操作。
```bash
#vi /etc/profile
```
在profile文件的最后边增加一句：export PATH=$PATH:/opt/eclipse；保存，退出。
```bash
#source /etc/profile
```
尝试运行下面命令，就可以启动eclipse应用程序。
```bash
#eclipse
```
解释：由于eclipse安装包被解压到了/opt/eclipse下，可执行程序eclipse就在这个目录下，但是这个目录并不是系统搜索应用程序的默认目录。这里通过在系统环境变量文件中增加export PATH=$PATH:/opt/eclipse一个搜索应用程序的目录，这样就可以在终端下直接输入命令$eclipse打开该软件。

新的问题：当终端输入eclipse后，eclipse就会被打开。但是，在终端下就不能进行其它的操作，必须关闭eclipse，才能重新在终端上输入命令。

## 解决“终端输入eclipse后，在终端下就不能进行其它的操作”的问题
一种办法是在输入命令时候，加上一个“&”符号，也就是输入命令#eclipse&，就能解决问题。虽然多一个符号也没什么大不了，但是不符合我们的习惯，可以通过如下配置，实现理想的效果。
```bash
#vi /etc/profile
```
在profile文件的最后边增加一句：alias eclipse='eclipse&'；保存，退出。
```bash
#source /etc/profile
```
这样操作之后，在终端输入#eclipse之后，eclipse软件在后台启动，并不会影响到终端的继续使用。

## 在ubuntu桌面上添加eclipse应用程序图标
虽然，通过终端可以运行eclipse，但是如果能像“windows那样，直接在桌面上双击应用程序，应用程序自动打开”，使用eclipse将会更加方便，通过以下操作可以实现。

1. 用vi在用户到桌面目录创建一个名为“eclipse.desktop”的文件，并添加以下内容：
    ```bash
	[Desktop Entry]
	Name=Eclipse
	Comment=Eclipse IDE
	Exec=/opt/eclipse/eclipse 注释：设置应用程序位置
	Icon=/opt/eclipse/icon.xpm 注释：设置图标
	Terminal=false
	Type=Application
	Categories=Application;Development;
	```
2. 然后给该文件赋权 ,运行"#chmod 777 eclipse.desktop"命令
    
    补充：如果是用deb包安装的话，可以把/usr/share/applications/下eclipse.desktop文件拷贝到桌面目录即可。

通过以上两步操作，就可以在桌面上看到eclipse的图标，双击就可以运行该程序。
