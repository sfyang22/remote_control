/* Includes ------------------------------------------------------------------*/
#include "jpeg_spi.h"
#include <stdlib.h>

//MSP初始化配置请看在TOUCH文件已经配置

SPI_HandleTypeDef hspi2;

const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x01, 0x07, 0x0E, 0x01}; // 定义一个静态发送地址

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 0x0;
    hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_ENABLE;
    hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;

    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        HardError_Handler();
    }

}


static uint8_t SPI_RW(uint8_t byte)
{
    uint8_t rec_byte;

    HAL_SPI_TransmitReceive(&hspi2, &byte, &rec_byte, 1, 100);

    return rec_byte;
}


/********************************************************
函数功能：nRF24L01+引脚初始化
入口参数：无
返回  值：无
*********************************************************/
void nRF24L01P_Init(void)
{
    MX_SPI2_Init();
    JPEG_CS_HIGH;
}


/********************************************************
函数功能：写寄存器的值（单字节）
入口参数：reg:寄存器映射地址（格式：NRF_WRITE_REG｜reg）
					value:寄存器的值
返回  值：状态寄存器的值
*********************************************************/
uint8_t nRF24L01P_NRF_WRITE_REG(uint8_t reg, uint8_t value)
{
    uint8_t status;

    JPEG_CS_LOW;
    status = SPI_RW(reg);
    SPI_RW(value);
    JPEG_CS_HIGH;

    return(status);
}


/********************************************************
函数功能：写寄存器的值（多字节）
入口参数：reg:寄存器映射地址（格式：NRF_WRITE_REG｜reg）
					pBuf:写数据首地址
					bytes:写数据字节数
返回  值：状态寄存器的值
*********************************************************/
uint8_t nRF24L01P_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_ctr;

    JPEG_CS_LOW;
    status = SPI_RW(reg);

    for(byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        SPI_RW(*pBuf++);

    JPEG_CS_HIGH;

    return(status);
}


/********************************************************
函数功能：读取寄存器的值（单字节）
入口参数：reg:寄存器映射地址（格式：READ_REG｜reg）
返回  值：寄存器值
*********************************************************/
uint8_t nRF24L01P_Read_Reg(uint8_t reg)
{
    uint8_t value;

    JPEG_CS_LOW;
    SPI_RW(reg);
    value = SPI_RW(0);
    JPEG_CS_HIGH;

    return(value);
}

	
/********************************************************
函数功能：读取寄存器的值（多字节）
入口参数：reg:寄存器映射地址（READ_REG｜reg）
					pBuf:接收缓冲区的首地址
					bytes:读取字节数
返回  值：状态寄存器的值
*********************************************************/
uint8_t nRF24L01P_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_ctr;

    JPEG_CS_LOW;
    status = SPI_RW(reg);

    for(byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        pBuf[byte_ctr] = SPI_RW(0);                   //读取数据，低字节在前

    JPEG_CS_HIGH;

    return(status);
}


/********************************************************
函数功能：nRF24L01+接收模式初始化
入口参数：无
返回  值：无
*********************************************************/
void nRF24L01P_RX_Mode(void)
{
    JPEG_CE_LOW;
    nRF24L01P_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 接收设备接收通道0使用和发送设备相同的发送地址
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + EN_AA, 0x01);               						// 使能接收通道0自动应答
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + EN_RXADDR, 0x01);           						// 使能接收通道0
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RF_CH, 40);                 						// 选择射频通道0x40
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// 接收通道0选择和发送通道相同有效数据宽度
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RF_SETUP, 0x0f);            						// 数据传输率2Mbps，发射功率0dBm，低噪声放大器增益(nRF24L01+忽略该位）
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + CONFIG, 0x0f);              						// CRC使能，16位CRC校验，上电，接收模式
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + STATUS, 0xff);  												//清除所有的中断标志位
    JPEG_CE_HIGH;                                           											// 拉高CE启动接收设备
}	


/********************************************************
函数功能：nRF24L01+发送模式初始化
入口参数：无
返回  值：无
*********************************************************/
void nRF24L01P_TX_Mode(void)
{
    JPEG_CE_LOW;
    nRF24L01P_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
    nRF24L01P_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同

    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + EN_AA, 0x01);       										// 使能接收通道0自动应答
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + EN_RXADDR, 0x01);   										// 使能接收通道0
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + SETUP_RETR, 0x0a);  										// 自动重发延时等待250us+86us，自动重发10次
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RF_CH, 40);         										// 选择射频通道0x40
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RF_SETUP, 0x0f);    										// 数据传输率2Mbps，发射功率0dBm，低噪声放大器增益(nRF24L01+忽略该位）
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + CONFIG, 0x0e);      										// CRC使能，16位CRC校验，上电
    //JPEG_CE_HIGH;
}


/********************************************************
函数功能：读取接收数据
入口参数：rxbuf:接收数据存放首地址
返回  值：0:接收到数据
          1:没有接收到数据
*********************************************************/
uint8_t nRF24L01P_RxPacket(uint8_t *rxbuf)
{
    uint8_t state;
    state = nRF24L01P_Read_Reg(STATUS);  			                 //读取状态寄存器的值
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + STATUS, state);            //清除RX_DS中断标志

    if(state & RX_DR)								                           //接收到数据
    {
        nRF24L01P_Read_Buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH);   //读取数据
        nRF24L01P_NRF_WRITE_REG(FLUSH_RX, 0xff);					             //清除RX FIFO寄存器
        return 0;
    }

    return 1;                                                   //没收到任何数据
}


/********************************************************
函数功能：发送一个数据包
入口参数：txbuf:要发送的数据
返回  值：0x10:达到最大重发次数，发送失败
          0x20:发送成功
          0xff:发送失败
*********************************************************/
uint8_t nRF24L01P_TxPacket(uint8_t *txbuf)
{
    uint8_t state;
    JPEG_CE_LOW;																									//CE拉低，使能24L01配置
    nRF24L01P_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	//写数据到TX FIFO,32个字节
    JPEG_CE_HIGH;																										//CE置高，使能发送

    while(IRQ == 1);																				//等待发送完成

    state = nRF24L01P_Read_Reg(STATUS);  											//读取状态寄存器的值
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + STATUS, state); 						//清除TX_DS或MAX_RT中断标志

    if(state & MAX_RT)																			 //达到最大重发次数
    {
        nRF24L01P_NRF_WRITE_REG(FLUSH_TX, 0xff);										//清除TX FIFO寄存器
        return MAX_RT;
    }

    if(state & TX_DS)																			  //发送完成
    {
        return TX_DS;
    }

    return 0XFF;																						//发送失败
}








/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
