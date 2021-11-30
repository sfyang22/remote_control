#ifndef __LED_H_
#define __LED_H_

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

/* GPIO define start */
#define LED_PORT		GPIOA
#define LED_PIN			GPIO_PIN_8
#define LED_CLK			__HAL_RCC_GPIOA_CLK_ENABLE
/* GPIO define end */

/* GPIO function define start */
#define LED_HIGH		HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET)
#define LED_LOW			HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET)
/* GPIO function define end */

void LED_Init(void);


#endif


