#ifndef __BUZZ_H_
#define __BUZZ_H_


/* buzz Connected */
/*
    PE5     ------> buzz

*/

#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "stm32h7xx_it.h"

/* GPIO define start */
#define BUZZ_PORT			GPIOE
#define BUZZ_PIN			GPIO_PIN_5
#define BUZZ_CLK			__HAL_RCC_GPIOE_CLK_ENABLE
/* GPIO define end */

/* GPIO function define start */
#define BUZZ_OPEN()			HAL_GPIO_WritePin(BUZZ_PORT, BUZZ_PIN, GPIO_PIN_RESET)
#define BUZZ_CLOSE()			HAL_GPIO_WritePin(BUZZ_PORT, BUZZ_PIN, GPIO_PIN_SET)
/* GPIO function define end */
void BUZZ_Init(void);

#endif
