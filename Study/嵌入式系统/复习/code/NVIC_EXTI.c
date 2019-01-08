/*
    修改GPIO点亮LED灯程序，改为用中断实现。
*/

// 初始化EXTI外部中断
void exti_init()
{
    EXTI_InitTypeDef exti_init;
    exti_init.EXTI_Line = EXTI_Line0;   // PA0中断检测
    exti_init.EXTI_LineCmd = ENABLE;
    exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_init.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_Init(&exti_init);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PortSource0);
}

// 初始化NVIC()
void nvic_init()
{
    NVIC_InitTypeDef nvic_init;
    nvic_init.NVIC_IRQChannel = EXTI0_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPeermptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&nvic_init);
}

// 中断处理函数
void EXTI0_IRQHandler()
{
    if(read_key())
        led_light();
    else led_dark();

    EXTI_ClearITPendingBits(EXTI_Line0);
}

int main()
{
    gpio_init();
    exti_init();
    nvic_init();
    while(1);

    return 0;
}
