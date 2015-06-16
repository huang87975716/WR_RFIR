/***************************************************************************
;========================================================================
;Company	:	HBG      Copyright (c) 2011
;Author		:	Kalvin
;版权信息	:	Copyright (c) 2011 HBG Hi Tech
;File		: 	ARC_Disp_COM.C
;Development	: 	VC++ 6.0
;Data		:	22012-11
;Customer	:	浩博高
;说明：空调万用遥控器显示程序
;-------------------------History list-----------------------------------
*****************************************************************************/
/***************************************************************************
*****************************************************************************/
#include "remote.h"
/***************************************************************************
 DEFINE LCD_DRV VARIABLE
*****************************************************************************/
u8 Ar_Disp_DATA[16];		//显示映射区

struct  DISP_PARA	ST_DP_D1_M1;
struct  DISP_PARA	ST_DP_D2_M1;
struct  DISP_PARA	ST_DP_D8_M1;
struct  DISP_PARA	ST_DP_D9_M1;
struct  DISP_PARA	ST_DP_D10_M1;
struct  DISP_PARA	ST_DP_D4_M1;
struct  DISP_PARA	ST_DP_D5_M1;
struct  DISP_PARA	ST_DP_D6_M1;
struct  DISP_PARA	ST_DP_MODE_M1;
struct  DISP_PARA	ST_DP_FANLEV_M1;	//风量显示入口参数
struct  DISP_PARA	ST_DP_SDFX_M1;		//手动风向显示入口参数
struct  DISP_PARA	ST_DP_ZDFX_M1;		//自动风向显示入口参数
struct  DISP_PARA	ST_DP_D7_M1;
struct  DISP_PARA	ST_DP_D3_M1;

/***************************************************************************
;	排列数据表
*****************************************************************************/
//Define:Bit6-0:	高ABCDEFG低
const u8 TAB_PTN_DIGIT[]={0x7e,		//"0"
                            0x30,		//"1"
                            0x6d,		//"2"
                            0x79,		//"3"
                            0x33,		//"4"
                            0x5b,		//"5"
                            0x5f,		//"6"
                            0x70,		//"7"
                            0x7f,		//"8"
                            0x7b,		//"9"
                            0x77,		//"A"
                            0x1f,		//"B"
                            0x4e,		//"C"
                            0x3d,		//"D"
                            0x4f,		//"E"
                            0x47,		//"F"
                            0x00};		//" "	主要用于闪烁灭

const u8 TAB_PTN_DIGIT2[]={0x00,		//"0"
			     0x06,		//"1"
			     0x05,		//"2"
			     0x00};		//" "	主要用于闪烁灭 

//Define:Bit4-0:制热,送风,抽湿,制冷,自动
const u8 TAB_PTN_MODE[]={0x00,			//灭
                           0x01,			//自动
                           0x02,			//制冷
                           0x04,			//抽湿
                           0x08,			//送风
                           0x10};			//制热
//Define:Bit4-0:3,2,1,AUTO
const u8 TAB_PTN_FS[]={0x00,			//灭  
                         0x01,			//AUTO
                         0x02,			//1   
                         0x06,			//2   
                         0x0e,			//3   
                         0x00};			//静音
//风向
const u8 TAB_PTN_FX[]={0x00,
                         0x3f,
                         0x03,
                         0x05,
                         0x09,
                         0x11,
                         0x21,
                         0x00};

/***************************************************************************
;	M1: MODEL 1 , KK26 seg,com 对应表
*****************************************************************************/
const u8 TAB_SEGCOM_D7_M1[]={ComBase*1+SegBase*1,	//AGDE段码
			       ComBase*0+SegBase*1,	//C
			       ComBase*2+SegBase*1};	//B

const u8 TAB_SEGCOM_D2_M1[]={ComBase*2+SegBase*21,		//G段码   
			       ComBase*1+SegBase*22,		//F   
                               ComBase*2+SegBase*22,		//E                                           
                               ComBase*3+SegBase*22,		//D                                           
                               ComBase*2+SegBase*20,		//C                                           
                               ComBase*1+SegBase*20,		//B                                           
                               ComBase*0+SegBase*21};		//A                                           

const u8 TAB_SEGCOM_D8_M1[]={ComBase*1+SegBase*2,	//G段码 
			 ComBase*2+SegBase*2,		//F     
			 ComBase*0+SegBase*2,		//E     
			 ComBase*0+SegBase*3,		//D     
			 ComBase*1+SegBase*3,		//C     
			 ComBase*2+SegBase*3,		//B     
			 ComBase*3+SegBase*3};		//A     

