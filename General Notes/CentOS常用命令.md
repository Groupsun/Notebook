# CentOS(Linux)常用命令

CentOS命令与Linux一样。

## Linux的常用基本命令
首先启动Linux。启动完毕后需要进行用户的登录，选择登陆的用户不同自然权限也不一样，其中“系统管理员”拥有最高权限。

在启动Linux后屏幕出现如下界面显示：
```bash
　　……

　　Red Hat Linux release 9 (Shrike)

　　Kernel2.4.20.8 on an i686

　　login:
```
输入：root(管理员名)后，计算机显示输口令(password:)，输入你的口令即可。当计算机出现一个“＃”提示 符时，表明你登录成功!

屏幕显示Linux提示符：
```bash
[root@localhost root]#
```
这里需要说明的是“Red Hat Linux release 9 (Shrike)”表示当前使用的操作系统的名称及版本。“2.4.20.8”表示Linux操作系统的核心版本编号。“i686”表示该台电脑使用的CPU的等级。

下面我们来介绍常用基本命令

## 注销，关机，重启
1. Logout 注销是登陆的相对操作，登陆系统后，若要离开系统，用户只要直接下达logout命令即可：
    ```bash
    　　[root@localhost root]#logout

    　　Red Hat Linuxrelease 9(Shike)

    　　Kernel2.4.20.8 on an i686

    　　Login: ← 回到登陆的画面
    ```

2. 关机或重新启动的shutdown命令

    　　Shutdown命令可以关闭所有程序，依照用户的需要，重新启动或关机。

    参数说明如下：

    立即关机：－h 参数让系统立即关机。范例如下：
    ```bash
    　　[root@localhost root]#shutdown –h now ← 要求系统立即关机
    ```

    指定关机时间：time参数可指定关机的时间；或设置多久时间后运行shutdown命令，范例如下：
    ```bash
    　　[root@localhost root]#shutdown now ← 立刻关机

    　　[root@localhost root]#shutdown +5 ← 5分钟后关机

    　　[root@localhost root]#shutdown 10:30 ← 在10：30时关机
    ```
    　　关机后自动重启：－r 参数设置关机后重新启动。范例如下：
    ```bash
    　　[root@localhost root]#shutdown -r now ← 立刻关闭系统并重启

    　　[root@localhost root]#shutdown -r 23:59 ← 指定在23：59时重启动
    ```
3. 重新启动计算机的reboot命令

    顾名思义，reboot命令是用来重新启动系统的。常用的参数如下：

    -f 参数：不依正常的程序运行关机，直接关闭系统并重新启动计算机。

    -I 参数：在在重新启动之前关闭所有网络接口。

    虽然reboot命令有个参数可以使用，但是一般只需要单独运行reboot命令就可以了

## 文件与目录的操作
1. 列出文件列表的ls命令

    ls（list）命令是非常有用的命令，用来显示当前目录中的文件和子目录列表。配合参数的使用，能以不同的方式显示目录内容。范例如下：

    显示当前目录的内容：
    ```bash
    　　[tony@free tony]$ ls

    　　Desktop mail myinstall.log test.txt ← 有两个目录及两个文件夹
    ```
    当运行ls命令时，并不会显示名称以“.”开头的文件。因此可加上“-a”参数指定要列出这些文件。范例如下：
    ```bash
    　　[tony@free tony]$ ls –a
    ```
    以“-s”参数显示每个文件所有的空间，并以“-S”参数指定按所有占用空间的大小排序。范例如下：
    ```bash
    　　[tony@free tony]$ ls –s –S

    　　4 Desktop 4 mail 24 myinstall.log 4 test.txt
    ```
    在ls命令后直接加上欲显示的目录路径，就会列出该目录的内容。范例如下：
    ```bash
    　　[tony@free tony]$ ls –l/usr/games
    ```
2. 切换目录的cd命令

    　　cd(change directory)命令可让用户切当前所在的目录。范例如下：
    ```bash
    　　[tony@free home]$ cd tony ← 切换到当前目录下的tony子目录


    　　[tony@free tony]$ cd .. ← 切换到上一层目录

    　　[tony@free home]$ cd / ← 切换到系统根目录

    　　[tony@free /]$ cd ← 切换到用户主目录

    　　 [tony@free tony]$ cd /usr/bin ← 切换到/usr/bin目录
    ```
