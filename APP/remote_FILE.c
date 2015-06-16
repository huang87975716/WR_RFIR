#include "remote.h"
#include "stdafx.h"
#include "remote.h"


int	 devNo_UART = -1;
int	 devNo_FURC = -1;
int	 devNo_FARC = -1;
/***************************************************************************************************/
HANDLE	HANDLE_BUF[3] = {NULL,NULL,NULL};
OVERLAPPED m_OverlappedRead;
OVERLAPPED m_OverlappedWrite;
/***************************************************************************************************/
extern int devNo_UART;
/***************************************************************************************************/
void Remote_File_ReadData(u8 id, u32 V_DATA_ADDR,u8 *VP_DATA,u8 V_DATA_CNT)
{
	s16 status;
	u32	cut_addr;

	cut_addr = HB_Seek(V_DATA_ADDR, id);
	status = HB_read(VP_DATA, V_DATA_CNT, id);
}
/***************************************************************************************************/

/***************************************************************************************************/
void HB_sleep(int time_ms)
{
	Sleep(time_ms);
}
/***************************************************************************************************/

/***************************************************************************************************/
int HB_open(char *devName, int mode)
{
	CString	 CdevName;
	HANDLE m_hCom;
	int i;
	DWORD dw;

	CdevName = devName;
	if(CdevName=="uart_ir")
	{
		m_hCom = CreateFile( "COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
		dw =GetLastError(); 
		if (m_hCom==(HANDLE)-1) {  AfxMessageBox("打开COM1失败"); return false; }
   
		memset( &m_OverlappedRead,0, sizeof( OVERLAPPED ) );
		memset( &m_OverlappedWrite,0, sizeof( OVERLAPPED ) );
		COMMTIMEOUTS CommTimeOuts;
		CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
		CommTimeOuts.ReadTotalTimeoutConstant = 0;
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
		CommTimeOuts.WriteTotalTimeoutConstant = 5000;
		SetCommTimeouts( m_hCom, &CommTimeOuts );
		
		m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
		m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );


		DCB dcb_UART_IR;
		dcb_UART_IR.DCBlength=sizeof(DCB);
		BOOL cc =	GetCommState(m_hCom,&dcb_UART_IR);	
		dcb_UART_IR.BaudRate = CBR_19200;
		//dcb_UART_IR.fParity =TRUE;
		//dcb_UART_IR.fNull = FALSE;
		dcb_UART_IR.StopBits = ONESTOPBIT;
		dcb_UART_IR.Parity =  NOPARITY;
		dcb_UART_IR.ByteSize = 8;
		BOOL bb =SetCommState(m_hCom,&dcb_UART_IR);
		dw =GetLastError(); 

		if( !SetCommState( m_hCom, &dcb_UART_IR ) ||!SetupComm( m_hCom,10000,10000 ) ||m_OverlappedRead.hEvent == NULL ||m_OverlappedWrite.hEvent == NULL)
		{
			DWORD dwError = GetLastError();
			if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
			if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
			CloseHandle( m_hCom );
			return -1;
		}

	}
	else
	{
		m_hCom = CreateFile( devName, mode, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL );
		dw =GetLastError(); 
		if (m_hCom==(HANDLE)-1) {  AfxMessageBox('打开'+devName+'失败'); return false; }
	}
	for(i=0;i<3;i++)
	{
		if(HANDLE_BUF[i]==NULL) 
		{	
			HANDLE_BUF[i]=m_hCom;
			break;
		}
	}
	return i;
}
/***************************************************************************************************/

/***************************************************************************************************/
int HB_close(int devNo)
{
	HANDLE m_hCom;

	m_hCom = HANDLE_BUF[devNo];
	if(m_hCom == NULL) return FALSE;

	BOOL STATE = CloseHandle(m_hCom);
	HANDLE_BUF[devNo] = NULL;

	return STATE;
}
/***************************************************************************************************/

