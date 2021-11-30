#include "touch.h"
#include "lcd.h"
#include "rtthread.h"
#include "rthw.h"


SPI_HandleTypeDef hspi1;

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 0x0;
    hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_02CYCLE;
    hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_08CYCLE;
    hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        HardError_Handler();
    }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(spiHandle->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI1_MspInit 0 */
        CS_GPIO_CLK();
        PEN_GPIO_CLK();
		JPEG_CS_GPIO_CLK();
        /* USER CODE END SPI1_MspInit 0 */
        /* SPI1 clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();
		
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**SPI1 GPIO Configuration
        PA4     ------> SPI1_NSS
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI1_MspInit 1 */
        GPIO_InitStruct.Pin = CS_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(CS_GPIO_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = PEN_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(PEN_GPIO_PORT, &GPIO_InitStruct);
		
        /* USER CODE END SPI1_MspInit 1 */
    }
    else if(spiHandle->Instance == SPI2)
    {
        /* USER CODE BEGIN SPI2_MspInit 0 */

        /* USER CODE END SPI2_MspInit 0 */
        /* SPI2 clock enable */
        __HAL_RCC_SPI2_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**SPI2 GPIO Configuration
        PC1     ------> SPI2_MOSI
        PC2_C     ------> SPI2_MISO
        PB10     ------> SPI2_SCK
		PC0     ------> SPI2_CS
        */
        GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = JPEG_CS_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(JPEG_CS_GPIO_PORT, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = JPEG_CE_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(JPEG_CE_GPIO_PORT, &GPIO_InitStruct);

        /* SPI2 DMA Init */
        /* SPI2_RX Init */

        /* SPI2 interrupt Init */
        //HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
        //HAL_NVIC_EnableIRQ(SPI2_IRQn);
        /* USER CODE BEGIN SPI2_MspInit 1 */

        /* USER CODE END SPI2_MspInit 1 */
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
    if(spiHandle->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI1_MspDeInit 0 */

        /* USER CODE END SPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration
        PA4     ------> SPI1_NSS
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI
        */
        HAL_GPIO_DeInit(GPIOA, CS_GPIO_PIN | PEN_GPIO_PIN | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

        /* USER CODE BEGIN SPI1_MspDeInit 1 */

        /* USER CODE END SPI1_MspDeInit 1 */
    }
	else if(spiHandle->Instance == SPI2)
    {
        /* USER CODE BEGIN SPI2_MspDeInit 0 */

        /* USER CODE END SPI2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**SPI2 GPIO Configuration
        PC1     ------> SPI2_MOSI
        PC2_C     ------> SPI2_MISO
        PB10     ------> SPI2_SCK
		PC0     ------> SPI2_CS
		PC3     ------> NRF_CE
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_0 | GPIO_PIN_3);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

        /* SPI2 DMA DeInit */
        HAL_DMA_DeInit(spiHandle->hdmarx);
        HAL_DMA_DeInit(spiHandle->hdmatx);

        /* SPI2 interrupt Deinit */
        HAL_NVIC_DisableIRQ(SPI2_IRQn);
        /* USER CODE BEGIN SPI2_MspDeInit 1 */

        /* USER CODE END SPI2_MspDeInit 1 */
    }
}

//功能： 初始化触摸功能
//输入： 无
//返回： 无
void LCD_TOUCH_Init(void)
{
    /* SPI Init */
    MX_SPI1_Init();
    CS_HIGH;
}

//功能： 发送8bit到XPT2046
//输入： data 8位命令
//返回： 无
static void XPT2046_SendData(uint8_t data)
{
    HAL_SPI_Transmit(&hspi1, &data, 1, 10);
}

//功能： 从XPT2046接收8bit
//输入： 无
//返回： 8位数据
static uint8_t XPT2046_RecData(void)
{
    uint8_t data;
    HAL_SPI_Receive(&hspi1, &data, 1, 10);
    return data;
}

//功能： 从XPT2046接收12bit
//输入： 无
//返回： 12位数据
static uint16_t XPT2046_RecValue(void)
{
	uint16_t dat;
    uint8_t dat1, dat2;
    HAL_SPI_Receive(&hspi1, &dat1, 1, 10);
	HAL_SPI_Receive(&hspi1, &dat2, 1, 10);

	dat = (((dat1 << 8) | dat2) >> 3) & 0xfff;
	
    return dat;
}


//功能： 获取触点坐标
//输入： TOUCH结构体
//返回： 0 有效 1 无效
#if 0
uint8_t TOUCH_GetOffset(TOUCH *touch)
{
    uint16_t temp = 0;

    /* ENABLE XPT2046 */
    CS_LOW;
    /* to get the offset of X */
    XPT2046_SendData(X_OFFSET);
    rt_thread_delay(1);

    temp = XPT2046_RecData();
    temp <<= 8;
    temp |= XPT2046_RecData();
    temp >>= 3;
    touch->x = ((temp & 0xfff) * LCD_X_LENGTH - 230) / 4096 * x_rate;
    //touch->x = temp & 0xfff ;

    /* to get the offset of Y */
    XPT2046_SendData(Y_OFFSET);
    rt_thread_delay(1);

    temp = XPT2046_RecData();
    temp <<= 8;
    temp |= XPT2046_RecData();
    temp >>= 3;
    touch->y = ((temp & 0xfff) * LCD_Y_LENGTH - 276) / 4096 * y_rate;
    //touch->y = temp & 0xfff;

    /* DISABLE XPT2046 */
    CS_HIGH;

    /* 若超出边界， 返回错误 */
    /*
    if((touch->x > LCD_X_LENGTH) || (touch->y > LCD_Y_LENGTH))
    	return 1;
    else
    	return 0;
    */
    return 0;

}
#endif

//功能： 获取触点坐标
//输入： X_OFFSET 或 Y_OFFSET
//返回：
uint16_t XPT2046_ReadAdc_Fliter(uint8_t channel)
{
    uint8_t i, j, min;
    uint16_t temp;
    uint16_t tempXY[SAMP_CNT];

    static uint16_t adc_x = 0, adc_y = 0;

    if(PEN_READ() == 0)
    {
		CS_LOW;
        for(i = 0; i < SAMP_CNT; i++)
        {
            XPT2046_SendData(channel);
			rt_thread_delay(1);
            tempXY[i] = XPT2046_RecValue();
        }
		CS_HIGH;
        // 降序排列
        for (i = 0; i < SAMP_CNT - 1; i++)
        {
            min = i;

            for (j = i + 1; j < SAMP_CNT; j++)
            {
                if (tempXY[min] > tempXY[j])
                    min = j;
            }

            temp = tempXY[i];
            tempXY[i] = tempXY[min];
            tempXY[min] = temp;
        }
		
		
// 设定阈值
        if ((tempXY[SAMP_CNT_DIV2] - tempXY[SAMP_CNT_DIV2 - 1]) > 5)
        {
            /* 若两个中间值相差太远，则舍弃这个新数据，返回上一次的触摸数据 */
            if (channel == Y_OFFSET)
                return adc_x; //x 通道
            else
                return adc_y; //y 通道
        }

// 求中间值的均值
        if (channel == Y_OFFSET)
        {
            adc_x = (tempXY[SAMP_CNT_DIV2] + tempXY[SAMP_CNT_DIV2 - 1]) / 2;
            return adc_x;
        }
        else
        {
            adc_y = (tempXY[SAMP_CNT_DIV2] + tempXY[SAMP_CNT_DIV2 - 1]) / 2;
            return adc_y;
        }
    }
    else
    {
        return 0; //没有触摸，返回 0
    }


}


