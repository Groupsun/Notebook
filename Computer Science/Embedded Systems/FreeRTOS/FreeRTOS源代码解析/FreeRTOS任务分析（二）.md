# FreeRTOS任务分析（二）

上一节分析了静态任务是怎么创建的，下面来分析动态任务的详细创建流程。

FreeRTOS创建动态任务的函数是xTaskCreate()，也是最常用的函数API接口之一。该函数有效的前提是configSUPPORT_DYNAMIC_ALLOCATION的取值为1。

```C
BaseType_t xTaskCreate(	TaskFunction_t pxTaskCode,
							const char * const pcName,
							const configSTACK_DEPTH_TYPE usStackDepth,
							void * const pvParameters,
							UBaseType_t uxPriority,
							TaskHandle_t * const pxCreatedTask )
	{
		TCB_t *pxNewTCB;
		BaseType_t xReturn;

		/* 如果机器的堆栈往高地址的方向增长 */
		#if( portSTACK_GROWTH > 0 )
		{
			/* 分配内存空间给TCB，分配的方法依赖于机器上对malloc函数的实现，实际上就是使用了FreeRTOS提供的5中内存分配方法中的哪一种 */
			pxNewTCB = ( TCB_t * ) pvPortMalloc( sizeof( TCB_t ) );
			
			if( pxNewTCB != NULL )
			{
				/* 分配内存给任务栈空间 */
				pxNewTCB->pxStack = ( StackType_t * ) pvPortMalloc( ( ( ( size_t ) usStackDepth ) * sizeof( StackType_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

				if( pxNewTCB->pxStack == NULL )
				{
					/* 无法分配内存空间给任务栈空间，释放TCB */
					vPortFree( pxNewTCB );
					pxNewTCB = NULL;
				}
			}
		}
		#else /* portSTACK_GROWTH */
		{
			/* 如果机器的堆栈往低地址的方向增长，则先分配内存空间给任务栈空间，然后再分配内存给TCB */
			
			StackType_t *pxStack;
			pxStack = pvPortMalloc( ( ( ( size_t ) usStackDepth ) * sizeof( StackType_t ) ) );

			if( pxStack != NULL )
			{
				pxNewTCB = ( TCB_t * ) pvPortMalloc( sizeof( TCB_t ) );

				if( pxNewTCB != NULL )
				{
					pxNewTCB->pxStack = pxStack;
				}
				else
				{
					vPortFree( pxStack );
				}
			}
			else
			{
				pxNewTCB = NULL;
			}
		}
		#endif /* portSTACK_GROWTH */

		if( pxNewTCB != NULL )
		{
			#if( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
			{
				/* 标记为使用动态内存分配的方法生成的任务堆栈 */
				pxNewTCB->ucStaticallyAllocated = tskDYNAMICALLY_ALLOCATED_STACK_AND_TCB;
			}
			#endif /* configSUPPORT_STATIC_ALLOCATION */
			
			/* 初始化TCB，同时加入准备态任务列表中 */
			prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) usStackDepth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, NULL );
			prvAddNewTaskToReadyList( pxNewTCB );
			xReturn = pdPASS;
		}
		else
		{
			xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
		}

		return xReturn;
	}
```

实际上这个函数和xTaskCreateStatic()差别不大，只是xTaskCreateStatic()在实现里面不会对任务所需要的TCB以及任务堆栈进行初始化，而是要用户自己提供（静态分配内存），也就是说这部分用户自己申请的内存空间在该任务删除的时候需要自己释放，否则会造成内存泄漏。而xTaskCreate()则会在函数实现中创建堆栈空间，当该任务被删除的时候，内核也会自动释放着部分的内存空间。分配内存的方法取决于FreeRTOS为该机器使用何种分配方式（Heap_1.c ~ Heap_5.c）。此外还要考虑机器堆栈的增长方向，是往高地址方向增长还是往低地址方向增长。为了避免栈溢出的时候对TCB造成破坏，因此当栈空间往高地址方向增长的时候，先分配TCB，再分配栈空间。当栈空间往低地址方向增长的时候，先分配栈空间，再分配TCB。

### vTaskDelayUntil()与vTaskDelay()

vTaskDelayUntil()与vTaskDelay()都会使调用它的任务进入阻塞态，进而发生任务间的上下文切换。两者的区别在于，vTaskDelay()处于阻塞态的时间是由调用函数时的参数决定的，但是离开阻塞态的时间不是绝对的，它与vTaskDelay()在任务执行函数中什么时候调用有关系。而调用vTaskDelayUntil()的函数则会在参数决定的时间内处于阻塞态，并且与何时调用这个函数无关，并且当参数决定的时间一过，这个任务就会退出阻塞态。

