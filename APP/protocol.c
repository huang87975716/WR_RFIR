/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : protocol.c
* Abstract    : 通讯协议的具体实现
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
*
*****************************************************************************
*/



#include "includes.h"
/********************************************************************************
 * FunctionName: SendAck
 *
 * Description : 发送应答信号
 *
 * Parameters  : USARTx-发送通道，pProto-欲应答的协议.
 *
 * Returns     : None.
 *******************************************************************************/
void SendIDAck(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto)
{
    IDPROTOCOL_t proto;
    proto.header = 0xAA;
    proto.direction = pProto->direction >> 4;
    proto.direction &= 0x0F;
    proto.direction |= (pProto->direction << 4);
    proto.msgref = pProto->msgref & 0x3F;
    proto.command = pProto->command;
    proto.length = 0;

    proto.checksum = proto.header;
    proto.checksum += proto.direction;
    proto.checksum += proto.msgref;
    proto.checksum += proto.command;
    proto.checksum += proto.length;

    SendIDProtocol(USARTx, &proto);
}

#if  0





/********************************************************************************
 * FunctionName: SendAck
 *
 * Description : 发送应答信号
 *
 * Parameters  : USARTx-发送通道，pProto-欲应答的协议.
 *
 * Returns     : None.
 *******************************************************************************/
void SendAck(USART_TypeDef *USARTx, PROTOCOL_t *pProto)
{
    PROTOCOL_t proto;
    proto.header = 0xAA;
    proto.direction = pProto->direction >> 4;
    proto.direction &= 0x0F;
    proto.direction |= (pProto->direction << 4);
    proto.msgref = pProto->msgref & 0x3F;
    proto.command = pProto->command;
    proto.length = 0;

    proto.checksum = proto.header;
    proto.checksum += proto.direction;
    proto.checksum += proto.msgref;
    proto.checksum += proto.command;
    proto.checksum += proto.length;

    SendProtocol(USARTx, &proto);
}
#endif

/********************************************************************************
 * FunctionName: SendProtocol
 *
 * Description : 发送协议数据
 *
 * Parameters  : USARTx-发送的通道。pProto-欲发送的协议.
 *
 * Returns     : None.
 *******************************************************************************/
void SendIDProtocol(USART_TypeDef *USARTx, IDPROTOCOL_t *pProto)
{
      INT32U i;
    INT8U *p = (INT8U *)pProto;
#if (NEED_ACK_EN)
    ACK_t *pAck;
#endif

#if (NEED_ACK_EN)
    if ((INT32U)USARTx == (INT32U)USART1)
    {
        pAck = gACK[0];
    }
    else
    {
        pAck = gACK[1];
    }
#endif

    // 通过串口2转发协议
    for (i=0; i<5; i++)
    {
        USARTx->DR = *p++;
        while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    }
    for (i=0; i<pProto->length; i++)
    {
        USARTx->DR = *p++;
        while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    }
    USARTx->DR = pProto->checksum;
    while ((USARTx->SR & USART_FLAG_TXE) == RESET);
}


/********************************************************************************
 * FunctionName: SendProtocol
 *
 * Description : 发送协议数据
 *
 * Parameters  : USARTx-发送的通道。pProto-欲发送的协议.
 *
 * Returns     : None.
 *******************************************************************************/
