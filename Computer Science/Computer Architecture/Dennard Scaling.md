Dennard Scaling: IBM的Dennard在1974年的文章中提出来的定律：
    当MOSFET（金属氧化物半导体场效应晶体管）的物理尺寸缩小时，其功率密度保持不变（开关晶体管所需要的电压和电流都降低了）。
当晶体管制程缩小到65nm以下的时候，由于量子物理效应开始“泄露”电子：丹纳德缩放失效 - 随着尺寸的缩小，开关晶体管所需要的电压和电流并没有成比例的降低。
随着MOSFET尺寸越来越小，热量泄露越来越严重。可以理解为，动态功率中以热功散发的比重越来越大。CMOS电路中的动态功率与电容负载、电压以及频率相关：
Power_dynamic ~ 1/2 x CapacitiveLoad x Voltage^2 x Frequencyswitched
因此，处理器主频的增长速度也随之减缓。即使摩尔定律仍然在继续，但是往同样面积的芯片当中塞更多的晶体管并不能等比例的增强芯片的性能。 频率墙。
解决？ - 什么都不做（某个核空闲时，停止该核的时钟，暗硅）；DVFS（Dynamic Voltage-Frequency Scaling，动态电压-频率缩放）；超频；为典型的情况做出设计（过热降频）。“三栅”晶体管减轻电子泄露。多核 - 避开频率墙。