3. 创建目录的mkdir命令

    Mkdir(make directory)命令可用来创建子目录。下面的范例会在所在目录下创建tool子目录。
    ```bash
    　　[tony@free tony]$ mkdir tool
    ```
4. 删除目录的rmdir命令

    rmdir(remove directory)命令可用来删除“空”的子目录。范例如下：
    ```bash
    　　[tony@free tony]$ rmdir tool ← 删除tool目录
    ```
5. 复制文件的cp命令

    cp(copy)命令可以将文件从一处复制到另一处。一般在使用cp命令将以个文件复制成另一个文件或复制到某个目录时，需要指定原始文件名与目的文件名或目录。范例如下：
    ```bash
    　　[tony@free tony]$ cp data1.txt data2.txt ← 将data1.txt复制成data2.txt

    　　[tony@free tony]$ cp data3.txt /tmp/data ← 将data3复制到/tmp/data目录中
    ```
    显示复制过程：加入-v参数可显示命令执行过程。范例如下：
    ```bash
    　　[tony@free tony]$ cp zip.txt zip2.txt ← 一般状态下不会显示复制过程

    　　[tony@free tony]$ cp –v zip.txt zip3.txt ← 以-v显示复制过程

    　　 zip.txt ->zip3.txt
    ```
    递回复制：加入“-R”参数可同时复制目录下的所有文件及子目录。范例如下：
    ```bash
    　　[tony@free tony]$ cp –v –R * backup ← 将所有文件（含子目录文件）复制到backup目录
    ```
6. 删除文件或目录的rm命令

    rm(remove)命令可以删除文件或目录。范例如下：
    ```bash
    　　[tony@free tony]$ rm myfile ← 删除指定的文件

    　　[tony@free tony]$ rm * ← 删除当前目录中的所有文件
    ```
    rm命令的常用参数如下：

    强迫删除：使用-f参数时，rm命令会直接删除文件，不再询问。范例如下：
    ```bash
    　　[tony@free tony]$ rm –f *.txt ← 强迫删除文件
    ```
    递回删除：-r 也是一个相当常用的参数，使用此参数可同时删除指定目录下的所有文件及子目录。范例如下：
    ```bash
    　　[tony@free tony]$ rm –r data ← 删除data目录（含data目录下所有文件和子目录）

    　　[tony@free tony]$ rm –r * ← 删除所有文件（含当前目录所有文件，所有子目录和子目录下的文件）
    ```
    强制删除指定目录：当使用-r参数删除目录时，若该目录下有许多子目录及文件，则系统会不间断地询问，以确认您的确要删除目录或文件。若已确定要删除所存目录及文件，则可以使用-rf参数，如此一来，系统将直接删除该目录中所有的文件及子目录，不再询问。范例如下：
    ```bash
    　　[tony@free tony]$ rm –rf tmp 强制删除tmp目录及该目录下所有文件及子目录
    ```
    显示删除过程：使用-v 参数。范例如下：
    ```bash
    　　[tony@free tony]$ rm –v
    ```
7. 让显示画面暂停的more命令

    为了避免画面显示瞬间就闪过去，用户可以使用more命令，让画面在显示满一页时暂停，此时可按空格键继续显示下一个画面，或按Q键停止显示。

    当使用ls命令查看文件列表时，若文件太多则可以配合more命令使用。范例如下：
    ```bash
    　　[tony@free bin]$ ls –al 　 more
    ```
    单独使用more命令时，可用来显示文字文件的内容。范例如下：
    ```bash
    　　[tony@free tony]$ more data.txt
    ```
8. 连接文件的cat命令

    cat(concatenate) 命令可以显示文件的内容（经常和more命令搭配使用），或是将数个文件合并成一个文件。范例如下：

    逐页显示 preface.txt的内容：
    ```bash
    　　[tony@free tony]$ cat preface.txt 　 more
    ```
    将preface.txt 附加到outline.txt文件之后：
    ```bash
    　　[tony@free tony]$ cat preface.txt >> outline.txt
    ```
    将new.txt和info.txt合并成readme.txt文件：
    ```bash
    　　[tony@free tony]$ cat new.txt info.txt > readme.txt 
    ```
