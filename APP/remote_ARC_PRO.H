/***************************************************************************
*****************************************************************************/
#ifndef _REMOTE_ARC_RPO_H
#define _REMOTE_ARC_RPO_H
/*****************************************************************************/
/*====================================================================		
//====================================================================
//====================================================================
//	一般性说明
//====================================================================
// 一些限制
// 1. 驱动数据区位操作目前不能超过256Bit
// 2. 变量区最大扩展到16B
// 3. 输出缓冲区做大为36B,如果有超过，需要特殊处理。
// 4. 定时时间变量（3个）在功能程序中要做倒计时，定时时间到要清定时状态变量（2个）
//====================================================================
//	ARC DAT CONSTANT DEFINE
//====================================================================*/
/*DRV DATA 驱动数据区 命令字*/
#define	CMD_DAT_FEnd		0xFF
#define	CMD_DAT_SET		0x10
#define	CMD_DAT_CHECK	0x20	//CMD_DAT_WCON		0x20
#define	CMD_DAT_VAROR		0x30
#define	CMD_DAT_VARWRITE	0x40
#define	CMD_DAT_VAR		0x50
#define	CMD_DAT_OR			0x60
#define	CMD_DAT_WRITE		0x70
#define	CMD_DAT_JMP		0x80
#define	CMD_DAT_JNZ		0x90
#define	CMD_DAT_JNB		0xA0
#define	CMD_DAT_CALL		0xB0
#define	CMD_DAT_SETB		0xC0
#define	CMD_DAT_CLRB		0xD0
#define	CMD_DAT_MOV		0xE0
#define	CMD_DAT_JUDGE		0xF0	//判断标志位，按照标志位取值
//====================================================================
#define	JUDGE_POWER_FLAG	0x10	//开关
#define	JUDGE_SLEEP_FLAG	0x20	//睡眠
#define	JUDGE_ELEHOT_FLAG	0x30	//电热
#define	JUDGE_TIME_FLAG	0x40		//有定开，有定关，无定时
#define	CLOCK_TEN_MIN		0x50	//时钟十分钟
#define	TIMEON_TEN_MIN		0x60	//定开十分钟
#define	TIMEOFF_TEN_MIN	0x70	//定关十分钟
//====================================================================
/*CMDFlag命令字设置参数*/
#define	CF_REV			0x01		//取反/反向（默认是递增，有此标志，表示递减）
#define	CF_SERIES		0x02		//连续（表格只放最少值和最大值）
#define	CF_SER			0x02		//连续(简)
#define	CF_16BITS		0x04		//表示数据表为16位数据，默认为8位
#define	CF_VERR			0x0FF	//控制变量数据超出范围
//==============================================================
/*CMD_DAT_CHECK	类型*/
#define	CT_SUM8			0x00		//按字节求和，CMD_DAT_CHECK+CT_SUM8
#define	CT_SUM4			0x01		//按4位（高低）求和
#define	CT_SUM4L		0x02		//按低4位求和
#define	CT_SUM4H		0x03		//按高4位求和
#define	CT_EOR8			0x04		//按字节异或
#define	CT_EOR4			0x05		//按4位（高低）异或
#define	CT_EOR4L		0x06		//按低4位异或
#define	CT_EOR4H		0x07		//按高4位异或
#define	CT_NUM0		0x08		//计算0的个数
//================================================================================================
/* 空调变量数组 ADR_VAR[]说明 */
#define	ADR_VAR_START	0x00				//空调变量的存放位置（需要根据程序修改）
#define	ADR_MODE		0		//模式------------》变化范围"0~4"对应模式详见"模式常量"定义
#define	ADR_TMP		1		//温度------------》变化范围"0~16"对应温度"16~32"
#define	ADR_FANLEV		2		//风量------------》变化范围"0~3"对应风量"自动，低，中，高"
#define	ADR_FANDIR		3		//风向------------》按自动风向键清0，按手动风向键加1（在1~8之间变化）（用于自动风向和手动风向数据在同一位置）
#define	ADR_AFANDIR		4		//自动风向----》按自动风向键改变（在0~8之间变化），按手动风向键不变（用于自动风向和手动风向数据在不同位置）
#define	ADR_MFANDIR	5		//手动风向----》按手动风向键改变（在0~8之间变化），按自动风向键不变（用于自动风向和手动风向数据在不同位置,或只有一个风向键的时候）
#define	ADR_CKHOUR		6		//时钟小时
#define	ADR_TIMEON		7		//定开时间（小时）----》变化范围"0~12"对应定开时间"0~12"小时（只要用于定开定关数据在不同位置的时候）
#define	ADR_TIMEOFF		8		//定关时间（小时）----》变化范围"0~12"对应定关时间"0~12"小时（只要用于定开定关数据在不同位置的时候）
#define	ADR_KEYVAL		9		//键值------------》详见"按键常量"定义,空键:0FFH
#define	ADR_SYSFLAG	10		//功能标志---》详见"功能标志"常量定义
#define	ADR_TMSTATE7	11		//定时状态（7种），详见"定时状态常量（7态）"
#define	ADR_TMSTATE3	12		//定时状态（3种），详见"定时状态常量（3态）"
//ADR_TM_MIN		EQU	13	//定时分钟
#define	ADR_CKMIN		13		//时钟分钟
//#define	ADR_TMON_10MINS	14		//定时开分钟
//#define	ADR_TMOFF_10MINS	15		//定时关分钟
//---------------------------------------------------------------------------------------------
//偏移地址大于15的的变量，只能用命令 "CMD_DAT_VAR" 读取
//#define	ADR_TMON_DISHOUR			16	//定开时间差小时
//#define	ADR_TMON_DISMIN			17	//定开时间差分钟
//#define	ADR_TMOFF_DISHOUR			18	//定关时间差小时
//#define	ADR_TMOFF_DISMIN			19	//定关时间差分钟
//注意：程序上电或更换码组"空调变量"都要重新初始化
//=========================================================================================
//... 其它可以补充
//ADR_VAR
#define	ADR_VAR_LEN		14	//20		//输入数据缓冲区在数组ADR_VAR中的起始位置
/*ARC 温度常量*/
#define	ARC_TMP_L		0		//最低温度为0+16摄氏度
#define	ARC_TMP_H		16		//最高温度为16+16摄氏度
/*ARC 模式常量 ADR_MODE*/
#define	ARC_MODE_AUTO	0		//自动
#define	ARC_MODE_COOL	1		//制冷
#define	ARC_MODE_DHMD	2		//抽湿
#define	ARC_MODE_FAN	3		//送风
#define	ARC_MODE_HEAT	4		//制热
/*ARC ADR_FANDIR风向常量*/
#define	ARC_FANDIR_1	0		//自动 ???
#define	ARC_FANDIR_2	1		//手动1
#define	ARC_FANDIR_3	2		//手动2
#define	ARC_FANDIR_4	3		//手动3
#define	ARC_FANDIR_5	4		//手动4
#define	ARC_FANDIR_6	5		//手动5
#define	ARC_FANDIR_7	6		//手动6
#define	ARC_FANDIR_8	7		//手动7
#define	ARC_FANDIR_9	8		//手动8
#define	ARC_FANDIR_10	9		//手动9
/*ARC  ADR_AFANDIR 自动风向常量*/
#define	ARC_AFANDIR_1	0		//自动风向1
#define	ARC_AFANDIR_2	1		//自动风向2
#define	ARC_AFANDIR_3	2		//自动风向3
#define	ARC_AFANDIR_4	3		//自动风向4
#define	ARC_AFANDIR_5	4		//自动风向5
#define	ARC_AFANDIR_6	5		//自动风向6
#define	ARC_AFANDIR_7	6		//自动风向7
#define	ARC_AFANDIR_8	7		//自动风向8
#define	ARC_AFANDIR_9	8		//自动风向9
#define	ARC_AFANDIR_10	9		//自动风向10
/*ARC  ADR_MFANDIR 手动风向常量*/
#define	ARC_MFANDIR_1	0		//手动风向1
#define	ARC_MFANDIR_2	1		//手动风向2
#define	ARC_MFANDIR_3	2		//手动风向3
#define	ARC_MFANDIR_4	3		//手动风向4
#define	ARC_MFANDIR_5	4		//手动风向5
#define	ARC_MFANDIR_6	5		//手动风向6
#define	ARC_MFANDIR_7	6		//手动风向7
#define	ARC_MFANDIR_8	7		//手动风向8
#define	ARC_MFANDIR_9	8		//手动风向9
#define	ARC_MFANDIR_10	9		//手动风向10

