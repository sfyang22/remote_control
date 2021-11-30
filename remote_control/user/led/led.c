#include "led.h"

static void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStrutre;
	
	LED_CLK();
	
	GPIO_InitStrutre.Pin = LED_PIN;
	GPIO_InitStrutre.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStrutre.Pull = GPIO_NOPULL;
	GPIO_InitStrutre.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStrutre);
	
}

void LED_Init(void)
{
	GPIO_Config();
}