9. 移动或更换文件，目录名称的mv命令

    mv（move）命令可以将文件及目录移动到另一个目录下面，或更换文件及目录的名称。范例如下：
    ```bash
    　　[tony@free backup]$ mv a.txt .. ← 将a.txt文件移到上层目录

    　　[tony@free backup]$ mv z1.txt z3.txt ← 将 z1.txt改名成z3.txt

    　　[tony@free backup]$ cd.. ← 切换到上一层目录

    　　[tony@free tony]$ mv backup.. ← backup 目录上移一层
    ```
10. 显示当前所在目录的pwd命令

    pwd(print working directory)命令可显示用户当前所在的目录。范例如下：
    ```bash
    　　[tony@free tony]$ pwd

    　　/home/tony ← 当前所在目录为“/home/tony”
    ```
11. 查找文件的locate命令

    locate命令可用来搜索包含指定条件字符串的文件或目录。范例如下：
    ```bash
    　　[tony@free tony]$ locate zh_CN 列出所有包含“zh_CN”字符串的文件和目录。
    ```
    由于locate命令是从系统中保存文件及目录名称的数据库中搜索文件，虽然系统会定时更新数据库，但对于刚新增或删除的文件、目录，

    仍然可能会因为数据库尚未更新而无法查得，此时可用root身份运行updatedb命令更新，为此数据库得内容正确。

12. 搜索字符串得grep命令

    grep命令可以搜索特定字符串来并显示出来，一般用来过滤先前得结果，避免显示太多不必要得信息。范例如下：
    ```bash
    　　[tony@free etc]$ grep text *.conf ← 搜索当前目录中扩展名为.conf且包含“text”字符串得文件。

    　　grep:amd.conf: ← 拒绝不符权限得操作

    　　grep:diskcheck.conf: ← 拒绝不符权限得操作

    　　grep:grub.conf ← 拒绝不符权限得操作
    ```
    若您是使用一般权限的用户运行，上例的输出结果会包含很多如“拒绝不符权限的操作之类的错误信息，可使用-s参数消除。
    ```bash
    　　 [tony@free etc]$ grep –s text *.conf
    ``` 
13. 重导与管道

    重导(redirect)可将某命令的结果输出导文件中，它有两中命令：“>”和“>>”。“>”可将结果输出到文件中，该文件原有的内容会被删除：“>>”则将结果附加到文件中，原文件内容不会被清除。范例如下：
    ```bash
    　　[tony@free tony]$ ls –a>dir.txt ← 将ls –a命令执行结果输出到dir.txt文件。

    　　[tony@free tony]$ cat data1.txt >> data2.txt ← 将 data1.txt 内容附加到data2.txt文件之后。
    ```
    通道(pipe)命令的符号是“　”，可将某命令的结果输出给另一命令，一下范例将ls命令的输出结果传给grep命令过滤：
    ```bash
    　　[tony@free etc]$ ls 　 gerp conf ← 搜索并显示ls命令运行结果中包含有“conf”字符串
    ```
    在举一个删除文件或目录的例子，可以利用yes命令重复输出“y”字符的特性，将结果传给rm命令，如此即可避免重复输入“y”：
    ```bash
    　　[tony@free tony]$ yes 　 rm –r mydir
    ```

## 使用光盘及软盘
在Linux的文字模式下要使用光盘或软盘，并不是只将光盘或软盘放入即可，用户需要运行加载的命令，才可读写数据。所谓加载就是将存储介质（如光盘和软盘）指定成系统中的某个目录（如/mnt/cdrom或mnt/floppy）。通过直接存取此加载目录，即可读写存储介质中的数据。

以下就来看看文字模式下的加载及卸载命令。

1. 加载的mount命令

    要使用光盘时先把光盘放入光驱，然后执行加载的mount命令，将光盘加载至系统中：
    ```bash
    　　[tony@free tony]# mount/dev/cdrom/mut/cdrom ← 加载光盘
    ```
    同理，使用软盘之前也需要和光盘一样，必须先加载后才能使用：
    ```bash
    　　[tony@free tony]# mount/dev/fd0/mut/floppy ← 加载软盘
    ```
2. 卸载的umount命令

    如果不需要使用光盘或软盘，则需先执行卸载命令之后，才能将光盘或软盘退出。范例如下：
    ```bash
    　　[tony@free tony]# umount / mnt/cdrom ← 光盘卸载
    ```
    在不使用软盘时执行umount 命令卸载软盘，再将软盘拿出
    ```bash
    　　[tony@free tony]# umount / mnt/ ← 软盘卸载
    ```

