/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : msg.h
* Abstract    : 消息列表
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
*
*****************************************************************************
*/

#ifndef __MSG_H
#define __MSG_H

/**
 * 消息定义，以下所列的消息，顺序不可以随便调整
 */
typedef enum tagMSG
{
    // for protocol
    UART1_BUFF0_RECV = 0,
    UART1_BUFF1_RECV = 1,
    UART2_BUFF0_RECV = 2,
    UART2_BUFF1_RECV = 3,

    // for led refresh
    EXPRESSION_ALL_ON       = 4,
    EXPRESSION_ALL_OFF      = 5,
    EXPRESSION_SMILE        = 6,
    EXPRESSION_CRY          = 7,
    EXPRESSION_SQUINT       = 8,
    EXPRESSION_SUPRISED     = 9,
    EXPRESSION_SMILE_CLOSEMOUTH     = 10,
    EXPRESSION_SMILE_OPENMOUTH      = 11,
    EXPRESSION_OPENEYE_SMILE        = 12,
    EXPRESSION_SQUINT_OPENMOUTH     = 13,

    EXPRESSION_DEFENCE      = 0x14,     // 布防
    EXPRESSION_NODEFENCE    = 0x15,     // 撤防
    EXPRESSION_MUSIC        = 0x16,     // 播放音乐
    EXPRESSION_LOW_BAT      = 0x17,     // 低电压
    EXPRESSION_CHARGE       = 0x18,     // 充电
    EXPRESSION_WARNING      = 0x19,     // 报警
    EXPRESSION_STUDY        = 0x1A,     // 学习
    EXPRESSION_NET_CFG      = 0x1B,     //网络配置（wps）


    // for led refresh


    // for infrad signal
    IR_HEAD_UP,
    IR_HEAD_DOWN,
    IR_HEAD_LEFT,
    IR_HEAD_RIGHT,
    IR_BODY_RIGHT,
    IR_BODY_AHEAD,
    IR_BODY_BACK,
    IR_BODY_LEFT,
    IR_VOLUME_ADD,
    IR_VOLUME_SUB,
    IR_SECURITY,
    IR_CHARGING,
    IR_SLEEP,
    IR_AUDIO_PLAY,
    IR_AUDIO_PREV,
    IR_AUDIO_NEXT,
    IR_HEAD_STOP,
    IR_BODY_STOP,
    IR_NET_CFG,  //网络配置（wps）

    // for rf315m signal
    RF_315M_SIGNAL,
    // for FIR signal
    FIR_SIGNAL,
    // for rf31b signal
    RFM31B_SIGNAL,
    
    //for CHARGERSTATUES signal
    CHARGERSTATUES_SIGNAL ,
    
    //for CHARGERCONTROL signal
    CHARGERCTR_SIGNAL ,
    

    //for si4432 signal
    SI4432_Read_SIGNAL,
    SI4432_Send_SIGNAL,

    //for DHT11
      CHT_Read_SIGNAL,

    //for asr
      ASR_Read_SIGNAL,



     //for   request 4432send
    RQS4432_Startcharger_SIGNAL,
    RQS4432_Stopcharger_SIGNAL,

    PROTOCOL_MSG_END = 0xFFFFFFF
}MSG_t;
#endif