/*ARC ADR_FANLEV 风量常量*/
#define	ARC_FANLEV_1	0		//风量1 自动
#define	ARC_FANLEV_2	1		//风量2 低风
#define	ARC_FANLEV_3	2		//风量3 中风
#define	ARC_FANLEV_4	3		//风量4 高风

/*ARC 开关机常量*/
#define	ARC_POWER_ON		0
#define	ARC_POWER_OFF		1

/*ARC (ADR_KEYVAL)按键常量*/
#define	DAT_KEY_NUM_MAX		0x10
#define	ARC_NO_KEY			0x0FF	//空键
#define	ARC_KEY_POWER		0	//电源
#define	ARC_KEY_TUP			1	//温度+
#define	ARC_KEY_TDOWN		2	//温度-
#define	ARC_KEY_MODE		3	//模式
#define	ARC_KEY_FANLEV		4	//风量
#define	ARC_KEY_AFANDIR		5	//自动风向
#define	ARC_KEY_MFANDIR		6	//手动风向
#define	ARC_KEY_TIMEON		7	//定开
#define	ARC_KEY_TIMEOFF		8	//定关
//ARC_KEY_TIME		EQU	7	//定时设置
#define	ARC_KEY_TIMECL		9	//取消定时
#define	ARC_KEY_SLEEP		10	//睡眠
#define	ARC_KEY_HEAT		11	//电辅热
#define	ARC_KEY_STRONG		12	//强力
#define	ARC_KEY_LIGHT		13	//灯光
#define	ARC_KEY_AIRCLEAR	14	//空清
#define	ARC_KEY_ECONOMIC	15	//经济
#define	ARC_KEY_CLOCK		0x10		//时钟

