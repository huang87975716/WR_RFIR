/***************************************************************************
;========================================================================
;Company	:	HBG      Copyright (c) 2011
;Author		:	Kalvin
;版权信息	:	Copyright (c) 2011 HBG Hi Tech
;File		: 	ARC_DAT_PRO.C
;Development	: 	VC++ 6.0
;Data		:	2012-11
;Customer	:	浩博高
;-------------------------History list-----------------------------------
*****************************************************************************
*****************************************************************************/
#include "remote.h"
/*****************************************************************************
		const define
*****************************************************************************/
const u8 MIN_CHANGE_TAB[]={0,10,20,30,40,50,60};		//分钟转换
//按键使能表
//bit0:自动风向，bit1:睡眠，bit2:电辅热，bit3:强力，bit4:灯光，bit5:空清/净化/健康,bit6:经济,bit7:手动风向
const u8 KEY_CHOICE_TAB[]={ARC_KEY_AFANDIR,ARC_KEY_SLEEP,ARC_KEY_HEAT,ARC_KEY_STRONG,ARC_KEY_LIGHT,ARC_KEY_AIRCLEAR,ARC_KEY_ECONOMIC,ARC_KEY_MFANDIR};
/*****************************************************************************
   		variable  define
*****************************************************************************/
//u8 VarState_KeyPro;			//按键处理状态，没有按键按下时要清0        ；0：没有处理
u8 	VarFlag;
u8	ADR_TMP_BAK;		//温度备份
u8	ADR_FANLEV_BAK;		//风量备份
u8	TmClk_SetState;		//定时或时钟，设置状态
u8	TmMin;			//定时的分钟计数器
u8	ADR_VAR[ADR_VAR_LEN];		//控制变量数组
u8   ARC_TIMEOFF_BAK,ARC_TIMEON_BAK;
extern type_updatatime ARC_updataTM;
extern int	 devNo_FARC;
/*******************************************************************************
   		声名
*****************************************************************************/
//extern const unsigned int TEMP_Bound_Array[][2];
//extern const unsigned int SYS_CHARACTER_Array[];
//extern const unsigned int KEY_ENABLE_Array[];
//extern const unsigned int TIME_CHARACTER_Array[];
//extern 	const u16  ARC_FILE_ADDR[];
//;=========================================================
//;发码标志
//;input: V_Skey_val
//;output:VarFlag,ADR_VAR[]
//;=========================================================

void SET_VarFlag_SEND(u8 V_Skey_val)
{
	VarFlag=VarFlag|VarFlag_SEND;
	ADR_VAR[ADR_VAR_START+ADR_KEYVAL]=V_Skey_val;

}

/*========================================
;十六进制转十进制		
;input:	ACC
;output:	VarTemp0(个位),VarTemp1(十位),VarTemp2(百位)
;========================================*/
void HEX_TO_DEC(u8 V_data,u8 *VP_one,u8 *VP_ten,u8 *VP_hundred)
{
u8 V_ten,V_hundred;

	V_hundred=0;
	V_ten=0;
	
	while(V_data>=100)
	{
		V_data=V_data-100;
		V_hundred++;
	}
	while(V_data>=10)
	{
		V_data=V_data-10;
		V_ten++;
	}
	*VP_hundred=V_hundred;
	*VP_ten=V_ten;
	*VP_one=V_data;
}

/*==========================================================
; 读取系统特征表（显示+睡眠 方式标志）
;input: none
;output: A
;===========================================================*/
u8 READ_SYS_CHARACTER(u16 V_group_num)
{
u8   V_TEMP1,V_FILE_ID;
u32	V_DATA_ADDR;
//int	r_size;

	if(V_group_num >= EXT_FILE_DATA)
	{	
		V_FILE_ID = V_group_num&(~EXT_FILE_DATA);
		V_DATA_ADDR = arc_exdata_addr[V_FILE_ID];
		Remote_File_ReadData(devNo_FARC,(V_DATA_ADDR+FILE_ARC_SYSCHAT_OFS),&V_TEMP1,1);

	}
	else
		V_TEMP1=SYS_CHARACTER_Array[V_group_num];



	return V_TEMP1;
}

/*==========================================================
; 读取定时特征表（定时方式，定时设置）
;input: none
;output: A
;===========================================================*/
u8 READ_TIME_CHARACTER(u16 V_group_num)
{
u8   V_TEMP1,V_FILE_ID;
u32	V_DATA_ADDR;
//int		r_size;

	if(V_group_num >= EXT_FILE_DATA)
	{	
		V_FILE_ID = V_group_num&(~EXT_FILE_DATA);
		V_DATA_ADDR = arc_exdata_addr[V_FILE_ID];
		Remote_File_ReadData(devNo_FARC,(V_DATA_ADDR+FILE_ARC_TIMECHAT_OFS),&V_TEMP1,1);
	}
	else
	V_TEMP1=TIME_CHARACTER_Array[V_group_num];
	
	return V_TEMP1;
}
/*===========================================================*/
void	JUDGE_POWER_ONOFF(u16 V_group_num)
{
u8 V_TEMP1,V_TEMP2,V_TEMP3;

	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
	V_TEMP1=V_TEMP1&TM_SET_MASK;
	if(V_TEMP1==CON_TM_SET_TWO_PT)
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		if(V_TEMP2==ARC_T3_TIMEON)
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3&(~SYSFLAG_POWER);
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
		else if(V_TEMP2==ARC_T3_TIMEOFF)
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3|SYSFLAG_POWER;
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
	}
}

/*===========================================================
;电源键处理
;说明:	1.开或关机
;	2.取消定时
;input:	ADR_SYSFLAG,ADR_TMSTATE7,ADR_TMSTATE3
;output:	ADR_SYSFLAG,ADR_TMSTATE7,ADR_TMSTATE3
;===========================================================*/
/*-----------------清除定时设置----------------------------*/
void	CLR_TIME_SETTING(void)
{
u8	V_TEMP1;

	if(TmClk_SetState==TmClk_SetState_TMON)
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE7];
		V_TEMP1=V_TEMP1&(~ARC_T7_TIMEONP);		//清打开定开标志
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=V_TEMP1;
		
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		V_TEMP1=V_TEMP1&(~ARC_T3_TIMEON);		//清有定开标志
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
		
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP1=V_TEMP1&(~SYSFLAG_TIME);		//清有定时标志
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
		
		ADR_VAR[ADR_VAR_START+ADR_TIMEON] = 0;
		TmClk_SetState=0;
	}
	else if(TmClk_SetState==TmClk_SetState_TMOFF)
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE7];
		V_TEMP1=V_TEMP1&(~ARC_T7_TIMEOFFP);		//清打开定开标志
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=V_TEMP1;
		
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		V_TEMP1=V_TEMP1&(~ARC_T3_TIMEOFF);		//清有定开标志
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
		
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP1=V_TEMP1&(~SYSFLAG_TIME);		//清有定时标志
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
		
		ADR_VAR[ADR_VAR_START+ADR_TIMEOFF] = 0;
		TmClk_SetState=0;
	}

	
}
/***********************************************************
-----------取消定时开--------------------------------
***********************************************************/
void USE_TIMEON_CANCEL(u16 V_group_num,u8 key_val)
{
u8 V_TEMP1,V_TEMP2;

	ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEONCL;	//取消定开
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	V_TEMP1=V_TEMP1&(~ARC_T3_TIMEON);
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
	if(V_TEMP1==0)
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2&(~SYSFLAG_TIME);		//clr 定时标志
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	key_val=ARC_KEY_TIMECL;
/*	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
	V_TEMP1=V_TEMP1&TM_SET_MASK;
	if(V_TEMP1==CON_TM_MODE_FABSOLUTE)
	{	//显示为绝对时间,发码为相对时间（时间差）
		ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=0;
		ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR]=0;
		ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN]=0;
	}
	else if(V_TEMP1!=CON_TM_MODE_ABSOLUTE)*/
//	{	//清相对定时开时间
		ADR_VAR[ADR_VAR_START+ADR_TIMEON]=0;
//	}
	JUDGE_POWER_ONOFF(V_group_num);
	SET_VarFlag_SEND(key_val);
}
/**********************************************************
;-----------取消定时关--------------------------------
***********************************************************/
void USE_TIMEOFF_CANCEL(u16 V_group_num,u8 key_val)
{
u8 V_TEMP1,V_TEMP2;

	ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFCL;
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	V_TEMP1=V_TEMP1&(~ARC_T3_TIMEOFF);		//取消定关
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
	if(V_TEMP1==0)
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2&(~SYSFLAG_TIME);	//clr 定时标志
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	key_val=ARC_KEY_TIMECL;
/*	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
	V_TEMP1=V_TEMP1&TM_SET_MASK;
	if(V_TEMP1==CON_TM_MODE_FABSOLUTE)
	{  //显示为绝对时间,发码为相对时间（时间差）
	
		ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=0;
		ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR]=0;
		ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN]=0;
	}
	else if(V_TEMP1!=CON_TM_MODE_ABSOLUTE)*/
//	{	//清相对定时关时间
		ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;
//	}
	JUDGE_POWER_ONOFF(V_group_num);
	SET_VarFlag_SEND(key_val);
}

