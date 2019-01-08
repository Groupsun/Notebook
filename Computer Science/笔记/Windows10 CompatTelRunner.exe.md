# Windows10 CompatTelRunner.exe

Windows10开机后一段时间内磁盘占用率异常的高，发现是这个进程惹的祸，而且只是一个用户改善反馈的进程。直接禁用就好了：

1. ![avatar](./pictures/win10_compatTelRunner.png)

2. 右键点开【这台电脑】，点【管理】，点【服务和应用程序】点【服务】，在右边框里【superfetch】、【windows search】、【HomeGroupListener】、【HomeGroupProvider】的启动类型设置成【禁用】。

You can disable the scheduled tasks that starts CompatTelTunner.exe by looking in the Task Scheduler.

Computer Management – System Tools – Task Schedule Library – Microsoft – Windows – Application Experience or Start - Run - taskschd.msc

Name: Microsoft Compatibility Appraiser

Location: \Microsoft\Windows\Application Experience

Collects program telemetry information if opted-in to the Microsoft Customer Experience Improvement Program.

Right click on “Microsoft Compatibility Appraiser” and select “Disable” By default, it is set to start if there is ANY network connection.

The executable is located here: C:\Windows\System32\CompatTelRunner.ex

You may also want to look at the following:
Customer Experience Improvement Program states

If the user has consented to participate in the Windows Customer Experience Improvement Program, this job collects and sends usage data to Microsoft. However, it is set to run even if opted out