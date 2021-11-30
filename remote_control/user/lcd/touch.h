#ifndef __TOUCH_H_
#define __TOUCH_H_

/**TOUCH Connect def
PA4     ------> T_CS
PA5     ------> TCLK
PA6     ------> T_MISO
PA7     ------> T_MOSI
PA2     ------> T_PEN
*/

/* includes */
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"
#include "jpeg_spi.h"

/* user def */
//²ÉÑùÂÊ
#define SAMP_CNT 4
#define SAMP_CNT_DIV2 2

/* xpt2046_command */
#define X_OFFSET			0x90
#define Y_OFFSET			0xD0


/* touch_structure */
typedef struct{
	uint16_t x;
	uint16_t y;
}TOUCH;


/* GPIO def */
#define CS_GPIO_PORT						GPIOA
#define	CS_GPIO_PIN							GPIO_PIN_4
#define CS_GPIO_CLK							__HAL_RCC_GPIOA_CLK_ENABLE

#define PEN_GPIO_PORT						GPIOA
#define	PEN_GPIO_PIN						GPIO_PIN_2
#define PEN_GPIO_CLK						__HAL_RCC_GPIOA_CLK_ENABLE

/* JPEG_SPI_CS */
#define JPEG_CS_GPIO_PORT					GPIOC
#define	JPEG_CS_GPIO_PIN					GPIO_PIN_0
#define JPEG_CS_GPIO_CLK					__HAL_RCC_GPIOC_CLK_ENABLE
/* JPEG_SPI_CE */
#define JPEG_CE_GPIO_PORT					GPIOC
#define	JPEG_CE_GPIO_PIN					GPIO_PIN_3
#define JPEG_CE_GPIO_CLK					__HAL_RCC_GPIOC_CLK_ENABLE

/* GPIO control function */
#define CS_HIGH								HAL_GPIO_WritePin(CS_GPIO_PORT, CS_GPIO_PIN, GPIO_PIN_SET)		
#define CS_LOW								HAL_GPIO_WritePin(CS_GPIO_PORT, CS_GPIO_PIN, GPIO_PIN_RESET)
/* JPEG_SPI_CS */
#define JPEG_CS_HIGH						HAL_GPIO_WritePin(JPEG_CS_GPIO_PORT, JPEG_CS_GPIO_PIN, GPIO_PIN_SET)		
#define JPEG_CS_LOW							HAL_GPIO_WritePin(JPEG_CS_GPIO_PORT, JPEG_CS_GPIO_PIN, GPIO_PIN_RESET)
/* JPEG_SPI_CE */
#define JPEG_CE_HIGH						HAL_GPIO_WritePin(JPEG_CE_GPIO_PORT, JPEG_CE_GPIO_PIN, GPIO_PIN_SET)		
#define JPEG_CE_LOW							HAL_GPIO_WritePin(JPEG_CE_GPIO_PORT, JPEG_CE_GPIO_PIN, GPIO_PIN_RESET)

#define PEN_READ()							HAL_GPIO_ReadPin(PEN_GPIO_PORT, PEN_GPIO_PIN)		



/* function */
void LCD_TOUCH_Init(void);
//uint8_t TOUCH_GetOffset(TOUCH *touch);
uint16_t XPT2046_ReadAdc_Fliter(uint8_t channel);
void TOUCH_First_Calibrate(void);
uint8_t _Calibrate(void);

#endif






