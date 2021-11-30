/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "jpeg_uart.h"

/* USER CODE BEGIN 0 */
static uint8_t photo[JPEG_SIZE];
/* USER CODE END 0 */


void JPEG_UART_Init(void)
{	
	MX_DMA_Init();
	MX_USART2_UART_Init();
}
		

//显示JPEG图片
//返回：一帧字节
uint32_t JPEG_Display(void)
{
	uint32_t rec_length;					//接收长度

	if(__HAL_DMA_GET_FLAG(&hdma_usart2_rx, DMA_FLAG_TCIF0_4) != RESET || __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)			//获取传输完成标志位
	{
		GUI_JPEG_Draw(photo, JPEG_SIZE, 0, 20);										//绘画JPEG图片
		
		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx, DMA_FLAG_TCIF0_4);				//清除标志位
		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx, DMA_FLAG_HTIF0_4);				//清除标志位
		__HAL_DMA_CLEAR_FLAG(&hdma_usart2_rx, DMA_FLAG_TEIF0_4);				//清除标志位
		__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_ORE);							//清除溢出标志位，不清楚，溢出会卡死

		HAL_UART_DMAStop(&huart2);				//停止DMA传输
		rec_length = JPEG_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);			//返回接收数据长度
		HAL_UART_DMAResume(&huart2);			//重启DMA传输
		HAL_UART_Receive_DMA(&huart2, photo, JPEG_SIZE);			//重新打开DMA接收
		//rt_kprintf("length = %d\r\n", rec_length);
		return rec_length;
	}
	
	return 0;
}

void JPEF_FirstRec(void)
{
	HAL_UART_Receive_DMA(&huart2, photo, JPEG_SIZE);
}


/* USER CODE END 1 */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
