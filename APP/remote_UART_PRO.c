#include "remote.h"
//#include "stdafx.h"
/*******************************************************************
��������
********************************************************************/
char		gLeaningIRState=0;		// =0 û��ѧϰ״̬��=1 ����ѧϰ״̫

s16  arc_group;
u8	len_buffer[250];
u8  arc_len_buffer[250];
u8	updata_arc_disp = 0;
u8	updata_ICVER_disp = 0;
/*******************************************************************
����
********************************************************************/
extern int	 devNo_FURC;
extern int	 devNo_FARC;
extern type_ex_ctl	ex_ctl;
extern type_remote_ctl remote_ctl_c;
extern u16 current_group[7];	//��ǰ����
/*******************************************************************
����������
********************************************************************/
//const u16  URC_FILE_ADDR[]={0x1800,0x1800};
//const u16  ARC_FILE_ADDR[]={0x1A00,0x1A00};
/*******************************************************************
 --------------UART ͨ�Ŵ������ -------------------
********************************************************************/
/*-------------------------------------------------------------------
��������checksum
-------------------------------------------------------------------*/
s16	Remote_CompareChksum(u8 *buffer)
{	
	u8	i,V_CHECKSUM=0;
	if((buffer[0] == START_DATA)&&(buffer[2] < 250))	//���������������255������
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
��HB1682�˳�����״̬/ѧϰ״̬
-------------------------------------------------------------------*/
void Remote_Uart_CmdTxExitIR(void)		//��400Byte 0x00��IRIC�˳�����ģʽ
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
����HB1682
-------------------------------------------------------------------*/
void Remote_Uart_CmdTxWakeUp(void)		//��1Byte 0x00��IRIC ��˯��ģʽ�л�������ģʽs
{
	u8 buffer[1];

	buffer[0] = 0;
	//Remote_Uart_TxData(buffer,1);//modefied by jaosn, need to be fixed 
	//HB_sleep(50);//modefied by jaosn, need to be fixed 

}

/*-------------------------------------------------------------------
ͨ�������� ��URC �뺯��
CMD_URC_INDEX       0x10     //���� URC ���豸�š�����š���ֵ��IRIC,������IRIC�����ڲ���
DNUM + 0x10 (������)+ DEVICENUM��1Bbyte�� + GROUPNUM��2Bbyte�� + KEYVAL��1Bbyte�� +CHECKSUM��1Bbyte��
-------------------------------------------------------------------*/
void Remote_Uart_CmdTxURCIndex(u8 V_DEVICE,u16 V_GROUPNUM,u8 V_KEYVAL)
{
	u8 buffer[9];
	
  Remote_Uart_CmdTxWakeUp();
	
	buffer[1] = CMD_URC_INDEX;	 //������
	buffer[2] = 4; //�����ֽ���
	buffer[3] = V_DEVICE;       //�豸��
	buffer[4] = V_GROUPNUM&(0xFF);      //����ŵ�λ
	buffer[5] = V_GROUPNUM>>8;          //����Ÿ�λ
	buffer[6] = V_KEYVAL;               //��ֵ
	Remote_GetChksum(buffer);

// 	RemoteRxData.wRxLen = 5;
// 	RemoteRxData.wRxPos = 0;
// 	RemoteRxData.RemoteCMD = CMD_URC_INDEX;
	
	//Remote_Uart_TxData(buffer,sizeof(buffer));//modefied by jaosn, need to be fixed
}


