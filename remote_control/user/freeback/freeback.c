#include "freeback.h"
#include "stdlib.h"

//震动输出 左
//输入：震动大小
static void FREEBACK_LeftStrength(uint16_t compare)
{	
	TIM3->CCR3 = compare;
}
//震动输出 右
//输入：震动大小
static void FREEBACK_RightStrength(uint16_t compare)
{
	TIM3->CCR1 = compare;
}

//震动反馈初始化
void FREEBACK_Init(void)
{	
	//MX_TIM3_Init();
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	FREEBACK_LeftStrength(0);
	FREEBACK_RightStrength(0);
}

//控制震动
//输入：pitch俯仰角差值 roll横滚角差值 P反馈力度
//返回：无
void FREEBACK_Contorl(int8_t pitch, int8_t roll, int16_t P) 
{
	int16_t left, right;
	
	left = right = P * abs(pitch);
	
	if(roll > 0)
	{
		left += P * roll * roll;
	}
	else
	{
		roll = -roll;
		right += P * roll * roll;
	}
	
	FREEBACK_LeftStrength((uint16_t)left);
	FREEBACK_RightStrength((uint16_t)right);
}


