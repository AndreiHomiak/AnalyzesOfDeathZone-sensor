////////////////////////////////////////////////////////////////////////////////
//
// File: /MyDrivers/S25FL127S.c
// Description:
// Author(s): SaverHar as an engineer "SKB WEST"
// Copyright(s):
// Version:
// Creation date: 11-October-2017
//
////////////////////////////////////////////////////////////////////////////////

#include "stm32f10x_spi.h"
#include "s25fl127s.h"

void sFLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

  //Инициализация выводов SPI
  sFLASH_LowLevel_Init();
    
  //Выбор микросхемы памяти
  sFLASH_CS_HIGH();

  //Настройка режима работы SPI
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(sFLASH_SPI, &SPI_InitStructure);

  //Включение SPI
  SPI_Cmd(sFLASH_SPI, ENABLE);
}

void sFLASH_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //Запуск тактирования порта и SPI
  RCC_APB2PeriphClockCmd(sFLASH_CS_GPIO_CLK | sFLASH_SPI_MOSI_GPIO_CLK | 
                         sFLASH_SPI_MISO_GPIO_CLK | sFLASH_SPI_SCK_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(sFLASH_SPI_CLK, ENABLE);
  
  //Настройка пина SCK
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(sFLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  //Настройка пина MOSI
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MOSI_PIN;
  GPIO_Init(sFLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  //Настройка пина MISO
  GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(sFLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
  //Настройка пина CS
  GPIO_InitStructure.GPIO_Pin = sFLASH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(sFLASH_CS_GPIO_PORT, &GPIO_InitStructure);
}

void sFLASH_WriteEnable(void)
{
  sFLASH_CS_LOW();                      //Подключение микросхемы памяти
  sFLASH_SendByte(sFLASH_CMD_WREN);     //Отправка команды разрешения записи
  sFLASH_CS_HIGH();                     //Отключение микросхемы памяти
}

uint8_t sFLASH_SendByte(uint8_t byte)
{
  //Ожидание освобождения региста DR
  while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);
  //Отправка данных по SPI
  SPI_I2S_SendData(sFLASH_SPI, byte);
  //Ожидание получения данных в регистр DR
  while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  //Возврат полученного из SPI байта
  return SPI_I2S_ReceiveData(sFLASH_SPI);
}

void sFLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  sFLASH_CS_LOW();                      //Подключение микросхемы памяти
  sFLASH_SendByte(sFLASH_CMD_RDSR);     //Отправка команды чтения статусного регистра
  
  do//Ожидание особождения микросхемы памяти
  { //Отправка пустого сообщения для получения данных из регистра
    flashstatus = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
  } //Пока микросхема занята
  while ((flashstatus & sFLASH_WIP_FLAG) == SET); 
  
  sFLASH_CS_HIGH();                     //Отключение микросхемы памяти
}

void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  
  sFLASH_WriteEnable();                 //Разрешение записи в память
  sFLASH_CS_LOW();                      //Подключение микросхемы памяти
  sFLASH_SendByte(sFLASH_CMD_WRITE);    //Отправка команды записи в память
  //Отправка адреса побайтно
  sFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  sFLASH_SendByte(WriteAddr & 0xFF);

  while (NumByteToWrite--)              //Пока есть данные для записи
  {
    sFLASH_SendByte(*pBuffer);          //Отправка байта данных
    pBuffer++;                          //Переход к следующему байту
  }

  sFLASH_CS_HIGH();                     //Отключение микросхемы памяти
  sFLASH_WaitForWriteEnd();             //Ожидание особождения микросхемы памяти
}

void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % sFLASH_SPI_PAGESIZE;
  count = sFLASH_SPI_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
  NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

  if (Addr == 0) //Если запись в начало страницы
  {
    if (NumOfPage == 0) //Если число байт для записи меньше размера страницы
    {
      sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else //Число байт для записи больше размера страницы
    {
      while (NumOfPage--)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else //Запись не в начало страницы
  {
    if (NumOfPage == 0) //Если число байт для записи меньше размера страницы
    {
      if (NumOfSingle > count) //Если не влазит до конца этой страницы
      {
        temp = NumOfSingle - count;

        sFLASH_WritePage(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        sFLASH_WritePage(pBuffer, WriteAddr, temp);
      }
      else //Влазит в эту страницу
      {
        sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else //Число байт для записи больше размера страницы
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

      sFLASH_WritePage(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  sFLASH_CS_LOW();                      //Подключение микросхемы памяти
  sFLASH_SendByte(sFLASH_CMD_READ);     //Отправка команды чтения из памяти
  //Отправка адреса побайтно
  sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  sFLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  sFLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  { //Отправка пустого байта для получения данных по SPI
    *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    pBuffer++;                          //Переход к следующему байту буфера
  }

  sFLASH_CS_HIGH();                     //Отключение микросхемы памяти
}