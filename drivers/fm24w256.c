////////////////////////////////////////////////////////////////////////////////
//
// File: /MyDriver/fm24w256.c
// Description: Functions work with Fram
// Author(s): RiSuS as an engineer "SKB WEST"
// Copyright(s):
// Version: V1.0.0
// Date: 18-May-2015
//
////////////////////////////////////////////////////////////////////////////////

#include "stm32f10x_i2c.h"
#include "stm32f10x_dma.h"
#include "fm24w256.h"

__IO uint32_t  waitFramTimeout ;


void ee_write_8t(uint8_t data, uint16_t addr)
{
  writeFRAM(addr, (uint8_t*)(&data), 1);
}

void ee_write_16t(uint16_t data, uint16_t addr)
{
  writeFRAM(addr, (uint8_t*)(&data), 2);
}

void ee_write_32t(uint32_t data, uint16_t addr)
{
  writeFRAM(addr, (uint8_t*)(&data), 4);
}

uint8_t ee_read_8t(uint16_t addr)
{
  uint8_t data = 0;
  readFRAM((uint8_t*)(&data), addr, 1);
  return data;
}

uint16_t ee_read_16t(uint16_t addr)
{
  uint16_t data = 0;
  readFRAM((uint8_t*)(&data), addr, 2);
  return data;
}

uint32_t ee_read_32t(uint16_t addr)
{
  uint32_t data = 0;
  readFRAM((uint8_t*)(&data), addr, 4);
  return data;
}

uint8_t ee_read_8t_interval(uint16_t addr, uint8_t min, uint8_t max)
{
  uint8_t data = 0;
  readFRAM((uint8_t*)(&data), addr, 1);
  if(min<max) {
    if(data<min) {
      data = min;
      writeFRAM(addr, (uint8_t*)(&data), 1);
    }
    if(data>max) {
      data = max;
      writeFRAM(addr, (uint8_t*)(&data), 1);
    }
  }
  return data;
}

uint16_t ee_read_16t_interval(uint16_t addr, uint16_t min, uint16_t max)
{
  uint16_t data = 0;
  readFRAM((uint8_t*)(&data), addr, 2);
  if(min<max) {
    if(data<min) {
      data = min;
      writeFRAM(addr, (uint8_t*)(&data), 2);
    }
    if(data>max) {
      data = max;
      writeFRAM(addr, (uint8_t*)(&data), 2);
    }
  }
  return data;
}

uint32_t ee_read_32t_interval(uint16_t addr, uint32_t min, uint32_t max)
{
  uint32_t data = 0;
  readFRAM((uint8_t*)(&data), addr, 4);
  if(min<max) {
    if(data<min) {
      data = min;
      writeFRAM(addr, (uint8_t*)(&data), 4);
    }
    if(data>max) {
      data = max;
      writeFRAM(addr, (uint8_t*)(&data), 4);
    }
  }
  return data;
}

void initFRAM(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
  
  /*!< sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
  /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(I2C_SCL_GPIO_CLK | I2C_SDA_GPIO_CLK, ENABLE);
  /* Enable I2C1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2Cx, ENABLE);
  
  /*!< GPIO configuration */  
   /* Configure I2C1 pins: SCL and SDA -------------------------*/  
  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /* Connect PXx to I2C_SCL*/
  //GPIO_PinAFConfig(I2C_SCL_GPIO_PORT, I2C_SCL_SOURCE, I2C_SCL_AF);
  /* Connect PXx to I2C_SDA*/
  //GPIO_PinAFConfig(I2C_SDA_GPIO_PORT, I2C_SDA_SOURCE, I2C_SDA_AF);
  
  I2C_DeInit(I2Cx);
  /*!< I2C configuration */
  /* sEE_I2C configuration */
  I2C_StructInit(&I2C_InitStructure);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_AcknowledgedAddress_10bit;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_10bit;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  I2C_Init(I2Cx, &I2C_InitStructure);
  I2C_Cmd(I2Cx, ENABLE);
  I2C_AcknowledgeConfig(I2Cx, DISABLE);
}

