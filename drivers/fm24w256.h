////////////////////////////////////////////////////////////////////////////////
//
// File: /MyDriver/fm24w256.c
// Description: Header for functions work with Fram
// Author(s): RiSuS as an engineer "SKB WEST"
// Copyright(s):
// Version: V1.0.0
// Date: 18-May-2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __FM24W256_H
#define __FM24W256_H

#define I2C_SCL_GPIO_CLK                RCC_APB2Periph_GPIOB        // GPIOB
#define I2C_SDA_GPIO_CLK                RCC_APB2Periph_GPIOB        // GPIOB
#define I2C_SCL_GPIO_PORT               GPIOB                       // GPIOB
#define I2C_SDA_GPIO_PORT               GPIOB                       // GPIOB
#define I2C_SCL_PIN                     GPIO_Pin_6                  // PB.06
#define I2C_SDA_PIN                     GPIO_Pin_7                  // PB.07
#define I2C_SCL_SOURCE                  GPIO_PinSource6
#define I2C_SDA_SOURCE                  GPIO_PinSource7
//#define I2C_SCL_AF                      GPIO_AF_I2C1
//#define I2C_SDA_AF                      GPIO_AF_I2C1

#define I2Cx                            I2C1
#define RCC_APB1Periph_I2Cx             RCC_APB1Periph_I2C1
#define I2C_SPEED                       400000
#define I2C_HW_ADDRESS                  0xA0

#define FRAM_FLAG_TIMEOUT               ((uint32_t)0x10000)

#define MEMORY_CAPASITY                 ((uint32_t)32768)

#define WAIT_FRAM_FLAG(flag)              waitFramTimeout = FRAM_FLAG_TIMEOUT;\
                                          while(I2C_GetFlagStatus(I2Cx, flag)) {\
                                            if((waitFramTimeout--) == 0) return FRAM_TIMEOUT_UserCallback();\
                                          }

#define CHECK_FRAM_EVENT(event)           waitFramTimeout = FRAM_FLAG_TIMEOUT;\
                                          while(!I2C_CheckEvent(I2Cx, event)) {\
                                            if((waitFramTimeout--) == 0) return FRAM_TIMEOUT_UserCallback();\
                                          }

void ee_write_8t(uint8_t data, uint16_t addr);
void ee_write_16t(uint16_t data, uint16_t addr);
void ee_write_32t(uint32_t data, uint16_t addr);
uint8_t ee_read_8t(uint16_t addr);
uint16_t ee_read_16t(uint16_t addr);
uint32_t ee_read_32t(uint16_t addr);
uint8_t ee_read_8t_interval(uint16_t addr, uint8_t min, uint8_t max);
uint16_t ee_read_16t_interval(uint16_t addr, uint16_t min, uint16_t max);
uint32_t ee_read_32t_interval(uint16_t addr, uint32_t min, uint32_t max);

void initFRAM(void);
uint8_t testFRAM(void);
uint32_t writeFRAM(uint32_t WriteAddr, uint8_t* pBuffer,  uint32_t NumByteToWrite);
uint32_t readFRAM(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint32_t FRAM_TIMEOUT_UserCallback(void);
#endif // __FM24W256_H