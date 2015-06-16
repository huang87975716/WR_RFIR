/***************************************************************************
========================================================================
Company	:	HBG      Copyright (c) 2011
Author		:	Kalvin				
版权信息	:	Copyright (c) 2011 HBG Hi Tech	
File		: 	ARC_DRV.h	
VER		:	1.0.0		
Development	: 	VC++ 6.0	
Data		:	2011-7-29
Customer	:	浩博高			
-------------------------History list-----------------------------------
*****************************************************************************/
#ifndef _REMOTE_ARC_USE_H
#define _REMOTE_ARC_USE_H
//===========================================================
// DEFINE  CONSTANT 
//===========================================================
#define  CON_MODE_MAX			4		//最大模式值，0~4共5种模式
//#define  CON_TEMP_AUTOMODE		25-16		//自动模式温度值
//#define  CON_TIME_MAX			12		//定时最长时间

//#define  CON_ADR_Group_MAX		1000-1		//实际码组数

//-----------------------------------------------------------//
//VarFlag 标志
//#define  VarFlag_KEYNOP		0x01	//00000001B	//没有按键按下标志
//#define  VarFlag_KEYPRO		0x02	//00000010B	//有按键按下标志
#define  VarFlag_DISP_L		0x04	//00000100B	//显示灯光信息标志
#define  VarFlag_USE_DISABLE	0x08	//00001000B	//操作失效标志,当这个标志位为1时，温度，模式，风向，风量不能调节，也不显示，只显示时间，定开
#define  VarFlag_SEARCH		0x10	//00010000B	//搜索状态标示
#define  VarFlag_SEND		0x20	//00100000B	//发码标志
#define  VarFlag_FLASH		0x40	//01000000B	//500 MS 
//#define  LIE_FLAG		0x80	//10000000B	//假数据标志
//TmClk_SetState	定时设置状态
#define  TmClk_Seting_NONE			0		//没有进入设置定时或时钟状态
#define  TmClk_SetState_TMOFF			01		//进入定关设置状态
#define  TmClk_SetState_TMON			02		//进入定开设置状态
//#define  TmClk_SetState_ClkMin			03		//进入时钟分钟设置状态
//#define  TmClk_SetState_ClkHr			04		//进入时钟小时设置状态
//外部BIN数据文件数据位置
#define  FILE_ARC_KEYEN_OFS		0x10
#define  FILE_ARC_SYSCHAT_OFS		0x11
#define  FILE_ARC_TIMECHAT_OFS		0x12
#define  FILE_ARC_TEMPBD_OFS		0x13
//=============================================================
// DEFINE  VARIABLE
//=============================================================
//u8 VarState_KeyPro;			//按键处理状态，没有按键按下时要清0        ；0：没有处理

extern u8 	VarFlag;
extern u8	ADR_TMP_BAK;		//温度备份
extern u8	ADR_FANLEV_BAK;		//风量备份
extern u8	TmClk_SetState;		//定时或时钟，设置状态
extern u8	TmMin;			//定时的分钟计数器
//=============================================================
// DEFINE  const
//=============================================================
extern const unsigned char MIN_CHANGE_TAB[7];
extern const u8 KEY_CHOICE_TAB[8];
/********************************************************************
函数定义
*********************************************************************/
void HEX_TO_DEC(u8 V_data,u8 *VP_one,u8 *VP_ten,u8 *VP_hundred);
void READ_TEMP_BOUND(u16 V_group_num,u8 mode,u8 *VP_temp_min,u8 *VP_temp_max);
u8 ARC_KEY_ENCHECK(u16 V_group_num,u8 key_val);
u8 READ_SYS_CHARACTER(u16 V_group_num);
u8 READ_TIME_CHARACTER(u16 V_group_num);
void ARC_USE_PRO(u16 V_group_num,u8 key_val);
void ARC_CLK_TM_UPDATA(u16 V_group_num,u8 V_sysck_hour,u8 V_sysck_min);
void ADR_VAR_INIT(u8 clk_hour,u8 clk_min);
void ADR_VAR_INIT_Search(u16 V_group_num);


#endif






