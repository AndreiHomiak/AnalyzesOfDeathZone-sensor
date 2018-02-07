// can driver for analyzes of death zone
// sensor
// file: can.h

#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_can.h"
#include "misc.h"

#include <math.h>

#include "adc.h"
#include "timers.h"

//number ports can1 and can2
#define CAN1_PORT       GPIOA
#define CAN2_PORT       GPIOB

//number ports peripheri
#define CAN1_Periph     RCC_APB2Periph_GPIOB
#define CAN2_Periph     RCC_APB2Periph_GPIOB

//number pins can1 and can2
#define CAN1_RX         GPIO_Pin_11
#define CAN1_TX         GPIO_Pin_12
#define CAN2_RX         GPIO_Pin_12
#define CAN2_TX         GPIO_Pin_13

//variable
extern uint16_t flag_send_can;

extern uint8_t number_sensor;

//FUNCTIONS
void InitCan1(void);

void CAN1_RX0_IRQHandler(void);

void SendMsgToCan1(CanTxMsg *TxMessage);
void SendTestToCan1(void);
void SendMsgToCan(uint8_t *buf, uint32_t id);
void SendAdc1ToCan();
void SendAdc2ToCan();

//for debug
void SendValPrevToCan();
void SendValNowToCan();
void SendValToCan();

#endif // __CAN