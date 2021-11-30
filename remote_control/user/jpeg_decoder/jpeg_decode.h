/*
*********************************************************************************************************
*
*	模块名称 : 图片文件
*	文件名称 : decode_dma.h
*	版    本 : V1.0
*	说    明 : JPEG图片解码
*	修改记录 :
*		版本号   日期         作者       说明
*		V1.0    2019-05-11  Eric2013    正式发布
*
*	Copyright (C), 2019-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#ifndef __DECODE_DMA_H
#define __DECODE_DMA_H

#include "main.h"

/*
*********************************************************************************************************
*                                           宏定义
*********************************************************************************************************
*/
#define AutoMalloc     0                           /* 0 申请后不释放, 1 使用完毕后释放 */
#define LoadPicSize    1024*600*4                  /* 最大支持的加载的图片大小 */
#define DrawPicSize    1024*600*4                  /* 图片解码出来后，可以使用的缓冲大小 */
#define PicPixelFormat LTDC_PIXEL_FORMAT_RGB565    /* 当前显示屏使用的颜色格式 */

extern __IO uint32_t Jpeg_HWDecodingEnd;
extern JPEG_HandleTypeDef    JPEG_Handle;
extern  JPEG_ConfTypeDef       JPEG_Info;	

uint32_t JPEG_Decode_DMA(JPEG_HandleTypeDef *hjpeg, uint32_t FrameSourceAddress ,uint32_t FrameSize, uint32_t DestAddress);

#endif 

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
