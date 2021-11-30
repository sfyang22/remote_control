#ifndef __WIFI_UART_H_
#define __WIFI_UART_H_

/**WIFI Connect
PA11     ------> WIFI_TXD
PA12     ------> WIFI_RXD
PE0     ------> WIFI_AUX
*/

#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "stm32h7xx_it.h"
#include "uart.h"

#define REC_COUNT		9
#define MPU_COUNT		3

extern uint8_t rec_dat[];
extern uint8_t rec_dat_flag;
extern uint8_t mpu_dat[];
extern uint8_t mpu_dat_flag;

void WIFI_UART_Init(void);
void WIFI_SendData(uint8_t *dat, uint16_t len);
void WIFI_AUX_Waiting(void);

#endif
