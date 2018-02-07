// 09.01.2018
// Analyzes of death zone
// sensor
// Andrei Komiak

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "system_stm32f10x.h"

#include "adc.h"
#include "can.h"
#include "fm24w256.h"
#include "s25fl127s.h"
#include "timers.h"
#include "exti.h"
#include "pins.h"
#include "canHandler.h"

int main()
{
    InitADC1();
//    sFLASH_Init();
 //   initFRAM();
    InitCan1();
//    InitExtIRQ();
    InitPins();
    ReInitPins(5);
    InitTimers();
    
    TIM_Cmd(TIM1, ENABLE);
    
    while(1)
    {
        CanSpamer();
    }
    
  return 0;
}
