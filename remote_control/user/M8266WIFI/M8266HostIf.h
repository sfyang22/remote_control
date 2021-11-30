/********************************************************************
 * M8266HostIf.h
 * .Description
 *     header file of M8266WIFI Host Interface 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
#ifndef _M8266_HOST_IF_H_
#define _M8266_HOST_IF_H_

#include "sys.h"	
 
void M8266HostIf_Init(void);
void M8266HostIf_SPI_SetSpeed(u32 SPI_BaudRatePrescaler);

#endif

