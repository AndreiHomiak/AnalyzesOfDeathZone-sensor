#include "pins.h"

void InitPins()
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStruct.GPIO_Pin = DAC1 | DAC2 | DAC3 | DAC4 | DAC5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void ReInitPins(uint8_t val)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Pin = DAC1 | DAC2 | DAC3 | DAC4 | DAC5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    
    switch(val)
    {
        case 1:
            GPIO_InitStruct.GPIO_Pin = DAC1;
            GPIO_Init(GPIOC, &GPIO_InitStruct);
            GPIO_ResetBits(GPIOC, DAC1);
            break;
        
        case 2:
            GPIO_InitStruct.GPIO_Pin = DAC2;
            GPIO_Init(GPIOC, &GPIO_InitStruct);
            GPIO_ResetBits(GPIOC, DAC2);
            break;
        
        case 3:
            GPIO_InitStruct.GPIO_Pin = DAC3;
            GPIO_Init(GPIOC, &GPIO_InitStruct);
            GPIO_ResetBits(GPIOC, DAC3);
            break;
        
        case 4:
            GPIO_InitStruct.GPIO_Pin = DAC4;
            GPIO_Init(GPIOC, &GPIO_InitStruct);
            GPIO_ResetBits(GPIOC, DAC4);
            break;
            
        case 5:
            GPIO_InitStruct.GPIO_Pin = DAC5;
            GPIO_Init(GPIOC, &GPIO_InitStruct);
            GPIO_ResetBits(GPIOC, DAC5);
            break;
    }
}