/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : includes.h
* Abstract    : APP配置头文件
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
*
*****************************************************************************
*/

#ifndef __APP_H
#define __APP_H



#ifdef GLOBALS_RFIR
#define APP_EXTERN
#else
#define APP_EXTERN extern
#endif

typedef struct sysstate
{
    INT32U rfSignal;
    INT32U rfm31bSignal;
    INT32U firSignal;

}SYS_STATE_t;

APP_EXTERN SYS_STATE_t gSysState;

#endif
