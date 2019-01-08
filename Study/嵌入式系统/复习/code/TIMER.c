/*
    例程1：通过定时器实现1s的延迟，控制LED1灯每秒闪烁一次。
*/

// TIM1时钟模块使能
void rcc_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOA, ENABLE);
}

// 配置TIM时钟模块
void tim_init()
{
    TIM_TimeBaseInitTypeDef tim_init;
    tim_init.TIM_Period = 7200 - 1;
    tim_init.TIM_Prescaler = 10000 - 1;
    tim_init.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_init.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &tim_init);
    TIM_Cmd(TIM1);
}

// 配置定时器中断
void nvic_init()
{
    NVIC_InitTypeDef nvic_init;
    TIM_IT_Config(TIM1, TIM_IT_Update, ENABLE);
    nvic_init.NVIC_IRQChannel = TIM1_UP_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPeermptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&nvic_init);
}

// 中断处理函数
void TIM1_UP_IRQHandler()
{
    if(TIM_GetFlagStatus(TIM1, TIM_IT_Update))
        led_twinkle();

    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}


/*
    例程2：计数器功能。
*/

// TIM2模块时钟使能

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

// 初始化计数器
void TIM2_Configuration()
{
    TIM_TimeBaseInitTypeDef tim_init;
    tim_init.TIM_Period = 0xFFFFFF;
    tim_init.TIM_Prescaler = 0;
    tim_init.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_init.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_init);
    TIM_ETRClockMode1Config(TIM2, TIM_ExtTRGPrescaler_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
    TIM_Cmd(TIM2, ENABLE);
}

// 初始化中断

void nvic_init()
{
    NVIC_InitTypeDef nvic_init;
    TIM_ITConfig(TIM2, TIM_IT_Trigger, ENABLE);
    nvic_init.NVIC_IRQChannel = TIM2_IRQn;
    nivc_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&nvic_init);
}

void TIM2_IRQHandler()
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Trigger))
        usart_send_byte(TIM_GetCounter(TIM2));

    TIM_ClearITPendingBit(TIM2, TIM_IT_Trigger);
}

/*
    例程3：实现对按钮按下去的时间长度进行测量。
*/

// 使能TIM2时钟模块
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

void TIM2_Configuration()
{
    TIM_TimeBaseInitTypeDef tim_init;
    tim_init.TIM_Period = 72 - 1;
    tim_init.TIM_Prescaler = 0;
    tim_init.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_init.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &tim_init);
    TIM_Cmd(TIM1, ENABLE);

    TIM_ICInitTypeDef tim_ic_init;
    tim_ic_init.TIM_Channel = TIM_Channel_1;
    tim_ic_init.TIM_ICFilter = 0;
    tim_ic_init.TIM_ICPolarity = TIM_ICPolarity_Falling;
    tim_ic_init.TIM_ICSelection = TIM_ICSelection_DirectT1;
    tim_ic_init.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInit(TIM1, &tim_ic_init);

    NVIC_InitTypeDef nvic_init;
    TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1, ENABLE);
    nvic_init.IRQChannel = TIM2_IRQn;
    nvic_init.IRQChannelCmd = ENABLE;
    nvic_init.IRQChannelPeermptionPriority = 0;
    nvic_init.IRQChannelSubPriority = 0;
    NVIC_Init(&nvic_init);
}

extern struct capture_st m_capture;

void TIM2_IRQHandler()
{
    if(TIM_GetFlagStatus(TIM2, TIM_IT_Update))
    {
        if(m_capture.m_start == 1)
            m_capture.period_count++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
    if(TIM_GetFlagStatus(TIM2, TIM_IT_CC))
    {
        if(m_capture.m_start == 0)
        {
            m_capture.m_start = 1;
            m_capture.period_count = 0;
            m_capture.ccr_value = 0;
            TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);
            TIM_SetCounter(TIM2, 0);
        }
        else
        {
            TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1, DISABLE);
            TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Falling);
            m_capture.m_finish = 1;
            m_capture.m_start = 0;
            m_capture.ccr_value = TIM2->CCR;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }
}

/*
    例程4：PWM输出呼吸灯效果。
*/

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

void TIM2_Configuration()
{
    TIM_TimeBaseInitTypeDef tim_init;
    tim_init.TIM_Period = 1024 - 1;
    tim_init.TIM_Prescaler = 200 -1;
    tim_init.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_init.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_init);

    TIM_OCInitTypeDef tim_oc_init;
    tim_oc_init.TIM_OCMode = TIM_OCMode_PWM1;
    tim_oc_init.TIM_OCPolarity = TIM_OCPolarity_Low;
    tim_oc_init.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init.TIM_Pulse = 0;
    TIM_OC2Init(TIM2, &tim_oc_init);

    NVIC_InitTypeDef nvic_init;
    TIM_ClearFlag(TIM2, TIM_IT_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    nvic_init.NVIC_IRQChannel = TIM2_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&nvic_init);
    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler()
{
    if(TIM_GetFlagStatus(TIM2, TIM_IT_Update) == Set)
    {
        if(!--period_class)
        {
            period_class = 10;
            if(!++data_index >= 110)
                data_index = 0;
            TIM_SetCompare2(TIM2, data[data_index]);
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

/*
    通过PA6引脚作为输出产生PWM，然后PA8对PA6产生的PWM波形进行测量。
*/

void gpio_init()
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = GPIO_Pin_6;
    gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_8;
    gpio_init.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &gpio_init);
}

void TIM_Configuration()
{
    TIM_TimeBaseInitTypeDef tim_init;
    TIM_ICInitTypeDef tim_ic_init;
    TIM_OCInitTypeDef tim_oc_init;
    tim_init.TIM_Period = 8 - 1;
    tim_init.TIM_Prescaler = 72 - 1;
    tim_init.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_init.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &tim_init);

    tim_oc_init.TIM_OCMode = TIM_OCMode_PWM1;
    tim_oc_init.TIM_OCPolarity = TIM_OCPolarity_High;
    tim_oc_init.TIM_OCIdleState = TIM_OCIdleState_Reset;
    tim_oc_init.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init.TIM_OutputNState = TIM_OutputNState_DISABLE;
    tim_oc_init.TIM_Pulse = 4;
    TIM_OC1Init(TIM3, &tim_oc_init);
    TIM_Cmd(TIM3, ENABLE);

    tim_in_init.TIM_Channel = TIM_Channel_1;
    tim_in_init.TIM_ICPolarity = TIM_ICPolarity_Rising;
    tim_in_init.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    tim_in_init.TIM_ICSelection = TIM_ICSelection_DirectT1;
    tim_ic_init.TIM_ICFilter = 0;
    TIM_PWMIConfig(TIM1, &tim_ic_init);

    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Enable);
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
    TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
    nvic_config();
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

void nvic_config(void)
{
    NVIC_InitTypeDef nvic_init;
    nvic_init.NVIC_IRQChannel = TIM1_CC_IRQn;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&nvic_init);
}

void TIM1_CC_IRQHandler()
{
    uint16_t ic1 = TIM_GetCapture1(TIM1);
    uint16_t ic2 = TIM_GetCapture2(TIM1);

    if(ic1)
        printf("%d %d\n", ic1 + 1, ic2 + 1);
    
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
}
