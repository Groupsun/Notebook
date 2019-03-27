# FreeRTOS

包含一些目前存在的问题，以及一些使用过程中获得的经验等。

- HAL移植的Arduino FreeRTOS库中，没有vTaskDelayUntil()函数，仅包含FreeRTOS.h文件时表示未定义。
    - 已解决：需要在配置文件中将INCLUDE_vTaskDelayUntil设置为1。
- Idle Task Hook功能无法使用，vApplicationIdleHook()函数实现没有效果。
    - 检查FreeRTOS.h发现：

    ```C++
        #define configUSE_IDLE_HOOK                      1
    ```

    设置后依然没有效果。
- 在Task中改变Task的优先级没有效果（或者没有出现预期的结果）：
    - 程序中已经宏定义检查了INCLUDE_vTaskPrioritySet的值：

    ```C++
    #if ( INCLUDE_vTaskPrioritySet == 1 )
		Serial << "Allow to set the priority of the tasks \n";
	#endif
    ```
    
    - 当Task中不使用使Task阻塞的调用时，vTaskPrioritySet()不起作用
    - 当Task中使用使Task阻塞的调用时，整个系统宕机？（或者所有Task被阻塞，还不清楚Idle Task是否在运行）
- xPortGetFreeHeapSize()，获取堆中可用的堆栈空间的API函数编译显示未定义（仅包含了头文件FreeRTOS.h）
    - 更新：已解决。在使用的开发板NUCLEO_F030R8中，使用的内存管理方式是Heap_3，这种内存管理方式使用的是包装过的C原生malloc()以及free()函数，没有实现xProtGetFreeHeapSize()，原因是使用的是C内置的API函数，无法获取其申请空间的详细信息。
- 注意：在开发板NUCLEO_F030R8中，默认的分配堆空间的大小是3072字节，因此在创建任务时分配的栈空间不能太大，一般来说一些小的测试任务使用128字节就好了（configMINIMAL_STACK_SIZE）
- 在使用FreeRTOS定时器时，要注意包含的头文件路径问题：

    ```C++
    #include <FreeRTOS_STM.h>
    /* 实际路径以自身为主，可以在Arduino编译选项中了解编译路径 */
    #include <FreeRTOS/Source/include/timers.h>
    ```

- 包含了相应的头文件情况下，发现有关递归互斥量的API无法使用：
    - xSemaphoreCreateRecursiveMutex()
    - xSemaphoreTakeRecursive()
    - xSemaphoreGiveRecursive()
- Tick Hook功能无法使用，vApplicationTickHook()函数没有发挥效果。
    - 检查FreeRTOS.h配置文件发现：

    ```C++
    #define configUSE_TICK_HOOK                      1
    ```

    设置后依然没有效果。
- Arduino上FreeRTOS之后，串口是一种资源，如果任务间没有延时操作的话，那么可能会造成资源争用的情况出现。解决这个问题的方法有：
    - 使用临界区，通过taskENTER_CRITICAL()以及taskEXIT_CRITICAL()来解决问题，但是存在的不足是频繁的临界区进出会浪费系统资源，并且会导致平均响应时间变长。taskENTER_CRITICAL()通过关闭中断来达到临界区的效果。
    - 使用vTaskSuspendAll()以及xTaskResumeAll()。通过挂起（软件）调度器来解决问题。但是中断可以打断临界区中任务的执行。
    - 使用互斥量mutex，但是频繁的互斥量释放和获取也会造成系统资源的浪费。
    - 最佳的方法是使用守护进程的思想，使一个专门为串口输出的任务独占这个串口。在FreeRTOS中称为Gatekeeper：

    首先定义守护进程所要执行的函数原型：

    ```C++
    static void prvStdioGatekeeperTask(void *pvParameters)
    {
        char *prvStringToPrint;
        Serial.begin(115200);
        while(1)
        {
            xQueueReceive(xPrintQueue, &prvStringToPrint, portMAX_DELAY);
            Serial << prvStringToPrint;
        }
        vTaskDelete(NULL);
    }
    ```

    需要进行串口输出的任务，将所需要的输出的字符串放入一个队列当中，守护进程将会不断从该队列中取出字符串输出到串口当中。

    ```C++
    void vExampleTask(void *pvParameters)
    {
        /* Some initializations */
        while(1)
        {
            /* Do something */
            xQueueSend(xPrintQueue, &prvStringToPrint, portMAX_DELAY);
            // Delay or not
        }
        vTaskDelete(NULL);
    }
    ```

    最后，在setup中创建打印队列以及守护进程即可：

    ```C++
    void setup()
    {
        xPrintQueue = xQueueCreate(PRINT_QUEUE_SIZE, sizeof(char*));
        if(xPrintQueue != NULL)
        {
            xTaskCreate(prvStdioGatekeeperTask, "Gatekeeper", DEFAULT_TASK_STK_SIZE, NULL, 2, NULL)
            xTaskCreate(vExampleTask, "Example", DEFAULT_TASK_STK_SIZE, NULL, 1, NULL);

            vTaskStartScheduler();
        }
    }
    ```