# Arduino在Visual Studio中的调试
          
使用插件调试（Visual Micro Arduino Studio）
   不难看出Atmel Studio和Microsoft Visual Studio 都是一伙人开发的，整个开发界面方式非常相似，也都对Arduino进行了很好的支持。这里一并描述。

## 配置环境
首先打开开发软件，在菜单-工具-扩展与更新（英文版自行对照）
                  
找到Arduino ide for visual studio ，找不到就在右边搜索栏里直接输入Arduino，点击安装即可。
安装完成后就会菜单中就会出现vMicro选项。

当然Arduino 官方IDE软件也不能少 安装完后即可。

点击菜单-vMicro-Visual Micro Explorer 对Arduino 路径进行设置。
                 
IDE 和 Board中选择好自己的版本和板子、芯片型号。

以上步骤网上很多详细介绍。

特别说明：这个插件是付费的，不掏钱是不能进行调试的，可以根据自己情况选择一年的或者不限期的，20-45美刀 支持支付宝付款（实时汇率）。选择Non-commercial/student 就可以啦 土豪就无所谓了。掏钱了，就能收到 license key ，这里就不啰嗦了。
 
## 开始调试

打开Arduino project （选择一个arduino 文件进行调试，如果要从头开发就新建一个，该起名起名该保存保存）。
    
设置断点
根据vMicro官方说明断点设置有一点要求，可能和我们平时的设置有点差异。原文地址http://www.visualmicro.com/page/User-Guide.aspx?doc=Debugging-Walkthrough-3.html
         
要想显示数据需要进行设置
在断点处右键-条件/执行 
注：如果要查看LOOP内的数据时，最好设置一下断点触发条件，否则你会烦死的。
            
这里输入完，按回车就可以了
这个说明一下，设置条件的意义就是 当收到的这个字符串长度大于0 的时候再显示出这个字符串的值，然后停在这里别跑，如果选择了继续执行，程序就继续该干嘛干嘛去了。
如果要显示好几个数据值｛数据A｝｛数据B｝……这样就可以。如果这个要查看的数据写错了或者在当前位置非法，编译的时候就会报错。
硬件连接和设置
说完软件说硬件，不管是开发板还是自己的板子，调试需要串口接入，这里可以设置烧写和调试分别使用什么接口。
点击vMirco-Uploader，如果打算用串口烧写就选择好串口，当然你的板子需要支持串口写入（需要bootloader），如果打算使用别的烧写器就在programmer中选择对应的烧写器，可以勾选一直使用烧写器烧写的选项。
         
在调试选项中设置调试串口信息
vMirco-Debugger-Override Local Port / Speed
勾选Automatic Debugging 可以直接进入调试模式
     
串口无所谓是板载USB转232 还是RX TX 引出了再接USB-232 都可以
开始调试
点击vMirco-Build & Upload
等烧写完成后就进入了调试模式，这个时候串口窗口会打开
     
这里我们在串口发送一些数据，看一下断点处的情况，按回车发送   
此时 程序暂停在了断点位置，并把获取的信息显示出来
vMirco中Continue/Step Next 也会亮起，如果设置多个断点，点击即可进行下一步跟踪。
    
这里特别说明一下串口调试窗口中CStr按钮，vMirco官网给出了详细说明，原文地址http://www.visualmicro.com/page/User-Guide.aspx?doc=Serial-Monitor.html
这里可以输入带有格式符的内容，特别说明一下
 
如果输入了特殊字符数据将会显示乱码，不会影响程序正常解析，只是你看不到。
     
     
关闭串口或者点击vMirco-Stop Debug就可以结束调试。
使用Atmel-ICE进行调试
Arduino毕竟使用的还是Atmel的芯片，自从microchip收购了Atmel之后，对这一部分的扶持力度也有加强，之前大家总是说Arduino只能当玩具玩，做项目还是SMT32，现在官方给出的说法是Atmel的稳定性其实更适合工业环境（来自官方的说法）。在我看，使用什么系统搭建平台，完全取决于开发成本，这里说的成本包括开发的时间成本、开发配套的成本以及BOM成本，至于能不能胜任项目需求，我想各大厂家都会有相应的芯片可以选择。
Atmel-ICE 官方报价RMB594.31
 
BASIC版本就可以了 FULL KIT里面只多了很多接口，可以根据自己情况购买 ，这里使用的是328P的芯片，只有ICSP接口，所以只能使用6Pin的ISP线。
特别说明一下，Atmel-ICE自己没有供电，板子需要单独供电（引用togke（弘毅）老大的话"带供电的都是土路子，人家大厂的从来不带供电"）
硬件连接与设置
Atmel-ICE Basic 配线一边10Pin 一边 6Pin ，10Pin接在对应的AVR接口，6Pin接到板子的ICSP接口
      
 
这里注意线序，别插反了。
打开Atmel Studio 菜单中选择工具-Device Programming。
 
Tool中选择Atmel-ICE，芯片型号，点击Apply，程序会读出芯片相关信息。
 
点击Device signature 下方的Read （这一步很关键如果失败则无法进行后续工作），将会显示芯片当前的状态信息，以及内存、熔断标记。
    
在vMirco中Uploader-Programmer,选择Atmel Studio ATMEL-ICE (ISP)，Debugger中选择Atmel Studio Debugger 。
  
点击菜单调试-最下方程序Properties…
 
Tool选项卡中 Selected debugger/programmer 中选择Atmel-ICE
Interface 选择debugWIRE（ISP模式不能调试只可以烧写程序）
 
点击菜单调试-Start Debugging and Break 当然点击Continue 或者逐语句、逐过程、运行到光标处 都可以。
 
程序会弹出提示，修改DWEN标记，因为这里使用了debugWIRE方式进行调试，具体介绍网上很多，有兴趣的自行学习。这里只能同意。确认OK
   
开始调试
 
注：这里调试的文件不再是原来的ino文件而是cpp，设置断点。
 
这里就可以使用任何一个串口调试工具进行数据发送。
程序停到断点处，我们可以添加监视
 
右键要查看的变量选择添加监视
 
下方就可以显示对应变量的相应信息，如果操作内存右侧可以看到相应的变化
 
在Atmel-debugger 工具栏选项卡中可以查看IO引脚相应的数据变化
 
结束调试
结束调试的时候一点要点击菜单调试-Disable debugWIRE and Close按钮，否则后果DWEN标记为不能被改回。原因很简单debugWIRE占用了芯片的Reset引脚进行调试。
 
官方给出了明确说明
 
注：如果很不幸，没有按照标准作业操作，后面将无法在进行烧写操作，解决方案，打一个断点记得在cpp文件中，然后使用逐语句、逐过程、运行到光标处 都可以当开始调试的时候点击停止调试或者Disable debugWIRE and Close按钮进行挽回。之后最好再查看一下熔断标记位，将DWEN去掉。
 
再次调试
有时会出现正常结束调试完后，需要再次进行调试时报错的现象
