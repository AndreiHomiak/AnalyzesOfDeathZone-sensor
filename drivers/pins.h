// DAC for analyzes of death zone
// sensor
// file: pins.h

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#define DAC1    GPIO_Pin_0
#define DAC2    GPIO_Pin_1
#define DAC3    GPIO_Pin_2
#define DAC4    GPIO_Pin_3
#define DAC5    GPIO_Pin_4

void InitPins();
void ReInitPins(uint8_t val);