## 在后台运行程序
用户有时的程序有可能要花费很多时间，如果将它放在前台运行，将导致无法继续做其他事情，最好的方法就是将它放在后台运行，甚至可能希望在用户注销系统后，程序还可以继续运行。让我们看看那如何实现这一目的。
1. 在后台运行程序的&、bg命令

    将程序放到后台运行的最简单方法就是在命令最后加上“&”,范例如下：
    ```bash
    　　[tony@free root]# updatedb & ← 在后台执行 locate 数据库更新命令

    　　[tony@free root]# bg ← 将更新操作放到后台运行
    ```
2. 前台运行的程序fg命令

    如果用户当前已有程序在后台运行，可以输入fg命令，将它从背景中移到前台运行:
    ```bash
    　　[tony@free root]# fg ← 放到前台执行的命令会显示出来
    ```
3. 在退出后让程序继续运行的nohup命令

    此命令可使用户退出系统后，程序继续运行。范例如下：
    ```bash
    　　[tony@free rony]$ nohup myserver &
    ```
    　　然后用户就可以退出了，当再次登录的时候，可以用ps –aux命令看到程序仍在后台中运行。

## 任务调度命令
计算机有很多程序需要周期性的被运用，例如清理磁盘中不要的暂存盘、备份系统数据、检查远程服务器的邮件等。对于这些重复性的工作，其实不需要每次都辛苦地运行这些程序。可利用任务调度命令，指定系统定期在某个时间运行这些程序，轻轻松松完成想要执行的工作。

### 任务调度的crond常驻命令

crond是Linux用来定期执行程序的命令。当安装完成操作系统后，默认便会启动此任务调动命令。crond命令每分钟会定期检查是否有要执行的工作，如果有要执行的工作，便会自动执行该工作。由于任务调度中间的操作过程十分繁杂，我们只将任务调度文件的写法介绍给大家，供大家参阅：
```bash
　　Minute Hour Day Month DayOfWeek Command
```
在这段文字中除了“Command”是每次都必须指定的字段以外，其他自动皆可视需求自行决定是否指定。

## 任务调度的系统工作
/ect/crontab/文件是Linux系统工程的任务调度设置文件，其默认的内容如下：
```bash
　　SHELL=/bin/bash ← 指定执行任务调度工作时所使用的SHELL

　　PATH=/shin:/bin:/usr/sbin:/usr/bin ← 指定命令搜索的路径

　　MAILTO=root ← 指定将输出结果给root用户

　　HOME=/ ← 指定跟目录
```

## 任务调度的个人工作
除了上述任务调度的系统工作外，一般用户则可利用crintab命令，自行设置要定期执行的工作。

每个用户可执行crontab –e命令，编辑自己的任务调度设置文件，并在此文件加入要定期执行的工作。以下范例为tony用户编辑的任务调度设置文件：
```bash
　　[tony@free rony]$ crontab –e
```
执行上述命令后，即会进入VI文本编辑器自行编辑任务调度的工作。

## 删除调度工作任务
如果不想再定期执行任务调动中的工作，则可执行crontab –r命令删除所有任务调度的工作。范例如下：
```bash
　　[tony@free rony]$ crontab –r ← 删除任务调度中的工作

　　[tony@free rony]$ crontab -1 ← 再查看一次任务调度中的工作

　　no crontab for tony ← 已经没有任何任务调度工作
```
## 打包、压缩与解压缩
由于这是每一个Linux用户都会经常用到的基本功能，因此我们将介绍最常见到的打包、压缩和解压缩程序。

1. 打包文件的tar命令

    tar命令位于/bin目录中，它能将用户所指定的文件或目录打包成一个文件，不过它并不做压缩。一般Unix上常用的压缩方式是先用tar命令将许多文件打包成一个文件，再以gzip等压缩命令压缩文件。tar命令参数繁多，以下举例常用参数作说明：
    - 　　-c：创建一个新的tar文件；
    - 　　-v：显示运作过程信息；
    - 　　-f：在：指定文件名称；
    - 　　-z：调用gzip压缩命令执行压缩；
    - 　　-j：调用bzip2压缩命令执行压缩；
    - 　　-t：参看压缩文件内容；
    - 　　-x：解开tar文件。

    在此举一常用范例：
    ```bash
    　　[tony@free data]# tar cvf data.tar * ← 将目录下所有文件打包成data.tar

    　　[tony@free data]# tar cvf data.tar.gz * ← 将目录所有文件打包成data.tar再用gzip命令压缩

    　　[tony@free data]# tar tvf data.tar * ← 查看data.tar文件中包括了哪些文件

    　　[tony@free data]# tar xvf data.tar * ← 将data.tar解开 
    ```
