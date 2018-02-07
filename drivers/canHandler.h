// Can handler for analyzes of death zone
// sensor
// file: canHandler.h 


#ifndef __CAN_HANDLER_H
#define __CAN_HANDLER_H

#include "stm32f10x.h"
#include "stm32f10x_can.h"

#include "can.h"
#include "adc.h"
#include "timers.h"

#define VERSION_PO_MAJOR    0
#define VERSION_PO_MINOR    5
#define VERSION_PO_MICRO    0

#define CAN_ID_GLB  0x18FF0000
#define CAN_ID_CMD  0x0CEA0000
#define CAN_ID_ACK  0x10E80000
#define CAN_ID_REQ  0x18EA0000

#define CAN_IOU_ADRESS      0x90 //input-output unit
#define CAN_DCU_ADRESS      0x91 //display and control unit
#define CAN_SENSOR_ADRESS   0x92 //sensor

#define ADC1_VALUE          0x01
#define ADC2_VALUE          0x02
#define ADC1_VALUE_CONV     0x03
#define ADC2_VALUE_CONV     0x04
#define VERSION_PO          0x05
#define DISTANCE            0x06

//commands for sensor
#define GET_DISTANCE        0x01
#define GET_TIME            0x02
#define ON_SCAN             0x03
#define OFF_SCAN            0x04
#define GET_VERSION_PO      0x08
#define GET_ADC1_VALUE      0x09
#define GET_ADC2_VALUE      0x10
#define GET_ADC1_CONV       0x11
#define GET_ADC2_CONV       0x12


void CanSendMess(uint8_t name_mess);
void CanSpamer();
uint8_t CommandProcessing(CanRxMsg *rxMessage);
void CanSendAck(uint16_t unit_adress, uint8_t name_command, uint8_t rezult);

#endif //__CAN_HANDLER_H