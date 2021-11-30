#ifndef __JPEG_UART_H_
#define __JPEG_UART_H_

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"
#include "GUI.h"
#include "uart.h"
#include "dma.h"


#ifdef __cplusplus
extern "C" {
#endif

/* ∫Í∂®“Â */
#define JPEG_SIZE				930	

void JPEG_UART_Init(void);
uint32_t JPEG_Display(void);
void JPEF_FirstRec(void);


#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */




