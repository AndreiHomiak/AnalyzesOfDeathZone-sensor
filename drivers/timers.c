#include "timers.h"

uint8_t flag_clear_zone = 1;
uint8_t flag_deadline = 0;
uint8_t tick = 0;
uint8_t period = 0;
uint32_t miliseconds = 0;
uint32_t mcs100 = 0;
uint16_t value_prev = 65535;
uint16_t value_now = 12345;

void InitTimers(void)
{
    InitPinsForTimers();
    
//    InitTimer7();
 //   InitTimer1();
}

void InitTimer1(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 51 - 1; //48
    TIM_TimeBaseStructure.TIM_Prescaler = 6 - 1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM1, DISABLE);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
}

void TIM1_UP_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    
    if(period < GEN_PERIOD)
    {
        switch(tick)
        {
            case 0:
                GPIO_SetBits(GPIOA, TIM1_CH1);
                if(period == 0)
                {
                    TIM_Cmd(TIM7, ENABLE);
                }
                break;
            case 1:
                break;
            case 2:
                GPIO_ResetBits(GPIOA, TIM1_CH1);
                break;
            case 3:
                GPIO_SetBits(GPIOA, TIM1_CH2);
                break;
            case 4:
                break;
            case 5:
                GPIO_ResetBits(GPIOA, TIM1_CH2);
                break;
            default:
                tick = 0;
                GPIO_ResetBits(GPIOA, TIM1_CH1);// для страховки
                GPIO_ResetBits(GPIOA, TIM1_CH2);
                break;
        }
    }
    tick++;
    if(tick > 5)
    {
        period++;
        tick = 0;
    }
        
    if(period > MAX_PERIOD)
    {
        period = 0;
        tick = 0;
        TIM_Cmd(TIM1, DISABLE);
    }
}

void InitPinsForTimers(void)
{
    //инициализация TIM1_CH1 и TIM1_CH2
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Pin = TIM1_CH1 | TIM1_CH2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOA, TIM1_CH1);
    GPIO_ResetBits(GPIOA, TIM1_CH2);
    
        //TIM_CH3    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void InitTimer7(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 50;
    TIM_TimeBaseStructure.TIM_Prescaler = (36 * 4)-1;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, DISABLE);
   
    // Таймер 7
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM7_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

    mcs100++;
    if(mcs100 == 10)
    {
        miliseconds++;
        mcs100 = 0;
    }
    
    if((miliseconds == 3) && (mcs100 == 0))
    {
        NVIC_EnableIRQ(EXTI2_IRQn);
    }
    else if((miliseconds == 60) && (mcs100 == 0))
    {
        flag_deadline = 0x01;
        NVIC_DisableIRQ(EXTI2_IRQn);
        flag_send_can = 1;
    }
    else if((miliseconds > 100) && (mcs100 == 0))
    {
        miliseconds = 0;
        mcs100 = 0;
        flag_deadline = 0x00;
        TIM_Cmd(TIM7, DISABLE);
        TIM_Cmd(TIM1, ENABLE);
    }   
}

