#ifndef __LCD_DRV_H_
#define __LCD_DRV_H_

/************** LCD Connect *****************/
/*
    PE7   ------> LCD_D5
    PE8   ------> LCD_D6
    PE9   ------> LCD_D7
    PE10   ------> LCD_D8
    PE11   ------> LCD_D9
    PE12   ------> LCD_D10
    PE13   ------> LCD_D11
    PE14   ------> LCD_D12
    PE15   ------> LCD_D13
    PD8   ------> LCD_D14
    PD9   ------> LCD_D15
    PD10   ------> LCD_D16
    PD11   ------> LCD_RS
    PD14   ------> LCD_D1
    PD15   ------> LCD_D2
    PC7   ------> LCD_CS
    PD0   ------> LCD_D3
    PD1   ------> LCD_D4
    PD4   ------> LCD_RD
    PD5   ------> LCD_WR
	PB10   ------> LCD_RES
	PB1   ------> LCD_BLK
*/
/************** LCD Connect *****************/

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"

/************** USER CONFIG *****************/

#define      BACKGROUND		               BLUE   //Ĭ�ϱ�����ɫ



/************** USER CONFIG *****************/

/******************************* ���� ILI934 ��ʾ��������ɫ ********************************/
#define      WHITE		 		           0xFFFF	   //��ɫ
#define      BLACK                         0x0000	   //��ɫ 
#define      GREY                          0xF7DE	   //��ɫ 
#define      BLUE                          0x001F	   //��ɫ 
#define      BLUE2                         0x051F	   //ǳ��ɫ 
#define      RED                           0xF800	   //��ɫ 
#define      MAGENTA                       0xF81F	   //����ɫ�����ɫ 
#define      GREEN                         0x07E0	   //��ɫ 
#define      CYAN                          0x7FFF	   //����ɫ����ɫ 
#define      YELLOW                        0xFFE0	   //��ɫ 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF	

//����Һ��ɨ�跽����仯��XY���ؿ��
//����ILI9341_GramScan�������÷���ʱ���Զ�����
extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH; 

//Һ����ɨ��ģʽ
//������ѡֵΪ0-7
extern uint8_t LCD_SCAN_MODE;

/* lcd_address define start */	
#define LCD_CMD				(__IO uint16_t*)(0x60000000)		//��������
#define LCD_DAT				(__IO uint16_t*)(0x60020000)		//��������
/* lcd_address define end */

/* GPIO define start */
#define LCD_RST_PORT		GPIOB
#define LCD_RST_PIN			GPIO_PIN_0
#define LCD_RST_CLK			__HAL_RCC_GPIOB_CLK_ENABLE

#define LCD_BLK_PORT		GPIOB
#define LCD_BLK_PIN			GPIO_PIN_1
#define LCD_BLK_CLK			__HAL_RCC_GPIOB_CLK_ENABLE
/* GPIO define end */

/* GPIO function define start */
#define LCD_RST_HIGH		HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)
#define LCD_RST_LOW			HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)

#define LCD_BLK_HIGH		HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_SET)
#define LCD_BLK_LOW			HAL_GPIO_WritePin(LCD_BLK_PORT, LCD_BLK_PIN, GPIO_PIN_RESET)
/* GPIO function define end */


/*************************************** ����Ԥ�� ******************************************/

#define DEBUG_DELAY()              

/******************************* ���� ILI934 ��Ļ��С ********************************/
	
#define	ILI9341_LESS_PIXEL							240
#define	ILI9341_MORE_PIXEL							320

/******************************* ���� ILI934 �������� ********************************/
#define CMD_SetCoordinateX		 		    0x2A	     //����X����
#define CMD_SetCoordinateY		 		    0x2B	     //����Y����
#define	CMD_SetPixel		 		        0x2C	     //�������


#define RGB888_2_RGB565(R,G,B)		 (uint16_t)(((R & 0x1F) << 11) | ((G & 0x3F) << 5) | (B & 0x1F) )

/* function declare */
void LCD_Init(void);
void MPU_Config(void);
void LCD_Open_Window(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight);
void LCD_Clear(void);
void LCD_DrawPoint(uint16_t usX, uint16_t usY, uint16_t color);
void LCD_GramScan ( uint8_t ucOption );
void delay2(uint32_t i);
#endif 

