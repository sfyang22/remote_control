#include "freeback.h"
#include "stdlib.h"

//����� ��
//���룺�𶯴�С
static void FREEBACK_LeftStrength(uint16_t compare)
{	
	TIM3->CCR3 = compare;
}
//����� ��
//���룺�𶯴�С
static void FREEBACK_RightStrength(uint16_t compare)
{
	TIM3->CCR1 = compare;
}

//�𶯷�����ʼ��
void FREEBACK_Init(void)
{	
	//MX_TIM3_Init();
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	FREEBACK_LeftStrength(0);
	FREEBACK_RightStrength(0);
}

//������
//���룺pitch�����ǲ�ֵ roll����ǲ�ֵ P��������
//���أ���
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