/*--------------------------------------------------------------------*/
void USE_POWER_KEY_PRO(u16 V_group_num,u8 key_val)
{
u8	V_TEMP1,V_TEMP2,V_TEMP3;

//	if(VarState_KeyPro!=0) return;
//
//	VarState_KeyPro++;
	CLR_TIME_SETTING();
	V_TEMP3=READ_SYS_CHARACTER(V_group_num);
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if(((V_TEMP1&SYSFLAG_SLEEP)!=0)&&((V_TEMP3&F_SleepNHours) !=0))ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;
	V_TEMP1=V_TEMP1&(~(SYSFLAG_STRONG+SYSFLAG_ECONOMIC+SYSFLAG_HEAT+SYSFLAG_SLEEP+SYSFLAG_AIRCLEAR));
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
	key_val=ARC_KEY_POWER;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if((V_TEMP1&SYSFLAG_POWER)==0)
	{
		V_TEMP1=V_TEMP1|SYSFLAG_POWER;				//开机
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
		V_TEMP1=READ_TIME_CHARACTER(V_group_num);
		if((V_TEMP1&F_NoTunoffTime)!=0)
		{
			SET_VarFlag_SEND(key_val);
			return;
			
		}
		
	}
	else
	{
		V_TEMP1=V_TEMP1&(~SYSFLAG_POWER);				//关机
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
		V_TEMP1=READ_TIME_CHARACTER(V_group_num);
		if((V_TEMP1&F_NoTunoffTime)!=0)
		{
			SET_VarFlag_SEND(key_val);
			return;
		}
	}
	//Cancel TIME
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if((V_TEMP1&ARC_T3_TIMEOFF)!=0) USE_TIMEOFF_CANCEL(V_group_num,key_val);	//取消定时关
	if((V_TEMP1&ARC_T3_TIMEON)!=0) USE_TIMEON_CANCEL(V_group_num,key_val);	//取消定时开

	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if(((V_TEMP1&F_DonotClrScreen)==0)&&((V_TEMP2&SYSFLAG_POWER)==0))
		VarFlag=VarFlag|VarFlag_USE_DISABLE;
	else	VarFlag=VarFlag&(~VarFlag_USE_DISABLE);

	key_val=ARC_KEY_POWER;
	SET_VarFlag_SEND(key_val);
}

/*===========================================================
;读取自动，除湿 模式的温度范围
;input: ADR_MODE
;output:IR_DRV_vtmp0(最小值),IR_DRV_vtmp1(最大值)
;===========================================================*/
void READ_TEMP_BOUND(u16 V_group_num,u8 mode,u8 *VP_temp_min,u8 *VP_temp_max)
{
u8   V_TEMP1,V_tempmax,V_tempmin,V_FILE_ID;
u32	V_DATA_ADDR;
//int		r_size;
	if(V_group_num >= EXT_FILE_DATA)
	{	
		V_FILE_ID = V_group_num&(~EXT_FILE_DATA);
		V_DATA_ADDR = arc_exdata_addr[V_FILE_ID];
		if(mode==ARC_MODE_AUTO)
		{
			Remote_File_ReadData(devNo_FARC,(V_DATA_ADDR+FILE_ARC_TEMPBD_OFS),&V_TEMP1,1);
		}
		else	 
		{
			Remote_File_ReadData(devNo_FARC,(V_DATA_ADDR+FILE_ARC_TEMPBD_OFS+1),&V_TEMP1,1);
		}
	}
	else
	{
		if(mode==ARC_MODE_AUTO)
			V_TEMP1=TEMP_Bound_Array[V_group_num][0];
		else	V_TEMP1=TEMP_Bound_Array[V_group_num][1];
	}
	
	V_tempmax=V_TEMP1>>4;	//最大值
	V_tempmin=V_TEMP1&0x0f;	//最小值
			
	*VP_temp_max=V_tempmax;
	*VP_temp_min=V_tempmin;
	
}

/*===========================================================
;温度加处理
;说明:;1.每按一次温度减1,长安连加
;	  2.送风模式温度固定为25度
;	  3.制冷，制暖 温度变量的值范围0~16表示16~32摄氏度
;	  4.自动，除湿模式，在温度范围表"TEMP_MAX_TAB"控制的范围内调节
;input:	ADR_TMP,ADR_MODE
;output:	ADR_TMP
;===========================================================*/
void	USE_TEMPUP_KEY_PRO(u16 V_group_num,u8 key_val)
{
u8	V_TEMP1,V_TEMP2,V_Temperat_Min,V_Temperat_Max;

//	if((VarState_KeyPro==1)&&(keydn_tm_cnt<CON_DELAY_1S))
//		return;
//	if((VarState_KeyPro>1)&&(keydn_tm_cnt<CON_DELAY_200MS))
//		return;
//	if((VarState_KeyPro==1)||(VarState_KeyPro==0))
//		VarState_KeyPro++;
	
//	keydn_tm_cnt=0;
	switch(TmClk_SetState)
	{
		case	TmClk_Seting_NONE:
			if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
			V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_MODE];
			if(V_TEMP1==ARC_MODE_FAN)  return;
			if(V_TEMP1==ARC_MODE_HEAT)
			{
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMP];
				if(V_TEMP1<32)
				{
					V_TEMP1++;
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP1;
					ADR_TMP_BAK=V_TEMP1;	//备份温度
				}
			}
			else if((V_TEMP1==ARC_MODE_AUTO)||(V_TEMP1==ARC_MODE_DHMD))
			{
			//	if((SYSTEMFLAG&GREENSEL)==0)
			//	{
			//		V_Temperat_Max=GREEN_TEMPH;
			//		V_Temperat_Min=GREEN_TEMPL;
			//	}
			//	else
			//	{
					V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
					READ_TEMP_BOUND(V_group_num,V_TEMP2,&V_Temperat_Min,&V_Temperat_Max);	//*/
			//	}
				if(V_Temperat_Min==V_Temperat_Max)
				{
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_Temperat_Min;
					return;
				}
				else
				{
					V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMP];
					if(V_TEMP2>=V_Temperat_Max)
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_Temperat_Max;
					else 
					{
						V_TEMP2++;
						ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
					}
					if(V_Temperat_Max>=(26-16))
					ADR_TMP_BAK=ADR_VAR[ADR_VAR_START+ADR_TMP];	//备份温度
				}
				
			}
			else
			{
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMP];
				if(V_TEMP2<16)	//32-16
				{
					V_TEMP2++;
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
					ADR_TMP_BAK=ADR_VAR[ADR_VAR_START+ADR_TMP];	//备份温度
				}
			
			}
			SET_VarFlag_SEND(key_val);
			break;
			
		//调整定时间/时钟
		case	TmClk_SetState_TMOFF:		//定关调节+
			//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
			//V_TEMP1=V_TEMP1&TM_MODE_MASK;
			//if(V_TEMP1==CON_TM_MODE_OPPOSITE)
			//{	//相对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
				if(V_TEMP2<12) 
				{
					V_TEMP2++;
					ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
				}
				//return;
			//}
/*			else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
			{	//显示为绝对时间,发码也为绝对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
				V_TEMP2++;
				ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP2;
				if(V_TEMP2>=6)
				{
					V_TEMP2=0;
					ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP2;
				
					V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
					V_TEMP2++;
					ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
					if(V_TEMP2>=24)
					{	
						V_TEMP2=0;
						ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
					}
				}
				//return;
			}
			else
			{	//显示为绝对时间,发码为相对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
				V_TEMP2++;
				if(V_TEMP2>=144)	//24*6=144
					V_TEMP2=0;
				ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
				
				//return;
			}
*/			
			break;
		case	TmClk_SetState_TMON:		//定开调节+
			//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
			//V_TEMP1=V_TEMP1&TM_MODE_MASK;
			//if(V_TEMP1==CON_TM_MODE_OPPOSITE)
			//{	//相对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
				if(V_TEMP2<12) 
				{
					V_TEMP2++;
					ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
				}
				//return;
			//}
