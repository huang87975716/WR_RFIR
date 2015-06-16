/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : taskProtoSend.c
* Abstract    : 专用于协议发送的任务实现
* Author      : wenwu
*
* History
* date          author      notes
* 2012-09-01    wenwu       File Created.
*
*****************************************************************************
*/

#include "includes.h"
u8 protosend_buf[3]={0};
/********************************************************************************
 * FunctionName: TaskProtoSend
 *
 * Description : 协议发送任务
 *
 * Parameters  :
 *
 * Returns     : None.
 *******************************************************************************/
void TaskProtoSend(void *p_arg)
{
    INT8U err;
    INT8U *msg;

   static uint8_t IRSendStaMach = 0;
   static u8 cmd=0,DeviceName=0,DeviceCode=0;
   // SendString("TaskProtoSend \n");
    p_arg = p_arg;

    for (;;)
    {
          // 等待200ms，若没接收到消息就超时
        msg =  OSQPend(ProtoSendMBox, 0, &err);

           memcpy(protosend_buf,msg,3);
           cmd=protosend_buf[0];
           DeviceName=protosend_buf[1];
           DeviceCode=protosend_buf[2];
           Timout_Count2=200;//清空LEDcnt
           LED_Send(0);//
#if 1
                learn_DeviceName=DeviceName;
                learn_KeyCode= DeviceCode;
              //  IRSendStaMach =0;//状态机复位
                if(cmd==0x32)//红外发射
                {
                    if((learn_DeviceName >=1)&&(learn_DeviceName <=10)) //空调遥控
                        {
                              LED_Learn(1);
                               WatchDog_Feed(); //喂狗
                              Flash_Read(ARC_ADDR, ct361_Buffer, 208);    //读取FLASH ID存入BUF
                               WatchDog_Feed(); //喂狗
                              Send_IRLenData(USART2,0x0d,ct361_Buffer);   //空调
                             // SendString("空调发码");
                               LED_Learn(0);
                               WatchDog_Feed(); //喂狗
                              DelayMs(150);  //  250=625ms     //50 =125MS  25=62MS
                              WatchDog_Feed(); //喂狗
													    DelayMs(150);
													    WatchDog_Feed(); //喂狗
                            //  OSTimeDly(100);
                              Send_Stop(USART2);
                        }
                       else if ((learn_DeviceName>=11)&&(learn_DeviceName<=20))//电视遥控
                        {
                              LED_Learn(1);
                               WatchDog_Feed(); //喂狗
                              Flash_Read(URC_ADDR, ct361_Buffer, 208);    //读取FLASH ID存入BUF
                               WatchDog_Feed(); //喂狗
                              Send_IRLenData(USART2,0x07,ct361_Buffer);   //电视
                                 LED_Learn(0);
                           //    SendString("电视发码");
                               WatchDog_Feed(); //喂狗
                               DelayMs(30);
                               WatchDog_Feed(); //喂狗
                              //OSTimeDly(100);
                              Send_Stop(USART2);

                          }
                DeviceName=0,DeviceCode=0;
                learn_DeviceName =0;//清空 设备号  键值 ct361_buffer
                learn_KeyCode =0;
                }

                else if(cmd==0x33)//红外学习
                {
                           if((learn_DeviceName>=1)&&(learn_DeviceName<=10))//空调
                        {
                            Send_IRcmd(USART2, (INT8U)(CMD_ARCLEN_START)); //(CMD_URCLEN_START  CMD_IRIC_VERSION
                           // learn_cmd=1;  //开启学习
                             WatchDog_Feed(); //喂狗
                            OSQPost(TestMBox, (void*)Stu_On); //正在学习信号
                        }
                       else if ((learn_DeviceName>=11)&&(learn_DeviceName<=20))//电视
                        {
                            Send_IRcmd(USART2, (INT8U)(CMD_URCLEN_START)); //(CMD_URCLEN_START  CMD_IRIC_VERSION
                            //learn_cmd=1; //开启学习
                             WatchDog_Feed(); //喂狗
                            OSQPost(TestMBox, (void*)Stu_On); //正在学习信号
                        }
                     DeviceName=0,DeviceCode=0;
                }
                Empty_Array(ct361_Buffer);//清空发射缓存


#endif
        //SendString("sendproto");
      //  OSTimeDly(200);
        #if 0
       // 超时没有收到消息的处理
        if (err == OS_TIMEOUT)
        {
            // 上一条指令是脑袋控制指令
            if ((lastIRMsg>=IR_HEAD_UP)&&(lastIRMsg<=IR_HEAD_RIGHT))
            {
                 // 发出停止信号
                SendIRCtrl(USART2, (INT8U)(IR_HEAD_STOP-IR_HEAD_UP));
            }
            // 上一条指令是身体控制指令
            else if ((lastIRMsg >= IR_BODY_RIGHT) && (lastIRMsg <= IR_BODY_LEFT))
            {
               // 发出停止信号
                SendIRCtrl(USART2, (INT8U)(IR_BODY_STOP-IR_HEAD_UP));
            }
            lastIRMsg = 0;
            continue;
        }
         // 红外消息判断
        if ((msg >=IR_HEAD_UP) && (msg <= IR_NET_CFG))
        {
            // 与上次消息一样，则不做任何处理
            if (msg == lastIRMsg)
            {
                continue;
            }
            // 新的消息
            else
            {
                // 上一条指令是脑袋控制指令
                if ((lastIRMsg>=IR_HEAD_UP)&&(lastIRMsg<=IR_HEAD_RIGHT))
                {
                   // 发出停止信号
                    SendIRCtrl(USART2, (INT8U)(IR_HEAD_STOP-IR_HEAD_UP));
                }
                // 上一条指令是身体控制指令
                else if ((lastIRMsg >= IR_BODY_RIGHT) && (lastIRMsg <= IR_BODY_LEFT))
                {
                     // 发出停止信号
                    SendIRCtrl(USART2, (INT8U)(IR_BODY_STOP-IR_HEAD_UP));
                }
                  // 记录上次消息
                lastIRMsg = msg;
            }
        }

        switch (msg)
        {
        // 红外遥控类指令发送
        case IR_HEAD_UP:
        case IR_HEAD_DOWN:
        case IR_HEAD_LEFT:
        case IR_HEAD_RIGHT:
        case IR_BODY_RIGHT:
        case IR_BODY_AHEAD:
        case IR_BODY_BACK:
        case IR_BODY_LEFT:
        case IR_HEAD_STOP:
        case IR_BODY_STOP:
            SendIRCtrl(USART2, (INT8U)(msg-IR_HEAD_UP));
            break;
        case IR_VOLUME_ADD:
        case IR_VOLUME_SUB:
        case IR_SECURITY:
        case IR_CHARGING:
        case IR_SLEEP:
        case IR_AUDIO_PLAY:
        case IR_AUDIO_PREV:
        case IR_AUDIO_NEXT:
        case IR_NET_CFG:
            SendIRCtrl(USART1, (INT8U)(msg-IR_HEAD_UP));
            break;
           // 315M信号
        case RF_315M_SIGNAL:
            SendRF315MSignal(USART1, gSysState.rfSignal);
            break;
            //充电座引导码  发送给驱动板
        case  FIR_SIGNAL :
            if(gSysState.firSignal)
            SendFIRSignal(USART2, gSysState.firSignal);
            break;
            //充电状态指令  发送给驱动板
         case  CHARGERSTATUES_SIGNAL :
        // SendChargerSignal(USART2, );
            break;



           // RFM31B信号
       // case RFM31B_SIGNAL:
       // SendRFM31BSignal(USART1, gSysState.rfm31bSignal);
       //   break;
        default:
            break;
        }
       #endif
    }
}
