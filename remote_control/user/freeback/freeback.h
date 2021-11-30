#ifndef __FREEBACK_H_
#define __FREEBACK_H_

/**freeback Connected
PC8     ------> motorL
PC6		------> motorR
*/

#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "stm32h7xx_it.h"
#include "tim.h"

void FREEBACK_Init(void);
void FREEBACK_Contorl(int8_t pitch, int8_t roll, int16_t P);

#endif







