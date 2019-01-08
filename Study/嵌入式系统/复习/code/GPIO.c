/*
    GPIO口配置示例。
    设定PA1口作为输出控制LED灯（低电平有效），PA0口作为按钮输入检测。
*/

// 时钟配置
void rcc_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}

// GPIO口配置
void gpio_init()
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = GPIO_Pin_1;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &gpio_init);
    gpio_init.GPIO_Pin = GPIO_Pin_0;
    gpio_init.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &gpio_init);
}

// 灯亮
void led_light()
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

// 灯灭
void led_dark()
{
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

// 获取按键信息
char read_key()
{
    return !ReadInputDataBit(GPIOA, GPIO_Pin_0);
}

int main()
{
    rcc_init();
    gpio_init();
    while(1)
    {
        if(read_key())
            led_light();
        else led_dark();
    }
    return 0;
}
