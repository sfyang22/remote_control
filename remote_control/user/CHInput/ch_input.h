#ifndef __CH_INPUT_H_
#define __CH_INPUT_H_


/* ch_input Connected */
/*
    PC4     ------> SERVO1
    PC5     ------> SERVO2
    PC1     ------> ROCKER1
    PA0     ------> ROCKER2
    PA1     ------> LiPO_POWER

*/

#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "stm32h7xx_it.h"
#include "adc.h"
#include "dma.h"



void CH_INPUT_Init(void);
void CH_INPUT_StartSample(uint16_t* ch_value) ;
float MCU_Tempreture(void);
#endif