/*			else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
			{	//显示为绝对时间,发码也为绝对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
				V_TEMP2++;
				ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP2;
				if(V_TEMP2>=6)
				{
					V_TEMP2=0;
					ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP2;
				
					V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
					V_TEMP2++;
					ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
					if(V_TEMP2>=24)
					{	
						V_TEMP2=0;
						ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
					}
				}
				//return;
			}
			else
			{	//显示为绝对时间,发码为相对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
				V_TEMP2++;
				if(V_TEMP2>=144)	//24*6=144
					V_TEMP2=0;
				ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
				
				//return;
			}
*/		
			break;
/*		case	TmClk_SetState_ClkMin:
		
			break;
		case	TmClk_SetState_ClkHr:
		
			break;
*/	
		default:
			break;
		
	}
	return;
}
	
/*;===========================================================
;温度减处理
;说明:1.每按一次温度减1,长安连减
;	  2.送风模式温度固定为25度
;	  3.制冷，制暖 温度变量的值范围0~16表示16~32摄氏度
;	  4.自动，除湿模式，在温度范围表"TEMP_MAX_TAB"控制的范围内调节
;input:	ADR_TMP,ADR_MODE
;output:	ADR_TMP
;===========================================================*/
void USE_TEMPDN_KEY_PRO(u16 V_group_num,u8 key_val)
{
u8	V_TEMP1,V_TEMP2,V_Temperat_Min,V_Temperat_Max;

//	if((VarState_KeyPro==1)&&(keydn_tm_cnt<CON_DELAY_1S))
//		return;
//	if((VarState_KeyPro>1)&&(keydn_tm_cnt<CON_DELAY_200MS))
//		return;
//	if((VarState_KeyPro==1)||(VarState_KeyPro==0))
//		VarState_KeyPro++;
	
//	keydn_tm_cnt=0;
	switch(TmClk_SetState)
	{
		case	TmClk_Seting_NONE:
			if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
			V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_MODE];
			if(V_TEMP1==ARC_MODE_FAN)  return;
			if(V_TEMP1==ARC_MODE_HEAT)
			{
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMP];
				if(V_TEMP1>0)	
				{
					V_TEMP1--;
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP1;
					ADR_TMP_BAK=V_TEMP1;	//备份温度
				}
			}
			else if((V_TEMP1==ARC_MODE_AUTO)||(V_TEMP1==ARC_MODE_DHMD))
			{
//				if((SYSTEMFLAG&GREENSEL)==0)
//				{
//					V_Temperat_Max=GREEN_TEMPH;
//					V_Temperat_Min=GREEN_TEMPL;
//				}
//				else
//				{
					V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
					READ_TEMP_BOUND(V_group_num,V_TEMP2,&V_Temperat_Min,&V_Temperat_Max);	//*/
//				}
				if(V_Temperat_Min==V_Temperat_Max)
				{
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_Temperat_Min;
					return;
				}
				else
				{
					V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMP];
					if(V_TEMP2<=V_Temperat_Min)
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_Temperat_Min;
					else 
					{
						V_TEMP2--;
						ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
					}
					if(V_Temperat_Max>=(26-16))
					ADR_TMP_BAK=ADR_VAR[ADR_VAR_START+ADR_TMP];	//备份温度
				}
				
			}
			else
			{
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMP];
				if(V_TEMP2>0)
				{
					V_TEMP2--;
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
					ADR_TMP_BAK=ADR_VAR[ADR_VAR_START+ADR_TMP];	//备份温度
				}
			
			}
			SET_VarFlag_SEND(key_val);
			break;
			
		//调整定时间/时钟
		case	TmClk_SetState_TMOFF:		//定关调节-
			TmMin=0;
			//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
			//V_TEMP1=V_TEMP1&TM_MODE_MASK;
			//if(V_TEMP1==CON_TM_MODE_OPPOSITE)
			//{	//相对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
				if(V_TEMP2>1) 
				{
					V_TEMP2--;
					ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
				}
				//return;
			//}
/*			else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
			{	//显示为绝对时间,发码也为绝对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
				if(V_TEMP2==0)
				{
					V_TEMP2=5;
					ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP2;
				
					V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];

					if(V_TEMP2==0)
						V_TEMP2=23;
					else
						V_TEMP2--;
					ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
				}
				else
				{
					V_TEMP2--;
					ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP2;
				}
				//return;
			}
			else
			{	//显示为绝对时间,发码为相对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
				if(V_TEMP2==0)
					V_TEMP2=143;	//24*6=144
				else
					V_TEMP2--;
				ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
				
				//return;
			}
			*/
			break;
		case	TmClk_SetState_TMON:		//定开调节-
			TmMin=0;
			//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
			//V_TEMP1=V_TEMP1&TM_MODE_MASK;
			//if(V_TEMP1==CON_TM_MODE_OPPOSITE)
			//{	//相对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
				if(V_TEMP2>1) 
					V_TEMP2--;
				//else
				//	V_TEMP2=23;
					ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
	
				//return;
			//}
/*			else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
			{	//显示为绝对时间,发码也为绝对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
				if(V_TEMP2==0)
				{
					V_TEMP2=5;
					ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP2;
				
					V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];

					if(V_TEMP2==0)
						V_TEMP2=23;
					else
						V_TEMP2--;
					ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
				}
				else
				{
					V_TEMP2--;
					ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP2;
				}
				//return;
			}
			else
			{	//显示为绝对时间,发码为相对时间
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
				if(V_TEMP2==0)
					V_TEMP2=143;	//24*6=144
				else
					V_TEMP2--;
				ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
				
				//return;
			}
	*/	
			break;
/*		case	TmClk_SetState_ClkMin:
		
			break;
		case	TmClk_SetState_ClkHr:
			
			break;
*/		
		default:
			break;
		
	}
	return;
}
/*===========================================================
;模式处理
;说明:1.每按一次模式加1,五种模式循环
;	  2.每种模的温度值,风量值是否要独立保存??
;input:	ADR_MODE
;output:	ADR_TMP,ADR_MODE
;===========================================================*/
void USE_MODE_KEY_PRO(u16 V_group_num,u8 key_val)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3,V_Temperat_Min,V_Temperat_Max;

	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	
	ADR_VAR[ADR_VAR_START+ADR_TMP]=ADR_TMP_BAK;	//恢复设定温度
	ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ADR_FANLEV_BAK;
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	V_TEMP1=V_TEMP1&(~(SYSFLAG_HEAT+SYSFLAG_STRONG+SYSFLAG_ECONOMIC));	//取消电辅热，强力，经济
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_MODE];
	if(V_TEMP1>=CON_MODE_MAX)
	{
		V_TEMP1=ARC_MODE_AUTO;
		ADR_VAR[ADR_VAR_START+ADR_MODE]=V_TEMP1;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
		READ_TEMP_BOUND(V_group_num,V_TEMP2,&V_Temperat_Min,&V_Temperat_Max);	//*/
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMP];
		if((V_TEMP2<V_Temperat_Min)||(V_TEMP2>V_Temperat_Max))
		{
			V_TEMP2=(V_Temperat_Min+V_Temperat_Max)/2;	//中间温度值
			ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
		}
		
	}
	else
	{
		V_TEMP1++;
		ADR_VAR[ADR_VAR_START+ADR_MODE]=V_TEMP1;
		if(V_TEMP1==ARC_MODE_FAN)
		{	//送风模式
			ADR_VAR[ADR_VAR_START+ADR_TMP]=25-16;	//20120901
			if(ADR_FANLEV_BAK==ARC_FANLEV_1)
				ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ARC_FANLEV_2;
			else
				ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ADR_FANLEV_BAK;
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP2=V_TEMP2&(~SYSFLAG_SLEEP);			//送风 模式清睡眠
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;

		}
		else if(V_TEMP1==ARC_MODE_DHMD)
		{	//除湿模式
			V_TEMP3=READ_SYS_CHARACTER(V_group_num);
			ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ARC_FANLEV_2;		//低风量
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			if(((V_TEMP2&SYSFLAG_SLEEP)!=0)&&((V_TEMP3&F_SleepNHours)!=0))  ADR_VAR[ADR_VAR_START+ADR_TIMEOFF] = 0;
			V_TEMP2=V_TEMP2&(~SYSFLAG_SLEEP);			//除湿 模式清睡眠
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
		
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
			READ_TEMP_BOUND(V_group_num,V_TEMP2,&V_Temperat_Min,&V_Temperat_Max);	//*/
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMP];
			if((V_TEMP2<V_Temperat_Min)||(V_TEMP2>V_Temperat_Max))
			{
				V_TEMP2=(V_Temperat_Min+V_Temperat_Max)/2;	//中间温度值
				ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
			}
			
			
		}
	}
	SET_VarFlag_SEND(key_val);
	//return;
}

