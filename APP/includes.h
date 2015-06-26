/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : includes.h
* Abstract    : 头文件合集，只要包含这一个头文件就可�?
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
*
*****************************************************************************
*/

#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <string.h>
#include <stdlib.h>
#include "stdio.h"
#include "stdlib.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_flash.h"
#include "stm32f10x.h"
#include "os_cpu.h"
#include "os_cfg.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "task.h"
#include "Msg.h"
#include "si4432.h"
#include "Encrypt.h"
#include "spi.h"
#include "spiflash.h"
#include "base.h"
#include "bsp.h"
#include "protocol.h"
#include "dataflash.h"
#include "stm32f10x_it.h"
#include "STM32Init.h"
#include "stm32f10x_tim.h"
#include "system_stm32f10x.h"
#include "IWatchDog.h"
#include "taskCT361InternalCommand.h"

/*��������*/
//extern void HardwareWSystemInit(void);
extern void MyFlagInit(void);
extern void DelayMs(u16 nTime);
extern void DelayMs(u16 nTime);
extern void MyTim3PWMInit(u16 Value1);
extern void MyTIM1CaptureInit(void);
#endif
