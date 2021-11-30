/*
*********************************************************************************************************
*
*	ģ������ : ͼƬ�ļ�
*	�ļ����� : decode_dma.c
*	��    �� : V1.0
*	˵    �� : JPEGͼƬ����
*	�޸ļ�¼ :
*		�汾��   ����         ����       ˵��
*		V1.0    2019-05-11  Eric2013    ��ʽ����
*
*	Copyright (C), 2019-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "jpeg_decode.h"
#include "GUI.h"


/*
*********************************************************************************************************
*	                                       �궨��
*********************************************************************************************************
*/
#define CHUNK_SIZE_IN  ((uint32_t)(64 * 1024))   /* �������ݴ�С����λ�ֽ� */
#define CHUNK_SIZE_OUT ((uint32_t)(64 * 1024))   /* ������ݴ�С����λ�ֽ� */


/*
*********************************************************************************************************
*	                                       ����
*********************************************************************************************************
*/
__IO uint32_t Jpeg_HWDecodingEnd = 0; /* ������ɱ�־��0��ʾ��ʼ���룬1��ʾ������� */
JPEG_HandleTypeDef    JPEG_Handle;
JPEG_ConfTypeDef      JPEG_Info;

uint32_t FrameBufferAddress;          /* JPEG��������ݵ�ַ */
uint32_t JPEGSourceAddress;           /* JPEG����Դ��ַ */
uint32_t Input_frameSize;             /* JPEGԴ���ݴ�С */
uint32_t Input_frameIndex;            /* JPEG��������У��Ѿ���������ݴ�С */


/*
*********************************************************************************************************
*	�� �� ��: JPEG_DECODE_Init
*	����˵��: ��ʼ��JEPG������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void JPEG_DECODE_Init(void)
{
	/* ��1����JPEG��ʼ�� ###########################################*/
	JPEG_Handle.Instance = JPEG;
	HAL_JPEG_Init(&JPEG_Handle); 
}