/*===========================================================
;风量处理
;说明:1.每按一次风量值加1,四种风量循环
;input:	ADR_FANLEV
;output:	ADR_FANLEV
;===========================================================*/
void USE_FANLEV_KEY_PRO(u8 key_val)
{
u8   V_TEMP1,V_TEMP2;

	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_FANLEV];
	if(V_TEMP1<ARC_FANLEV_4)
	{
		V_TEMP1++;
		ADR_VAR[ADR_VAR_START+ADR_FANLEV]=V_TEMP1;
		ADR_FANLEV_BAK=V_TEMP1;
	}
	else
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
		if(V_TEMP2!=ARC_MODE_FAN)
		{
			ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ARC_FANLEV_1;
			ADR_FANLEV_BAK=ARC_FANLEV_1;
		}
		else
		{	//送风模式	低、中、高
			
			ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ARC_FANLEV_2;
			ADR_FANLEV_BAK=ARC_FANLEV_2;
		}
	
	}
	SET_VarFlag_SEND(key_val);
}
	
/*===========================================================
;手动风向处理
;说明:1.每按一次ADR_MFANDIR,ADR_FANDIR值加1,九种风向循环
;input:	ADR_FANDIR,ADR_MFANDIR
;output:	ADR_FANDIR,ADR_MFANDIR
;===========================================================*/
void USE_MFANDIR_KEY_PRO(u8 key_val)
{
u8   V_TEMP1;

	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_FANDIR];
	if(V_TEMP1>=ARC_FANDIR_10)
		V_TEMP1=ARC_FANDIR_2;
	else
		V_TEMP1++;
	ADR_VAR[ADR_VAR_START+ADR_FANDIR]=V_TEMP1;
		
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_MFANDIR];
	if(V_TEMP1<ARC_MFANDIR_10)
		V_TEMP1++;
	else
		V_TEMP1=ARC_MFANDIR_1;
	ADR_VAR[ADR_VAR_START+ADR_MFANDIR]=V_TEMP1;
	
	SET_VarFlag_SEND(key_val);
}
/*===========================================================
;自动风向处理
;说明:1.每按一次ADR_AFANDIR值加1,九种风向循环,ADR_FANDIR清零
;input:	ADR_AFANDIR
;output:	ADR_FANDIR,ADR_AFANDIR
;===========================================================*/
void USE_AFANDIR_KEY_PRO(u8 key_val)
{
u8   V_TEMP1;

	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;

	ADR_VAR[ADR_VAR_START+ADR_FANDIR]=0;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_AFANDIR];
	if(V_TEMP1<ARC_AFANDIR_10)
		V_TEMP1++;
	else
		V_TEMP1=ARC_AFANDIR_1;
	ADR_VAR[ADR_VAR_START+ADR_AFANDIR]=V_TEMP1;
		
	SET_VarFlag_SEND(key_val);
}

/*===========================================================
;定时开设置处理
;说明:分三种情况
;	1.相对定时，最小单位为1小时，定时间存放在 ADR_TIMEON (单位小时)中,同时备份到ADR_TMON_10MINS(单位小时)中;
;	2.显示绝对定时，实际发码也为绝对定时，时间存放在 ADR_TIMEON（小时）和 ADR_TMON_10MINS（10分钟）中;
;	3.显示绝对定时，实际发码为相对定时，时间存放在  ADR_TMON_10MINS（单位10分钟）中,同时备份到ADR_TIMEON（单位10分钟）中;
;===========================================================*/
/*----------------------------------------------------------------------
;小时,分钟 转10分钟,;计算出相对时间
;input:	VarTemp1 (分钟),VarTemp2(小时),VarTemp3(当前的定时绝对时间单位为10分钟)
;output:	V_Time_10Min(10分钟)
;---------------------------------------------------------------------*/
/*u8 HourMin_Chang_Opt10MIN(u8 V_CLKmin,u8 V_CLKhour,u8 V_TM10min)
{
	u8 V_Time_10Min=0;
	while(V_CLKmin>=10)
	{
		V_CLKmin=V_CLKmin-10;
		V_Time_10Min++;
	}
	while(V_CLKhour>=1)
	{
		V_CLKhour=V_CLKhour-1;
		V_Time_10Min=V_Time_10Min+6;
	}
	if(V_TM10min<V_Time_10Min)
	{
		V_Time_10Min=144-V_Time_10Min+V_TM10min;		//24*6=144
	}
	else
	{
		V_Time_10Min=V_TM10min-V_Time_10Min;
	}
	return V_Time_10Min;
}*/
/*---------------------------------------------------------------------------------------
;将10分钟为单位的时间差值，转换为小时，分钟
;input:	ACC
;output:	u8 * V_Hour_point(小时值),u8 * V_Min_point(分钟值)
;---------------------------------------------------------------------------------------*/
/*void  F10MIN_TO_HOUR_MIN(u8  V_Time_10Min,u8 *V_Hour_point,u8 *V_Min_point)
{
	u8 V_TEMP=0;
	
	while(V_Time_10Min>=6)
	{
		V_Time_10Min=V_Time_10Min-6;
		V_TEMP++;
	}
	*V_Hour_point=V_TEMP;
	V_TEMP=MIN_CHANGE_TAB[V_Time_10Min];
	*V_Min_point=V_TEMP;
}*/
/*-----------------------------------------------------------------------
;相对定时定开定关可同时设方式CON_TM_SET_TWO_PT，确定定时时的开关机处理
;定开定关同时设，定开在前关机，定关在前开机
;单设定开关机，单设定关开机
;input: VarTemp0(TIMEON),VarTemp1(ADR_TIMEOFF)
;output: ADR_VAR_START+ADR_SYSFLAG
;-----------------------------------------------------------------------*/
void TIME_OPPOSITE_POWER_PRO(u16 V_group_num,u8 V_Time_On,u8 V_Time_off)
{
	u8 V_TEMP1,V_TEMP2,V_TEMP3;
	
	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
	V_TEMP1=V_TEMP1&TM_SET_MASK;
	if(V_TEMP1!=CON_TM_SET_TWO_PT) return;

	V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	V_TEMP2=V_TEMP2&(ARC_T3_TIMEOFF+ARC_T3_TIMEON);
	
	if(V_TEMP2==(ARC_T3_TIMEOFF+ARC_T3_TIMEON))
	{
		if(V_Time_On<V_Time_off)
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3&(~SYSFLAG_POWER);		//定开在前，关机
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
		else
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3|SYSFLAG_POWER;			//定关在前，开机
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
	}
	else if(V_TEMP2==ARC_T3_TIMEOFF)
	{
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP3=V_TEMP3|SYSFLAG_POWER;			//定关在前，开机
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
	}
	else
	{
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP3=V_TEMP3&(~SYSFLAG_POWER);		//定开在前，关机
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
	}
	
}