先来分析vTaskDelayUntil()的实现。vTaskDelayUntil()由于其唤醒时间的精确性，使得其一般用于让任务周期性执行：

```C
/* 
	pxPreviousWakeTime是标记任务上一次唤醒的时间，作为基准时间来计算下一次唤醒的时间
	xTimeIncrement是任务唤醒时刻之间的周期
 */
void vTaskDelayUntil( TickType_t * const pxPreviousWakeTime, const TickType_t xTimeIncrement )
{
	TickType_t xTimeToWake;
	BaseType_t xAlreadyYielded, xShouldDelay = pdFALSE;

	configASSERT( pxPreviousWakeTime );
	configASSERT( ( xTimeIncrement > 0U ) );
	configASSERT( uxSchedulerSuspended == 0 );
	
	/* 挂起调度器 */
	vTaskSuspendAll();
	{
		/* 用于记录当前时间 */
		const TickType_t xConstTickCount = xTickCount;	/* 初始值为configINITIAL_TICK_COUNT */

		/* 计算下一次被唤醒的时间 */
		xTimeToWake = *pxPreviousWakeTime + xTimeIncrement;

		/* 如果当前时间戳计数比上一次被唤醒的时间小，那么当前时间戳计数必然溢出了 */
		if( xConstTickCount < *pxPreviousWakeTime )
		{
			/* 此时如果下一次被唤醒的时间也溢出了，且当前时间戳计数比下一次要被唤醒的时间小，那么就要继续被阻塞 */
			if( ( xTimeToWake < *pxPreviousWakeTime ) && ( xTimeToWake > xConstTickCount ) )
			{
				xShouldDelay = pdTRUE;
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			/* 如果当前时间戳计数值没有溢出，那么如果下一次被唤醒的时间溢出了或者比当前时间戳计数要大，那么就要继续被阻塞 */
			if( ( xTimeToWake < *pxPreviousWakeTime ) || ( xTimeToWake > xConstTickCount ) )
			{
				xShouldDelay = pdTRUE;
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}

		/* 更新上一次被唤醒的时间 */
		*pxPreviousWakeTime = xTimeToWake;

		if( xShouldDelay != pdFALSE )
		{
			traceTASK_DELAY_UNTIL( xTimeToWake );

			/* 如果任务需要继续被阻塞，加入到阻塞列表当中 */
			prvAddCurrentTaskToDelayedList( xTimeToWake - xConstTickCount, pdFALSE );
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
	xAlreadyYielded = xTaskResumeAll();

	/* xTaskResumeAll没有正常执行（没有发生上下文切换），使用机器实现的任务让步强制上下文切换 */
	if( xAlreadyYielded == pdFALSE )
	{
		portYIELD_WITHIN_API();
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
}

static void prvAddCurrentTaskToDelayedList( TickType_t xTicksToWait, const BaseType_t xCanBlockIndefinitely )
{
	TickType_t xTimeToWake;
	const TickType_t xConstTickCount = xTickCount;

	#if( INCLUDE_xTaskAbortDelay == 1 )
	{
		/* 任务TCB中标记该任务进入阻塞态，并且没有跳出阻塞态 */
		pxCurrentTCB->ucDelayAborted = pdFALSE;
	}
	#endif

	/* 任务有可能在准备列表当中，将其删去 */
	if( uxListRemove( &( pxCurrentTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
	{
		/* 如果该任务是准备列表中唯一一个任务，则重设最高的准备态任务为当前任务 */
		portRESET_READY_PRIORITY( pxCurrentTCB->uxPriority, uxTopReadyPriority );
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}

	#if ( INCLUDE_vTaskSuspend == 1 )
	{
		if( ( xTicksToWait == portMAX_DELAY ) && ( xCanBlockIndefinitely != pdFALSE ) )
		{
			/* 任务会无限期的被阻塞，因此将其加入到挂起列表中而不是阻塞列表中 */
			vListInsertEnd( &xSuspendedTaskList, &( pxCurrentTCB->xStateListItem ) );
		}
		else
		{
			/* 计算若没有事件发生时下一次被唤醒的时间。结果有可能会溢出但是内核会正确处理 */
			xTimeToWake = xConstTickCount + xTicksToWait;

			/* 将处于阻塞态事件列表中的列表项设置为阻塞时间 */
			listSET_LIST_ITEM_VALUE( &( pxCurrentTCB->xStateListItem ), xTimeToWake );

			if( xTimeToWake < xConstTickCount )
			{
				/* 唤醒时间溢出，将其放在阻塞溢出列表当中 */
				vListInsert( pxOverflowDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );
			}
			else
			{
				/* 唤醒时间没有溢出，将其放在阻塞列表当中 */
				vListInsert( pxDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );

				/* 该任务被唤醒时间是最早的（位于阻塞列表头），更新下一个任务被唤醒时间 */
				if( xTimeToWake < xNextTaskUnblockTime )
				{
					xNextTaskUnblockTime = xTimeToWake;
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
		}
	}
	#else /* INCLUDE_vTaskSuspend */
	{
		xTimeToWake = xConstTickCount + xTicksToWait;

		listSET_LIST_ITEM_VALUE( &( pxCurrentTCB->xStateListItem ), xTimeToWake );

		if( xTimeToWake < xConstTickCount )
		{
			vListInsert( pxOverflowDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );
		}
		else
		{
			vListInsert( pxDelayedTaskList, &( pxCurrentTCB->xStateListItem ) );

			if( xTimeToWake < xNextTaskUnblockTime )
			{
				xNextTaskUnblockTime = xTimeToWake;
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		( void ) xCanBlockIndefinitely;
	}
	#endif /* INCLUDE_vTaskSuspend */
}
```