uint8_t testFRAM(void)
{
  uint32_t i, ret=0;;
  uint8_t data;

  
  //return 0;
  
  data = 0;
//  printf("Test FRAM started...\n");
//  printf("Write 0x00...");
  for(i=0;i<MEMORY_CAPASITY;i++) {
    writeFRAM(i, (uint8_t*)(&data), 1);
  }
//  printf("complete\n");
//  printf("Read 0x00...");
  for(i=0;i<MEMORY_CAPASITY;i++) {
    readFRAM((uint8_t*)(&data), i, 1);
    if(data!=0) ret++;
  }
//  printf("complete\n");
//  printf("Write 0xFF...");
  data = 0xFF;
  for(i=0;i<MEMORY_CAPASITY;i++) {
    writeFRAM(i, (uint8_t*)(&data), 1);
  }
//  printf("complete\n");
//  printf("Read 0xFF...");
  for(i=0;i<MEMORY_CAPASITY;i++) {
    readFRAM((uint8_t*)(&data), i, 1);
    if(data!=0xFF) ret++;
  }
//  printf("complete\n");
//  printf("Test FRAM completed!\n");
  return ret;
}

uint32_t writeFRAM(uint32_t WriteAddr, uint8_t* pBuffer,  uint32_t NumByteToWrite)
{
  uint32_t i;
  WAIT_FRAM_FLAG(I2C_FLAG_BUSY);
  I2C_GenerateSTART(I2Cx, ENABLE);
  CHECK_FRAM_EVENT(I2C_EVENT_MASTER_MODE_SELECT);
  I2C_Send7bitAddress(I2Cx, I2C_HW_ADDRESS, I2C_Direction_Transmitter);
  CHECK_FRAM_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
  I2C_SendData(I2Cx, (WriteAddr&0xFF00)>>8);
  CHECK_FRAM_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
  I2C_SendData(I2Cx, WriteAddr&0xFF);
  CHECK_FRAM_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
  for(i=0; i<NumByteToWrite; i++) {
    I2C_SendData(I2Cx, *pBuffer);
    pBuffer++;
    CHECK_FRAM_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
  }
  I2C_GenerateSTOP(I2Cx, ENABLE);
  WAIT_FRAM_FLAG(I2C_FLAG_BUSY);
  return 0;
}

uint32_t readFRAM(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
  uint32_t i;
  WAIT_FRAM_FLAG(I2C_FLAG_BUSY);
  I2C_GenerateSTART(I2Cx, ENABLE);
  CHECK_FRAM_EVENT(I2C_EVENT_MASTER_MODE_SELECT);
  I2C_Send7bitAddress(I2Cx, I2C_HW_ADDRESS, I2C_Direction_Transmitter);
  CHECK_FRAM_EVENT(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
  I2C_SendData(I2Cx, (ReadAddr&0xFF00)>>8);
  CHECK_FRAM_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
  I2C_SendData(I2Cx, ReadAddr&0xFF);
  CHECK_FRAM_EVENT(I2C_EVENT_MASTER_BYTE_TRANSMITTED);
  I2C_AcknowledgeConfig(I2Cx, DISABLE);
  for(i=0; i<NumByteToRead; i++) {
    I2C_GenerateSTART(I2Cx, ENABLE);
    CHECK_FRAM_EVENT(I2C_EVENT_MASTER_MODE_SELECT);
    I2C_Send7bitAddress(I2Cx, I2C_HW_ADDRESS, I2C_Direction_Receiver);
    CHECK_FRAM_EVENT(I2C_EVENT_MASTER_BYTE_RECEIVED);
    *pBuffer = I2C_ReceiveData(I2Cx);
    pBuffer++;
  }
  I2C_GenerateSTOP(I2Cx, ENABLE);
  WAIT_FRAM_FLAG(I2C_FLAG_BUSY);
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  return 0;  
}

uint32_t FRAM_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  while (1) {   
  }
}
