# 解决用chrome打开B站播放器乱码问题

一开始以为是编码的问题，结果打开工具发现编码已经是Unicode(UTF-8)，后来才发现原来是是字体渲染的问题。

解决方法：
在chrome地址栏输入：chrome://flags/#disable-direct-write 进入实验性功能，启用DirectWrite即可。
顺带一提chrome版本是48.0.2564.97