void SendProtocol(USART_TypeDef *USARTx, PROTOCOL_t *pProto)
{
    INT32U i;
    INT8U *p = (INT8U *)pProto;
    INT8U cnt=0;
#if (NEED_ACK_EN)
    ACK_t *pAck;
#endif

#if (NEED_ACK_EN)
    if ((INT32U)USARTx == (INT32U)USART1)
    {
        pAck = gACK[0];
    }
    else
    {
        pAck = gACK[1];
    }
#endif
/*
    USARTx->DR = 0x00;
      while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    USARTx->DR = 0x00;
      while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    USARTx->DR = 0x00;
    while ((USARTx->SR & USART_FLAG_TXE) == RESET);
    */
          USART2_SEND_DATA[0]=0;
          DMA_Cmd(DMA1_Channel7, DISABLE); //改变datasize前先要禁止通道工作
          DMA1_Channel7->CNDTR=1; //DMA1,传输数据量
          USART2_TX_Finish=0;//DMA传输开始标志量
          DMA_Cmd(DMA1_Channel7, ENABLE);
    DelayMs(30);

    // 通过串口2转发协议
    for (i=0; i<3; i++)
    {
      //  USARTx->DR = *p++;
       //delay_10us(20);
      //  while ((USARTx->SR & USART_FLAG_TXE) == RESET);
      USART2_SEND_DATA[cnt]=*p++;
      cnt++;
    }
    for (i=0; i<pProto->length; i++)
    {
       // USARTx->DR = *p++;
       // delay_10us(20);
       // while ((USARTx->SR & USART_FLAG_TXE) == RESET);
      USART2_SEND_DATA[cnt]=*p++;
      cnt++;
    }
    //USARTx->DR = pProto->checksum;
    //      delay_10us(20);
    //while ((USARTx->SR & USART_FLAG_TXE) == RESET);
      USART2_SEND_DATA[cnt]=pProto->checksum;//*p++;
      cnt++;

    //USARTx->DR = pProto->end;
        //  delay_10us(20);
   // while ((USARTx->SR & USART_FLAG_TXE) == RESET);
      USART2_SEND_DATA[cnt]=pProto->end;//*p++;
      cnt++;
      DMA_Cmd(DMA1_Channel7, DISABLE); //改变datasize前先要禁止通道工作
      DMA1_Channel7->CNDTR=cnt; //DMA1,传输数据量
      USART2_TX_Finish=0;//DMA传输开始标志量
      DMA_Cmd(DMA1_Channel7, ENABLE);


      cnt=0;
}


/********************************************************************************
 * FunctionName: Send_LRC
 *
 * Description : Sending the LRC  to study protocol.
 *
 * Parameters  : USARTx-Sending Port. irSignal-infrad signal in byte.
 *
 * Returns     : None.
 *******************************************************************************/
void Send_IRcmd(USART_TypeDef *USARTx, INT8U CMD)
{
    PROTOCOL_t IRCtrl;

    IRCtrl.header = 0x16;

    IRCtrl.command = CMD;

    IRCtrl.length = 0;


    IRCtrl.checksum =  IRCtrl.header;
    IRCtrl.checksum += IRCtrl.command;
    IRCtrl.checksum += IRCtrl.length;
    IRCtrl.checksum =  IRCtrl.checksum%256;
    IRCtrl.end      =   0x08;
    SendProtocol(USARTx, &IRCtrl);
}

void Send_Stop(USART_TypeDef *USARTx )
{   u16 i;
    for (i=0;i<400;i++)
    {
   //    USARTx->DR=(u8)0x00;
           USART2_SEND_DATA[0]=0;
         DMA_Cmd(DMA1_Channel7, DISABLE); //改变datasize前先要禁止通道工作
         DMA1_Channel7->CNDTR=1; //DMA1,传输数据量
         USART2_TX_Finish=0;//DMA传输开始标志量
         DMA_Cmd(DMA1_Channel7, ENABLE);
       delay_10us(15);//   1MS
    }
}


void Send_LenStop(USART_TypeDef *USARTx )
{   u16 i;
    for (i=0;i<400;i++)
    {
      // USARTx->DR=(u8)0x00;
        USART2_SEND_DATA[0]=0;
         DMA_Cmd(DMA1_Channel7, DISABLE); //改变datasize前先要禁止通道工作
        DMA1_Channel7->CNDTR=1; //DMA1,传输数据量
          USART2_TX_Finish=0;//DMA传输开始标志量
         DMA_Cmd(DMA1_Channel7, ENABLE);
       delay_10us(25);// 3515
    }
}

void Send_IRLenData(USART_TypeDef *USARTx, INT8U CMD,u8 *IRlen_buf)
{  u8 i;
    PROTOCOL_t IRCtrl;

    IRCtrl.header = 0x16;

    IRCtrl.command = CMD;

    IRCtrl.length = 208;
    for (i=0;i<IRCtrl.length;i++)  //4位ID
    {
      IRCtrl.data[i] =*(IRlen_buf+i);
    }

    IRCtrl.checksum =  IRCtrl.header;
    IRCtrl.checksum += IRCtrl.command;
    IRCtrl.checksum += IRCtrl.length;
    for (i=0;i<IRCtrl.length;i++)  //4位ID
    {
     IRCtrl.checksum+= IRCtrl.data[i];
    }

    IRCtrl.checksum =  IRCtrl.checksum%256;
    IRCtrl.end      =   0x08;
    SendProtocol(USARTx, &IRCtrl);
}

