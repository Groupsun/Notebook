/*
    利用滑动变阻器产生的电压，通过STM32芯片的PC0引脚采集。
    并通过串口把这个电压值传送到电脑。
*/

void adc_init()
{
    ADC_InitTypeDef adc_init;
    ADC_DeInit(ADC1);
    adc_init.ADC_Mode = ADC_Mode_Independent;
    adc_init.ADC_ScanConvMode = DISABLE;
    adc_init.ADC_ContinuousConvMode = ENABLE;
    adc_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    adc_init.ADC_DataAlign = ADC_DataAlign_Right
    adc_init.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &adc_init);
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibrationsStatus(ADC1);
    while(ADC_GetCalibrationsStatus(ADC1));
    ADC_StartCalibrationStatus(ADC1);
    while(ADC_GetCalibrationsStatus(ADC1));
}

int main()
{
    rcc_init();
    gpio_init();
    usart_init();
    adc_init();
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(1)
    {
        if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        {
            unsigned char voltage = 3.3f/4096 * ADC_GetConversionValue(ADC1);
            usart_send_data(voltage);
            delay_moment();
        }
    }
    return 0;
}
