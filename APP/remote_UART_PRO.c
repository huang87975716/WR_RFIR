#include "remote.h"
//#include "stdafx.h"
/*******************************************************************
变量定义
********************************************************************/
char		gLeaningIRState=0;		// =0 没有学习状态，=1 进入学习状太

s16  arc_group;
u8	len_buffer[250];
u8  arc_len_buffer[250];
u8	updata_arc_disp = 0;
u8	updata_ICVER_disp = 0;
/*******************************************************************
声名
********************************************************************/
extern int	 devNo_FURC;
extern int	 devNo_FARC;
extern type_ex_ctl	ex_ctl;
extern type_remote_ctl remote_ctl_c;
extern u16 current_group[7];	//当前码组
/*******************************************************************
常量数组义
********************************************************************/
//const u16  URC_FILE_ADDR[]={0x1800,0x1800};
//const u16  ARC_FILE_ADDR[]={0x1A00,0x1A00};
/*******************************************************************
 --------------UART 通信处理程序 -------------------
********************************************************************/
/*-------------------------------------------------------------------
计算数据checksum
-------------------------------------------------------------------*/
s16	Remote_CompareChksum(u8 *buffer)
{	
	u8	i,V_CHECKSUM=0;
	if((buffer[0] == START_DATA)&&(buffer[2] < 250))	//单次数据最长不超过255个数据
	{
		for(i = 0; i < (buffer[2]+3); i++)
         	 V_CHECKSUM += buffer[i];
	
		if((V_CHECKSUM==buffer[i]) && (buffer[i+1] == END_DATA)) 
			return 1;
	}
		return 0;
}
//=========================================
void Remote_GetChksum(u8 *buffer)
{	
	u8	i,V_CHECKSUM=0;

	buffer[0] = START_DATA;
	for(i = 0; i<(buffer[2]+3); i++)
  	V_CHECKSUM += buffer[i];

	buffer[i] = V_CHECKSUM;
	i++;
	buffer[i] = END_DATA;
	
}
/*-------------------------------------------------------------------
让HB1682退出发码状态/学习状态
-------------------------------------------------------------------*/
void Remote_Uart_CmdTxExitIR(void)		//发400Byte 0x00让IRIC退出发码模式
{
	//u8 buffer[100];
	int i;

// 	for(i=0;i<sizeof(RemoteTxData.TxBuf);i++)
// 		RemoteTxData.TxBuf[i] = 0;

// 	RemoteTxData.wTxLen = sizeof(RemoteTxData.TxBuf);
// 	RemoteTxData.wTxPos = 1;
// 	RemoteTxData.TxDone = 0;
// 	Remote_Uart_TxData(RemoteTxData.TxBuf,1);
}

/*-------------------------------------------------------------------
换醒HB1682
-------------------------------------------------------------------*/
void Remote_Uart_CmdTxWakeUp(void)		//发1Byte 0x00让IRIC 由睡眠模式切换到工作模式s
{
	u8 buffer[1];

	buffer[0] = 0;
	//Remote_Uart_TxData(buffer,1);//modefied by jaosn, need to be fixed 
	//HB_sleep(50);//modefied by jaosn, need to be fixed 

}

/*-------------------------------------------------------------------
通过索引号 发URC 码函数
CMD_URC_INDEX       0x10     //发送 URC 的设备号、码组号、键值给IRIC,并启动IRIC发送内部码
DNUM + 0x10 (命令字)+ DEVICENUM（1Bbyte） + GROUPNUM（2Bbyte） + KEYVAL（1Bbyte） +CHECKSUM（1Bbyte）
-------------------------------------------------------------------*/
void Remote_Uart_CmdTxURCIndex(u8 V_DEVICE,u16 V_GROUPNUM,u8 V_KEYVAL)
{
	u8 buffer[9];
	
  Remote_Uart_CmdTxWakeUp();
	
	buffer[1] = CMD_URC_INDEX;	 //命令字
	buffer[2] = 4; //数据字节数
	buffer[3] = V_DEVICE;       //设备号
	buffer[4] = V_GROUPNUM&(0xFF);      //码组号低位
	buffer[5] = V_GROUPNUM>>8;          //码组号高位
	buffer[6] = V_KEYVAL;               //键值
	Remote_GetChksum(buffer);

// 	RemoteRxData.wRxLen = 5;
// 	RemoteRxData.wRxPos = 0;
// 	RemoteRxData.RemoteCMD = CMD_URC_INDEX;
	
	//Remote_Uart_TxData(buffer,sizeof(buffer));//modefied by jaosn, need to be fixed
}