const u8 TAB_SEGCOM_D1_M1[]={ComBase*2+SegBase*24,		//G段码
                               ComBase*1+SegBase*24,		//F
                               ComBase*3+SegBase*24,		//E
                               ComBase*3+SegBase*23,		//D
                               ComBase*2+SegBase*23,		//C
                               ComBase*1+SegBase*23,		//B     
                               ComBase*0+SegBase*23};		//A
//MODE
const u8 TAB_SEGCOM_MODE_M1[]={ComBase*3+SegBase*25,	//ICON 自动 
                                 ComBase*1+SegBase*25,	//ICON 制冷 
                                 ComBase*0+SegBase*25,	//ICON 抽湿 
                                 ComBase*0+SegBase*22,	//ICON 送风 
                                 ComBase*2+SegBase*25};	//ICON 制热 
//FAN SPEED
const u8 TAB_SEGCOM_FS_M1[]={ComBase*0+SegBase*0,	//ICON 'A'
                               ComBase*3+SegBase*0,	//ICON 'T1'
                               ComBase*2+SegBase*0,	//ICON 'T2'
                               ComBase*1+SegBase*0};	//ICON 'T3'

const u8 TAB_SEGCOM_SDFX_M1[]={ComBase*3+SegBase*15,	//T1
                                 ComBase*1+SegBase*19,	//T2
                                 ComBase*2+SegBase*19,	//T3
                                 ComBase*3+SegBase*19,	//T4
                                 ComBase*2+SegBase*15,	//T5
                                 ComBase*1+SegBase*15};	//T6

const u8 TAB_SEGCOM_ZDFX_M1[]={ComBase*2+SegBase*18,	//T7
                                 ComBase*3+SegBase*18,	//T8
                                 ComBase*3+SegBase*16,	//T9
                                 ComBase*3+SegBase*17,	//T10
                                 ComBase*2+SegBase*17,	//T11
                                 ComBase*1+SegBase*17};	//T12

//;;;目前项目 TAB_SEGCOM_D1_M4 与 TAB_SEGCOM_D3_M2 一样,实际可删除一个
/***************************************************************************
;	显示一个变量(数据组)
;	In:	ACC,DISP_GROUP_DATA
;	Out:	None
;	Used:	X,Y
*****************************************************************************/
/*================================
;	LCD全亮
;	In:	None
;	Out:	None
;================================*/
/*void LCD_ALLON()
{
u8 i;

	for(i=0;i<16;i++)
	   Ar_Disp_DATA[i]=0xff;
}
*/
/*================================
;	LCD全灭
;	In:	None
;	Out:	None
;================================*/
/*void LCD_ALLOFF()
{
u8 i;

	for(i=0;i<16;i++)
	   Ar_Disp_DATA[i]=0x00;

}*/
/*================================
;	点亮一个点
;	In:	A
;	Out:	None
;	Used:	X
;================================*/
void LCD_ICON_On(u8 V_BitAddr)
{
u8 V_Temp1,V_Temp2,LCD_BMask;
	
	V_Temp1=V_BitAddr&0x07;			//取单字节位地址
	LCD_BMask=BIT_ADR_MASK_TAB[V_Temp1];	//取出Bit MASK 值
	V_Temp1=V_BitAddr>>3;			//得到Byte Adr
	if(V_Temp1>=16) return;
	V_Temp2=Ar_Disp_DATA[V_Temp1];
	V_Temp2=V_Temp2|LCD_BMask;
	Ar_Disp_DATA[V_Temp1]=V_Temp2;
	
}
/*================================
;	灭掉一个点
;	In:	A
;	Out:	None
;	Used:	X
;================================*/
void LCD_ICON_Off(u8 V_BitAddr)
{
u8 V_Temp1,V_Temp2,LCD_BMask;
	
	V_Temp1=V_BitAddr&0x07;			//取单字节位地址
	LCD_BMask=BIT_ADR_MASK_TAB[V_Temp1];	//取出Bit MASK 值
	LCD_BMask=~LCD_BMask;			//取反
	V_Temp1=V_BitAddr>>3;			//得到Byte Adr
	if(V_Temp1>=16) return;
	V_Temp2=Ar_Disp_DATA[V_Temp1];
	V_Temp2=V_Temp2&LCD_BMask;
	Ar_Disp_DATA[V_Temp1]=V_Temp2;

}

/*================================*/
void DISP_ICON_Group(u8 V_Data,struct DISP_PARA ST_DP_Group)
{
u8 DISP_ICON_CNT,DISP_ADR_OFF,V_rule,V_BitAdd,i,V_BitMask;
const u8 *VP_rule1=NULL,*VP_SegCom1=NULL;

	DISP_ICON_CNT=ST_DP_Group.V_SegNum;	//点数
	DISP_ADR_OFF=ST_DP_Group.V_AddrOff;	//偏移
	
	VP_rule1=ST_DP_Group.VP_rule;
	VP_rule1=VP_rule1+V_Data;
	V_rule=*VP_rule1;
	
	for(i=0;i<DISP_ICON_CNT;i++)
	{
		VP_SegCom1=ST_DP_Group.VP_SegCom;
		VP_SegCom1=VP_SegCom1+i;
		V_BitAdd=*VP_SegCom1;
		V_BitAdd=V_BitAdd+DISP_ADR_OFF;
		V_BitMask=BIT_ADR_MASK_TAB[i];
		if((V_rule&V_BitMask)!=0)
			LCD_ICON_On(V_BitAdd);
		else
			LCD_ICON_Off(V_BitAdd);
	}
	
}

