#define GLOBALS_CT361INTERNALCOMMAND
#include "includes.h"
void TaskCT361Search(void *p_arg)
{
	unsigned char j = 0;
	unsigned char temp[6];
	unsigned char Brand = 0;
	unsigned char *ptr_temp;
	INT8U err;
	
	(void) p_arg;
	while(1)
	{
		ptr_temp = (unsigned char *)OSQPend(CT361SEARCHMBox, 0, &err);
		Brand = *ptr_temp;
		for(j=CT361StatusGlobal.NowSearchingProgress+1;j<=g_remote_ATV_info[Brand][0] ;j++)
		{
			if(CT361StatusGlobal.SearchDisable==0)
			{
				temp[0] = 0x01;
				temp[1] = 0x04;
				temp[2] = 0x03;
				temp[3] = (unsigned char)(g_remote_ATV_info[Brand][j]);
				temp[4] = (g_remote_ATV_info[Brand][j]>>8);
				temp[5] = 0x02;
				SendCmdToCt361(&temp[0]);
				CT361StatusGlobal.NowSearchingProgress = j;
				CT361StatusGlobal.NowDeviceIndex = g_remote_ATV_info[Brand][j];
				OSTimeDlyHMSM(0,0,1,0);
				Send_Stop(USART2);
				WatchDog_Feed();
			}
			else break;
		}
		if(CT361StatusGlobal.NowSearchingProgress == \
											g_remote_ATV_info[Brand][0])
		{
			CT361StatusGlobal.IsSearching = 0;
			CT361StatusGlobal.NowSearchingProgress = 0;
			CT361StatusGlobal.SearchDisable = 1;
			SendString("Searching reaches the end");
		}
		
}
}


void TaskCT361Internal(void * p_arg)
{
  INT8U err;
  IDPROTOCOL_t *pProto;
	unsigned char strbuff[7];
	while(1)
	{
		pProto = (IDPROTOCOL_t *)OSQPend(CT361INTERNALMBox, 0, &err);
		switch (pProto->command)
		{
			case CT361_StartSearch :
				CT361StatusGlobal.SearchDisable = 0;
				CT361StatusGlobal.IsSearching = 1;
				if(pProto->data[0] == ATV) 
					OSQPost(CT361SEARCHMBox, (void*)((pProto->data)+1));
				break;
			case CT361_StopSearch:
				CT361StatusGlobal.SearchDisable = 1;
				CT361StatusGlobal.IsSearching = 0;
				Send_Stop(USART2);
				Hex2Str(CT361StatusGlobal.NowDeviceIndex,strbuff);
				SendString(strbuff);
				break;
			case CT361_SendIRCmdInteral:
				SendCT361InternalCMD(pProto->data);
				OSTimeDlyHMSM(0,0,1,0);
				Send_Stop(USART2);
				WatchDog_Feed();
				break;
			default:
				break;
		}
	}
}


void SendCT361InternalCMD(unsigned char *temp)
{
	
}

void SendCmdToCt361(unsigned char *data)
{
	PROTOCOL_t BufferToCt361;
	unsigned char i;
	BufferToCt361.checksum = 0;
	//Çå¿ÕBufferToCt361
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
}

void Hex2Str(unsigned short int data, u8 str[5])
{
	unsigned char i = 0;
	unsigned char strtemp;
	for(i = 0;i<4;i++)
	{
		if ((data&0x0F)<10) str[i] = (data&0x0F)+0x30;
		else str[i] = (data&0x0F)+0x37;
		data = data>>4;
	}
	for(i=0;i<2;i++) 
	{
		strtemp = str[i];
		str[i] = str[3-i];
		str[3-i] = strtemp;
	}
	str[4] = 13;
	str[5]=10;
	str[6]=0;
	
}

