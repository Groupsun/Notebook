# Screen

## 功能说明
使用telnet或SSH远程登录linux时，如果连接非正常中断（远程机器关闭），重新连接时，系统将开一个新的session，无法恢复原来的session。

screen命令可以解决这个问题。Screen工具是一个终端多路转接器，在本质上，这意味着你能够使用一个单一的终端窗口运行多终端的应用。

## 语法
```bash
screen [-AmRvx -ls -wipe][-d <作业名称>][-h <行数>][-r <作业名称>][-s ][-S <作业名称>]
```
screen为多重视窗管理程序。此处所谓的视窗，是指一个全屏幕的文字模式画面。通常只有在使用telnet登入主机或是使用老式的终端机时，才有可能用到screen程序。

## 用法
1. 开启一个screen并进入：
```bash
screen -S test1     #这里用-S表示给screen起名字，以后切换的时候比较方便。这时就进去了screen test1了。里面可以跑一些程序。
```

2. 退出该screen：
```bash
Ctrl-a d                       #这里的ctrl-a表示同时按ctrl键和a键，然后再单独按d键。这时退到跟终端。
```

3. 在根下查看开启的所有screen：
```bash
screen -ls                  # 比如会有25764.test1     (Detached)等。
```

4. 在根下进入某个screen：
```bash
screen -r test1          #这时又进入到指定的screen session中，或者screen -r 25764。
```

5. 关闭该session：
```bash
exit                              #在该screen中退出，退到根下。
```

6. 强制连接，踢掉原来的
```bash
screen -D -r 20791
screen -x -r 20791（共享原来的）
```

7. 当前窗口假死，强制关闭当前窗口，跳到下一个
```bash
Ctrl-a  K
```

8. 上翻
```bash
Ctrl-a  Ctrl-[  
```

### 补充说明
screen为多重视窗管理程序。此处所谓的视窗，是指一个全屏幕的文字模式画面。通常只有在使用telnet登入主机或是使用老式的终端机时，才有可能用到screen程序。

## 参数：
-A 　将所有的视窗都调整为目前终端机的大小。

-d <作业名称> 将指定的screen作业离线。

-h <行数> 指定视窗的缓冲区行数。

-m 　即使目前已在作业中的screen作业，仍强制建立新的screen作业。

-r <作业名称> 恢复离线的screen作业。

-R 　先试图恢复离线的作业。若找不到离线的作业，即建立新的screen作业。

-s 　指定建立新视窗时，所要执行的shell。

-S <作业名称> 指定screen作业的名称。

-v 　显示版本信息。

-x 　恢复之前离线的screen作业。

-ls或--list 　显示目前所有的screen作业。

-wipe 　检查目前所有的screen作业，并删除已经无法使用的screen作业。

### 常用screen参数
在每个screen session 下，所有命令都以 ctrl+a(C-a) 开始。

C-a ? -> Help，显示简单说明

C-a c -> Create，开启新的 window

C-a n -> Next，切换到下个 window 

C-a p -> Previous，前一个 window 

C-a 0..9-> 切换到第 0..9 个window

Ctrl+a [Space] -> 由視窗0循序換到視窗9

C-a C-a -> 在两个最近使用的 window 间切换 

C-a x -> 锁住当前的 window，需用用户密码解锁

C-a d -> detach，暂时离开当前session，将目前的 screen session (可能含有多个 windows) 丢到后台执行，并会回到还没进 screen 时的状态，此时在 screen session 里    每个 window 内运行的 process (无论是前台/后台)都在继续执行，即使 logout 也不影响。 

C-a z -> 把当前session放到后台执行，用 shell 的 fg 命令則可回去。

C-a w -> Windows，列出已开启的 windows 有那些 

C-a t -> Time，显示当前时间，和系统的 load 

C-a K -> kill window，强行关闭当前的 window

C-a [ -> 进入 copy mode，在 copy mode 下可以回滚、搜索、复制就像用使用 vi 一样

    C-b Backward，PageUp
    C-f Forward，PageDown
    H(大写) High，将光标移至左上角 
    L Low，将光标移至左下角 
    0 移到行首 
    $ 行末 
    w forward one word，以字为单位往前移 
    b backward one word，以字为单位往后移 
    Space 第一次按为标记区起点，第二次按为终点 
    Esc 结束 copy mode 

C-a ] -> Paste，把刚刚在 copy mode 选定的内容贴上

C-a : ->输入命令(quit/split等)：

除了依次退出/杀死当前Screen会话中所有窗口这种方法之外，还可以使用快捷键C-a :，然后输入quit命令退出Screen会话。需要注意的是，这样退出会杀死所有窗口并退出其中运行的所有程序。其实C-a :这个快捷键允许用户直接输入的命令有很多，包括分屏可以输入split等，这也是实现Screen功能的一个途径，不过个人认为还是快捷键比较方便些。


来自 <http://blog.csdn.net/lykangjia/article/details/17717065> 