/*********************************************
;模式
**********************************************/
void DISP_ARC_MODE(u16 V_group_num)
{
u8   V_TEMP1,V_TEMP2;

	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	if(((V_TEMP1&F_DonotDisp)==0)&&((VarFlag&VarFlag_USE_DISABLE)==0))
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
		V_TEMP2++;
		DISP_ICON_Group(V_TEMP2,ST_DP_MODE_M1);
	}
	else
	{
		DISP_ICON_Group(DONT_DISP,ST_DP_MODE_M1);
	}
	
}

/*********************************************
;温度
;自动，除湿 模式的温度跟据范围显示温度/图标/不显示
;送风模式不显示温度
**********************************************/
void DISP_ARC_TEMP(u16 V_group_num)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3,V_TEMP4,V_Temperat_Min,V_Temperat_Max;

	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
	if(((V_TEMP1&F_DonotDisp)==0)&&(V_TEMP2!=ARC_MODE_FAN))
	{
		if((V_TEMP2==ARC_MODE_AUTO)||(V_TEMP2==ARC_MODE_DHMD))
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			if(((V_TEMP3&SYSFLAG_POWER)==0)&&((V_TEMP1&F_DonotClrScreen)==0))
			{
				ICON_OFF_DC_M1();	//.C
				DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D1_M1);
				DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D2_M1);

			}
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_MODE];
			READ_TEMP_BOUND(V_group_num,V_TEMP3,&V_Temperat_Min,&V_Temperat_Max);
			if(V_Temperat_Max<(27-16))
			{
				//DISP_ARC_TMP_VALUE_OFF();
				ICON_OFF_DC_M1();		//.C
				DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D1_M1);
				DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D2_M1);
				ICON_ON_TMP_M1();		//温度
				if(V_Temperat_Min==V_Temperat_Max)
				{
					//温度不可调，数据又不温度表范围内不显示
					ICON_OFF_TUP();
					ICON_OFF_TDN();
					return;
				}
				else
				{	//显示往上/往下
					V_TEMP1=V_Temperat_Max%2;
					V_TEMP2=V_Temperat_Max/2;
					V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TMP];
					if((V_TEMP1==0)||(V_TEMP3!=V_TEMP2+1))
					{
						if(V_TEMP2>V_TEMP3)
						{
							ICON_ON_TDN();	//显示温度已下调
							return;
						}
						else if(V_TEMP2<V_TEMP3)
						{	
							ICON_ON_TUP();	//显示温度已上调
							return;
						}
						else
						{
							ICON_OFF_TUP();	//只显示中间图标,没有不显
							ICON_OFF_TDN();
							return;
						}
					}
					else
					{
						ICON_OFF_TUP();	//只显示中间图标,没有不显
						ICON_OFF_TDN();
						return;	
					}
				}	
				
			}
			else
			{//显示温度值
				V_TEMP1=READ_SYS_CHARACTER(V_group_num);
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
				if(((V_TEMP2&SYSFLAG_POWER)!=0)||(V_TEMP1&F_DonotClrScreen)!=0)
				{
					ICON_OFF_TUP();
					ICON_OFF_TDN();
					ICON_ON_DC_M1();		//.C
					V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TMP];
					V_TEMP4=V_TEMP4+16;	//温度变量值为0~16，对应温度显示16~25度
					HEX_TO_DEC(V_TEMP4,&V_TEMP1,&V_TEMP2,&V_TEMP3);
					DISP_ICON_Group(V_TEMP2,ST_DP_D1_M1);
					DISP_ICON_Group(V_TEMP1,ST_DP_D2_M1);
					ICON_ON_TMP_M1();		//温度
					return;
				}

			}
			
			
		}
		else
		{//显示温度值
			V_TEMP1=READ_SYS_CHARACTER(V_group_num);
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			if(((V_TEMP2&SYSFLAG_POWER)!=0)||(V_TEMP1&F_DonotClrScreen)!=0)
			{
				ICON_OFF_TUP();
				ICON_OFF_TDN();
				ICON_ON_DC_M1();		//.C
				V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TMP];
				V_TEMP4=V_TEMP4+16;	//温度变量值为0~16，对应温度显示16~25度
				HEX_TO_DEC(V_TEMP4,&V_TEMP1,&V_TEMP2,&V_TEMP3);
				DISP_ICON_Group(V_TEMP2,ST_DP_D1_M1);
				DISP_ICON_Group(V_TEMP1,ST_DP_D2_M1);
				ICON_ON_TMP_M1();		//温度
				return;
			}
		
		
		}
	
	}
	else
	{
	//温度不可调，数据又不在温度表范围内不显示
		ICON_OFF_TUP();
		ICON_OFF_TDN();
		ICON_OFF_TMP_M1();		//温度
		//DISP_ARC_TMP_VALUE_OFF();
		ICON_OFF_DC_M1();		//.C
		DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D1_M1);
		DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D2_M1);

	}
	
	
}

