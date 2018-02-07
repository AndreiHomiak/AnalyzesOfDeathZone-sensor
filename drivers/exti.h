// External interrupt for analyzes of death zone
// sensor
// file: exti.h

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"

#include "timers.h"

void InitExtIRQ();
void EXTI2_IRQHandler(void);