#include "lcd_drv.h"


//����Һ��ɨ�跽����仯��XY���ؿ��
//����ILI9341_GramScan�������÷���ʱ���Զ�����
uint16_t LCD_X_LENGTH = ILI9341_LESS_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9341_MORE_PIXEL;

//Һ����ɨ��ģʽ����������Ҫ���ڷ���ѡ�������ļ������
//������ѡֵΪ0-7
//����ILI9341_GramScan�������÷���ʱ���Զ�����
//LCD�ճ�ʼ�����ʱ��ʹ�ñ�Ĭ��ֵ
uint8_t LCD_SCAN_MODE = 3;

/* HandleTypeDef */
SRAM_HandleTypeDef hsram1;

void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    /* Disables the MPU */
    HAL_MPU_Disable();
    /** Initializes and configures the Region and the memory to be protected
    */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0x60000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256MB;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    /* Enables the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}


/* FMC initialization function */
void FMC_Init(void)
{
    FMC_NORSRAM_TimingTypeDef Timing = {0};

    /** Perform the SRAM1 memory initialization sequence
    */
    hsram1.Instance = FMC_NORSRAM_DEVICE;
    hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram1.Init */
    hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
    hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
    hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
    hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
    hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
    hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
    hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
    hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
    hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
    hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
    hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
    /* Timing */
    Timing.AddressSetupTime = 2;
    Timing.AddressHoldTime = 0;
    Timing.DataSetupTime = 4;
    Timing.BusTurnAroundDuration = 0;
    Timing.CLKDivision = 0;
    Timing.DataLatency = 0;
    Timing.AccessMode = FMC_ACCESS_MODE_B;
    /* ExtTiming */

    if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
    {
        HardError_Handler();	
    }
}

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (FMC_Initialized)
    {
        return;
    }

    FMC_Initialized = 1;

    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /** FMC GPIO Configuration
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD11   ------> FMC_A16
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PC7   ------> FMC_NE1
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    */
    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
                          | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
                          | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
                          | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1
                          | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_FMC;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USER CODE BEGIN FMC_MspInit 1 */

    /* USER CODE END FMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* sramHandle)
{
    HAL_FMC_MspInit();
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void)
{
    if (FMC_DeInitialized)
    {
        return;
    }

    FMC_DeInitialized = 1;
    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_DISABLE();

    /** FMC GPIO Configuration
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD11   ------> FMC_A16
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PC7   ------> FMC_NE1
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    */

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
                    | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
                    | GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
                    | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1
                    | GPIO_PIN_4 | GPIO_PIN_5);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);

}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* sramHandle)
{
    HAL_FMC_MspDeInit();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

static void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitSturtures;
    /* clock Init */
    LCD_RST_CLK();
    LCD_BLK_CLK();

    GPIO_InitSturtures.Pin = LCD_RST_PIN;
    GPIO_InitSturtures.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitSturtures.Pull = GPIO_PULLUP;
    GPIO_InitSturtures.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_RST_PORT, &GPIO_InitSturtures);

//    GPIO_InitSturtures.Pin = LCD_BLK_PIN;
//    HAL_GPIO_Init(LCD_BLK_PORT, &GPIO_InitSturtures);

}




/**
  * @brief  ��LCDд������
  * @param  usCmd :Ҫд��������Ĵ�����ַ��
  * @retval ��
  */
static __inline void LCD_Write_Cmd ( uint16_t usCmd )
{
    *(LCD_CMD) = usCmd;
}


/**
  * @brief  ��LCDд������
  * @param  usData :Ҫд�������
  * @retval ��
  */
static __inline void LCD_Write_Data ( uint16_t usData )
{
    * (LCD_DAT) = usData;

}


/**
  * @brief  ��LCD��ȡ����
  * @param  ��
  * @retval ��ȡ��������
  */
static __inline uint16_t LCD_Read_Data ( void )
{
    return ( *LCD_DAT );

}

/**
  * @brief  ��LCD��ȡһ�����ص�
  * @param  ��
  * @retval ��ȡ��������
  */