/***********************************************
;风量
************************************************/
void DISP_ARC_FANLEV(u16 V_group_num)
{
u8   V_TEMP1,V_TEMP2;

	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	if(((V_TEMP1&F_DonotDisp)==0)&&((VarFlag&VarFlag_USE_DISABLE)==0))
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_FANLEV];
		V_TEMP2++;
		DISP_ICON_Group(V_TEMP2,ST_DP_FANLEV_M1);
		
	}
	else
	{
		DISP_ICON_Group(DONT_DISP,ST_DP_FANLEV_M1);
	}

}

/***********************************************
;风向
************************************************/

/*--------------------------------------------------
;--------------自动手动风向显示--------------------
---------------------------------------------------*/
void DISP_AMFAN(u16 V_group_num)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_FANDIR];		//风向
	if(V_TEMP1==0)
	{
		V_TEMP2=ARC_KEY_ENCHECK(V_group_num,ARC_KEY_AFANDIR);	//按键使能判断
		if(V_TEMP2!=0)
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_AFANDIR];
			if(V_TEMP3>=6) V_TEMP3=V_TEMP3-6;
			V_TEMP3++;
			DISP_ICON_Group(V_TEMP3,ST_DP_ZDFX_M1);
			DISP_ICON_Group(DONT_DISP,ST_DP_SDFX_M1);	//不显示手动风向
		}
		else
		{	
			DISP_ICON_Group(DONT_DISP,ST_DP_ZDFX_M1);	//不显示自动风向
		}
	}
	else
	{	//手动风向
		V_TEMP2=ARC_KEY_ENCHECK(V_group_num,ARC_KEY_MFANDIR);	//按键使能判断
		if(V_TEMP2!=0)
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_MFANDIR];
			if(V_TEMP3>=6) V_TEMP3=V_TEMP3-6;
			V_TEMP3++;
			DISP_ICON_Group(V_TEMP3,ST_DP_SDFX_M1);
			DISP_ICON_Group(DONT_DISP,ST_DP_ZDFX_M1);	//不显示自动风向
		}
		else
		{
			DISP_ICON_Group(DONT_DISP,ST_DP_SDFX_M1);	//不显示手动风向
		}

	}
}
/*---------------------------------------------------------
;-------------------上下左右风向---------------------------
-----------------------------------------------------------*/
void DISP_UP_DN_LEF_RIG_FAN()
{
u8   V_TEMP1;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_AFANDIR];
	if(V_TEMP1>=6) V_TEMP1=V_TEMP1-5;
	V_TEMP1++;
	DISP_ICON_Group(V_TEMP1,ST_DP_ZDFX_M1);		//左右风向
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_MFANDIR];
	if(V_TEMP1>=6) V_TEMP1=V_TEMP1-5;
	V_TEMP1++;
	DISP_ICON_Group(V_TEMP1,ST_DP_SDFX_M1);		//上下风向
}

/*-------------------------------------------------------------------*/
void DISP_ARC_FANDIR(u16 V_group_num)
{
u8   V_TEMP1;

	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	if(((V_TEMP1&F_DonotDisp)==0)&&((VarFlag&VarFlag_USE_DISABLE)==0))
	{
		if((V_TEMP1&F_FANDIR_LR)==0)
			DISP_AMFAN(V_group_num);	//自动手动风向显示
		else
			DISP_UP_DN_LEF_RIG_FAN();	//上下左右风向
			
	}
	else
	{
		DISP_ICON_Group(DONT_DISP,ST_DP_SDFX_M1);
		DISP_ICON_Group(DONT_DISP,ST_DP_ZDFX_M1);
	}
	
}

