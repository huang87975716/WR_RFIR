/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : protocol.h
* Abstract    : 通讯协议的相关定义
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
* 2014-04-02    wuwei       File Modify.
*
*****************************************************************************
*/


#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#ifdef  GLOBALS_RFIR
#define PROTOCOL_EXT
#else
#define PROTOCOL_EXT extern
#endif

// 电路板ID号定义
//#define ID_GM8126           1
//#define ID_LEDPANEL         2
//#define ID_DRVBOARD         3
#define ID_RFIRBOARD         8
#define ID_DESTMASK         0x0F
#define ID_SOURCEMASK       0xF0


//#define CMD_URC_INDEX                        0X01      //URC内部码发送命令(普通设备万用遥控器)//modefied by jaosn, need to be fixed 
//#define CMD_ARC_INDEX                        0X02      //ARC内部码发送命令（空调设备万用遥控器//modefied by jaosn, need to be fixed 
#define CMD_URCLEN_START                     0X06      //进入URC学习命令  电视
#define CMD_ARCLEN_START                     0X0C      //进入ARC学习命令  空调
#define CMD_IRIC_VERSION                     0X08      //获取版本信息

// 指令列表
#define CMD_SETID                            0x42    // 写从机ID




#define CMD_CHT                              0x60    // 温湿度读取指令
#define CMD_ASR                              0x64   // 语音识别指令


/***********充电相关*****************/


#define CMD_CHARGERCTR                       0x8F    // 充电控制指令
//#define CMD_LEARN4432ID                      0x90   // 4432ID学习指令

/***********串口转发4432相关**********/

#define CMD_SMARTHOME                        0x32    // 智能家居红外转发指令
#define CMD_CHARGER_4432CMD                  0X83     // 充电座请求4432指令
#define CMD_SMARTLEARN                       0x33    // 智能家居红外转发学习

/***********4432相关转串口**********/
#define CMD_FALLING                          0x30    // 跌倒传感器指令
#define CMD_CHARGER_4432FB                   0X82     // 充电座4432指令反馈
#define CMD_SMARTLEARN_FB                    0x34    // 智能家居红外转发学习状态反馈


typedef struct tagProtocolID
{
    uint8_t header;
    uint8_t direction;
    uint8_t msgref;
    uint8_t command;
    uint8_t length;
    uint8_t data[8];
    uint8_t checksum;
    uint8_t dataIndex;
    uint8_t buffswitch; // ????????
}IDPROTOCOL_t;

typedef struct tagProtocol
{
    uint8_t header;
    uint8_t command;
    uint8_t length;
    uint8_t data[208];
    uint8_t checksum;
    uint8_t end;
    uint8_t dataIndex;
    uint8_t buffswitch; // 缓存切换
}PROTOCOL_t;

typedef struct si44432tagProtocol
{

   //  uint64_t  rfslaveid;

    uint8_t rf_header;
    uint8_t rfdirection;
    uint8_t rfmsgref;
    uint8_t rfcommand;
    uint8_t rflength;
    uint8_t rfid[4];   //4字节ID
    uint8_t rfdata[4];
  //  uint8_t checksum;
    uint8_t rfdataIndex;
   // uint8_t buffswitch; // 缓存切换
}SI4432PROTOCOL_t;





// 决定是否需要ACK
#define NEED_ACK_EN     0

typedef struct ack
{
    INT16U needACK;    // 期望得到的应答
    INT16U recvACK;    // 实际得到的应答
}ACK_t;

// 查看是否需要应答，消息类型为CMD，需要应答
#define IS_PROTO_NEED_ACK(pProto) ((pProto->msgref & 0xC0) == 0xC0)

// Uart1和Uart2的接收缓冲区分开来，以防止两个串口同时接收到数据而出错。
PROTOCOL_EXT IDPROTOCOL_t gU1RecvBuff[2];
PROTOCOL_EXT PROTOCOL_t gU2RecvBuff[2];
//SI4432
PROTOCOL_EXT SI4432PROTOCOL_t SI4432RecvBuff[2];

// 检测应答缓冲区
PROTOCOL_EXT ACK_t gACK[2];
//
extern PROTOCOL_t *u2p;

// 应答
void SendAck(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto);
void SendIDAck(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto);
// 主动发送
void SendProtocol(USART_TypeDef *USARTx, PROTOCOL_t *pProto);
void SendIDProtocol(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto);

//发送开始学习
void Send_IRcmd(USART_TypeDef *USARTx, INT8U CMD);
//发送学习命令
void Send_IRLenData(USART_TypeDef *USARTx, INT8U CMD,u8 *IRlen_buf);
//发送停止命令
void Send_Stop(USART_TypeDef *USARTx);
//发送学习停止命令
void Send_LenStop(USART_TypeDef *USARTx);
//协议解析
void ReV_CH361ReVProtocol(INT8U *buf);
#endif