/*
*********************************************************************************************************
*    �� �� ��: JPEG_X_Draw
*    ����˵��: Ӳ��JPEG����
*    ��    ��: ---
*    �� �� ֵ: �����Ƿ�ɹ�
*********************************************************************************************************
*/
int JPEG_X_Draw(GUI_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0) 
{
    U8 *ppData;
    GUI_LOCK();

	
    _Context.xPos      = x0;
    _Context.yPos      = y0;
    _Context.pfGetData = pfGetData;
    _Context.pVoid     = p;
    _Context.Error     = 0;

    /* ��ʼ��Ӳ��JPEG��������ռ�  */
    if (_IsInitialized == 0) 
    {
        _IsInitialized = 1;
        JPEG_Handle.Instance = JPEG;
        HAL_JPEG_Init(&JPEG_Handle);  
        
#if AutoMalloc == 0
        /* ����һ���ڴ�ռ䣬���ڼ���JPEGͼƬ */
        _Context.hWorkBuffer = GUI_ALLOC_AllocNoInit(LoadPicSize);
        _Context.pWorkBuffer = GUI_ALLOC_h2p(_Context.hWorkBuffer);

        /* ����һ���ڴ�ռ䣬���ڴ�Ž�����ɵ����� */
        _Context.hOutBuffer = GUI_ALLOC_AllocNoInit(DrawPicSize);
        _Context.pOutBuffer = GUI_ALLOC_h2p(_Context.hOutBuffer);
#endif
    }

#if AutoMalloc == 1
    /* ����һ���ڴ�ռ䣬���ڼ���JPEGͼƬ */
    _Context.hWorkBuffer = GUI_ALLOC_AllocNoInit(LoadPicSize);
    _Context.pWorkBuffer = GUI_ALLOC_h2p(_Context.hWorkBuffer);

    /* ����һ���ڴ�ռ䣬���ڴ�Ž�����ɵ����� */
    _Context.hOutBuffer = GUI_ALLOC_AllocNoInit(DrawPicSize);
    _Context.pOutBuffer = GUI_ALLOC_h2p(_Context.hOutBuffer);    
#endif
    
    /* ��ȡJPEG���ݣ������� */
    _Context.NumBytesInBuffer  = _Context.pfGetData(_Context.pVoid, (const U8 **)&ppData, LoadPicSize, 0);

    JPEG_Decode_DMA(&JPEG_Handle, (uint32_t)ppData,  _Context.NumBytesInBuffer, (uint32_t)_Context.pWorkBuffer);
    
    /* ������� */
    while(Jpeg_HWDecodingEnd == 0)
    {
    }
    
    /* ��ȡJPEGͼƬ��ʽ��Ϣ������ɫ��ʽת�� */
    HAL_JPEG_GetInfo(&JPEG_Handle, &JPEG_Info);    
    DMA2D_Copy_YCbCr_To_RGB((uint32_t *)_Context.pWorkBuffer, 
                            (uint32_t *)_Context.pOutBuffer , 
                            0, 
                            0, 
                            JPEG_Info.ImageWidth, 
                            JPEG_Info.ImageHeight, 
                            PicPixelFormat,
                            JPEG_Info.ChromaSubsampling);

    /* ����JPEGͼƬ */
    _DrawBitmap(_Context.xPos, _Context.yPos, (void const *)_Context.pOutBuffer , JPEG_Info.ImageWidth, JPEG_Info.ImageHeight, JPEG_Info.ImageWidth*2, 16);

#if AutoMalloc == 1
    /* �ͷŶ�̬�ڴ�hMem */
    GUI_ALLOC_Free(_Context.hWorkBuffer);
    GUI_ALLOC_Free(_Context.hOutBuffer );
#endif
    
    GUI_UNLOCK();
    return _Context.Error;
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_MspInit
*	����˵��: ��ʼ��JEPG����Ҫ�ĵײ�
*	��    ��: JPEG_HandleTypeDef���ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
	/* �����������������Ϊ��̬�ֲ���������ȫ�ֱ�������Ϊ�˳���JPEG�����Ҫʹ�� */
	static MDMA_HandleTypeDef   hmdmaIn;
	static MDMA_HandleTypeDef   hmdmaOut;  

	/* ʹ��JPEGʱ�� */
	__HAL_RCC_JPGDECEN_CLK_ENABLE();

	/* ʹ��MDMAʱ�� */
	__HAL_RCC_MDMA_CLK_ENABLE();

	/* ʹ��JPEG�жϲ��������ȼ� */
	HAL_NVIC_SetPriority(JPEG_IRQn, 0x07, 0x00);
	HAL_NVIC_EnableIRQ(JPEG_IRQn);  
	
	/* JPEG�����MDMA���� ###########################################*/
	hmdmaIn.Instance                = MDMA_Channel7;                   /* ʹ��MDMAͨ��7 */
	hmdmaIn.Init.Priority           = MDMA_PRIORITY_HIGH;              /* ���ȼ��� */
	hmdmaIn.Init.Endianness         = MDMA_LITTLE_ENDIANNESS_PRESERVE; /* С�˸�ʽ */
	hmdmaIn.Init.SourceInc          = MDMA_SRC_INC_BYTE;               /* Դ��ַ�ֽڵ��� */
	hmdmaIn.Init.DestinationInc     = MDMA_DEST_INC_DISABLE;           /* Ŀ�ĵ�ַ������ */
	hmdmaIn.Init.SourceDataSize     = MDMA_SRC_DATASIZE_BYTE;          /* Դ��ַ���ݿ���ֽ� */
	hmdmaIn.Init.DestDataSize       = MDMA_DEST_DATASIZE_WORD;         /* Ŀ�ĵ�ַ���ݿ���ֽ� */
	hmdmaIn.Init.DataAlignment      = MDMA_DATAALIGN_PACKENABLE;       /* С�ˣ��Ҷ��� */
	hmdmaIn.Init.SourceBurst        = MDMA_SOURCE_BURST_32BEATS;       /* Դ����ͻ�����䣬32�� */
	hmdmaIn.Init.DestBurst          = MDMA_DEST_BURST_16BEATS;         /* Ŀ������ͻ�����䣬16�� */
	
	hmdmaIn.Init.SourceBlockAddressOffset = 0;  /* ����block���䣬buffer�����ò��� */
	hmdmaIn.Init.DestBlockAddressOffset   = 0;  /* ����block���䣬buffer�����ò��� */
  
	hmdmaIn.Init.Request = MDMA_REQUEST_JPEG_INFIFO_TH;  	 /* JPEG��FIFO��ֵ�����ж� */
	hmdmaIn.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER; /* ʹ��MDMA��buffer���� */
	hmdmaIn.Init.BufferTransferLength = 32;                  /* ÿ�δ���32���ֽڣ�JPEG��FIFO��ֵ��32�ֽ� */

	/* ����MDMA�ľ����JPEG */
	__HAL_LINKDMA(hjpeg, hdmain, hmdmaIn);

	/* �ȸ�λ��Ȼ������MDMA */
	HAL_MDMA_DeInit(&hmdmaIn); 
	HAL_MDMA_Init(&hmdmaIn);

	/* JPEG�����MDMA���� ###########################################*/
	hmdmaOut.Instance             = MDMA_Channel6;                   /* ʹ��MDMAͨ��6 */
	hmdmaOut.Init.Priority        = MDMA_PRIORITY_VERY_HIGH;         /* ���ȼ���� */
	hmdmaOut.Init.Endianness      = MDMA_LITTLE_ENDIANNESS_PRESERVE; /* С�˸�ʽ */
	hmdmaOut.Init.SourceInc       = MDMA_SRC_INC_DISABLE;            /* Դ���ݵ�ַ��ֹ���� */
	hmdmaOut.Init.DestinationInc  = MDMA_DEST_INC_BYTE;              /* Ŀ�����ݵ�ַ�ֽ����� */
	hmdmaOut.Init.SourceDataSize  = MDMA_SRC_DATASIZE_WORD;          /* Դ��ַ���ݿ���� */
	hmdmaOut.Init.DestDataSize    = MDMA_DEST_DATASIZE_BYTE;         /* Ŀ�ĵ�ַ���ݿ���ֽ� */
	hmdmaOut.Init.DataAlignment   = MDMA_DATAALIGN_PACKENABLE;       /* С�ˣ��Ҷ��� */  
	hmdmaOut.Init.SourceBurst     = MDMA_SOURCE_BURST_32BEATS;       /* Դ����ͻ�����䣬32�� */
	hmdmaOut.Init.DestBurst       = MDMA_DEST_BURST_32BEATS;         /* Ŀ������ͻ�����䣬16�� */
	
	hmdmaOut.Init.SourceBlockAddressOffset = 0;  /* ����block���䣬buffer�����ò��� */
	hmdmaOut.Init.DestBlockAddressOffset   = 0;  /* ����block���䣬buffer�����ò��� */

	hmdmaOut.Init.Request              = MDMA_REQUEST_JPEG_OUTFIFO_TH;  /* JPEG��FIFO��ֵ�����ж� */
	hmdmaOut.Init.TransferTriggerMode  = MDMA_BUFFER_TRANSFER;          /* ʹ��MDMA��buffer���� */ 
	hmdmaOut.Init.BufferTransferLength = 32;                            /* ÿ�δ���32���ֽڣ�JPEG��FIFO��ֵ��32�ֽ� */

	/* �ȸ�λ��Ȼ������MDMA */
	HAL_MDMA_DeInit(&hmdmaOut);  
	HAL_MDMA_Init(&hmdmaOut);

	/* ����MDMA�ľ����JPEG */
	__HAL_LINKDMA(hjpeg, hdmaout, hmdmaOut);

	/* ʹ��MDMA�жϲ��������ȼ� */
	HAL_NVIC_SetPriority(MDMA_IRQn, 0x08, 0x00);
	HAL_NVIC_EnableIRQ(MDMA_IRQn);
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_MspDeInit
*	����˵��: ��λJPEG
*	��    ��: JPEG_HandleTypeDef���ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_MspDeInit(JPEG_HandleTypeDef *hjpeg)
{
	/* �ر�MDMA�ж� */
	HAL_NVIC_DisableIRQ(MDMA_IRQn);

	/* ��λMDMA��JPEG������� */
	HAL_MDMA_DeInit(hjpeg->hdmain);
	HAL_MDMA_DeInit(hjpeg->hdmaout);
}

/*
*********************************************************************************************************
*	�� �� ��: JPEG_Decode_DMA
*	����˵��: JPEG����
*	��    ��: hjpeg               JPEG_HandleTypeDef���ָ��
*             FrameSourceAddress  ���ݵ�ַ
*             FrameSize           ���ݴ�С
*             DestAddress         Ŀ�����ݵ�ַ
*	�� �� ֵ: HAL_ERROR��ʾ����ʧ�ܣ�HAL_OK��ʾ���óɹ�
*             HAL_BUSY��ʾæ�������У���HAL_TIMEOUT��ʾʱ�����
*********************************************************************************************************
*/
uint32_t JPEG_Decode_DMA(JPEG_HandleTypeDef *hjpeg, uint32_t FrameSourceAddress ,uint32_t FrameSize, uint32_t DestAddress)
{
	JPEGSourceAddress =  FrameSourceAddress ;
	FrameBufferAddress = DestAddress;
	Input_frameIndex = 0;
	Input_frameSize = FrameSize;

	/* ���ñ�־��0��ʾ��ʼ���룬1��ʾ������� */
	Jpeg_HWDecodingEnd = 0;

	/* ����JPEG���� */
	HAL_JPEG_Decode_DMA(hjpeg ,(uint8_t *)JPEGSourceAddress ,CHUNK_SIZE_IN ,(uint8_t *)FrameBufferAddress ,CHUNK_SIZE_OUT);

	return HAL_OK;
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_InfoReadyCallback
*	����˵��: JPEG�ж������һ���ص���������������δʹ��
*	��    ��: hjpeg    JPEG_HandleTypeDef ���ָ��
*             pInfo    JPEG_ConfTypeDef   JPEG������Ϣ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_InfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *pInfo)
{  
	
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_GetDataCallback
*	����˵��: JPEG�ص����������ڴ������ַ��ȡ�����ݼ�������
*	��    ��: hjpeg          JPEG_HandleTypeDef ���ָ��
*             NbDecodedData  ��һ���Ѿ���������ݴ�С����λ�ֽ�  
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_GetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t NbDecodedData)
{
	uint32_t inDataLength; 
	
	/* �����Ѿ���������ݴ�С */
	Input_frameIndex += NbDecodedData;
	
	/* �����ǰ�Ѿ����������С�����ļ���С���������� */
	if( Input_frameIndex < Input_frameSize)
	{
		/* ���½�������λ�� */
		JPEGSourceAddress = JPEGSourceAddress + NbDecodedData;

		/* ������һ��Ҫ��������ݴ�С */
		if((Input_frameSize - Input_frameIndex) >= CHUNK_SIZE_IN)
		{
			inDataLength = CHUNK_SIZE_IN;
		}
		else
		{
			inDataLength = Input_frameSize - Input_frameIndex;
		}    
	}
	else
	{
		inDataLength = 0; 
	}
	
	/* �������뻺�� */
	HAL_JPEG_ConfigInputBuffer(hjpeg,(uint8_t *)JPEGSourceAddress, inDataLength);    
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_DataReadyCallback
*	����˵��: JPEG�ص�������������������ַ����
*	��    ��: hjpeg         JPEG_HandleTypeDef ���ָ��
*             pDataOut      ������ݻ���
*             OutDataLength ������ݴ�С����λ�ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_DataReadyCallback (JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t OutDataLength)
{
	/* ����JPEG�����ַ */  
	FrameBufferAddress += OutDataLength;

	HAL_JPEG_ConfigOutputBuffer(hjpeg, (uint8_t *)FrameBufferAddress, CHUNK_SIZE_OUT); 
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_ErrorCallback
*	����˵��: JPEG����ص�
*	��    ��: hjpeg    JPEG_HandleTypeDef ���ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_ErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	HardError_Handler();
}

/*
*********************************************************************************************************
*	�� �� ��: HAL_JPEG_DecodeCpltCallback
*	����˵��: JPEG��������ж�
*	��    ��: hjpeg    JPEG_HandleTypeDef ���ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_JPEG_DecodeCpltCallback(JPEG_HandleTypeDef *hjpeg)
{    
	Jpeg_HWDecodingEnd = 1; 
}

/*
*********************************************************************************************************
*	�� �� ��: JPEG_IRQHandler
*	����˵��: JPEG�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void JPEG_IRQHandler(void)
{
	HAL_JPEG_IRQHandler(&JPEG_Handle);
}

/*
*********************************************************************************************************
*	�� �� ��: MDMA_IRQHandler
*	����˵��: MDMA�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MDMA_IRQHandler(void)
{
	HAL_MDMA_IRQHandler(JPEG_Handle.hdmain);
	HAL_MDMA_IRQHandler(JPEG_Handle.hdmaout);  
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