/*************************************************
;定时关显示
;input: ADR_TIMEOFF,ADR_TMOFF_10MINS,ADR_TMSTATE3
;output: none
*************************************************/
/*----------------------------------------------
;显示定时关小时
-----------------------------------------------*/
void DISP_TMOFF_HOUR(u8 V_TMOffHours)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3;

	HEX_TO_DEC(V_TMOffHours,&V_TEMP1,&V_TEMP2,&V_TEMP3);
	DISP_ICON_Group(V_TEMP1,ST_DP_D4_M1);
	DISP_ICON_Group(V_TEMP2,ST_DP_D3_M1);
	
}
/*-----------------------------------------------------------
;显示定时关分钟
;input: x
;output: none
;-----------------------------------------------------------*/
/*void DISP_TMOFF_MIN(u8 V_TMOff10Min)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3,V_TMOffMin;

	V_TMOffMin=MIN_CHANGE_TAB[V_TMOff10Min];
	HEX_TO_DEC(V_TMOffMin,&V_TEMP1,&V_TEMP2,&V_TEMP3);
	DISP_ICON_Group(V_TEMP1,ST_DP_D6_M1);
	DISP_ICON_Group(V_TEMP2,ST_DP_D5_M1);
	
}*/
/*----------------------------------------------------------
;10分钟变量转小时变量+10份钟变量
;input:V_In10Min
;output: VP_Out10Min(10分钟)，VP_OutHours（小时）
;----------------------------------------------------------*/
/*void TenMinS_TURN_HOURS(u8 V_In10Min,u8 *VP_Out10Min,u8 *VP_OutHours)
{
u8   V_OutHours=0;

	while(V_In10Min>=6)
	{
		V_In10Min=V_In10Min-6;
		V_OutHours++;
	}
	if(V_OutHours>=24) V_OutHours=V_OutHours-24;
	
	*VP_OutHours=V_OutHours;
	*VP_Out10Min=V_In10Min;
}*/
//-----------------------------------------------
//显示定关时间
//-----------------------------------------------
void DISP_ARC_TMOFF_ON(u16 V_group_num)
{
u8   V_TEMP2;


//	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
//	V_TEMP1=V_TEMP1&TM_MODE_MASK;
//	if(V_TEMP1==CON_TM_MODE_OPPOSITE)
//	{	//相对时间
		ICON_OFF_DOT2_M1();
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
		DISP_TMOFF_HOUR(V_TEMP2);
		DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D6_M1);
		DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D5_M1);

//	}
/*	else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
	{	//;显示为绝对定时间，发码也为绝对定时
		ICON_ON_DOT2_M1();
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
		DISP_TMOFF_HOUR(V_TEMP2);
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
		DISP_TMOFF_MIN(V_TEMP2);
	}
	else
	{ //显示为绝对时间,发码为相对时间
		ICON_ON_DOT2_M1();
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
		TenMinS_TURN_HOURS(V_TEMP2,&V_TEMP3,&V_TEMP4);
		DISP_TMOFF_HOUR(V_TEMP4);
		DISP_TMOFF_MIN(V_TEMP3);
	}
*/	
}

/*----------------------------------------------------------------------------*/
void DISP_ARC_TMOFF(u16 V_group_num)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3;

	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if(((V_TEMP1&F_SleepNHours)!=0)&&((V_TEMP2&SYSFLAG_SLEEP)!=0)) return;	//显示睡眠小时，不显示定关
	if(TmClk_SetState!=TmClk_SetState_TMOFF)
	{
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		if((V_TEMP3&ARC_T3_TIMEOFF)!=0)
		{
			ICON_ON_TMOFF_M1();
			ICON_OFF_SETTMOFF_M1();
			DISP_ARC_TMOFF_ON(V_group_num);
		}
		else
		{
			ICON_OFF_DOT2_M1();		//不显示
			ICON_OFF_TMOFF_M1();
			ICON_OFF_SETTMOFF_M1();
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D4_M1);
			DISP_ICON_Group(0,ST_DP_D3_M1);	//灭
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D6_M1);
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D5_M1);
			
		}
	}
	else
	{
		if((VarFlag&VarFlag_FLASH)==0) 
		{
			ICON_ON_SETTMOFF_M1();
			ICON_OFF_TMOFF_M1();
			DISP_ARC_TMOFF_ON(V_group_num);
		}
		else
		{
			ICON_OFF_DOT2_M1();		//不显示
			ICON_OFF_TMOFF_M1();
			ICON_OFF_SETTMOFF_M1();
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D4_M1);
			DISP_ICON_Group(0,ST_DP_D3_M1);	//灭
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D6_M1);
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D5_M1);

		}
	
	}

}


/***************************************************
;定时开显示
;input: ADR_TIMEON,ADR_TMON_10MINS,ADR_TMSTATE3
;output: none
****************************************************/
/*----------------------------------------------
;显示定时开小时
-----------------------------------------------*/
void DISP_TMON_HOUR(u8 V_TMOnHours)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3;
	
	HEX_TO_DEC(V_TMOnHours,&V_TEMP1,&V_TEMP2,&V_TEMP3);
	DISP_ICON_Group(V_TEMP1,ST_DP_D8_M1);
	DISP_ICON_Group(V_TEMP2,ST_DP_D7_M1);
}

