#include "buzz.h"

void BUZZ_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStrutre;
	
	BUZZ_CLK();
	
	GPIO_InitStrutre.Pin = BUZZ_PIN;
	GPIO_InitStrutre.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStrutre.Pull = GPIO_NOPULL;
	GPIO_InitStrutre.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(BUZZ_PORT, &GPIO_InitStrutre);
	
	BUZZ_CLOSE();
}


