#ifndef _REMOTE_FILE_H_
#define _REMOTE_FILE_H_
#include "remote.h"

//#define REMOTE_FILE_ID_URC		0		//URC.BIN
//#define REMOTE_FILE_ID_ARC		1		//ARC.BIN

void HB_sleep(int time_ms);
void Remote_File_ReadData(u8 id, u32 V_DATA_ADDR,u8 *VP_DATA,u8 V_DATA_CNT);
int HB_open(char *devName, int mode);
int HB_close(int devNo);
int HB_read(u8 *buffer, int size, int devNo);
int HB_write(u8 *buffer, int size, int devNo);
u32 HB_Seek(u32 ADDR, int devNo);
s16 Remote_Uart_RxData(u8 *VP_DATA, s16 V_DATA_CNT);
void  Remote_Uart_TxData(u8 *VP_DATA,s16 V_DATA_CNT);
#endif