/*-----------------------------------------------------------
;显示定时开分钟
;input: x
;output: none
;-----------------------------------------------------------*/
/*void DISP_TMON_MIN(u8 V_TMOn10Min)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3,V_TMOnMin;

	V_TMOnMin=MIN_CHANGE_TAB[V_TMOn10Min];
	HEX_TO_DEC(V_TMOnMin,&V_TEMP1,&V_TEMP2,&V_TEMP3);
	DISP_ICON_Group(V_TEMP1,ST_DP_D10_M1);
	DISP_ICON_Group(V_TEMP2,ST_DP_D9_M1);
	
}*/
//-------------------------------
//显示定开时间
//-------------------------------
void DISP_ARC_TMON_ON(u16 V_group_num)
{
u8   V_TEMP2;


//	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
//	V_TEMP1=V_TEMP1&TM_MODE_MASK;
//	if(V_TEMP1==CON_TM_MODE_OPPOSITE)
//	{	//相对时间
		ICON_OFF_DOT1_M1();
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
		DISP_TMON_HOUR(V_TEMP2);
		DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D10_M1);
		DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D9_M1);

//	}
/*	else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
	{	//;显示为绝对定时间，发码也为绝对定时
		ICON_ON_DOT1_M1();
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
		DISP_TMON_HOUR(V_TEMP2);
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
		DISP_TMON_MIN(V_TEMP2);
	}
	else
	{ //显示为绝对时间,发码为相对时间
		ICON_ON_DOT1_M1();
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
		TenMinS_TURN_HOURS(V_TEMP2,&V_TEMP3,&V_TEMP4);
		DISP_TMON_HOUR(V_TEMP4);
		DISP_TMON_MIN(V_TEMP3);
	}
*/	
}

/*-----------------------------------------------------------------------*/
void DISP_ARC_TMON(u16 V_group_num)
{
u8   V_TEMP1;

	if(TmClk_SetState!=TmClk_SetState_TMON)
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		if((V_TEMP1&ARC_T3_TIMEON)!=0) 
		{
			ICON_ON_TMON_M1();
			ICON_OFF_SETTMON_M1();
			DISP_ARC_TMON_ON(V_group_num);
		}
		else
		{
			ICON_OFF_DOT1_M1();		//不显示
			ICON_OFF_TMON_M1();
			ICON_OFF_SETTMON_M1();
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D8_M1);
			DISP_ICON_Group(0,ST_DP_D7_M1);	//灭
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D10_M1);
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D9_M1);
			
		}
	}
	else
	{
		if((VarFlag&VarFlag_FLASH)==0) 
		{
			ICON_ON_SETTMON_M1();
			ICON_OFF_TMON_M1();
			DISP_ARC_TMON_ON(V_group_num);
		}
		else
		{
			ICON_OFF_DOT1_M1();		//不显示
			ICON_OFF_TMON_M1();
			ICON_OFF_SETTMON_M1();
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D8_M1);
			DISP_ICON_Group(0,ST_DP_D7_M1);	//灭
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D10_M1);
			DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D9_M1);

		}
	
	}

}