2. 压缩与解压缩

    tar命令本身没有压缩能力，但是可以在产生的tar文件后，立即使用其他压缩命令来压缩，省区需要输入两次命令的麻烦。

    使用-z参数来解开最常见的.tar.gz文件：
    ```bash
    　　 [tony@free data]# tar –zxvf foo.tar.gz ←将文件解开至当前目录下
    ```
    使用-j参数解开tar.bz2压缩文件：
    ```bash
    　　 [tony@free data]# tar –jxvf linux-2.4.20tar.bz2 ←将文件解开至当前目录下
    ```
    使用-Z参数指定以compress命令压缩
    ```bash
    　　 [tony@free data]# tar –cZvf prcture.tar.Z*.tif 将该目录下所有.tif打包并命令压缩成.tar.Z文件
    ```

## 其他常用命令
Linux可用的命令相当多，本章我们只举例几个常用的命令进行说明，在以后的章节中，还会接触到许多其他命令。

1. 修改密码的passwd命令

    passwd（password）命令可让用户变更密码。范例如下：
    ```bash
    　　[tony@free tony]$ passwd

    　　Changing password for user tony

    　　Changing password for tony

    　　(current)UNIX password: ← 输入原密码

    　　New password: ← 输入新密码

    　　Retype new password ← 在此输入新密码

    　　passwd: all authentication tokens updated successfully ← 密码修改成功
    ```
2. 创建引导盘的mkbootdisk命令

    如果安装系统时，并没有制作引导盘，或者引导盘已经损害，可以在安装系统之后，利用mkbootdisk命令创建一张新的引导盘：
    ```bash
    　　[root@free root]# mkbootdisk ‘uname -r’
    ```
    执行上述指令便可以成功的创建一张引导盘了。请保存好出盘，已备紧急之用。

3. 显示与设置时间的date、clock和ntpdate命令

    date命令可以显示当前日期时间。范例如下：
    ```bash
    　　[tony@free rony]$ date

    　　-9月 8 10:00:00 CST 2006
    ```
    CST为中部标准时间

    clock命令也可以显示出系统当前的日期与时间，不过clock命令默认不允许一般用户执行，请用root账号执行：
    ```bash
    　　[root@free root]# clock

    　　公元2006年9月8日（周五）10时00分00秒 0.112604 seconds
    ```
    如果系统时间不正确要想更改，可以使用date命令来设置时间。用root账号如下操作：
    ```bash
    　　[root@free root]# date 09091200 ← 将时间设定为9月9日12点00分
    ```
    用户有时可能会苦于不知道标准时间。没关系，当前网络上也有校对时服务器提供的标准时间。因此可执行ntpdate命令，将系统时间设成与校时服务器一致：
    ```bash
    　　[root@free root]# ntpdate stdtime.microsoft.com ← 与微软校时服务器校时。
    ```
    然后再执行一次date命令，就会发现系统时间已经更改。不过这样还没有结束，还需要执行clock –w 命令将更改的时间写入计算机的CMOS中，这样下次启动时才会使用更改过的时间。 范例如下：
    ```bash
    　　[root@free root]# clock –w
    ```

常用的基本命令我们就先介绍到这里，现在我们来讲些新手在使用过程中遇到的一些疑难问题的解决方法以及在操作过程中的一些应用技巧。 

### 如何进入文字模式
当安装Linux时，可选择自动后要进入文字模式或时图形模式。如果选择的时文字模式则可略过此说明；若是直接进入X Window的图形模式，仍可以使用下列方式，进入文字模式：
1. 在X Window中打开文字模式窗口

    在默认的GNOME窗口环境为例，在X Windows 下进入文字模式最简单的方式，就是在桌面空白处单击鼠标右键，执行“新增终端命令”，打开文字模式窗口。在文字模式窗口中可以用 Shift+Page Up和Shift+Page Dwon 键来卷动窗口内容。

