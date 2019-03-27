# FreeRTOS任务分析（三）

## FreeRTOS任务切换分析 

任务的另一个比较重要的问题就是任务的切换，也就是上下文切换的过程。任务切换的首要任务是取得优先级最高的处于就绪态的任务，为了更快的找到优先级最高的就绪任务，任务代码通常是精心设计过的，在不同的平台上，甚至会使用硬件平台特有的指令，使用硬件移植层所提供的内嵌的汇编代码。下面以Cortex-M3为例子，讲述FreeRTOS任务切换的过程：

FreeRTOS有两种方法可以引起任务的切换：

1. 任务主动让步，如常用的调用vTaskDelay()、vTaskDelayUntil()函数等，或者直接调用taskTIELD()进行主动让步。
2. 系统节拍的时钟中断。

前文也有提到，在Cortex-M3中，任务的上下文切换都是通过使能PendSV中断来实现的。PendSV是一类可延迟的异常，因此可以在ISR执行完毕后才执行PendSV异常处理例程。在PendSV中断服务程序中，找到最高优先级的就绪任务，然后让这个任务获得CPU运行权，从而完成任务切换。在CM3中，对于第一种方法，函数中会执行到的宏定义是：

```C
#define portYIELD()				__asm volatile ( "	SVC	%0	\n" :: "i" (portSVC_YIELD) : "memory" )
#define portYIELD_WITHIN_API() 													\
{																				\
	/* Set a PendSV to request a context switch. */								\
	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;								\
																				\
	/* Barriers are normally not required but do ensure the code is completely	\
	within the specified behaviour for the architecture. */						\
	__asm volatile( "dsb" ::: "memory" );										\
	__asm volatile( "isb" );													\
}
```

前文也曾分析过，PendSV的中断是通过产生代码：

portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;

来实现的，通过将NVIC中的PendSV置位，将PendSV中断设置为挂起状态，等到优先级高于PendSV的中断执行完毕后，PendSV中断服务程序将会被执行：

```C
void xPortPendSVHandler( void )
{
	/* This is a naked function. */

	__asm volatile
	(
	"	mrs r0, psp							\n" /* 保存psp的栈顶地址到r0中 */
	"	isb									\n"	/* 指令清流水线 */
	"										\n"
	"	ldr	r3, pxCurrentTCBConst			\n" /* r3保存当前TCB的地址 */
	"	ldr	r2, [r3]						\n"	/* r2保存当前任务堆栈地址的地址 */
	"										\n"
	"	stmdb r0!, {r4-r11}					\n" /* 将当前任务的寄存器r4 ~ r11保存到堆栈中，并更新栈顶地址 */
	"	str r0, [r2]						\n" /* 将新的堆栈地址保存到TCB中 */
	"										\n"
	"	stmdb sp!, {r3, r14}				\n"	/* 临时保存r3以及r14的内容 */
	"	mov r0, %0							\n"	/* 进入临界区 */
	"	msr basepri, r0						\n"	
	"	bl vTaskSwitchContext				\n"	/* 切换任务 */
	"	mov r0, #0							\n"
	"	msr basepri, r0						\n" /* 退出临界区 */
	"	ldmia sp!, {r3, r14}				\n"	/* 恢复r3和r14的内容，r3在vTaskSwitchContext中可能已经被修改，指向新的TCB的地址 */
	"										\n" 
	"	ldr r1, [r3]						\n"
	"	ldr r0, [r1]						\n" /* 当前TCB的栈顶地址保存在r0中	*/
	"	ldmia r0!, {r4-r11}					\n" /* r4 ~ r11出栈 */
	"	msr psp, r0							\n"	/* 当前TCB栈顶地址即为新的PSP栈顶地址 */
	"	isb									\n"
	"	bx r14								\n"	/* 异常返回，r14中保留有异常返回所需要的信息 */
	"										\n"
	"	.align 4							\n"
	"pxCurrentTCBConst: .word pxCurrentTCB	\n"
	::"i"(configMAX_SYSCALL_INTERRUPT_PRIORITY)
	);
}
```

上述代码就是FreeRTOS在针对CM3中的任务切换的函数，也就是PendSV异常处理的函数。在分析该函数前首先要说明Cortex-M3的硬件架构方面的内容：

#### 主进程堆栈指针MSP以及进程堆栈指针PSP

在Cortex-M3中有两类堆栈，一类是用于操作系统内核以及处理异常所使用的主进程堆栈，MSP主进程对扎指针就是指向该堆栈的栈顶的指针，主进程堆栈只有一个。另一类是用于每个任务的堆栈PSP，PSP指向当前运行的用户创建的任务的堆栈栈顶，用户创建的任务可以有多个，所以用户堆栈也可以有多个。

在进行PendSV异常来切换上下文的时候。处理异常的是操作系统内核，所以运行PendSV异常处理代码的时候，使用的堆栈是主进程堆栈，因此这时候的SP指的是MSP。切换上下文的工作简单来说，就是将当前运行任务中需要软件保存的寄存器保存到该任务使用的任务堆栈当中，然后将更新后的任务堆栈栈顶地址保存。接着进入临界区，选择处于就绪态的优先级最高的任务调度运行。接着退出临界区，将需要软件恢复的寄存器恢复，将新的任务堆栈栈顶地址赋给psp，最后退出异常处理程序。

下面来说明整个上下文切换的流程：

PendSV中断产生，硬件自动将R0 ~ R3、R12、PC、LR、xPSR压入堆栈。因此，上述寄存器不需要软件进行保存，同时在软件代码中可以使用R0 ~ R3、R12作为自用。

```
msr r0, psp
isb
```

将psp内容保存在r0中，r0的内容为当前运行任务的任务栈顶地址。接着清空指令流水线。

```
ldr r3, pxCurrentTCBConst
ldr r2, [r3]
```

r3保存着当前运行任务的TCB地址，r2保存着当前运行任务的TCB的第一个成员。通过前文我们知道，这个成员是指向任务栈顶的指针，也就是说保存着当前运行任务的地址的指针的地址。

```
stmdb r0!, {r4-r11}
```

将寄存器r4 ~ r11保存到当前运行任务的堆栈当中，并且更新堆栈栈顶地址（保存在r0中）。

```
str r0, [r2]
```

将新的堆栈地址更新TCB中的栈顶指针。

```
stmdb sp!, {r3, r14}
```

保存r3、r14到主堆栈中。因为在运行vTaskSwitchContext后返回，r3指向的时TCB的地址，这个TCB可能已经发生变化（指向了新的最高优先级的地址）。而r14保存着异常返回后需要的信息。

```
mov r0, %0
msr basepri, r0
bl vTaskSwitchContext
mov r0, #0
msr basepri, r0	
```

进入临界区，执行vTaskSwitchContext，pxCurrentTCB可能会发生变更。执行完毕后退出临界区。

```
ldmia sp!, {r3, r14}
```

恢复r3、r14，此时r3指向的可能是另一个任务的TCB了。

```
ldr r1, [r3]
ldr r0, [r1]
```

r0保存的是新的TCB的任务堆栈栈顶地址。

```
ldmia r0!, {r4-r11}
msr psp, r0
```

将寄存器r4 ~ r11软件出栈，并将新的任务堆栈栈顶赋给psp。

```
isb
bx r14
```

清指令流水线，执行bx r14则退出异常。异常退出后硬件自动将r0 ~ r3、R12、LR、PC、xPSR出栈。

至此，任务切换的工作完成。