uint16_t Read_Pixel_Format(void)
{

    LCD_Write_Cmd(0x0C);
    LCD_Read_Data();

    return LCD_Read_Data();

}

/**
 * @brief  LCD����LED����
 * @param  enumState �������Ƿ�ʹ�ܱ���LED
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg ENABLE :ʹ�ܱ���LED
  *     @arg DISABLE :���ñ���LED
 * @retval ��
 */
void LCD_BackLed_Control ( FunctionalState enumState )
{
    if ( enumState )
        LCD_BLK_LOW;
    else
        LCD_BLK_HIGH;
}

/**
 * @brief  LCD ��ʱ
 * @param  ��
 * @retval ��
 */
static void LCD_Delay(uint32_t co)
{
    uint32_t i, j;

    for(i = 0; i < co; i++)
        for(j = 0; j < 500; j++);
}


/**
 * @brief  LCD �����λ
 * @param  ��
 * @retval ��
 */
void LCD_Rst ( void )
{
    LCD_RST_LOW;
    LCD_Delay(1000);
    LCD_RST_HIGH;
    LCD_Delay(1000);

}


/**
 * @brief  ��ʼ��LCD�Ĵ���
 * @param  ��
 * @retval ��
 */
static void LCD_REG_Config ( void )
{
    /*  Power control B (CFh)  */
    DEBUG_DELAY  ();
    LCD_Write_Cmd ( 0xCF  );
    LCD_Write_Data ( 0x00  );
    LCD_Write_Data ( 0x81  );
    LCD_Write_Data ( 0x30  );

    /*  Power on sequence control (EDh) */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0xED );
    LCD_Write_Data ( 0x64 );
    LCD_Write_Data ( 0x03 );
    LCD_Write_Data ( 0x12 );
    LCD_Write_Data ( 0x81 );

    /*  Driver timing control A (E8h) */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0xE8 );
    LCD_Write_Data ( 0x85 );
    LCD_Write_Data ( 0x10 );
    LCD_Write_Data ( 0x78 );

    /*  Power control A (CBh) */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0xCB );
    LCD_Write_Data ( 0x39 );
    LCD_Write_Data ( 0x2C );
    LCD_Write_Data ( 0x00 );
    LCD_Write_Data ( 0x34 );
    LCD_Write_Data ( 0x02 );

    /* Pump ratio control (F7h) */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0xF7 );
    LCD_Write_Data ( 0x20 );

    /* Driver timing control B */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0xEA );
    LCD_Write_Data ( 0x00 );
    LCD_Write_Data ( 0x00 );

    /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0xB1 );
    LCD_Write_Data ( 0x00 );
    LCD_Write_Data ( 0x1B );

    /*  Display Function Control (B6h) */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0xB6 );
    LCD_Write_Data ( 0x0A );
    LCD_Write_Data ( 0xA2 );

    /* Power Control 1 (C0h) */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0xC0 );
    LCD_Write_Data ( 0x35 );

    /* Power Control 2 (C1h) */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0xC1 );
    LCD_Write_Data ( 0x11 );

    /* VCOM Control 1 (C5h) */
    LCD_Write_Cmd ( 0xC5 );
    LCD_Write_Data ( 0x45 );
    LCD_Write_Data ( 0x45 );

    /*  VCOM Control 2 (C7h)  */
    LCD_Write_Cmd ( 0xC7 );
    LCD_Write_Data ( 0xA2 );

    /* Enable 3G (F2h) */
    LCD_Write_Cmd ( 0xF2 );
    LCD_Write_Data ( 0x00 );

    /* Gamma Set (26h) */
    LCD_Write_Cmd ( 0x26 );
    LCD_Write_Data ( 0x01 );
    DEBUG_DELAY ();

    /* Positive Gamma Correction */
    LCD_Write_Cmd ( 0xE0 ); //Set Gamma
    LCD_Write_Data ( 0x0F );
    LCD_Write_Data ( 0x26 );
    LCD_Write_Data ( 0x24 );
    LCD_Write_Data ( 0x0B );
    LCD_Write_Data ( 0x0E );
    LCD_Write_Data ( 0x09 );
    LCD_Write_Data ( 0x54 );
    LCD_Write_Data ( 0xA8 );
    LCD_Write_Data ( 0x46 );
    LCD_Write_Data ( 0x0C );
    LCD_Write_Data ( 0x17 );
    LCD_Write_Data ( 0x09 );
    LCD_Write_Data ( 0x0F );
    LCD_Write_Data ( 0x07 );
    LCD_Write_Data ( 0x00 );

    /* Negative Gamma Correction (E1h) */