/*;==================================
;睡眠显示
;==================================*/
void DISP_ARC_SLEEP(u16 V_group_num)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3,V_TEMP4;

	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	if((V_TEMP1&F_DonotDisp)!=0) return;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if((V_TEMP1&SYSFLAG_SLEEP)!=0)
	{
		ICON_ON_DC_SLEEP();
		V_TEMP2=READ_SYS_CHARACTER(V_group_num);
		if((V_TEMP2&F_SleepNHours)==0) return;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		if((V_TEMP2&ARC_T3_TIMEOFF)==0)
		{	//显示睡眠时间
			//SET睡眠时间ICON
			DISP_ON_SLEEP_ICON();
			V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
			HEX_TO_DEC(V_TEMP1,&V_TEMP2,&V_TEMP3,&V_TEMP4);
			DISP_ICON_Group(V_TEMP2,ST_DP_D6_M1);
			if(V_TEMP3!=0)  
			{
				DISP_ICON_Group(V_TEMP3,ST_DP_D5_M1);
				return;
			}
			else
			{
				DISP_ICON_Group(INDEX_DX_OFF,ST_DP_D5_M1);
				return;
			}

		}
		
	}
	DISP_OFF_SLEEP_ICON();
	ICON_OFF_DC_SLEEP();
	
}
/*==================================
;显示电辅热
;==================================*/
void DISP_ARC_HEAT()
{
u8   V_TEMP1;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if((V_TEMP1&SYSFLAG_HEAT)==0)
		ICON_OFF_DC_HEAT();
	else
		ICON_ON_DC_HEAT();
}
/*==================================
;显示强力
;==================================*/
void DISP_ARC_STRONG()
{
u8   V_TEMP1;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if((V_TEMP1&SYSFLAG_STRONG)==0)
		ICON_OFF_DC_STRONG();
	else
		ICON_ON_DC_STRONG();
}
/*==================================
;显示灯光
;==================================*/
void DISP_ARC_LIGHT()
{
u8   V_TEMP1;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if(((V_TEMP1&SYSFLAG_LIGHT)==0)||((VarFlag&VarFlag_DISP_L)==0))
		ICON_OFF_DC_LIGHT();
	else
		ICON_ON_DC_LIGHT();
}
/*==================================
;显示空清
;==================================*/
void DISP_ARC_AIRCLEAR()
{
u8   V_TEMP1;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if((V_TEMP1&SYSFLAG_AIRCLEAR)==0)
		ICON_OFF_DC_AIRCLEAR();
	else
		ICON_ON_DC_AIRCLEAR();
}
/*;==================================
;显示经济
;==================================*/
void DISP_ARC_ECONOMIC()
{
u8   V_TEMP1;

	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if((V_TEMP1&SYSFLAG_ECONOMIC)==0)
		ICON_OFF_DC_ECONOMIC();
	else
		ICON_ON_DC_ECONOMIC();
}
/*========================================
;十六进制转十进制
;input:    VarTemp0,VarTemp1
;output:	*VP_ones(个位),*VP_tens(十位),*VP_hundreds(百位),*VP_thousands(千位)
;========================================*/
/*
void HEX_TO_DEC2(u16 V_data,u8 *VP_ones,u8 *VP_tens,u8 *VP_hundreds,u8 *VP_thousands )
{
u8 V_tens=0,V_hundreds=0,V_thousands=0;

	while(V_data>=1000)
	{
		V_data=V_data-1000;
		V_thousands++;
	}
		while(V_data>=100)
	{
		V_data=V_data-100;
		V_hundreds++;
	}
	
	while(V_data>=10)
	{
		V_data=V_data-10;
		V_tens++;
	}
	
	*VP_ones=V_data;
	*VP_tens=V_tens;
	*VP_hundreds=V_hundreds;
	*VP_thousands=V_thousands;

}
*/
/*====================================================================================
;	public function
;====================================================================================*/
//发码标志
void ICON_ON_SendCode_M1(u8 *VP_arc_disp)
{
	u8 i;
	LCD_ICON_On(ComBase*0+SegBase*20);
	for(i=0;i<16;i++)
	{
		*VP_arc_disp=Ar_Disp_DATA[i];
		VP_arc_disp++;
	}
}
void ICON_OFF_SendCode_M1(u8 *VP_arc_disp)
{
	u8 i;
	LCD_ICON_Off(ComBase*0+SegBase*20);
	for(i=0;i<16;i++)
	{
		*VP_arc_disp=Ar_Disp_DATA[i];
		VP_arc_disp++;
	}
}
//---------------------------------------