而vTaskDelay()的实现则要简单许多：

```C
void vTaskDelay( const TickType_t xTicksToDelay )
{
	BaseType_t xAlreadyYielded = pdFALSE;

	/* A delay time of zero just forces a reschedule. */
	if( xTicksToDelay > ( TickType_t ) 0U )
	{
		configASSERT( uxSchedulerSuspended == 0 );
		vTaskSuspendAll();
		{
			traceTASK_DELAY();

			/* A task that is removed from the event list while the
			scheduler is suspended will not get placed in the ready
			list or removed from the blocked list until the scheduler
			is resumed.

			This task cannot be in an event list as it is the currently
			executing task. */
			prvAddCurrentTaskToDelayedList( xTicksToDelay, pdFALSE );
		}
		xAlreadyYielded = xTaskResumeAll();
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}

	/* Force a reschedule if xTaskResumeAll has not already done so, we may
	have put ourselves to sleep. */
	if( xAlreadyYielded == pdFALSE )
	{
		portYIELD_WITHIN_API();
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
}
```

可以看出，vTaskDelay()没有记录上一次被唤醒的时间，因此它下一次的唤醒时间是不确定的，依赖于任务中什么时候调用vTaskDelay()这个函数。

### 获取任务状态接口eTaskGetState()

eTaskGetState()的作用是返回调用该接口函数时参数指定的任务所处的状态。

这个函数在实现方面比较简单，简单思路就是首先判断所要返回的状态的任务是否就是当前调用该接口函数的任务，如果是的话就直接返回运行态（eRunning），否则查找阻塞态的两个列表，如果该任务存在列表项在这两个列表中的话就返回阻塞态。接着如果系统配置允许挂起任务，就在挂起态的任务列表中查找，这里存在一种特殊情况就是一个任务正在等待一个事件，这个任务处于阻塞态，但是在阻塞态的两个列表中都不会存在。如果任务都不在上面的列表中，且允许删除任务，则寻找该任务是否在删除任务释放列表中。最后，如果以上列表都不存在该任务，那么该任务就只能在准备态的列表中了。

### 任务优先级获取与设置函数

接下来是一系列有关任务优先级获取以及设置的函数，包括uxTaskPriorityGet()、uxTaskPriorityGetFromISR()、vTaskPrioritySet()。

uxTaskPriorityGet()的参数是所需要获取的优先级的任务句柄，如果参数为NULL，则获取的就是当前任务的优先级，函数中调用了该宏定义实现：

```C
#define prvGetTCBFromHandle( pxHandle ) ( ( ( pxHandle ) == NULL ) ? pxCurrentTCB : ( pxHandle ) )
```

值得注意的是，uxTaskPriorityGet()的中断服务实现版本uxTaskPriorityGetFromISR()。因为在FreeROTS中，一些中断的优先级比系统调用规定的最高优先级还要高（不可屏蔽中断），在这些中断中调用FreeRTOS中普通的API是不允许的，必须要使用以ISR结尾的API才可以使用。这些API被赋予了小于等于系统调用规定的最高优先级，以此来防止对中断的让步（实时系统是无法容忍中断被延时的）。

