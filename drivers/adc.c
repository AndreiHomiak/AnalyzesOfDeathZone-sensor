#include "adc.h"

void InitADC1()
{
    //Injected channel
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    GPIO_InitStructure.GPIO_Pin =   PIN1 | PIN2;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    ADC_InitStructure.ADC_ContinuousConvMode =  ENABLE;
    ADC_InitStructure.ADC_DataAlign =           ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv =    ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_Mode =                ADC_Mode_Independent;
    ADC_InitStructure.ADC_NbrOfChannel =        2;
    ADC_InitStructure.ADC_ScanConvMode =        ENABLE;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_InjectedSequencerLengthConfig(ADC1, 2);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
    ADC_InjectedChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_7Cycles5);
    
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);
    
    ADC_Cmd (ADC1, ENABLE);
    
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    
    ADC_AutoInjectedConvCmd(ADC1, ENABLE);
    ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC)==RESET);
}

float ConvertADCvalueToVoltageFloat(uint16_t ADC_value)
{
    float Voltage[] = {0, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 48};
    float ValueADC[] = {0x0000, 0x0468, 0x04C0, 0x050A, 0x055A, 0x05A8, 0x05FC, 0x0643, 0x0690, 0x06E0, 0x0730, 0x0782, 
                         0x07CA, 0x081F, 0x0868, 0x08B3, 0x0902, 0x094F, 0x099F, 0x09E5, 0x0A39, 0x0A7C, 0x0AD0, 0x0B1F, 0x1000};
    int size = sizeof(Voltage) / sizeof(float);
    int diap = 0;
    
    for(int i = 0; i < size; i++)
    {
        if(ADC_value > ValueADC[i])
            diap++;
        else
            break;
    }
    
    float k, b, rezf;
    
    if(diap != 0)
    {
        k = (Voltage[diap] - Voltage[diap - 1]) / (ValueADC[diap] - ValueADC[diap - 1]);
        b = Voltage[diap] - k * ValueADC[diap];
        rezf = k * (float)ADC_value + b;
        return rezf;
    }
    if(ADC_value > ValueADC[size])
    {
        return ValueADC[size];
    }
    if(diap == 0)
    {
        return Voltage[0];
    }
    return 0;
}

uint16_t GetValueFromADC1(void)
{
    return (uint16_t)ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
}

uint16_t GetValueFromADC2(void)
{
    return (uint16_t)ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_2);
}