/*ADR_SYSFLAG(功能标志)常量定义*/
#define	SYSFLAG_POWER	0x01	//00000001B		//开关机标志----》开关=1，关机=0
#define	ADSF_POWER		0				//开关机标志位置
#define	SYSFLAG_SLEEP	0x02	//00000010B		//睡眠标志--------》睡眠开=1，睡眠关=0
#define	ADSF_SLEEP		1				//睡眠标志位置
#define	SYSFLAG_HEAT	0x04	//00000100B		//电辅热标志----》电辅热开=1，电辅热关=0
#define	ADSF_HEAT		2				//电辅热标志位置
#define	SYSFLAG_TIME	0x08	//00001000B		//定时标志-------》 有定时= 1，没有定时 = 0
#define	ADSF_TIME		3				//定时标志位置
#define	SYSFLAG_STRONG	 0x10	//00010000B		//强力标志-------》 强力开= 1，强力关 = 0
#define	ADSF_STRONG		4				//强力标志位置
#define	SYSFLAG_LIGHT	0x20	//00100000B		//灯光标志-------》 灯光开= 1，灯光关 = 0
#define	ADSF_LIGHT		5				//灯光标志位置
#define	SYSFLAG_AIRCLEAR	0x40	//01000000B		//空清/净化/健康标志-------》 空清/净化/健康开= 1，空清/净化/健康关 = 0
#define	ADSF_AIRCLEAR	6				//空清/净化/健康标志位置
#define	SYSFLAG_ECONOMIC	0x80	//10000000B		//经济标志-------》 经济开= 1，经济关 = 0
#define	ADSF_ECONOMIC	7				//经济标志位置


/*ARC ADR_TMSTATE7定时状态常量（7态）*/
#define	ARC_T7_NOTIME		0	//没有定时
#define	ARC_T7_TIMEONP		1	//打开定开
#define	ARC_T7_TIMEOFFP		2	//打开定关
#define	ARC_T7_TIMEONW		3	//等待定开状态
#define	ARC_T7_TIMEOFFW		4	//等待定关状态
#define	ARC_T7_TIMEONCL		5	//取消定开
#define	ARC_T7_TIMEOFFCL	6	//取消定关
/*ADR_TMSTATE3 定时状态常量（3态）*/
#define	ARC_T3_NOTIME		0		//没有定时
#define	ARC_T3_TIMEON		1		//有定开
#define	ARC_T3_TIMEOFF		2		//有定关
#define	ARC_T3_TIMEONOFF	3		//同时有定开定关
//------------------------------------------------------------------------------
/*DAT PRO程序常量*/
#define	READ_AGAIN		0x55	//重新读数据
#define	READ_NEXT		0x0		//读下一个数据
/*=========================================================================
 MACROS DEFINE
 ==========================================================================*/

//#define	 DP_DAT_DES_OFS_Read()	{DataBuf[DP_Dat_Head+DP_Dat_Des_OFS];}
/*=========================================================================
ARC DAT PUBLIC FUNCTION & VAR
==========================================================================*/
/*=========================================================================
variable announce
==========================================================================*/
extern u8	ADR_VAR[ADR_VAR_LEN];		//控制变量数组
extern u8 	CMDFlag;		//命令,命令附加标志
extern u16	DP_Dat_Head,Wdata16;
extern u16	DP_Dat_Des_OFS;
extern u8 ADR_FANLEV_BAK,ADR_TMP_BAK;


/*********************variable define*********************************************/

/******************************************************************************/
/*=========================================================================
define const
==========================================================================*/


#endif


