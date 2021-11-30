#ifndef __LCD_BLK_H_
#define __LCD_BLK_H_

#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "stm32h7xx_it.h"
#include "tim.h"



void LCD_BLK_Init(void);
void LCD_BLK_Adjust(uint8_t compare);

#endif