2. 切换虚拟主控制台进入文字模式

    Linux主机在主控制台(console)下提供了7个虚拟主控台，在每一个虚拟主控台中可以运行各自的程序。可以在登陆X Window系统后的任何时间，按下Ctrl+Alt+Fn键来切换到其他的虚拟主控台。其中的Fn是指F1～F7的功能键。

3. 启动直接进入文字模式

    要设置启动时直接进入文字模式，可以使用任何一个文本编辑器，打开/etc/inittab文件，在文件中查找“id:5:initdefault:”这一段文字，并将其改为“id:3:initdefault:”即可。

### /etc/inittab设置错误，导致无法启动

若修改/etc/inittab后无法正常启动，则可以在启动时采用单人模式进入系统，重新修改inittab设置文件以解决问题。

若使用GRUB为引导装载程序时，只要在启动显示菜单画面时，按a键，并在命令行输入一下参数以进入单人模式：
```bash
grub append > ro root=LABEL=/s ← 只要在命令行原来的语句后，加“S”即可
```
使用LILO为引导装载程序时，同样在启动显示LILO菜单画面时，按Ctrl+X键，切换到文字模式的LILO登录画面，并输入以下参数即可：
```bash
boot: linux s ← 表示系统直接讲进入单人模式
```
### 如何查询命令的用法

在linux系统中，如果用户某命令的功能不大清楚，可以使用man命令查询帮助：
```bash
　　[root@free root]# man shutdown ← 以man命令查询稍后要介绍的shutdown命令的用法。
```
　　大多数命令的语法，还可以通过-h或—help参数查询。例如shutdown命令的语法可以运行shutdown –h或上述的man shutdown 命令查得。

### 避免按Ctrl+Alt+Del重新启动系统

在linux中直接按下Ctrl+Alt+Del三个键后就会重新启动，如果不希望任何人利用这组组合键随意重新启动计算机，请用文本编辑器修改/etc/inittab文件：
```bash
　　#ca::ctrlaltdel:/sbin/shut down –t3 –r now ← 在此之前加上“＃”。
```
存盘后重新启动计算，以后就无法用Ctrl+Alt+Del 键重新启动了。

### 文字模式下的中文信息出现乱码，怎么办
在此版的Red Hat Linux中，若是在X Window打开文字模式窗口，以文字模式操作，则所有中文文件名、月份，甚至部分信息都可以正常的以中文显示。但在文字模式的虚拟控制台中，这些中文信息，则会变成乱码，此时请如下操作，可将此信息改成英文显示：
```bash
　　[root@free root]$ LANG=C

　　[root@free root]# ls –l
```
运行LANG=C命令后原来以中文显示（乱码）的部分，变成英文了

若想改回原来的设置，则只要再执行LANG=zh_CN命令即可：
```bash
　　[root@free root]$ LANG=zh_CN
```

### 看不到中文文件名

如果加载的存储介质中含有中文文件名，需要再运行mount命令，再加上“-o iocharset＝cp950”参数，这样才能看到此保存媒体内的中文文件名。例如加载光盘就可以执行以下命令：
```bash
　　[root@free root]# mount –o iochatset=cp950/dev/cdrom/mnt/cdrom
```

### 如何调换光盘
当光盘已经被加载成为一个目录时，按下光驱上面的退出按钮，将无法退出光盘，必须先将光盘卸载后，才能退出光盘。

若当前所在之处就是光盘加载目录（如/mnt/cdrom）,或有其他用户正在此目录下，将无法成功的卸载它，当然也不能退出光盘：
```bash
　　[root@free root]# umount/mnt/cdrom

　　umount:/mnt/cdrom: device is busy ← 此光盘正在被使用中
```
先将工作目录切换到别处，或要求其他用户离开此目录，才可卸载目录并退出光盘。而更换关盘之后，记得要将光盘再次加载才能使用。

### 允许一般用户加载光盘或软盘
Linux 操作系统默认只允许root用户才能运行mount命令，若是一般用户执行上述命令，则会出现以下的错误信息：
```bash
　　[root@free root]# mount/dev/cdrom/mut/cdrom

　　mount:only root can do that ← 只有root用户才能执行此命令
```
因此若要允许一般用户也能加载光盘或软盘，请修改/ect/fstab/设置文件:
```bash
    /dev/cdrom/mut/cdrom udf,iso9660 noauto,owner,kudzu,ro,user ← 若要让一般用户也加载光盘，请在此处加上“，user”项目。
```