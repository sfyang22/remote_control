/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif
	
	
	
/*************GPIO Conected*************
        PC1     ------> NRF_MOSI
        PC2_C     ------> NRF_MISO
        PB10     ------> NRF_SCK
		PC0     ------> NRF_CSN
		PC3     ------> NRF_CE
*************GPIO Conected*************/

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_it.h"
#include "touch.h"

#define TX_ADR_WIDTH   5  				// 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 32  				// 数据通道有效数据宽度

//********************************************************************************************************************//
// SPI(nRF24L01+) commands
#define NRF_READ_REG        0x00  // Define read command to register
#define NRF_WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NOP             0xFF  // Define No Operation, might be used to read status register

//********************************************************************************************************************//
// SPI(nRF24L01+) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define RPD             0x09  // 'Received Power Detector' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address

//********************************************************************************************************************//
// STATUS Register 
#define RX_DR						0x40  /**/
#define TX_DS						0x20
#define MAX_RT					0x10

//********************************************************************************************************************//
//                                        FUNCTION's PROTOTYPES                                                       //
//********************************************************************************************************************//
//nRF24L01+ API Functions
void nRF24L01P_Init(void); //24L01+ Pin Init
uint8_t nRF24L01P_Write_Reg(uint8_t reg, uint8_t value); 
uint8_t nRF24L01P_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t bytes);
uint8_t nRF24L01P_Read_Reg(uint8_t reg);
uint8_t nRF24L01P_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);

void nRF24L01P_RX_Mode(void);
void nRF24L01P_TX_Mode(void);
uint8_t nRF24L01P_RxPacket(uint8_t *rxbuf);
uint8_t nRF24L01P_TxPacket(uint8_t *txbuf);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