/*
    LCD_Write_Cmd ( 0XE1 ); 		//������Ļ���� GUI�л��Զ�����
    LCD_Write_Data ( 0x00 );s
    LCD_Write_Data ( 0x19 );
    LCD_Write_Data ( 0x1B );
    LCD_Write_Data ( 0x04 );
    LCD_Write_Data ( 0x10 );
    LCD_Write_Data ( 0x07 );
    LCD_Write_Data ( 0x2A );
    LCD_Write_Data ( 0x47 );
    LCD_Write_Data ( 0x39 );
    LCD_Write_Data ( 0x03 );
    LCD_Write_Data ( 0x06 );
    LCD_Write_Data ( 0x06 );
    LCD_Write_Data ( 0x30 );
    LCD_Write_Data ( 0x38 );
    LCD_Write_Data ( 0x0F );
*/
	/* Set the LCD scan mode */
	LCD_GramScan(LCD_SCAN_MODE);

    /*  Pixel Format Set (3Ah)  */
    DEBUG_DELAY ();
    LCD_Write_Cmd ( 0x3a );
    LCD_Write_Data ( 0x55 );

    /* Sleep Out (11h)  */
    LCD_Write_Cmd ( 0x11 );
    DEBUG_DELAY ();
    DEBUG_DELAY ();
    LCD_Delay(80000);					//�������ʱ�Ǳ����

    /* Display ON (29h) */
    LCD_Write_Cmd ( 0x29 );


}

/**
 * @brief  ��ʼ��LCD
 * @param  ��
 * @retval ��
 */

void LCD_Init(void)
{
    /* config */
    //MPU_Config();
	LCD_Delay(50000);
    FMC_Init();
    LCD_GPIO_Config();
    /* GPIO Init */
    LCD_BackLed_Control(ENABLE);
    LCD_Rst();
    /* lcd init reg */
    LCD_REG_Config();
}

/**
 * @brief  ����һ��ָ����С�Ĵ���
 * @param  ��
 * @retval ��
 */
void LCD_Open_Window(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight)
{
    /* column address control set */
    LCD_Write_Cmd ( CMD_SetCoordinateX );
    LCD_Write_Data ( (usX & 0xFF00) >> 8 );
    LCD_Write_Data ( (usX & 0x00FF) );
    LCD_Write_Data ( ((usX + usWidth) & 0xFF00) >> 8 );
    LCD_Write_Data ( ((usX + usWidth) & 0x00FF) );

    LCD_Write_Cmd ( CMD_SetCoordinateY );
    LCD_Write_Data ( (usY & 0xFF00) >> 8 );
    LCD_Write_Data ( (usY & 0x00FF) );
    LCD_Write_Data ( ((usY + usHeight) & 0xFF00) >> 8 );
    LCD_Write_Data ( ((usY + usHeight) & 0x00FF) );
}

//LCD�����ص㺯��
//���룺usX X���� usY Y���� color ��ɫ��16λRGB��
void LCD_DrawPoint(uint16_t usX, uint16_t usY, uint16_t color)
{
    LCD_Open_Window(usX, usY, usX, usY);
    LCD_Write_Cmd(CMD_SetPixel);

	LCD_Write_Data(color);
}

/**
 * @brief  LCD����
 * @param  ��
 * @retval ��
 */
