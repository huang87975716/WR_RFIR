/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : taskProtocol.c
* Abstract    : 协议的具体实现.
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
 * FunctionName: TaskProtocol
 *
 * Description : 协议处理任务
 *
 * Parameters  : p_arg-任务参数
 *
 * Returns     : None.
 *******************************************************************************/
 

//testing codes for CT361
#define CT361 9
#define CT361_StartSearch 				0xF1
#define	CT361_StopSearch					0xF2
#define CT361_SendIRCmdInteral		0xF3
#define CT361_KeepOnSearching			0xF4
#define	CT361_SearchResutls				0xE1
#define CT361_SearchReachTheEnd		0xE2
#define ATV 0x03
extern  const u16 g_remote_ATV_info[166][65];
void StartSearch(unsigned char Brand);
void SendCmdToCt361(unsigned char data[6]);
unsigned char CT361_InternalCommand(unsigned char command,unsigned char data[8]);


void TaskProtocol(void *p_arg)
{
    MSG_t msg;
     INT8U err , i;
     IDPROTOCOL_t *pProto;
  //   INT8U *p;

 // SendString("TaskProtocol \n");
    for (;;)
    {
        msg = (MSG_t)OSQPend(ProtocolMBox, 0, &err);
      //  OSTimeDly(3000);
         //SendString("taskprotocol");

        switch (msg)
        {
            // 串口1接收到协议数据
            case UART1_BUFF0_RECV:
            case UART1_BUFF1_RECV:
                pProto = &gU1RecvBuff[msg - UART1_BUFF0_RECV];
								if ((pProto->direction & ID_DESTMASK) == ID_RFIRBOARD)
								{
										if (pProto->command == CMD_SETID)
										{
											Flash_Write(RFIR_ID_ADDR,pProto->data,4);//读取buf存入FLASH
													SendString("\n\rID setok:");
												Flash_Read(RFIR_ID_ADDR, MY_ID, 4);    //读取FLASH ID存入BUF
													for(i=0;i<4;i++)//sizeof(ID)
													{
														SendString(";");
														Outint(MY_ID[i]);
													}
										}
								}
									// 查看是否需要应答
								if (IS_PROTO_NEED_ACK(pProto))
								{
										// 返回应答数据
										SendIDAck(USART1, pProto);
								}
                
								CT361_InternalCommand(pProto->command, pProto->data);
								break;
            default:
                break;
        }

    }
}


unsigned char CT361_InternalCommand(unsigned char command,unsigned char *data)
{
				switch (command)
				{
					case CT361_StartSearch :
						if(data[0] == ATV)
						{
							StartSearch( data[1] );
						}
						break;
					case CT361_StopSearch:
						
						break;
					case CT361_SendIRCmdInteral:
						
						break;
					case CT361_KeepOnSearching:
						
						break;
					default:
						break;
				}
				return 1;
}



void StartSearch(unsigned char Brand)
{
	unsigned char j = 0;
	unsigned char temp[6];
	for(j=1;j<g_remote_ATV_info[Brand][0]+1;j++)
	{
		temp[0] = 0x01;
		temp[1] = 0x04;
		temp[2] = 0x03;
		temp[3] = (unsigned char)(g_remote_ATV_info[Brand][j]);
		temp[4] = (g_remote_ATV_info[Brand][j]>>8);
		temp[5] = 0x02;
		SendCmdToCt361(&temp[0]);
	}
}

void SendCmdToCt361(unsigned char *data)
{
	PROTOCOL_t BufferToCt361;
	unsigned char i;
	BufferToCt361.checksum = 0;
	//清空BufferToCt361
	BufferToCt361.header = 0x16;
	BufferToCt361.command = data[0];
	BufferToCt361.length = data[1];
	BufferToCt361.data[0] = data[2];
	BufferToCt361.data[1] = data[3];
	BufferToCt361.data[2] = data[4];
	BufferToCt361.data[3] = data[5];	
	for (i = 0;i<BufferToCt361.length;i++)
	{
		BufferToCt361.checksum = BufferToCt361.checksum + data[i+2];
	}
	BufferToCt361.checksum = BufferToCt361.checksum + BufferToCt361.header \
														+	BufferToCt361.command + BufferToCt361.length;
	
	BufferToCt361.end = 0x08;
	
	SendProtocol(USART2,&BufferToCt361);
	DelayMs(300);
	Send_Stop(USART2);
	WatchDog_Feed();
}
