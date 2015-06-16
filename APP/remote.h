#ifndef _REMOTE_H_
#define _REMOTE_H_


//#define WIN32
/***  Type definition ***/

typedef char               s8;
typedef unsigned char      u8;
typedef short              s16;
typedef unsigned short     u16;
typedef int                s32;
typedef unsigned int  		 u32;

/***  General purpose defines ***/

#define FALSE   0
#define TRUE    1
#define OK      1
#define OFF     0
#define ON      1
#define CLR     0
#define SET     1
#define NULL	  0

#include "remote_UART_PRO.h"
#include "remote_URC_PRO.h"
#include "remote_urc_index.h"
#include "remote_ioctl.h"


#endif



