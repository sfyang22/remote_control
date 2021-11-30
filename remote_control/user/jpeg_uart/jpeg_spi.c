/* Includes ------------------------------------------------------------------*/
#include "jpeg_spi.h"
#include <stdlib.h>

//MSP��ʼ�������뿴��TOUCH�ļ��Ѿ�����

SPI_HandleTypeDef hspi2;

const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x01, 0x07, 0x0E, 0x01}; // ����һ����̬���͵�ַ

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
�������ܣ�nRF24L01+���ų�ʼ��
��ڲ�������
����  ֵ����
*********************************************************/
void nRF24L01P_Init(void)
{
    MX_SPI2_Init();
    JPEG_CS_HIGH;
}


/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��NRF_WRITE_REG��reg��
					value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
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
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��NRF_WRITE_REG��reg��
					pBuf:д�����׵�ַ
					bytes:д�����ֽ���
����  ֵ��״̬�Ĵ�����ֵ
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
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��READ_REG��reg��
����  ֵ���Ĵ���ֵ
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
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ��READ_REG��reg��
					pBuf:���ջ��������׵�ַ
					bytes:��ȡ�ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t nRF24L01P_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status, byte_ctr;

    JPEG_CS_LOW;
    status = SPI_RW(reg);

    for(byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
        pBuf[byte_ctr] = SPI_RW(0);                   //��ȡ���ݣ����ֽ���ǰ

    JPEG_CS_HIGH;

    return(status);
}


/********************************************************
�������ܣ�nRF24L01+����ģʽ��ʼ��
��ڲ�������
����  ֵ����
*********************************************************/
void nRF24L01P_RX_Mode(void)
{
    JPEG_CE_LOW;
    nRF24L01P_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + EN_AA, 0x01);               						// ʹ�ܽ���ͨ��0�Զ�Ӧ��
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + EN_RXADDR, 0x01);           						// ʹ�ܽ���ͨ��0
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RF_CH, 40);                 						// ѡ����Ƶͨ��0x40
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  						// ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RF_SETUP, 0x0f);            						// ���ݴ�����2Mbps�����书��0dBm���������Ŵ�������(nRF24L01+���Ը�λ��
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + CONFIG, 0x0f);              						// CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + STATUS, 0xff);  												//������е��жϱ�־λ
    JPEG_CE_HIGH;                                           											// ����CE���������豸
}	


/********************************************************
�������ܣ�nRF24L01+����ģʽ��ʼ��
��ڲ�������
����  ֵ����
*********************************************************/
void nRF24L01P_TX_Mode(void)
{
    JPEG_CE_LOW;
    nRF24L01P_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
    nRF24L01P_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ

    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + EN_AA, 0x01);       										// ʹ�ܽ���ͨ��0�Զ�Ӧ��
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + EN_RXADDR, 0x01);   										// ʹ�ܽ���ͨ��0
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + SETUP_RETR, 0x0a);  										// �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RF_CH, 40);         										// ѡ����Ƶͨ��0x40
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + RF_SETUP, 0x0f);    										// ���ݴ�����2Mbps�����书��0dBm���������Ŵ�������(nRF24L01+���Ը�λ��
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + CONFIG, 0x0e);      										// CRCʹ�ܣ�16λCRCУ�飬�ϵ�
    //JPEG_CE_HIGH;
}


/********************************************************
�������ܣ���ȡ��������
��ڲ�����rxbuf:�������ݴ���׵�ַ
����  ֵ��0:���յ�����
          1:û�н��յ�����
*********************************************************/
uint8_t nRF24L01P_RxPacket(uint8_t *rxbuf)
{
    uint8_t state;
    state = nRF24L01P_Read_Reg(STATUS);  			                 //��ȡ״̬�Ĵ�����ֵ
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + STATUS, state);            //���RX_DS�жϱ�־

    if(state & RX_DR)								                           //���յ�����
    {
        nRF24L01P_Read_Buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH);   //��ȡ����
        nRF24L01P_NRF_WRITE_REG(FLUSH_RX, 0xff);					             //���RX FIFO�Ĵ���
        return 0;
    }

    return 1;                                                   //û�յ��κ�����
}


/********************************************************
�������ܣ�����һ�����ݰ�
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ��0x10:�ﵽ����ط�����������ʧ��
          0x20:���ͳɹ�
          0xff:����ʧ��
*********************************************************/
uint8_t nRF24L01P_TxPacket(uint8_t *txbuf)
{
    uint8_t state;
    JPEG_CE_LOW;																									//CE���ͣ�ʹ��24L01����
    nRF24L01P_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	//д���ݵ�TX FIFO,32���ֽ�
    JPEG_CE_HIGH;																										//CE�øߣ�ʹ�ܷ���

    while(IRQ == 1);																				//�ȴ��������

    state = nRF24L01P_Read_Reg(STATUS);  											//��ȡ״̬�Ĵ�����ֵ
    nRF24L01P_NRF_WRITE_REG(NRF_WRITE_REG + STATUS, state); 						//���TX_DS��MAX_RT�жϱ�־

    if(state & MAX_RT)																			 //�ﵽ����ط�����
    {
        nRF24L01P_NRF_WRITE_REG(FLUSH_TX, 0xff);										//���TX FIFO�Ĵ���
        return MAX_RT;
    }

    if(state & TX_DS)																			  //�������
    {
        return TX_DS;
    }

    return 0XFF;																						//����ʧ��
}








/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