重设任务的优先级也比较简单，需要注意的地方就是是否允许使用互斥量。如果使用了互斥量，要注意TCB中的基准优先级以及当前优先级是否相等。如果该任务是处于准备态的话，那么就要先将该任务的列表项从准备态列表中删除，重设优先级后再放入准备态列表中。

### 任务挂起函数vTaskSuspend()

当配置文件中INCLUDE_vTaskSuspend设置为1时，表示系统允许使用任务挂起函数。该函数将该任务列表项删除（准备态列表、阻塞态列表），然后将其插入到挂起列表的尾部。最后若调度器正在运行，则进行主动让步，如果调度器不在运行，则进行上下文切换，当调度器再启动时从切换了的上下文中开始运行。

### 任务重启函数vTaskResume()

任务重启函数所做的工作比较简单，只需要判断目标任务是否处于挂起态，然后将其从挂起态中移除，然后加入到准备态列表中即可。判断是否处于挂起态需要考虑多个因素：是否在挂起态列表中、是否在ISR中已经被重启、是否是无限阻塞的任务等等。

### 调度器启动函数vTaskStartScheduler()

在Cortex-M3架构中，FreeRTOS为了任务启动和任务切换使用了三个异常SVC、PendSV以及SysTick。SVC用于任务启动，有些操作系统不允许应用程序直接访问硬件，而是通过一些系统服务函数，通过SVC来调用。PendSV（可挂起系统调用）用于完成任务切换，它的最大特性是如果当前有优先级比它高的中断正在运行，则PendSV会推迟执行。Systick用于产生系统节拍时钟，提供一个使劲按片，如果多个任务共享同一个优先级，则每次Systick中断，下一个任务将获得一个时间片。

vTaskStartScheduler()是一个很重要的函数，因为FreeRTOS的启动往往就需要启动调度器。同时启动调度器会做许多初始化的工作，下面对这个函数进行详细的分析：

```C
void vTaskStartScheduler( void )
{
	BaseType_t xReturn;

	/* 创建空闲任务 */
	#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	{
		/* 如果支持静态分配内存则静态分配内存空间给空闲任务使用 */
		StaticTask_t *pxIdleTaskTCBBuffer = NULL;
		StackType_t *pxIdleTaskStackBuffer = NULL;
		uint32_t ulIdleTaskStackSize;

		/* 此时空闲任务的TCB以及任务栈空间使用的是用户自定义的任务栈空间 */
		vApplicationGetIdleTaskMemory( &pxIdleTaskTCBBuffer, &pxIdleTaskStackBuffer, &ulIdleTaskStackSize );
		xIdleTaskHandle = xTaskCreateStatic(	prvIdleTask,
												configIDLE_TASK_NAME,
												ulIdleTaskStackSize,
												( void * ) NULL, 
												portPRIVILEGE_BIT,		/* 空闲任务处于特权模式？在CM3中=0x80000000 */ 
												pxIdleTaskStackBuffer,
												pxIdleTaskTCBBuffer );

		if( xIdleTaskHandle != NULL )
		{
			xReturn = pdPASS;
		}
		else
		{
			xReturn = pdFAIL;
		}
	}
	#else
	{
		/* 空闲任务使用动态分配内存的方法来创建 */
		xReturn = xTaskCreate(	prvIdleTask,
								configIDLE_TASK_NAME,
								configMINIMAL_STACK_SIZE,
								( void * ) NULL,
								portPRIVILEGE_BIT,
								&xIdleTaskHandle );
	}
	#endif /* configSUPPORT_STATIC_ALLOCATION */

	#if ( configUSE_TIMERS == 1 )
	{
		if( xReturn == pdPASS )
		{
			xReturn = xTimerCreateTimerTask();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
	#endif /* configUSE_TIMERS */

	if( xReturn == pdPASS )
	{
		#ifdef FREERTOS_TASKS_C_ADDITIONS_INIT
		{
			freertos_tasks_c_additions_init();
		}
		#endif

		/* 中断在这里被关闭，防止时间片中断在调用xPortStartScheduler()之前或者调用时发生。新创建的任务的堆栈中会包含一个中断切换的状态字，当中断重新开启时会在执行的第一个任务时自动切换至中断 */
		portDISABLE_INTERRUPTS();

		#if ( configUSE_NEWLIB_REENTRANT == 1 )
		{
			_impure_ptr = &( pxCurrentTCB->xNewLib_reent );
		}
		#endif /* configUSE_NEWLIB_REENTRANT */

		xNextTaskUnblockTime = portMAX_DELAY;	/* 启动调度器将下一次任务阻塞时间设为最大 */
		xSchedulerRunning = pdTRUE;
		xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;

		portCONFIGURE_TIMER_FOR_RUN_TIME_STATS();

		traceTASK_SWITCHED_IN();

		/* 设置时间片使用特定硬件层的函数 */
		if( xPortStartScheduler() != pdFALSE )
		{
			/* 如果调度器启动成功，不会到达此处。该函数将不会返回 */
		}
		else
		{
			/* 只有当调用xTaskEndScheduler()的时候才会到达 */
		}
	}
	else
	{
		/* 如果内存空间不足以创建空闲任务，则会到达此处 */
		configASSERT( xReturn != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY );
	}

	( void ) xIdleTaskHandle;
}
```