/*-----------------------------------------------------------------------
;绝对定时定开定关可同时设方式CON_TM_SET_TWO_PT，确定定时时的开关机处理
;定开定关同时设，定开在前关机，定关在前开机
;单设定开关机，单设定关开机
;input: VarTemp0(TIMEON MIN),VarTemp1(TIMEON HOUR),VarTemp2(TIMEOFF MIN),VarTemp3(TIMEOFF HOUR)
;output: ADR_VAR_START+ADR_SYSFLAG
;-----------------------------------------------------------------------*/
//----------将相对时间转为绝对时间---------------------------------
/*void TIME_ABSOLUTE_TCal(u8 *VP_TIME_10MIN,u8 *VP_TIME_HOUR)
{
	u8 V_TIME_10MIN,V_TIME_HOUR,V_CLOCK_MIN,V_LOCK_HOUR;
	u16	v_time,v_clock;
	
	V_TIME_10MIN=*VP_TIME_10MIN;
	V_TIME_10MIN=MIN_CHANGE_TAB[V_TIME_10MIN];
	V_TIME_HOUR=*VP_TIME_HOUR;
	V_CLOCK_MIN=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
	V_LOCK_HOUR=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
	v_time=V_TIME_HOUR*60+V_TIME_10MIN;
	v_clock=V_LOCK_HOUR*60+V_CLOCK_MIN;
	if(v_time<v_clock) V_TIME_HOUR=V_TIME_HOUR+24;
	
	if(V_TIME_10MIN>=V_CLOCK_MIN) V_TIME_10MIN=V_TIME_10MIN-V_CLOCK_MIN;
	else
	{
		V_TIME_HOUR--;
		V_TIME_10MIN=V_TIME_10MIN+60-V_CLOCK_MIN;
	}
	V_TIME_HOUR=V_TIME_HOUR-V_LOCK_HOUR;
	
	*VP_TIME_10MIN=V_TIME_10MIN;
	*VP_TIME_HOUR=V_TIME_HOUR;
	
}
//-------------------------------------------------------------------------------------------------------------
void TIME_ABSOLUTE_POWER_PRO(u16 V_group_num,u8 V_TMON_10MIN,u8 V_TMON_HOUR,u8 V_TMOFF_10MIN,u8 V_TMOFF_HOUR)
{
	u16 V_TEMP1,V_TEMP2,V_TEMP3;
	
	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
	V_TEMP1=V_TEMP1&TM_SET_MASK;
	
	if(V_TEMP1!=CON_TM_SET_TWO_PT) return;
	V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	V_TEMP2=V_TEMP2&(ARC_T3_TIMEOFF+ARC_T3_TIMEON);
	
	if(V_TEMP2==(ARC_T3_TIMEOFF+ARC_T3_TIMEON))
	{
		TIME_ABSOLUTE_TCal(&V_TMON_10MIN,&V_TMON_HOUR);
		TIME_ABSOLUTE_TCal(&V_TMOFF_10MIN,&V_TMOFF_HOUR);
		V_TEMP1=V_TMON_HOUR*60+V_TMON_10MIN;
		V_TEMP2=V_TMOFF_HOUR*60+V_TMOFF_10MIN;
		if(V_TEMP1>=V_TEMP2)
		{	//定关在前，开机
			
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3|SYSFLAG_POWER;
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
		else
		{	//定开在前，关机
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3&(~SYSFLAG_POWER);
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
	}
	else
	{
		if(V_TEMP2==ARC_T3_TIMEOFF)
		{	//定关在前，开机
			
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3|SYSFLAG_POWER;
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
		else
		{	//定开在前，关机
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3&(~SYSFLAG_POWER);
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
	
	}
	
}
*/
/*---------------------------------------------------------------------------------------*/

void USE_TIMEON_KEY_PRO(u16 V_group_num,u8 key_val)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3,V_TEMP4;
u8	*point1=NULL,*point2=NULL,*point3=NULL;
//	if((SYSTEMFLAG&MULITCODE)!=0) return;
	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
	if((V_TEMP1&F_Disable_TMON)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
		//调整时间里，不能设置定时;定关在设置状态不能设置定开。 
	//if((TmClk_SetState==TmClk_SetState_ClkHr)||(TmClk_SetState==TmClk_SetState_ClkMin))  return;
	if(TmClk_SetState==TmClk_SetState_TMOFF) return;
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if((TmClk_SetState==TmClk_SetState_TMON)||((V_TEMP1&ARC_T3_TIMEON)==0))
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=READ_TIME_CHARACTER(V_group_num);
		V_TEMP2=V_TEMP2&TM_SET_MASK;

		if((V_TEMP2!=CON_TM_SET_ONE)&&(V_TEMP2!=CON_TM_SET_TWO)&&(V_TEMP2!=CON_TM_SET_TWO_PT))
		{	SET_VarFlag_SEND(key_val);}	//定时键一按键即发码
		else
		{
			if((V_TEMP2==CON_TM_SET_ONE)&&((V_TEMP1&SYSFLAG_POWER)!=0))  return;	//20120905

			if(TmClk_SetState!=TmClk_SetState_TMON)
			{
				TmClk_SetState=TmClk_SetState_TMON;
				ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEONP;		//打开定开
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
				V_TEMP1=V_TEMP1|ARC_T3_TIMEON;				//有定开
				ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
				
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
				V_TEMP1=V_TEMP1|SYSFLAG_TIME;				//有定时
				ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
				
				ADR_VAR[ADR_VAR_START+ADR_TIMEON] = ARC_TIMEON_BAK;
				
/*
				V_TEMP1=READ_TIME_CHARACTER(V_group_num);
				V_TEMP1=V_TEMP1&TM_MODE_MASK;
				switch(V_TEMP1)
				{
					case CON_TM_MODE_OPPOSITE:	//相对定时
					
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
						if(V_TEMP2>=24+1)
						{
							V_TEMP2=1;
							ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP2;
						}
						ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
						break;
						
					case CON_TM_MODE_ABSOLUTE:	//绝对定时
						
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
						if(V_TEMP2>=6)
						{
							ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=0;
							V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							if(V_TEMP1>=24) 
							ADR_VAR[ADR_VAR_START+ADR_TIMEON]=0;
						}
						break;
						
					case CON_TM_MODE_FABSOLUTE:	//假绝对定时（显示绝对发码相对）

						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
						V_TEMP4=HourMin_Chang_Opt10MIN(V_TEMP1,V_TEMP2,V_TEMP3);
						ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP4;
						point1=&ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR];
						point2=&ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN];
						F10MIN_TO_HOUR_MIN(V_TEMP4,point1,point2);	//;将10分钟为单位的时间差值，转换为小时，分钟
						
						break;
					default:
						break;
						
				}
*/				
				
			}
			else	//确认定时
			{
				//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
				//V_TEMP1=V_TEMP1&TM_MODE_MASK;
				//switch(V_TEMP1)
				//{
				//	case CON_TM_MODE_OPPOSITE:	//相对定时

						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
						if(V_TEMP1!=0)
						{
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
							if(((V_TEMP2&ARC_T3_TIMEOFF)==0)||(V_TEMP3!=V_TEMP4))	//定开和定关不能设置一样的时间
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							//	ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP1;		//备份定时时间
								ARC_TIMEON_BAK = V_TEMP1;
								TmMin=0;
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
								TIME_OPPOSITE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2);	//开关机处理
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}
						
						}
						//break;
						
/*					case CON_TM_MODE_ABSOLUTE:	//绝对定时
	
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
						V_TEMP3=MIN_CHANGE_TAB[V_TEMP2];
						V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
						if((V_TEMP1!=V_TEMP2)||(V_TEMP3!=V_TEMP4))
						{
							V_TEMP5=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
							if(((V_TEMP5&ARC_T3_TIMEOFF)==0)||(V_TEMP1!=V_TEMP2)||(V_TEMP3!=V_TEMP4))
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
								V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
								V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
								TIME_ABSOLUTE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2,V_TEMP3,V_TEMP4);	//绝对定时开关机处理
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}

						}
						break;
						
					case CON_TM_MODE_FABSOLUTE:	//假绝对定时（显示绝对发码相对）
					
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
						V_TEMP4=HourMin_Chang_Opt10MIN(V_TEMP1,V_TEMP2,V_TEMP3);
						ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP4;
						point1=&ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR];
						point2=&ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN];
						F10MIN_TO_HOUR_MIN(V_TEMP4,point1,point2);	//;将10分钟为单位的时间差值，转换为小时，分钟
					
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
						if(V_TEMP1!=0)
						{
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];	
							if(((V_TEMP2&ARC_T3_TIMEOFF)==0)||(V_TEMP3!=V_TEMP4))	//定开和定关不能设置一样的时间
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
								HEX_TO_DEC(V_TEMP1,&V_TEMP2,&V_TEMP3,&V_TEMP4);
								TmMin=V_TEMP2;
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
								TIME_OPPOSITE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2);	//开关机处理
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}
						}
					
						break;
						
					default:
						break;
				}*/
			}
		}
		
		
	}
	else
	{	//取消定开
	
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEONCL;	//取消定开
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		V_TEMP1=V_TEMP1&(~ARC_T3_TIMEON);
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
		if(V_TEMP1==0)
		{	
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP2=V_TEMP2&(~SYSFLAG_TIME);	//clr 定时标志
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
		}
		key_val=ARC_KEY_TIMECL;	//取消定时键
/*		V_TEMP1=READ_TIME_CHARACTER(V_group_num);
		V_TEMP1=V_TEMP1&TM_MODE_MASK;
		if(V_TEMP1==CON_TM_MODE_FABSOLUTE)
		{	//显示为绝对时间,发码为相对时间（时间差）
			ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=0;
			ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR]=0;
			ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN]=0;
		
		}
		else if(V_TEMP1==CON_TM_MODE_OPPOSITE)*/
			ADR_VAR[ADR_VAR_START+ADR_TIMEON]=0;		//清相对定时开时间
		
		JUDGE_POWER_ONOFF(V_group_num);
		SET_VarFlag_SEND(key_val);
		
	}

}



