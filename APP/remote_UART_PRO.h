/*****************************************************************************/
#ifndef _REMOTE_UART_PRO_H
#define _REMOTE_UART_PRO_H

/********************************************************************
;CONST DEFINE
*********************************************************************/
#define	START_DATA	0x16
#define	END_DATA		0x08
//通信命令定义
#define	CMD_URC_INDEX       0x01     //发送 URC 的设备号、码组号、键值给IRIC,并启动IRIC发送内部码
#define	CMD_ARC_INDEX       0x02     //发送 ARC 的码组号、空调状态变量给IRIC,并启动IRIC发送内部空调码
#define	CMD_URC_FID           0x03     //发送 URC 的数据文件号和键值给IRIC,并启动IRIC从MPU读取数据并发送红外码
#define	CMD_ARC_FID           0x04     //发送 ARC 的数据文件号和空调状态变量给IRIC,并启动IRIC从MPU读取数据并发送红外码
#define	CMD_FILE_DATA       0x05     //将在数据文件中读到的数据发送给IRIC（与IRIC向MPU发送的命令CMD_READ_FILE对应）
#define	CMD_LEN_START       0x06     //发送 学习命令给IRIC,并启动IRIC进入学习状态（与IRIC向MPU发送学习数据的命令CMD_LEN_DATA对应）
#define	CMD_LEN_DATA        0x07     //发送 LRC数据给IRIC,并启动IRIC发送该数据（与IRIC向MPU发送学习数据的命令CMD_LEN_DATA相同）
//#define	CMD_READ_FILE       0x80     //发送读数据文件命令给MPU，MPU根据文件号FILEID，偏移地址ADDR_OFS，需要读取的数据个数DataCnt,将数据取出后发给IRIC
#define	CMD_ARC_VAR          0x0A    //发送 检测IRIC程序版本命令给IRIC,并启动IRIC将版本号发给MPU（与IRIC向MPU发送版本信息的命令CMD_IRIC_VER相同）
#define	CMD_IRIC_VER          0x08    //IRIC将修正后的空调状态变量发给MPU,MPU收到后覆盖原来的空调状态变量（与MPU向IRIC发送空调命令CMD_ARC_FID，CMD_ARC_INDEX对应）
#define   CMD_ARC_LEN_START  0x0c	//空调学习命令
#define   CMD_ARC_LEN_DATA   0x0d    //发空调学习数据命令
/*********************************************************************/
//学习数据长度定义
#define LEARN_DATA_LEN			208  // 116
//外部文件
#define EXT_FILE_DATA		0x8000
/**************学习返回状态****************************************/
#define NO_LEARN_DATA	0
//#define  LEN_FLAS		0xff

#define  C_LEN_TIMEOUT	20000/500		//学习超时时间

/********************************************************************
宏定义(这里有宏定义只作参考，不同的平台要修改)
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
	u8 RxBuf[256];//接收Buffer
	s16 wRxPos; //当前接收字节位置
	s16 wRxLen; //要接收的字节数
}tagSerialPortRx;

typedef struct
{
	//u8 RemoteCMD;
	u8 TxDone;
	u8 TxBuf[500];//接收Buffer
	s16 wTxPos; //当前接收字节位置
	s16 wTxLen; //要接收的字节数
}tagSerialPortTx;

typedef struct
{
	int ActFileNo;		//当前文件
	s32 StarAddr; 	//当前外挂码组的存放的起始地址
}st_file_index;

/********************************************************************
函数定义
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








