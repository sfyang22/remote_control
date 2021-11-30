/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "stm32h7xx_it.h"
#include "rtthread.h"
#include "rthw.h"
#include "bsp_debug_usart.h"
#include "jpeg_uart.h"
#include "jpeg_spi.h"


/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef UartHandle;
extern UART_HandleTypeDef huart2;

/******************************************************************************/
/*            Cortex Processor Interruption and Exception Handlers         */
/******************************************************************************/
/* 全局变量 */
uint32_t temp, length;
uint8_t buf[2];					//暂时变量

/* 硬件错误中断 */
void  HardError_Handler(void)
{
	__disable_irq();
    rt_kprintf("Hard fault!!\r\n");

    while(1);
}



/**
  * @brief This function handles DMA1 stream0 global interrupt.rx
  */
void DMA1_Stream0_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */
    rt_interrupt_enter();
    /* USER CODE END DMA1_Stream0_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart2_rx);
    /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END DMA1_Stream0_IRQn 1 */
}


/**
  * @brief This function handles DMA1 stream1 global interrupt.tx
  */
void DMA1_Stream1_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */
    rt_interrupt_enter();
    /* USER CODE END DMA1_Stream1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart2_tx);
    /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{

    /* USER CODE BEGIN USART2_IRQn 0 */
    rt_interrupt_enter();

    /* USER CODE END USART2_IRQn 0 */
    HAL_UART_IRQHandler(&huart2);

    /* USER CODE BEGIN USART2_IRQn 1 */
    rt_interrupt_leave();
    /* USER CODE END USART2_IRQn 1 */
}



/**
  * @brief This function handles USART2 RxCpltcallback.
  */

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
 // HAL_DMA_IRQHandler(&hdma_spi2_rx);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

  /* USER CODE END DMA1_Stream3_IRQn 0 */
  //HAL_DMA_IRQHandler(&hdma_spi2_tx);
  /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

  /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**	
  * @brief This function handles SPI2 global interrupt.
  */
void SPI2_IRQHandler(void)
{
  /* USER CODE BEGIN SPI2_IRQn 0 */

  /* USER CODE END SPI2_IRQn 0 */
 // HAL_SPI_IRQHandler(&hspi2);
  /* USER CODE BEGIN SPI2_IRQn 1 */

  /* USER CODE END SPI2_IRQn 1 */
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