/*===========================================================
;定时关设置处理
;说明:分三种情况
;	1.相对定时，最小单位为1小时，定时间存放在 ADR_TIMEOFF (单位小时)中,同时备份到ADR_TMOFF_10MINS(单位小时)中;
;	2.显示绝对定时，实际发码也为绝对定时，时间存放在 ADR_TIMEOFF（小时）和 ADR_TMOFF_10MINS（10分钟）中;
;	3.显示绝对定时，实际发码为相对定时，时间存放在  ADR_TMOFF_10MINS（单位10分钟）中,同时备份到ADR_TIMEOFF（单位10分钟）中;
;===========================================================*/
void USE_TIMEOFF_KEY_PRO(u16 V_group_num,u8 key_val)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3,V_TEMP4;
u8	*point1=NULL,*point2=NULL,*point3=NULL;
	//if((SYSTEMFLAG&MULITCODE)!=0) return;
	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
	if((V_TEMP1&F_Disable_TMOFF)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	
	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	if(V_TEMP1&F_SleepNHours)
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		if(V_TEMP1&SYSFLAG_SLEEP)	return;		//设置连续睡眠几小时，没定关
	}

		//调整时间里，不能设置定时;定开在设置状态不能设置定关。 
	if(TmClk_SetState==TmClk_SetState_TMON) return;
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if((TmClk_SetState==TmClk_SetState_TMOFF)||((V_TEMP1&ARC_T3_TIMEOFF)==0))
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=READ_TIME_CHARACTER(V_group_num);
		V_TEMP2=V_TEMP2&TM_SET_MASK;

		if((V_TEMP2!=CON_TM_SET_ONE)&&(V_TEMP2!=CON_TM_SET_TWO)&&(V_TEMP2!=CON_TM_SET_TWO_PT))
		{	SET_VarFlag_SEND(key_val);}	//定时键一按键即发码
		else
		{
			if((V_TEMP2==CON_TM_SET_ONE)&&((V_TEMP1&SYSFLAG_POWER)==0))  return;		//20120905

			if(TmClk_SetState!=TmClk_SetState_TMOFF)
			{
				TmClk_SetState=TmClk_SetState_TMOFF;
				ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFP;		//打开定关
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
				V_TEMP1=V_TEMP1|ARC_T3_TIMEOFF;				//有定关
				ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
				
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
				V_TEMP1=V_TEMP1|SYSFLAG_TIME;				//有定时
				ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;

				ADR_VAR[ADR_VAR_START+ADR_TIMEOFF] = ARC_TIMEOFF_BAK;

/*
				V_TEMP1=READ_TIME_CHARACTER(V_group_num);
				V_TEMP1=V_TEMP1&TM_MODE_MASK;
				switch(V_TEMP1)
				{
					case CON_TM_MODE_OPPOSITE:	//相对定时
					
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
						if(V_TEMP2>=24+1)
						{
							V_TEMP2=1;
							ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP2;
						}
						ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
						break;
						
					case CON_TM_MODE_ABSOLUTE:	//绝对定时
						
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
						if(V_TEMP2>=6)
						{
							ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=0;
							V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
							if(V_TEMP1>=24) 
							ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;
						}
						break;
						
					case CON_TM_MODE_FABSOLUTE:	//假绝对定时（显示绝对发码相对）

						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
						V_TEMP4=HourMin_Chang_Opt10MIN(V_TEMP1,V_TEMP2,V_TEMP3);
						ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP4;
						point1=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR];
						point2=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN];
						F10MIN_TO_HOUR_MIN(V_TEMP4,point1,point2);	//;将10分钟为单位的时间差值，转换为小时，分钟
						
						break;
					default:
						break;
						
				}
			*/	
				
			}
			else	//确认定时
			{
				//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
				//V_TEMP1=V_TEMP1&TM_MODE_MASK;
				//switch(V_TEMP1)
				//{
				//	case CON_TM_MODE_OPPOSITE:	//相对定时

						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
						if(V_TEMP1!=0)
						{
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							if(((V_TEMP2&ARC_T3_TIMEON)==0)||(V_TEMP3!=V_TEMP4))	//定开和定关不能设置一样的时间
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
								//ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP1;		//备份定时时间
								ARC_TIMEOFF_BAK = V_TEMP1;
								TmMin=0;
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
								TIME_OPPOSITE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2);	//开关机处理
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}
						
						}
						//break;
/*						
					case CON_TM_MODE_ABSOLUTE:	//绝对定时
	
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
						V_TEMP3=MIN_CHANGE_TAB[V_TEMP2];
						V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
						if((V_TEMP1!=V_TEMP2)||(V_TEMP3!=V_TEMP4))
						{
							V_TEMP5=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
							if(((V_TEMP5&ARC_T3_TIMEON)==0)||(V_TEMP1!=V_TEMP2)||(V_TEMP3!=V_TEMP4))
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
								V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
								V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
								TIME_ABSOLUTE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2,V_TEMP3,V_TEMP4);	//绝对定时开关机处理
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}

						}
						break;
						
					case CON_TM_MODE_FABSOLUTE:	//假绝对定时（显示绝对发码相对）
					
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
						V_TEMP4=HourMin_Chang_Opt10MIN(V_TEMP1,V_TEMP2,V_TEMP3);
						ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP4;
						point1=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR];
						point2=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN];
						F10MIN_TO_HOUR_MIN(V_TEMP4,point1,point2);	//;将10分钟为单位的时间差值，转换为小时，分钟
					
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
						if(V_TEMP1!=0)
						{
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];	
							if(((V_TEMP2&ARC_T3_TIMEON)==0)||(V_TEMP3!=V_TEMP4))	//定开和定关不能设置一样的时间
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
								HEX_TO_DEC(V_TEMP1,&V_TEMP2,&V_TEMP3,&V_TEMP4);
								TmMin=V_TEMP2;
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
								TIME_OPPOSITE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2);	//开关机处理
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}
						}
					
						break;
						
					default:
						break;
				}
				*/
			}
		}
		
		
	}
	else
	{	//取消定开
	
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFCL;	//取消定开
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		V_TEMP1=V_TEMP1&(~ARC_T3_TIMEOFF);
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
		if(V_TEMP1==0)
		{	
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP2=V_TEMP2&(~SYSFLAG_TIME);	//clr 定时标志
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
		}
		key_val=ARC_KEY_TIMECL;	//取消定时键
	/*	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
		V_TEMP1=V_TEMP1&TM_MODE_MASK;
		if(V_TEMP1==CON_TM_MODE_FABSOLUTE)
		{	//显示为绝对时间,发码为相对时间（时间差）
			ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=0;
			ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR]=0;
			ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN]=0;
		
		}
		else if(V_TEMP1==CON_TM_MODE_OPPOSITE)	*/
			ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;		//清相对定时开时间

		JUDGE_POWER_ONOFF(V_group_num);
		SET_VarFlag_SEND(key_val);
		
	}

}


/*===========================================================
;睡眠处理
;说明:1.将睡眠标志置1,或清0，睡眠时间与定时变量共用ADR_TIMEOFF
;input:	ADR_SYSFLAG，ADR_TIMEOFF
;output:	ADR_SYSFLAG，ADR_TIMEOFF
;===========================================================*/
void USE_SLEEP_KEY_PRO(u16 V_group_num,u8 key_val)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3;
u8	*point1=NULL,*point2=NULL,*point3=NULL;

	//if((SYSTEMFLAG&MULITCODE)!=0) return;
	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;	
	if(TmClk_SetState==TmClk_SetState_TMOFF) return;	//设置定关时，不能设置睡眠
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	V_TEMP1=V_TEMP1&(~SYSFLAG_STRONG);
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_MODE];
	
	if((V_TEMP1==ARC_MODE_DHMD)||(V_TEMP1==ARC_MODE_FAN)) return;	//除湿，送风模式不能启动睡眠
	
	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	if((V_TEMP1&F_SleepNHours)==0)
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=(V_TEMP2^SYSFLAG_SLEEP)&(~SYSFLAG_STRONG);	//启动睡眠，清强力
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	else
	{//睡眠可设时间
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		V_TEMP2=V_TEMP2&ARC_T3_TIMEOFF;
		if(V_TEMP2!=0)	return;
		
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		if((V_TEMP2&SYSFLAG_SLEEP)==0)
		{
			V_TEMP2=V_TEMP2|SYSFLAG_SLEEP;
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
			ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=1;
		}
		else if(ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]==0)
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3&(~SYSFLAG_SLEEP);
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
			ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;
		}
		else
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
			V_TEMP3++;
			ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP3;
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
			if(V_TEMP3>=11)	//最多睡眠10小时
			{
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
				V_TEMP1=V_TEMP1&(~SYSFLAG_SLEEP);
				ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
				
				ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;
			}
			
		}
	}
	SET_VarFlag_SEND(key_val);
	
}
/*===========================================================
;电辅热处理
;说明:1.将电辅热标志置1,或清0
;input:	ADR_SYSFLAG
;output:	ADR_SYSFLAG
;===========================================================*/
void USE_HEAT_KEY_PRO(u8 key_val)
{
u8   V_TEMP1,V_TEMP2;

	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_MODE];
	if(V_TEMP1==ARC_MODE_HEAT)
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2^SYSFLAG_HEAT;
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
		SET_VarFlag_SEND(key_val);
	}
	
}

