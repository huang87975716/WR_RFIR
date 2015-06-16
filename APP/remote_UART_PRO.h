/*****************************************************************************/
#ifndef _REMOTE_UART_PRO_H
#define _REMOTE_UART_PRO_H

/********************************************************************
;CONST DEFINE
*********************************************************************/
#define	START_DATA	0x16
#define	END_DATA		0x08
//ͨ�������
#define	CMD_URC_INDEX       0x01     //���� URC ���豸�š�����š���ֵ��IRIC,������IRIC�����ڲ���
#define	CMD_ARC_INDEX       0x02     //���� ARC ������š��յ�״̬������IRIC,������IRIC�����ڲ��յ���
#define	CMD_URC_FID           0x03     //���� URC �������ļ��źͼ�ֵ��IRIC,������IRIC��MPU��ȡ���ݲ����ͺ�����
#define	CMD_ARC_FID           0x04     //���� ARC �������ļ��źͿյ�״̬������IRIC,������IRIC��MPU��ȡ���ݲ����ͺ�����
#define	CMD_FILE_DATA       0x05     //���������ļ��ж��������ݷ��͸�IRIC����IRIC��MPU���͵�����CMD_READ_FILE��Ӧ��
#define	CMD_LEN_START       0x06     //���� ѧϰ�����IRIC,������IRIC����ѧϰ״̬����IRIC��MPU����ѧϰ���ݵ�����CMD_LEN_DATA��Ӧ��
#define	CMD_LEN_DATA        0x07     //���� LRC���ݸ�IRIC,������IRIC���͸����ݣ���IRIC��MPU����ѧϰ���ݵ�����CMD_LEN_DATA��ͬ��
//#define	CMD_READ_FILE       0x80     //���Ͷ������ļ������MPU��MPU�����ļ���FILEID��ƫ�Ƶ�ַADDR_OFS����Ҫ��ȡ�����ݸ���DataCnt,������ȡ���󷢸�IRIC
#define	CMD_ARC_VAR          0x0A    //���� ���IRIC����汾�����IRIC,������IRIC���汾�ŷ���MPU����IRIC��MPU���Ͱ汾��Ϣ������CMD_IRIC_VER��ͬ��
#define	CMD_IRIC_VER          0x08    //IRIC��������Ŀյ�״̬��������MPU,MPU�յ��󸲸�ԭ���Ŀյ�״̬��������MPU��IRIC���Ϳյ�����CMD_ARC_FID��CMD_ARC_INDEX��Ӧ��
#define   CMD_ARC_LEN_START  0x0c	//�յ�ѧϰ����
#define   CMD_ARC_LEN_DATA   0x0d    //���յ�ѧϰ��������
/*********************************************************************/
//ѧϰ���ݳ��ȶ���
#define LEARN_DATA_LEN			208  // 116
//�ⲿ�ļ�
#define EXT_FILE_DATA		0x8000
/**************ѧϰ����״̬****************************************/
#define NO_LEARN_DATA	0
//#define  LEN_FLAS		0xff

#define  C_LEN_TIMEOUT	20000/500		//ѧϰ��ʱʱ��

/********************************************************************
�궨��(�����к궨��ֻ���ο�����ͬ��ƽ̨Ҫ�޸�)
*********************************************************************/
/*#define ENABLE_UART_READ()		REN = 1
#define DISABLE_UART_READ()	REN = 0
#define CLR_UART_TI()		TI = 0
#define CLR_UART_RI()		RI = 0
#define SEND_UART_1BYTE(DATA)		SBUF = DATA
#define READ_UART_1BYTE()		SBUF
*/
typedef struct
{
	u8 RemoteCMD;
	u8 RxDone;
	u8 RxBuf[256];//����Buffer
	s16 wRxPos; //��ǰ�����ֽ�λ��
	s16 wRxLen; //Ҫ���յ��ֽ���
}tagSerialPortRx;

typedef struct
{
	//u8 RemoteCMD;
	u8 TxDone;
	u8 TxBuf[500];//����Buffer
	s16 wTxPos; //��ǰ�����ֽ�λ��
	s16 wTxLen; //Ҫ���յ��ֽ���
}tagSerialPortTx;

typedef struct
{
	int ActFileNo;		//��ǰ�ļ�
	s32 StarAddr; 	//��ǰ�������Ĵ�ŵ���ʼ��ַ
}st_file_index;

/********************************************************************
��������
*********************************************************************/
void Remote_Uart_CmdTxExitIR(void);

void Remote_Uart_CmdTxURCIndex(u8 V_DEVICE,u16 V_GROUPNUM,u8 V_KEYVAL);
void Remote_Uart_CmdTxARCIndex(u16 V_GROUPNUM,u8 *VP_ARCVAR);

void Remote_Uart_CmdTxLeaningBegin(void);
//s16  Remote_Uart_CmdTxLeaningReadData(u8 *VP_LEN_DAT);
void  Remote_Uart_CmdTxLeaningWriteData(u8 *VP_LRCDATA);

void  Remote_Uart_CmdTxURCFile(u8 V_FILEID, u8 V_KEYVAL);
void  Remote_Uart_CmdTxFdata(u8 V_FILEID, u8 *fd_buffer, u8 size);
void Remote_Uart_CmdTxARCFile(u8 V_FILEID,u8 *VP_ARCVAR);

//u8 Remote_Uart_CmdRxARCVAR(u8 *VP_ARCVAR);
void  Remote_Uart_CmdTxReadVersion(void);
void Uart_RxData_PRO(tagSerialPortRx *RemoteRxDataP );

void Remote_Uart_CmdArcTxLenBegin(void);
void  Remote_Uart_CmdArcTxLenData(u8 *VP_LRCDATA);

#endif








