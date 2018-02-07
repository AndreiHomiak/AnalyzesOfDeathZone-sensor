#include "canHandler.h"

void CanSendMess(uint8_t name_mess)
{
    uint8_t fSend = 1;
    uint16_t buf;
    float buff;
    CanTxMsg TxMessage;
    
    TxMessage.ExtId = CAN_ID_GLB | ((uint16_t)name_mess<<8) | CAN_SENSOR_ADRESS;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_Id_Extended;
    TxMessage.DLC = 8;
    TxMessage.Data[0] = 0xFF;
    TxMessage.Data[1] = 0xFF;
    TxMessage.Data[2] = 0xFF;
    TxMessage.Data[3] = 0xFF;
    TxMessage.Data[4] = 0xFF;
    TxMessage.Data[5] = 0xFF;
    TxMessage.Data[6] = 0xFF;
    TxMessage.Data[7] = 0xFF;
    
    switch(name_mess)
    {
        case ADC1_VALUE:
                buf = GetValueFromADC1();
                *(uint16_t *)(&TxMessage.Data[0]) = (uint16_t) buf;
            break;
            
        case ADC2_VALUE:
                buf = GetValueFromADC2();
                *(uint16_t *)(&TxMessage.Data[0]) = (uint16_t) buf;
            break;
            
        case ADC1_VALUE_CONV:
                buf = GetValueFromADC1();
                buff = ConvertADCvalueToVoltageFloat(buf);
                buf = (uint16_t) roundf(buff * 1000);
                *(uint16_t *)(&TxMessage.Data[0]) = (uint16_t) buf;
            break;
            
        case ADC2_VALUE_CONV:
                buf = GetValueFromADC2();
                buff = ConvertADCvalueToVoltageFloat(buf);
                buf = (uint16_t) roundf(buff * 1000);
                *(uint16_t *)(&TxMessage.Data[0]) = (uint16_t) buf;
            break;
            
        case VERSION_PO:
                TxMessage.Data[0] = VERSION_PO_MICRO;
                TxMessage.Data[1] = VERSION_PO_MINOR;
                TxMessage.Data[2] = VERSION_PO_MAJOR;
            break;
            
        case DISTANCE:
                TxMessage.Data[0] = number_sensor;
                *(uint16_t *)(&TxMessage.Data[1]) = (uint16_t) value_now;
                *(uint16_t *)(&TxMessage.Data[3]) = (uint16_t) value_prev;
            break;
            
        default:
            fSend = 0;
            break;
    }
    
    if(fSend)
        CAN_Transmit(CAN1, &TxMessage);
}

void CanSpamer()
{
    if(flag_send_can)
    {
        flag_send_can = 0;
        CanSendMess(DISTANCE);
    }
}

uint8_t CommandProcessing(CanRxMsg *rxMessage)
{
    uint32_t viewMsg = rxMessage->ExtId & 0xFFFF0000;
    uint8_t DA, SA, rezult = 0xFE;
    DA = (rxMessage->ExtId & 0x0000FF00) >> 8;
    SA =  rxMessage->ExtId & 0x000000FF;
    
    if(DA == CAN_SENSOR_ADRESS)
    {
        switch(viewMsg)
        {
            case CAN_ID_CMD:
                    
                    switch(rxMessage->Data[0])
                    {
                        case GET_DISTANCE:
                            CanSendMess(DISTANCE);
                            rezult = 0;
                            CanSendAck(SA, GET_DISTANCE, rezult);
                        break;
                        
                        case GET_TIME:
                            rezult = 0;
                            CanSendAck(SA, GET_TIME, rezult);
                        break;
                        
                        case GET_VERSION_PO:
                            CanSendMess(VERSION_PO);
                            rezult = 0;
                            CanSendAck(SA, GET_VERSION_PO, rezult);
                        break;
                        
                        case GET_ADC1_VALUE:
                            CanSendMess(ADC1_VALUE);
                            rezult = 0;
                            CanSendAck(SA, GET_ADC1_VALUE, rezult);
                        break;
                        
                        case GET_ADC2_VALUE:
                            CanSendMess(ADC2_VALUE);
                            rezult = 0;
                            CanSendAck(SA, GET_ADC2_VALUE, rezult);
                        break;
                        
                        case GET_ADC1_CONV:
                            CanSendMess(ADC1_VALUE_CONV);
                            rezult = 0;
                            CanSendAck(SA, GET_ADC1_CONV, rezult);
                        break;
                        
                        case GET_ADC2_CONV:
                            CanSendMess(ADC2_VALUE_CONV);
                            rezult = 0;
                            CanSendAck(SA, GET_ADC2_CONV, rezult);
                        break;
                        
                        case ON_SCAN:
                            TIM_Cmd(TIM1, ENABLE); //!!!!!!!!!
                            TIM_Cmd(TIM7, ENABLE); //!!!!!!!!!
                            rezult = 0;
                            CanSendAck(SA, ON_SCAN, rezult);
                        break;
                        
                        case OFF_SCAN:
                            TIM_Cmd(TIM1, DISABLE); //!!!!!!!!!
                            TIM_Cmd(TIM7, DISABLE); //!!!!!!!!!
                            rezult = 0;
                            CanSendAck(SA, OFF_SCAN, rezult);
                        break;
                    }
            break;
        }
    }
    return 0;
}

void CanSendAck(uint16_t unit_adress, uint8_t name_command, uint8_t rezult)
{
    CanTxMsg answerMsg;
    
    answerMsg.StdId = 0x00;
    answerMsg.ExtId = CAN_ID_ACK | ((uint8_t)unit_adress<<8) | CAN_SENSOR_ADRESS;
    answerMsg.IDE = CAN_Id_Extended;
    answerMsg.RTR = CAN_RTR_DATA;
    answerMsg.DLC = 8;
    answerMsg.Data[0] = name_command;
    answerMsg.Data[1] = rezult;
    answerMsg.Data[2] = 0xFF;
    answerMsg.Data[3] = 0xFF;
    answerMsg.Data[4] = 0xFF;
    answerMsg.Data[5] = 0xFF;
    answerMsg.Data[6] = 0xFF;
    answerMsg.Data[7] = 0xFF;
    
    CAN_Transmit(CAN1, &answerMsg);
}