void ReV_CH361ReVProtocol(INT8U *buf)
{
     u8 u2StaMach=0,i=0,j=0;

         if (u2StaMach == 0)
         {
            if (buf[i] == 0x16)  //head
            {
                u2p->header = buf[i];
                u2StaMach = 1;
                u2p->checksum = buf[i];
                i++;
            }
            else u2StaMach = 0;
         }
            if (u2StaMach == 1)   //cmd
         {
            u2p->command = buf[i];
            u2StaMach = 2;
            u2p->checksum += buf[i];
            i++;
         }

           if (u2StaMach ==2)  //length
         {
                u2p->length = buf[i];
                if (u2p->length == 0)  //0字节  数据不接收直接和校验
                {
                    u2StaMach = 4;

                }
                else if (u2p->length == 208)            //208字节或其他  进入数据接收
                {
                    u2StaMach = 3;

                }
                else    //其他字节舍弃
                    u2StaMach = 0;
                u2p->dataIndex = 0;
                u2p->checksum += buf[i];
                i++;
         }

          if (u2StaMach == 3)  //data recieve
        {
            for (j=0;j<208;j++)
            {
                u2p->data[u2p->dataIndex++] = buf[i];
                u2p->checksum += buf[i];
                i++;
            }
            //u2p->data[u2p->dataIndex++] = buf[i];
            // u2p->checksum += u2RecvCh;
            if (u2p->dataIndex == u2p->length)
            {
                u2StaMach = 4;
                u2p->dataIndex=0;
            }
        }
          if (u2StaMach == 4)
        {
             // 判断校验和
            if((u2p->checksum)%256 == buf[i])  //校验和MOD 256
            {
                u2StaMach = 5;
                i++;
            }
            else
            u2StaMach = 0;
        }
          if (u2StaMach == 5)
        {
             u2p->end = buf[i];
             i++;
			switch(u2p->command)
			{
				case 0x81:
					SendString("CT361 OK\n");//sent to PC
				break;
				case 0xC1:
					SendString("CT361 failed\n");//sent to PC
				break;
				default:
				break;
			}
             // 判断END
             if (learn_cmd==1)  //学习开始标志位允许学习
          {
          //   learn_cmd=0;//清空开始学习标志位
              Timout_Count=TIMOUT_20s;  //  超时检测标志复位
             if(((u2p->command)==0x86)||((u2p->command)==0x8c))  //协议end  电视学习成功 0x86  空调学习成功0x8c
            {
                    if((learn_DeviceName>=1)&&(learn_DeviceName<=10))//空调 1-10空调
                    {
                        if((learn_KeyCode>=1)&&(learn_KeyCode<=10))  //每个空调固定10键
                        {
                         // Flash_Write(ARC_ADDR,u2p->data,208);//读取buf存入FLASH   存入固定器件地址的键值上ARC1_ADDR+learn_DeviceName*2080+learn_KeyCode*208
                             Learn_State=1;//空调
                            OSQPost(TestMBox, (void*)Stu_Success); //学习成功信号
                        }
                    }
                   else if ((learn_DeviceName>=11)&&(learn_DeviceName<=20)) // 电视  10-19 电视
                   {
                       if((learn_KeyCode>=1)&&(learn_KeyCode<=30)) //每个电视固定30键
                      {
                       //  Flash_Write(URC_ADDR,u2p->data,208);//读取buf存入FLASH
                           Learn_State=2;//电视
                         OSQPost(TestMBox, (void*)Stu_Success); //学习成功信号
                      }
                   }
                   else   //超出键值范围
                   {
                       OSQPost(TestMBox, (void*)Stu_Fail); //学习失败信号
                   }
                u2StaMach = 0;
            }

            else if (((u2p->command)==0xc6)||((u2p->command)==0xcc))  //   电视学习失败0xc6  空调学习失败0xcc
            {
             OSQPost(TestMBox, (void*)Stu_Fail); //学习失败信号
             u2StaMach = 0;
            }
            /*
             else if((u2p->command==0x8d)||(u2p->command==0x87))//如果为 87 8D错误，重新返回接收
            {
                u2StaMach = 0;
            }
            */
            else //其他错误
            {
             OSQPost(TestMBox, (void*)(u2p->command)); //学习失败信号
             u2StaMach = 0;
            }
          //  learn_cmd=0;//清空开始学习标志位
          }
              u2StaMach = 0;
              Empty_Array(buf);//清空buf
        }
       //Send2Char(u2RecvCh);
}

