/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : taskProtoSend.c
* Abstract    : 红外学习任务 专用4432协议发射的任务实现
* Author      : wuwei
*
* History
* date          author      notes
* 2014-07-07    wuwei       File Created.
*
*****************************************************************************
*/

#include "includes.h"
extern u8 Ir_REV_OK;
extern u8 test_charger[];
extern PROTOCOL_t *u2p;

u8 learn_switch=0;  //学习状态
u8 learn_KeyCode=0; // 学习的键值 ( 空调<10  电视<30)
u8 learn_DeviceName=0;   // 电器属性（空调，电视OR其他）  0x00-0x1f 空调  0x20-3f 电视
//u8 len_ok=0;   //学习成功标志位  1:成功  0：不成功
u8 learn_cmd=0; //学习开始标志位  1:成功  0：不成功
u8 Learn_State=0;//  1空调学习  2电视学习
//u8 send_sig=0;  //发送信号 标志位 1：学习成功 2：学习失败 3：正在学习中
 //#define Operate_Page (32*256-1)//操作最后一个页
 //unsigned char M25P16_Buffer[M25P16_Page_Size]={0};//定义一个数组用于存放读取的内容，如果此数组定义到main内，程序不能正确执行，原因有待考证
 unsigned char ct361_Buffer[208]={0};//给CT361发射缓存
/********************************************************************************
 * FunctionName: Tasktest
 *
 * Description : 协议发送任务
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/
void TaskTest(void *p_arg)
{
    INT8U err ;
//    u16 i;
  //  unsigned char buf[]="0123456789abcdefghijklmnopqrstuvwxyz";
    INT32U msg;
    u8 send_sig=0;  //发送信号 标志位 1：学习成功 2：学习失败 3：正在学习中
    p_arg = p_arg;

    for (;;)
    {
    /********发射4432数据********/
    msg = (INT32U)OSQPend(TestMBox, 0, &err);
    send_sig= msg;
       if(send_sig!=0)
      {
           WatchDog_Feed(); //喂狗
          if(send_sig==1)
          {
              if (Learn_State==1)//空调学习成功
              {
                   WatchDog_Feed(); //喂狗
                 Send_LenStop(USART2);//停止学习
                  WatchDog_Feed(); //喂狗
                 Flash_Write(ARC_ADDR,u2p->data,208);//读取buf存入FLASH   存入固定器件地址的键值上ARC1_ADDR+learn_DeviceName*2080+learn_KeyCode*208
                 learn_cmd=0;//清空开始学习标志?
                 Learn_State=0;

                // SendString(" 空调学习成功！");
              }
              else if(Learn_State==2)//电视学习成功
              {
                   WatchDog_Feed(); //喂狗
                 Send_LenStop(USART2);//停止学习
                  WatchDog_Feed(); //喂狗
                 Flash_Write(URC_ADDR,u2p->data,208);//读取buf存入FLASH
                 learn_cmd=0;//清空开始学习标志?
                 Learn_State=0;

                // SendString(" 电视学习成功！");
              }
             Send_4432ID0 =  MY_ID[0];// my id
             Send_4432ID1 =  MY_ID[1];
             Send_4432ID2 =  MY_ID[2];
             Send_4432ID3 =  MY_ID[3];
             Send_4432DIR = 0x82 ;//方向
             Send_4432CMD = 0x34 ;//反馈指令
             Send_4432LENGTH= 3; //数据长度
             Send_4432DATA0 = send_sig;//学习成功
             Send_4432DATA1 = learn_DeviceName;//
             Send_4432DATA2 = learn_KeyCode;//
             Data_Encrypt(si4432cmd_senddata,18);   //
              WatchDog_Feed(); //喂狗
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //喂狗
               OSTimeDly(8);
                WatchDog_Feed(); //喂狗
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //喂狗
          }
          else if(send_sig==2)
          {
              learn_cmd=0;//清空开始学习标志?
              SendString(" 学习失败！");
             Send_4432ID0 =  MY_ID[0];// my id
             Send_4432ID1 =  MY_ID[1];
             Send_4432ID2 =  MY_ID[2];
             Send_4432ID3 =  MY_ID[3];
             Send_4432DIR = 0x82 ;//方向
             Send_4432CMD = 0x34 ;//反馈指令
             Send_4432LENGTH= 3; //数据长度
             Send_4432DATA0 = send_sig;//学习成功
             Send_4432DATA1 = learn_DeviceName;//
             Send_4432DATA2 = learn_KeyCode;//
             Data_Encrypt(si4432cmd_senddata,18);   //
              WatchDog_Feed(); //喂狗
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //喂狗
               OSTimeDly(5);
                WatchDog_Feed(); //喂狗
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //喂狗
          }
          else if(send_sig==3)
              SendString(" 正在学习！");
          else
          {
               learn_cmd=0;//清空开始学习标志?
               SendString(" 其他错误= ");
             Send_4432ID0 =  MY_ID[0];// my id
             Send_4432ID1 =  MY_ID[1];
             Send_4432ID2 =  MY_ID[2];
             Send_4432ID3 =  MY_ID[3];
             Send_4432DIR = 0x82 ;//方向
             Send_4432CMD = 0x34 ;//反馈指令
             Send_4432LENGTH= 3; //数据长度
             Send_4432DATA0 = send_sig;//学习成功
             Send_4432DATA1 = learn_DeviceName;//
             Send_4432DATA2 = learn_KeyCode;//
             Data_Encrypt(si4432cmd_senddata,18);   //
              WatchDog_Feed(); //喂狗
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //喂狗
               OSTimeDly(5);
                WatchDog_Feed(); //喂狗
             sending(send_data,Send_Legth);
              WatchDog_Feed(); //喂狗
             //  Outint(send_sig);
          }


         //if((send_sig==1)||(send_sig==2))//如果学习成功或失败 清空设备号和 键值 发送学习停止信号
            if (send_sig!=3)
          {
           //Send_LenStop(USART2);
           learn_DeviceName = 0;
           learn_KeyCode = 0;
           learn_cmd=0;//清空开始学习标志位
          }
         send_sig=0;
      }
//else  //接收4432信号send_sig==0

    }
}