/***************************************************************************************************/
int HB_read(u8 *buffer, int size, int devNo)
{
	HANDLE m_hCom;

	m_hCom = HANDLE_BUF[devNo];
	if(m_hCom == NULL) return FALSE;

	DWORD dwBytesRead = (DWORD)size;
	//BOOL ReadStatus = ReadFile( m_hCom,buffer,dwBytesRead,&dwBytesRead,&m_OverlappedRead);
	BOOL ReadStatus = ReadFile( m_hCom,buffer,dwBytesRead,&dwBytesRead,NULL);
	if( !ReadStatus ){
		if( GetLastError() == ERROR_IO_PENDING ){
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return dwBytesRead;
		}
		return FALSE;
	}
	
	return dwBytesRead;

}
/***************************************************************************************************/

/***************************************************************************************************/
int HB_write(u8 *buffer, int size, int devNo)
{
	HANDLE m_hCom;
	
	m_hCom = HANDLE_BUF[devNo];
	if(m_hCom == NULL) return FALSE;
	
	BOOL bWriteStat;
	DWORD dwBytesWritten = (DWORD)size;
	//bWriteStat = WriteFile( m_hCom, buffer, dwBytesWritten, &dwBytesWritten, &m_OverlappedWrite );
	bWriteStat = WriteFile( m_hCom, buffer, dwBytesWritten, &dwBytesWritten, NULL );
	if( !bWriteStat)
	{
		if ( GetLastError() == ERROR_IO_PENDING ) {
			WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 );
			return dwBytesWritten;
		}
		return 0;
	}

	return dwBytesWritten;

}
/***************************************************************************************************/

/***************************************************************************************************/
u32 HB_Seek(u32 ADDR, int devNo)
{
	HANDLE m_hCom;
	u32  status;

	m_hCom = HANDLE_BUF[devNo];
	if(m_hCom == NULL) return FALSE;
	status = SetFilePointer(m_hCom,ADDR,0,FILE_BEGIN);
	return status;
}

/*-------------------------------------------------------------------
读取UART 接收到的数据
-------------------------------------------------------------------*/
#ifdef WIN32
s16 Remote_Uart_RxData(u8 *VP_DATA, s16 V_DATA_CNT)
{
	HANDLE m_hCom;
	
	m_hCom = HANDLE_BUF[devNo_UART];
	if(m_hCom == NULL) return FALSE;
	
	DWORD dwBytesRead = (DWORD)V_DATA_CNT;
	BOOL ReadStatus = ReadFile( m_hCom,VP_DATA,dwBytesRead,&dwBytesRead,&m_OverlappedRead);
	if( !ReadStatus ){
		if( GetLastError() == ERROR_IO_PENDING ){
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return (s16)dwBytesRead;
		}
		return FALSE;
	}
	
	return (s16)dwBytesRead;
}
#else
s16 Remote_Uart_RxData(u8 *VP_DATA, s16 V_DATA_CNT)
{

}
#endif
/*-------------------------------------------------------------------
通过UART 发送数据
-------------------------------------------------------------------*/
#ifdef WIN32
void  Remote_Uart_TxData(u8 *VP_DATA,s16 V_DATA_CNT)
{
//	s16	 status;
//	u8    buffer[2];
	HANDLE m_hCom;

	m_hCom = HANDLE_BUF[devNo_UART];
	if(m_hCom == NULL) return;
	BOOL bWriteStat;
	//while(status = Remote_Uart_RxData(buffer, 1));		//清空UART
	bWriteStat = PurgeComm(m_hCom,PURGE_RXCLEAR);		//清空UART
	bWriteStat = PurgeComm(m_hCom,PURGE_TXCLEAR);		//清空UART
	DWORD dwBytesWritten = (DWORD)V_DATA_CNT;
	bWriteStat = WriteFile( m_hCom, VP_DATA, dwBytesWritten, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat)
	{
		if ( GetLastError() == ERROR_IO_PENDING ) {
			WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 );
			//return dwBytesWritten;
		}
		//return 0;
	}
	
	//return dwBytesWritten;
	
}
#else
void  Remote_Uart_TxData(u8 *VP_DATA,s16 V_DATA_CNT)
{

}
#endif