/***************************************************************************
;	函数入口参数
*****************************************************************************/
void F_DISP_PARA_init()
{
	ST_DP_D1_M1.V_SegNum=7;		//段数(共7段)
	ST_DP_D1_M1.V_AddrOff=0;	//地址便宜值(主要用于7段数据显示)
	ST_DP_D1_M1.VP_SegCom=&TAB_SEGCOM_D1_M1[0];
	ST_DP_D1_M1.VP_rule=&TAB_PTN_DIGIT[0];
	
	ST_DP_D2_M1.V_SegNum=7;		//段数(共7段)
	ST_DP_D2_M1.V_AddrOff=0;	//地址便宜值(主要用于7段数据显示)
	ST_DP_D2_M1.VP_SegCom=&TAB_SEGCOM_D2_M1[0];
	ST_DP_D2_M1.VP_rule=&TAB_PTN_DIGIT[0];

	ST_DP_D8_M1.V_SegNum=7;
	ST_DP_D8_M1.V_AddrOff=0;
	ST_DP_D8_M1.VP_SegCom=&TAB_SEGCOM_D8_M1[0];
	ST_DP_D8_M1.VP_rule=&TAB_PTN_DIGIT[0];

	ST_DP_D9_M1.V_SegNum=7;
	ST_DP_D9_M1.V_AddrOff=2;
	ST_DP_D9_M1.VP_SegCom=&TAB_SEGCOM_D8_M1[0];
	ST_DP_D9_M1.VP_rule=&TAB_PTN_DIGIT[0];
	
	ST_DP_D10_M1.V_SegNum=7;
	ST_DP_D10_M1.V_AddrOff=4;
	ST_DP_D10_M1.VP_SegCom=&TAB_SEGCOM_D8_M1[0];
	ST_DP_D10_M1.VP_rule=&TAB_PTN_DIGIT[0];

	ST_DP_D4_M1.V_SegNum=7;
	ST_DP_D4_M1.V_AddrOff=7;
	ST_DP_D4_M1.VP_SegCom=&TAB_SEGCOM_D8_M1[0];
	ST_DP_D4_M1.VP_rule=&TAB_PTN_DIGIT[0];

	ST_DP_D5_M1.V_SegNum=7;
	ST_DP_D5_M1.V_AddrOff=9;
	ST_DP_D5_M1.VP_SegCom=&TAB_SEGCOM_D8_M1[0];
	ST_DP_D5_M1.VP_rule=&TAB_PTN_DIGIT[0];

	ST_DP_D6_M1.V_SegNum=7;
	ST_DP_D6_M1.V_AddrOff=11;
	ST_DP_D6_M1.VP_SegCom=&TAB_SEGCOM_D8_M1[0];
	ST_DP_D6_M1.VP_rule=&TAB_PTN_DIGIT[0];

	ST_DP_MODE_M1.V_SegNum=5;
	ST_DP_MODE_M1.V_AddrOff=0;
	ST_DP_MODE_M1.VP_SegCom=&TAB_SEGCOM_MODE_M1[0];
	ST_DP_MODE_M1.VP_rule=&TAB_PTN_MODE[0];

	ST_DP_FANLEV_M1.V_SegNum=4;
	ST_DP_FANLEV_M1.V_AddrOff=0;
	ST_DP_FANLEV_M1.VP_SegCom=&TAB_SEGCOM_FS_M1[0];
	ST_DP_FANLEV_M1.VP_rule=&TAB_PTN_FS[0];

	ST_DP_SDFX_M1.V_SegNum=6;
	ST_DP_SDFX_M1.V_AddrOff=0;
	ST_DP_SDFX_M1.VP_SegCom=&TAB_SEGCOM_SDFX_M1[0];
	ST_DP_SDFX_M1.VP_rule=&TAB_PTN_FX[0];

	ST_DP_ZDFX_M1.V_SegNum=6;
	ST_DP_ZDFX_M1.V_AddrOff=0;
	ST_DP_ZDFX_M1.VP_SegCom=&TAB_SEGCOM_ZDFX_M1[0];
	ST_DP_ZDFX_M1.VP_rule=&TAB_PTN_FX[0];

	ST_DP_D7_M1.V_SegNum=3;
	ST_DP_D7_M1.V_AddrOff=0;
	ST_DP_D7_M1.VP_SegCom=&TAB_SEGCOM_D7_M1[0];
	ST_DP_D7_M1.VP_rule=&TAB_PTN_DIGIT2[0];

	ST_DP_D3_M1.V_SegNum=3;
	ST_DP_D3_M1.V_AddrOff=7;        //0;    2012-9-1 16:21
	ST_DP_D3_M1.VP_SegCom=&TAB_SEGCOM_D7_M1[0];
	ST_DP_D3_M1.VP_rule=&TAB_PTN_DIGIT2[0];
}

/*******************************************************
空调显示处理程序
说明：根据空调变量刷新显示映射区。
*******************************************************/
void ARC_DISPLAY_PRO(u16 V_group_num,u8 *VP_arc_disp)
{
u8   i,V_TEMP1,V_TEMP2;
u16 	characteristic_id;

	if(V_group_num >= EXT_FILE_DATA) characteristic_id = V_group_num;
	else characteristic_id = GROUP_CHANG_Array[V_group_num];
	
	F_DISP_PARA_init();
	V_TEMP1=READ_SYS_CHARACTER(characteristic_id);
	V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if(((V_TEMP1&F_DonotClrScreen)==0)&&((V_TEMP2&SYSFLAG_POWER)==0))
		VarFlag=VarFlag|VarFlag_USE_DISABLE;
	else	VarFlag=VarFlag&(~VarFlag_USE_DISABLE);
	
	DISP_ARC_MODE(characteristic_id);
	DISP_ARC_TEMP(characteristic_id);
	DISP_ARC_FANLEV(characteristic_id);		//风量
	DISP_ARC_FANDIR(characteristic_id);		//风向
	DISP_ARC_SLEEP(characteristic_id);		//睡眠
	DISP_ARC_HEAT();		//电辅热
	DISP_ARC_STRONG();		//强力
	DISP_ARC_LIGHT();		//灯光
	DISP_ARC_AIRCLEAR();	//空清
	DISP_ARC_ECONOMIC();	//经济
	DISP_ARC_TMOFF(characteristic_id);		//定时关
	DISP_ARC_TMON(characteristic_id);		//定时开


	for(i=0;i<16;i++)
	{
		*VP_arc_disp=Ar_Disp_DATA[i];
		VP_arc_disp++;
	}
	//return  &Ar_Disp_DATA[0];
}




