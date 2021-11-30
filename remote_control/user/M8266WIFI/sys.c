/********************************************************************
 * sys.c
 * .Description
 *     Source file for system clock configuration
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/

#include "stm32h7xx.h"
#include "sys.h"


void CPU_CACHE_Enable(void)
{
    SCB_EnableICache();
    SCB_EnableDCache();
  	SCB->CACR|=1<<2;
}