/*===========================================================
;强力处理
;说明:1.将强力标志置1,或清0
;只有制冷，制热模式 有效
;经济跟强力不同时存在
;input:	ADR_SYSFLAG
;output:	ADR_SYSFLAG
;===========================================================*/
void ARC_KEY_STRONG_PRO(u8 key_val)
{
u8   V_TEMP1,V_TEMP2;

	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if((V_TEMP1&SYSFLAG_SLEEP)!=0)	 return;	//睡眠开启，不能开强力
	
	V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
	if((V_TEMP2==ARC_MODE_HEAT)||(V_TEMP2==ARC_MODE_COOL))
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP1=(V_TEMP1^SYSFLAG_STRONG)&(~SYSFLAG_ECONOMIC);	//睡眠开启，不能开强力
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
		SET_VarFlag_SEND(key_val);
	}
	
}

/*===========================================================
;灯光处理
;说明:1.将灯光标志置1,或清0
;input:	ADR_SYSFLAG
;output:	ADR_SYSFLAG
;===========================================================*/
void ARC_KEY_LIGHT_PRO(u8 key_val)
{
u8   V_TEMP1;

//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	
	VarFlag=VarFlag|VarFlag_DISP_L;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	V_TEMP1=V_TEMP1^SYSFLAG_LIGHT;
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
	
	SET_VarFlag_SEND(key_val);
}

/*===========================================================
;空清/净化/健康处理
;说明:1.将空清标志置1,或清0
;input:	ADR_SYSFLAG
;output:	ADR_SYSFLAG
;===========================================================*/
void ARC_KEY_AIRCLEAR_PRO(u8 key_val)
{
u8   V_TEMP1;

	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	V_TEMP1=V_TEMP1^SYSFLAG_AIRCLEAR;
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
	
	SET_VarFlag_SEND(key_val);
}
/*===========================================================
;经济处理
;说明:1.将经济标志置1,或清0
;经济跟强力不同时存在
;除湿送风模式不能用经济
;input:	ADR_SYSFLAG
;output:	ADR_SYSFLAG
;============================================================*/
void ARC_KEY_ECONOMIC_PRO(u8 key_val)
{
u8   V_TEMP1;

	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_MODE];
	if((V_TEMP1==ARC_MODE_DHMD)||(V_TEMP1==ARC_MODE_FAN)) return;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	V_TEMP1=(V_TEMP1^SYSFLAG_ECONOMIC)&(~SYSFLAG_STRONG);	//清强力
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
	
	SET_VarFlag_SEND(key_val);
}

/*===========================================================
;时钟更新，定时时间更新(时钟1分钟更新1次)
;定时状态也在这里改变,打开定时状态经过此函数改变为等待定时时间到状态
;input:	Clock_Sec, ADR_TIMEON,ADR_TIMEOFF,ADR_TM_MIN,ADR_TMON_10MINS,ADR_TMOFF_10MINS,ADR_SYSFLAG,ADR_TMSTATE7,ADR_TMSTATE3
;output: 	ADR_CKMIN,ADR_CKHOUR,ADR_TIMEON,ADR_TIMEOFF,ADR_TM_MIN,ADR_TMON_10MINS,ADR_TMOFF_10MINS,ADR_SYSFLAG,ADR_TMSTATE7,ADR_TMSTATE3
;===========================================================*/

/*---------------------------------------------------------
;定时开倒计时
;---------------------------------------------------------*/
void ARC_TMON_UPDATA(u16 V_group_num)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3;
u8	*point1=NULL,*point2=NULL;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if((V_TEMP1&ARC_T3_TIMEON)==0) return;
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEONW;

//	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
//	V_TEMP1=V_TEMP1&TM_MODE_MASK;
//	if(V_TEMP1==CON_TM_MODE_OPPOSITE)
//	{	//相对时间
		if(TmMin<60) return;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
		if(V_TEMP2!=0)
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
			V_TEMP3--;
			ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP3;
			if(V_TEMP3!=0) return;
		}
		
//	}
/*	else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
	{	//显示为绝对时间,发码也为绝对时间
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
		if(V_TEMP2!=V_TEMP3) return;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
		V_TEMP2=MIN_CHANGE_TAB[V_TEMP2];
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
		if(V_TEMP2!=V_TEMP3) return;
	}
	else
	{	//显示为绝对时间,发码为相对时间(时间差)
	
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN];
		if(V_TEMP2!=0)
		{
			V_TEMP2--;
			ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN]=V_TEMP2;
		}
		if(TmMin<10) return;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		if(V_TEMP2!=(ARC_T3_TIMEON+ARC_T3_TIMEOFF)) TmMin=0;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
		if(V_TEMP2!=0)
		{
			V_TEMP2--;
			ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP2;
		}
		point1=&ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR];
		point2=&ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
		F10MIN_TO_HOUR_MIN(V_TEMP2,point1,point2);	//;将10分钟为单位的时间差值，转换为小时，分钟
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
		if(V_TEMP2!=0) return;
		
	}*/
	//定时开时间到
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if(V_TEMP1&ARC_T3_TIMEOFF)
	{
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFW;		//等定关
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2|SYSFLAG_POWER;		//开机
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	else
	{
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=0;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=(V_TEMP2&(~SYSFLAG_TIME))|SYSFLAG_POWER;	//清定时,开关
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	V_TEMP1=V_TEMP1&(~ARC_T3_TIMEON);
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
	//ICON_OFF_TMON_M1();
//	ADR_VAR_INIT();
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	V_TEMP1=V_TEMP1|SYSFLAG_POWER;
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
	//ARC_DISPLAY();
	//ARC_DISPLAY_PRO(V_group_num);
}

/*------------------------------------------------------
;定关倒计时
;------------------------------------------------------*/
void ARC_TMOFF_UPDATA(u16 V_group_num)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3;
u8	*point1=NULL,*point2=NULL;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if((V_TEMP1&ARC_T3_TIMEOFF)==0) return;

	ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFW;

//	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
//	V_TEMP1=V_TEMP1&TM_MODE_MASK;
//	if(V_TEMP1==CON_TM_MODE_OPPOSITE)
//	{	//相对时间
		if(TmMin<60) return;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
		if(V_TEMP2!=0)
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
			V_TEMP3--;
			ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP3;
			if(V_TEMP3!=0) return;
		}
		
