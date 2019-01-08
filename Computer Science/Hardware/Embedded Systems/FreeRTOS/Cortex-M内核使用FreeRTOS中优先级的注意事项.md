# Cortex-M内核使用FreeRTOS中优先级的注意事项

## 有效优先级

Cortex-M内核中优先级配置寄存器最多8位，因此，Cortex-M架构自身最多允许256级可编程优先级。但是绝大多数的微控制器只是使用了其中的一部分优先级。

在RTOS中，RTOS中断嵌套方案将有效的中断优先级分为两组：一组可以通过RTOS临界区屏蔽，另一组不受RTOS影响，在FreeRTOS配置文件中的宏configMAX_SYSCALL_INTERRUPT_PRIORITY则是两组中断优先级的边界。逻辑优先级高于此值的中断不受RTOS的影响，最优值取决于微控制器使用的优先级配置寄存器的位数。

## 与数值相反的优先级值和逻辑优先级设置

Cortex-M内核中，逻辑优先级和优先级值是反函数关系，也就是说，一个中断的优先级数值越低，逻辑优先级越高。

在FreeRTOS中，以“FromISR”结尾的FreeRTOS函数是具有中断调用保护的，但是这些函数也不可以被逻辑优先级高于configMAX_SYSCALL_INTERRUPT_PRIORITY的中断服务函数调用。也就是说，任何使用RTOS API的函数的中断服务例程的中断优先级**数值大于等于**configMAX_SYSCALL_INTERRUPT_PRIORITY的值，因此保证中断的逻辑优先级等于或者低于configMAX_SYSCALL_INTERRUPT_PRIORITY。

## Cortex-M内部优先级概述

Cortex-M内核的中断优先级寄存器是以最高位MSB对齐的，如果使用了3位来表达优先级，则这3个位位于bit5 ~ bit7。其它位可以设置为任意数值，为了兼容性起见最好都设置为1。

在FreeRTOS中，在ISR中调用RTOS API的函数的中断**逻辑优先级**必须低于或者等于configMAX_SYSCALL_INTERRUPT_PRIORITY。

可以在配置文件中设置宏configMAX_SYSCALL_INTERRUPT_PRIORITY以及configKERNEL_INTERRUPT_PRIORITY的值。这两个宏需要根据Cortex-M内核自身的情况进行设置，要**以最高有效位**对齐。比如某微控制器使用中断优先级寄存器中的对位，需要设置configKERNEL_INTERRUPT_PRIORITY为5，则有：

```C
#define configKERNEL_INTERRUPT_PRIORITY (5 << ( 8 - 3 ))
```

宏configKERNEL_INTERRUPT_PRIORITY制定RTOS内核使用的中断优先级，因为RTOS内核不能抢占用户任务，因此这个宏一般设置为硬件支持的最小优先级，比如在CM3内核的设置中有：

```C
#define configKERNEL_INTERRUPT_PRIORITY 255
```

在FreeRTOS中使用BASEPRI寄存器来实现临界区（BASERPRI是优先级屏蔽寄存器，优先级值大于或者等于BASEPRI中的值的中断都会被屏蔽）。那些需要在中断调用时保护的API函数，FreeRTOS使用BASEPRI实现中断保护临界区。当进入临界区时将BASEPRI的值设置为configMAX_SYSCALL_INTERRUPT_PRIORITY，退出临界区时恢复为原来的值或者设置为0。
