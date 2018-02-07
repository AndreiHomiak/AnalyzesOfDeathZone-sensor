////////////////////////////////////////////////////////////////////////////////
//
// File: /MyDrivers/S25FL127S.h
// Description:
// Author(s): SaverHar as an engineer "SKB WEST"
// Copyright(s):
// Version:
// Creation date: 05-October-2017
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __S25FL127S_H
#define __S25FL127S_H

/*#define SPI_CS_GPIO_CLK         RCC_APB2Periph_GPIOA        // GPIOA
#define SPI_SCK_GPIO_CLK        RCC_APB2Periph_GPIOA        // GPIOA
#define SPI_MISO_GPIO_CLK       RCC_APB2Periph_GPIOA        // GPIOA
#define SPI_MOSI_GPIO_CLK       RCC_APB2Periph_GPIOA        // GPIOA
#define SPI_CS_GPIO_PORT        GPIOA                       // GPIOA
#define SPI_SCK_GPIO_PORT       GPIOA                       // GPIOA
#define SPI_MISO_GPIO_PORT      GPIOA                       // GPIOA
#define SPI_MOSI_GPIO_PORT      GPIOA                       // GPIOA
#define SPI_CS_PIN              GPIO_Pin_4                  // PA.04
#define SPI_SCK_PIN             GPIO_Pin_5                  // PA.05
#define SPI_MISO_PIN            GPIO_Pin_6                  // PA.06
#define SPI_MOSI_PIN            GPIO_Pin_7                  // PA.07
#define SPI_CS_SOURCE           GPIO_PinSource4
#define SPI_SCK_SOURCE          GPIO_PinSource5
#define SPI_MISO_SOURCE         GPIO_PinSource6
#define SPI_MOSI_SOURCE         GPIO_PinSource7
//#define SPI_CS_AF               GPIO_AF_SPI1
//#define SPI_SCK_AF              GPIO_AF_SPI1
//#define SPI_MISO_AF             GPIO_AF_SPI1
//#define SPI_MOSI_AF             GPIO_AF_SPI1

#define SPIx                    SPI1
#define RCC_APB2Periph_SPIx     RCC_APB2Periph_SPI1*/

//******************************************************************************

#define sFLASH_SPI                       SPI1
#define sFLASH_SPI_CLK                   RCC_APB2Periph_SPI1
#define sFLASH_SPI_SCK_PIN               GPIO_Pin_5                  /* PA.05 */
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define sFLASH_SPI_SCK_GPIO_CLK          RCC_APB2Periph_GPIOA
#define sFLASH_SPI_MISO_PIN              GPIO_Pin_6                  /* PA.06 */
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MISO_GPIO_CLK         RCC_APB2Periph_GPIOA
#define sFLASH_SPI_MOSI_PIN              GPIO_Pin_7                  /* PA.07 */
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MOSI_GPIO_CLK         RCC_APB2Periph_GPIOA
#define sFLASH_CS_PIN                    GPIO_Pin_4                  /* PA.04 */
#define sFLASH_CS_GPIO_PORT              GPIOA                       /* GPIOA */
#define sFLASH_CS_GPIO_CLK               RCC_APB2Periph_GPIOA

//Команды для работы с микросхемой памяти
#define sFLASH_CMD_WRITE        0x02    //Запись данных в память
#define sFLASH_CMD_WRSR         0x01    //Запись в статусный регистр
#define sFLASH_CMD_WREN         0x06    //Рзрешение записи в память
#define sFLASH_CMD_READ         0x03    //Чтение данных из памяти
#define sFLASH_CMD_RDSR         0x05    //Чтение статусного регистра
#define sFLASH_CMD_RDID         0x9F    //Чтение идентификационной информации
#define sFLASH_CMD_SE           0xD8    //Очистка сектора
#define sFLASH_CMD_BE           0xC7    //Очистка всей памяти

#define sFLASH_WIP_FLAG         0x01    //Флаг заятости микросхемы (WIP)

#define sFLASH_DUMMY_BYTE       0xA5    //Фиктивный байт
#define sFLASH_SPI_PAGESIZE     0x100   //Размер страницы памяти

#define sFLASH_CS_LOW()         GPIO_ResetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
#define sFLASH_CS_HIGH()        GPIO_SetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)   

//******************************************************************************

void sFLASH_Init(void);
void sFLASH_LowLevel_Init(void);

void sFLASH_WriteEnable(void);
uint8_t sFLASH_SendByte(uint8_t byte);
void sFLASH_WaitForWriteEnd(void);

void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

//uint8_t testFRAM(void);
//uint32_t writeFRAM(uint32_t WriteAddr, uint8_t* pBuffer,  uint32_t NumByteToWrite);
//uint32_t readFRAM(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
//uint32_t FRAM_TIMEOUT_UserCallback(void);

#endif // __S25FL127S_H