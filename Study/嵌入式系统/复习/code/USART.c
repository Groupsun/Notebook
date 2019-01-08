/*
    配置串口查询方式接收主机信息并发送回给主机。
*/

// 初始化时钟
void rcc_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
}

// 初始化PA9和PA10，其中PA9是输出口，PA10是输入口。
void gpio_init()
{
    // PA9 复用TX引脚为输出口
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = GPIO_Pin_9;
    gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;      // 复用推挽输出
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_init);

    // PA10 复用RX引脚为输入口
    gpio_init.GPIO_Pin = GPIO_Pin_10;
    gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_init);
}

void usart_init()
{
    USART_InitTypeDef usart_init = {
        .USART_BaudRate = 9600,
        .USART_WordLength = USART_WordLength_8b,
        .USART_StopBits = USART_StopBits_1,
        .USART_Parity = USART_Parity_No,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
        .USART_Mode = USART_Mode_TX|USART_Mode_RX,
    }

    USART_Init(USART1, &usart_init);
    USART_Cmd(USART1, ENABLE);
}

void usart_send_byte(unsigned char data)
{
    USART_SendData(data);

    while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
}

unsigned char usart_recv_byte()
{
    while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));

    return USART_ReceiveData(USART1);
}

int main()
{
    rcc_init();
    gpio_init();
    usart_init();
    while(1)
    {
        data = usart_recv_byte();
        usart_send_byte(data);
    }

    return 0;
}
