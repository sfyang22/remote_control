#include "ch_input.h"

//遥控器通道初始化
void CH_INPUT_Init(void)
{
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_ADC3_Init();					//ADC初始化
	
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);			
	HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);		//ADC偏移自校准，需在启动前校准
	
	HAL_ADC_Start(&hadc3);			//开启ADC
}


//获取遥控器通道ADC值
void CH_INPUT_StartSample(uint16_t* ch_value) 
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t* ) ch_value, 5);
}


//获取CPU温度
float MCU_Tempreture(void)
{
	float temp,temp2;
	
	temp = (float)HAL_ADC_GetValue(&hadc3);
	temp2 = (110.0 - 30.0) / (*(unsigned short*)(0x1FF1E840) - *(unsigned short*)(0x1FF1E820));
	temp = (float)temp2*(temp - *(unsigned short*)(0x1FF1E820))+30;
	
	return temp;
}