其中使用了一个底层实现的函数xPortStartScheduler()，下面以Cortex-M3为例子说明。Cortex-M3底层实现在前面实现的部分用于确定最大的ISR调度优先级，由于在Cortex-M3中，优先级数值越高，优先级越低，而FreeRTOS则相反。因此为了保持FreeRTOS移植到CM3的健壮性，FreeRTOS的作者在编写Cortex-M3架构移植层代码时，特意增加了冗余代码。重点分析下vPortStartFirstTask()，底层启动第一个任务的函数：

```C
static void prvPortStartFirstTask( void )
{
	__asm volatile(
					" ldr r0, =0xE000ED08 	\n" /* VTOR寄存器，存储中断向量表的起始地址 */
					" ldr r0, [r0] 			\n"
					" ldr r0, [r0] 			\n" /* 取出向量表中的第一项,向量表第一项存储主堆栈指针MSP的初始值 */
					" msr msp, r0			\n" /* 重置MSP的值，FreeRTOS将会接管MSP */
					/*
						CPSIE i指令相当于往PRIMASK写0. 置1后，将关闭所有可屏蔽中断的异常，只剩NMI和硬fault，默认值为0
						CPSIE f指令相当于往FAULTMASK写0. 置1后，屏蔽除NMI外的所有异常（包括硬fault），默认值为0
					*/
					" cpsie i				\n" /* 全局使能中断 */
					" cpsie f				\n"
					" dsb					\n"
					" isb					\n"
					" svc 0					\n" /* 调用SVC启动第一个任务 */
					" nop					\n"
				);
}

void vPortSVCHandler( void )
{
	__asm volatile (
					"	ldr	r3, pxCurrentTCBConst2		\n" /* pxCurrentTCBConst2指向当前处于最高优先级任务的TCB */
					"	ldr r1, [r3]					\n"
					"	ldr r0, [r1]					\n" /* TCB中的第一个成员是指向任务堆栈栈顶的地址 */
					"	ldmia r0!, {r4-r11}				\n" /* 对于Cortex-M3架构，需要依次入栈xPSR、PC、LR、R12、R3~R0、R11~R4，其中R11~R4需要人为入栈，其它寄存器由硬件自动入栈。因此要将R11~R4人为出栈 */
					"	msr psp, r0						\n" /* 将最新的堆栈栈顶指针赋值给线程堆栈指针PSP */
					"	isb								\n"
					"	mov r0, #0 						\n"
					"	msr	basepri, r0					\n" /* 取消中断屏蔽 */
					"	orr r14, #0xd					\n"	/* Cortex-M3架构中，r14的值决定了从异常返回的模式，这里r14最后四位按位或上0x0d，表示返回时从进程堆栈中做出栈操作、返回后进入线程模式、返回Thumb状态。 */
					"	bx r14							\n"	/* thumb指令集，执行bx  r14指令后，硬件自动将寄存器xPSR、PC、LR、R12、R3~R0出栈，这时任务A的任务函数指针vTask_A会出栈到PC指针中，从而开始执行任务A。 */
					"									\n"
					"	.align 4						\n"
					"pxCurrentTCBConst2: .word pxCurrentTCB				\n"
				);
}
```

至此，第一个任务获得了调度器的调度执行，调度器正式开始运行。

### 调度器关闭函数vTaskEndScheduler()

调度器关闭函数vTaskEndScheduler()：

```C
void vTaskEndScheduler( void )
{
	/* Stop the scheduler interrupts and call the portable scheduler end
	routine so the original ISRs can be restored if necessary.  The port
	layer must ensure interrupts enable	bit is left in the correct state. */
	portDISABLE_INTERRUPTS();
	xSchedulerRunning = pdFALSE;
	vPortEndScheduler();
}
```

实际上该函数只是简单的将调度器的中断关闭，以此达到了关闭调度器的目的。然后将xSchedulerRunning设为pdFALSE即可。vPortEndScheduler()没有执行任何有效的操作。
