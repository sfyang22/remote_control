#include "lcd_blk.h"


void LCD_BLK_Init(void)
{
    MX_TIM3_Init();
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    LCD_BLK_Adjust(100);
	
}


void LCD_BLK_Adjust(uint8_t compare)
{
	TIM3->CCR4 = compare;
}
