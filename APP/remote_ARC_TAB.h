/***************************************************************************
*****************************************************************************/
#ifndef _REMOTE_ARC_TAB_h
#define _REMOTE_ARC_TAB_h
/****************************constant define********************************/
#define	TBOF_Auto_Mode	0 
#define	TBOF_DHMD_Mode	1 
/****************************constant define********************************/
#define	EnK_AFANDIR	0x01		//自动风向 
#define	EnK_SLEEP	0x02		// 睡眠   
#define	EnK_HEAT	0x04		// 电辅热 
#define	EnK_STRONG	0x08		// 强力   
#define	EnK_LIGHT	0x10		// 灯光   
#define	EnK_AIRCLEAR	0x20		// 空清/净化/健康 
#define	EnK_ECONOMIC	0x40		// 经济 
#define	EnK_MFANDIR	0x80		// 手动风向
/****************************constant define********************************/
#define	F_DonotClrScreen	0x10		//关机不关屏 
#define	F_DonotDisp	0x20		//只显示时间/定时/码组号，其它都不显示，针没屏，且遥控器显示与空调机不能做到一致的情况； 
#define	F_SleepNHours	0x40		//睡眠可设多少个小时 
#define	F_FANDIR_LR	0x80		//左右风向 显示有用  
/****************************constant define********************************/ 
#define	TM_MODE_MASK	0x3		//bit0~bit1		;程序用来识别位置（加数据时不用管这个定义） 
#define	CON_TM_MODE_OPPOSITE	0		//相对定时  
#define	CON_TM_MODE_ABSOLUTE	01		//绝对定时  
#define	CON_TM_MODE_FABSOLUTE	02		//假绝对定时（显示绝对发码相对）
#define	F_NoTunoffTime	0x4		//开关机不取消定时   
#define	TM_SET_MASK	0x70		//bit4~bit7	;程序用来识别位置（加数据时不用管这个定义）
#define	CON_TM_SET_ONE	0*0x10		//关机设定开，开机设定关 
#define	CON_TM_SET_TWO	1*0x10		//开关机定开定关可同时设 
#define	CON_TM_SET_TWO_PT	2*0x10		//开关机定开定关可同时设,定关在前确定后自动开机，定开在前确定后关机 
#define	CON_TM_SET_SENDKEY	3*0x10		//定时键一按键即发码 
#define	F_Disable_TMON	0x80		//没有定开   
#define	F_Disable_TMOFF	0x8		//没有定关       

/********************外部常量数组声名************************************/
extern const u8  GROUP_CHANG_Array[];
extern const u8 TEMP_Bound_Array[][2];
extern const u8 SYS_CHARACTER_Array[];
extern const u8 KEY_ENABLE_Array[];
extern const u8 TIME_CHARACTER_Array[];
extern const u8 BIT_ADR_MASK_TAB[];

#endif
