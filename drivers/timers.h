// Timers configurations for analyzes of death zone
// sensor
// file: timers.h

#ifndef __TIMERS_H
#define __TIMERS_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

#include "can.h"

#define TIM1_CH1    GPIO_Pin_8
#define TIM1_CH2    GPIO_Pin_9
#define TIM1_CH3    GPIO_Pin_2

#define GEN_PERIOD  7
#define MAX_PERIOD  7    

extern uint8_t flag_clear_zone;
extern uint8_t flag_deadline;

extern uint8_t tick;
extern uint8_t period;
extern uint32_t miliseconds;
extern uint32_t mcs100;
extern uint16_t value_prev;
extern uint16_t value_now;

void InitTimers(void);

void InitTimer1(void);
void InitTimer7(void);
void InitPinsForTimers(void);

void TIM1_UP_IRQHandler(void);
void TIM7_IRQHandler(void);

#endif // __TIMERS_H