//	}
/*	else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
	{	//显示为绝对时间,发码也为绝对时间
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
		if(V_TEMP2!=V_TEMP3) return;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
		V_TEMP2=MIN_CHANGE_TAB[V_TEMP2];
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
		if(V_TEMP2!=V_TEMP3) return;
	}
	else
	{	//显示为绝对时间,发码为相对时间(时间差)
	
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN];
		if(V_TEMP2!=0)
		{
			V_TEMP2--;
			ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN]=V_TEMP2;
		}
		if(TmMin<10) return;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		if(V_TEMP2!=(ARC_T3_TIMEON+ARC_T3_TIMEOFF)) TmMin=0;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
		if(V_TEMP2!=0)
		{
			V_TEMP2--;
			ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP2;
		}
		point1=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR];
		point2=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
		F10MIN_TO_HOUR_MIN(V_TEMP2,point1,point2);	//;将10分钟为单位的时间差值，转换为小时，分钟
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
		if(V_TEMP2!=0) return;
		
	}*/
	//定时关时间到
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if(V_TEMP1&ARC_T3_TIMEON)
	{
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFW;		//等定开
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2&(~SYSFLAG_POWER);		//关机
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	else
	{
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=0;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2&(~(SYSFLAG_TIME+SYSFLAG_POWER));	//清定时,关机
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	V_TEMP1=V_TEMP1&(~ARC_T3_TIMEOFF);
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
	//DISP_ARC_GROUP_1();
	//ICON_OFF_DOT2_M1();
	//ICON_OFF_TMOFF_M1();
	//ARC_DISPLAY();
	//ARC_DISPLAY_PRO(V_group_num);
}
/*---------------------------------------------------------------------------------*/
void ARC_CLK_TM_UPDATA(u16 V_group_num,u8 V_sysck_hour,u8 V_sysck_min)
{
//u8 V_TEMP1;
u16 	characteristic_id;

	if(V_group_num >= EXT_FILE_DATA) characteristic_id = V_group_num;
	else characteristic_id = GROUP_CHANG_Array[V_group_num];
	
	ADR_VAR[ADR_VAR_START+ADR_CKMIN]=V_sysck_min;
	ADR_VAR[ADR_VAR_START+ADR_CKHOUR]=V_sysck_hour;
	TmMin++;
	ARC_TMON_UPDATA(characteristic_id);
	ARC_TMOFF_UPDATA(characteristic_id);

//	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
//	V_TEMP1=V_TEMP1&TM_SET_MASK;
//	if(V_TEMP1!=CON_TM_MODE_FABSOLUTE)
//	{
		if(TmMin>=60) TmMin=0;
//	}
//	else
//	{
//		if(TmMin>=10) TmMin=0;
//	}
}


/*===========================================================
;按键使能判断
;input: VarTemp0
;output: A
;===========================================================*/
u8 ARC_KEY_ENCHECK(u16 V_group_num,u8 key_val)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3,i,V_FILE_ID;
u32	V_DATA_ADDR;
//int		r_size;
	for(i=0;i<8;i++)
	{
		V_TEMP1=KEY_CHOICE_TAB[i];
		if(key_val==V_TEMP1) break;
	}
	if(i==8) return 0xff;//jason need to be modefied ??????????
	V_TEMP1=BIT_ADR_MASK_TAB[i];
	
	if(V_group_num >= EXT_FILE_DATA)
	{
// 		V_FILE_ID = V_group_num&(~EXT_FILE_DATA);//jason need to be modefied ??????????
// 		V_DATA_ADDR = arc_exdata_addr[V_FILE_ID];
// 		Remote_File_ReadData(devNo_FARC,(V_DATA_ADDR+FILE_ARC_KEYEN_OFS),&V_TEMP2,1);
	}
	else   V_TEMP2=KEY_ENABLE_Array[V_group_num];

	
	V_TEMP3=V_TEMP2&V_TEMP1;
	return V_TEMP3;
}

/*;=================================================
;全局变量初始化
;=================================================*/
//第一次应用初始化，当用户使用过后，将这些变量存入flash中，下次从flash中读取
void ADR_VAR_INIT(u8 clk_hour,u8 clk_min)
{
	ADR_VAR[ADR_VAR_START+ADR_TMP]=25-16;
	ADR_TMP_BAK=25-16;
	ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ARC_FANLEV_4;
	ADR_FANLEV_BAK=ARC_FANLEV_4;
	ADR_VAR[ADR_VAR_START+ADR_FANDIR]=ARC_FANDIR_2;
	ADR_VAR[ADR_VAR_START+ADR_AFANDIR]=ARC_AFANDIR_1;
	ADR_VAR[ADR_VAR_START+ADR_MFANDIR]=ARC_MFANDIR_1;
	ADR_VAR[ADR_VAR_START+ADR_TIMEON]=0;
	ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=0;
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=0;
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=0;
	TmClk_SetState=0;
	ADR_VAR[ADR_VAR_START+ADR_MODE]=ARC_MODE_COOL;
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=0;
	ADR_VAR[ADR_VAR_START+ADR_CKHOUR]=clk_hour;
	ADR_VAR[ADR_VAR_START+ADR_CKMIN]=clk_min;
	ADR_VAR[ADR_VAR_START+ADR_KEYVAL]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN]=0;
	VarFlag=VarFlag_USE_DISABLE;
	TmMin=0;
	
}
//进入搜索状态变量初始化函数
void ADR_VAR_INIT_Search(u16 V_group_num)
{
u16 	V_FILE_ID;
u32 V_DATA_ADDR;
u8  temp;

	if(V_group_num < EXT_FILE_DATA) V_group_num = GROUP_CHANG_Array[V_group_num];
		
	ADR_VAR[ADR_VAR_START+ADR_TMP]=25-16;
	ADR_TMP_BAK=25-16;
	ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ARC_FANLEV_4;
	ADR_FANLEV_BAK=ARC_FANLEV_4;
	ADR_VAR[ADR_VAR_START+ADR_FANDIR]=ARC_FANDIR_2;
	ADR_VAR[ADR_VAR_START+ADR_AFANDIR]=ARC_AFANDIR_1;
	ADR_VAR[ADR_VAR_START+ADR_MFANDIR]=ARC_MFANDIR_1;
	ADR_VAR[ADR_VAR_START+ADR_TIMEON]=0;
	ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=0;
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=0;
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=0;
	TmClk_SetState=0;
	ADR_VAR[ADR_VAR_START+ADR_MODE]=ARC_MODE_COOL;

	if(V_group_num >= EXT_FILE_DATA)
	{
		V_FILE_ID = V_group_num&(~EXT_FILE_DATA);
		V_DATA_ADDR = arc_exdata_addr[V_FILE_ID];
		Remote_File_ReadData(devNo_FARC,(V_DATA_ADDR+FILE_ARC_KEYEN_OFS),&temp,1);
	}
	else { temp = KEY_ENABLE_Array[V_group_num];}
	
	if((temp & EnK_LIGHT) == 0) ADR_VAR[ADR_VAR_START+ADR_SYSFLAG] = SYSFLAG_POWER;
	else ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=SYSFLAG_LIGHT+SYSFLAG_POWER;
	//ADR_VAR[ADR_VAR_START+ADR_CKHOUR]=ARC_updataTM_P->clk_hour;
	//ADR_VAR[ADR_VAR_START+ADR_CKMIN]=ARC_updataTM_P->clk_min;
	ADR_VAR[ADR_VAR_START+ADR_KEYVAL]=ARC_KEY_POWER;        //ADR_KEYVAL;
	//ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR]=0;
	//ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN]=0;
	VarFlag=0;	//VarFlag_USE_DISABLE+VarFlag_SEARCH+VarFlag_SEND;
	TmMin=0;
}


/*===========================================================
;使用处理
;功能按键数目可以增减

;============================================================*/
void ARC_USE_PRO(u16 V_group_num,u8 key_val)
{
u8	V_TEMP1;
u16 	characteristic_id;

	if(V_group_num >= EXT_FILE_DATA) characteristic_id = V_group_num;
	else characteristic_id = GROUP_CHANG_Array[V_group_num];
	
	if(key_val>=DAT_KEY_NUM_MAX) return;
	
	V_TEMP1=ARC_KEY_ENCHECK(characteristic_id,key_val);	//按键使能判断
	if(V_TEMP1==0)  return;
	
	switch(key_val)
	{
		case	ARC_KEY_POWER:		//电源
			USE_POWER_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_TUP:		//温度+
			USE_TEMPUP_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_TDOWN:		//温度-
			USE_TEMPDN_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_MODE:		//模式
			USE_MODE_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_FANLEV:		//风量
			USE_FANLEV_KEY_PRO(key_val);
			break;
		case	ARC_KEY_AFANDIR:	//自动风向
			USE_AFANDIR_KEY_PRO(key_val);	//自动风向/上下
			break;
		case	ARC_KEY_MFANDIR:	//手动风向
			USE_MFANDIR_KEY_PRO(key_val);	//手动风向/左右
			break;
		case	ARC_KEY_TIMEON:		//定开
			USE_TIMEON_KEY_PRO(characteristic_id,key_val);	//定时开
			break;
		case	ARC_KEY_TIMEOFF:	//定关
			USE_TIMEOFF_KEY_PRO(characteristic_id,key_val);
			break;
//		case	ARC_KEY_TIMECL:		//取消定时
		
		case	ARC_KEY_SLEEP:		//睡眠
			USE_SLEEP_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_HEAT:		//电辅热
			USE_HEAT_KEY_PRO(key_val);
			break;
		case	ARC_KEY_STRONG:		//强力
			ARC_KEY_STRONG_PRO(key_val);
			break;
		case	ARC_KEY_LIGHT:		//灯光
			ARC_KEY_LIGHT_PRO(key_val);
			break;
		case	ARC_KEY_AIRCLEAR:	//空清
			ARC_KEY_AIRCLEAR_PRO(key_val);
			break;
		case	ARC_KEY_ECONOMIC:	//经济
			ARC_KEY_ECONOMIC_PRO(key_val);
			break;

		default:
			break;
	}
}


