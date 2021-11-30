#include "ch_input.h"

//ң����ͨ����ʼ��
void CH_INPUT_Init(void)
{
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_ADC3_Init();					//ADC��ʼ��
	
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);			
	HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);		//ADCƫ����У׼����������ǰУ׼
	
	HAL_ADC_Start(&hadc3);			//����ADC
}


//��ȡң����ͨ��ADCֵ
void CH_INPUT_StartSample(uint16_t* ch_value) 
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t* ) ch_value, 5);
}


//��ȡCPU�¶�
float MCU_Tempreture(void)
{
	float temp,temp2;
	
	temp = (float)HAL_ADC_GetValue(&hadc3);
	temp2 = (110.0 - 30.0) / (*(unsigned short*)(0x1FF1E840) - *(unsigned short*)(0x1FF1E820));
	temp = (float)temp2*(temp - *(unsigned short*)(0x1FF1E820))+30;
	
	return temp;
}