void LCD_Clear(void)
{
    uint32_t i;
    LCD_Open_Window(0, 0, LCD_X_LENGTH, LCD_Y_LENGTH);

    LCD_Write_Cmd(CMD_SetPixel);

    for(i = 0; i < LCD_X_LENGTH * LCD_Y_LENGTH; i++)
        LCD_Write_Data(BACKGROUND);
}

/**
 * @brief  ����ILI9341��GRAM��ɨ�跽�� 
 * @param  ucOption ��ѡ��GRAM��ɨ�跽�� 
 *     @arg 0-7 :������ѡֵΪ0-7��˸�����
 *
 *	����������0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 *				���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
 *		
 *	����0��2��4��6 ģʽ��X��������Ϊ240��Y��������Ϊ320
 *	����1��3��5��7 ģʽ��X��������Ϊ320��Y��������Ϊ240
 *
 *	���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����
 *	���� 3 ģʽΪ����ͷ����ʹ�õķ���
 *	���� 0 ģʽΪBMPͼƬ��ʾ����ʹ�õķ���
 *
 * @retval ��
 * @note  ����ͼ����A��ʾ���ϣ�V��ʾ���£�<��ʾ����>��ʾ����
					X��ʾX�ᣬY��ʾY��

------------------------------------------------------------
ģʽ0��				.		ģʽ1��		.	ģʽ2��			.	ģʽ3��					
					A		.					A		.		A					.		A									
					|		.					|		.		|					.		|							
					Y		.					X		.		Y					.		X					
					0		.					1		.		2					.		3					
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->	
------------------------------------------------------------	
ģʽ4��				.	ģʽ5��			.	ģʽ6��			.	ģʽ7��					
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->	 
					4		.					5		.		6					.		7	
					Y		.					X		.		Y					.		X						
					|		.					|		.		|					.		|							
					V		.					V		.		V					.		V		
---------------------------------------------------------				
											 LCD��ʾ��
								|-----------------|
								|			����Logo		|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|-----------------|
								��Ļ���棨��240����320��

 *******************************************************/
void LCD_GramScan ( uint8_t ucOption )
{	
	//������飬ֻ������0-7
	if(ucOption >7 )
		return;
	
	//����ģʽ����LCD_SCAN_MODE��ֵ����Ҫ���ڴ�����ѡ��������
	LCD_SCAN_MODE = ucOption;
	
	//����ģʽ����XY��������ؿ��
	if(ucOption%2 == 0)	
	{
		//0 2 4 6ģʽ��X�������ؿ��Ϊ240��Y����Ϊ320
		LCD_X_LENGTH = ILI9341_LESS_PIXEL;
		LCD_Y_LENGTH =	ILI9341_MORE_PIXEL;
	}
	else				
	{
		//1 3 5 7ģʽ��X�������ؿ��Ϊ320��Y����Ϊ240
		LCD_X_LENGTH = ILI9341_MORE_PIXEL;
		LCD_Y_LENGTH =	ILI9341_LESS_PIXEL; 
	}

	//0x36��������ĸ�3λ����������GRAMɨ�跽��	
	LCD_Write_Cmd ( 0x36 ); 
	LCD_Write_Data ( 0x08 |(ucOption<<5));//����ucOption��ֵ����LCD��������0-7��ģʽ
	LCD_Write_Cmd ( CMD_SetCoordinateX ); 
	LCD_Write_Data ( 0x00 );		/* x ��ʼ�����8λ */
	LCD_Write_Data ( 0x00 );		/* x ��ʼ�����8λ */
	LCD_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x ���������8λ */	
	LCD_Write_Data ( (LCD_X_LENGTH-1)&0xFF );				/* x ���������8λ */

	LCD_Write_Cmd ( CMD_SetCoordinateY ); 
	LCD_Write_Data ( 0x00 );		/* y ��ʼ�����8λ */
	LCD_Write_Data ( 0x00 );		/* y ��ʼ�����8λ */
	LCD_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );	/* y ���������8λ */	 
	LCD_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );				/* y ���������8λ */

	/* write gram start */
	LCD_Write_Cmd ( CMD_SetPixel );	
}

void delay2(uint32_t i)
{
	while(i--);
}


