/***************************************************************************
;========================================================================
;Company	:	HBG      Copyright (c) 2011
;Author		:	Kalvin
;��Ȩ��Ϣ	:	Copyright (c) 2011 HBG Hi Tech
;File		: 	ARC_DAT_PRO.C
;Development	: 	VC++ 6.0
;Data		:	2012-11
;Customer	:	�Ʋ���
;-------------------------History list-----------------------------------
*****************************************************************************
*****************************************************************************/
#include "remote.h"
/*****************************************************************************
		const define
*****************************************************************************/
const u8 MIN_CHANGE_TAB[]={0,10,20,30,40,50,60};		//����ת��
//����ʹ�ܱ�
//bit0:�Զ�����bit1:˯�ߣ�bit2:�縨�ȣ�bit3:ǿ����bit4:�ƹ⣬bit5:����/����/����,bit6:����,bit7:�ֶ�����
const u8 KEY_CHOICE_TAB[]={ARC_KEY_AFANDIR,ARC_KEY_SLEEP,ARC_KEY_HEAT,ARC_KEY_STRONG,ARC_KEY_LIGHT,ARC_KEY_AIRCLEAR,ARC_KEY_ECONOMIC,ARC_KEY_MFANDIR};
/*****************************************************************************
   		variable  define
*****************************************************************************/
//u8 VarState_KeyPro;			//��������״̬��û�а�������ʱҪ��0        ��0��û�д���
u8 	VarFlag;
u8	ADR_TMP_BAK;		//�¶ȱ���
u8	ADR_FANLEV_BAK;		//��������
u8	TmClk_SetState;		//��ʱ��ʱ�ӣ�����״̬
u8	TmMin;			//��ʱ�ķ��Ӽ�����
u8	ADR_VAR[ADR_VAR_LEN];		//���Ʊ�������
u8   ARC_TIMEOFF_BAK,ARC_TIMEON_BAK;
extern type_updatatime ARC_updataTM;
extern int	 devNo_FARC;
/*******************************************************************************
   		����
*****************************************************************************/
//extern const unsigned int TEMP_Bound_Array[][2];
//extern const unsigned int SYS_CHARACTER_Array[];
//extern const unsigned int KEY_ENABLE_Array[];
//extern const unsigned int TIME_CHARACTER_Array[];
//extern 	const u16  ARC_FILE_ADDR[];
//;=========================================================
//;�����־
//;input: V_Skey_val
//;output:VarFlag,ADR_VAR[]
//;=========================================================

void SET_VarFlag_SEND(u8 V_Skey_val)
{
	VarFlag=VarFlag|VarFlag_SEND;
	ADR_VAR[ADR_VAR_START+ADR_KEYVAL]=V_Skey_val;

}

/*========================================
;ʮ������תʮ����		
;input:	ACC
;output:	VarTemp0(��λ),VarTemp1(ʮλ),VarTemp2(��λ)
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
; ��ȡϵͳ��������ʾ+˯�� ��ʽ��־��
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
; ��ȡ��ʱ��������ʱ��ʽ����ʱ���ã�
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
;��Դ������
;˵��:	1.����ػ�
;	2.ȡ����ʱ
;input:	ADR_SYSFLAG,ADR_TMSTATE7,ADR_TMSTATE3
;output:	ADR_SYSFLAG,ADR_TMSTATE7,ADR_TMSTATE3
;===========================================================*/
/*-----------------�����ʱ����----------------------------*/
void	CLR_TIME_SETTING(void)
{
u8	V_TEMP1;

	if(TmClk_SetState==TmClk_SetState_TMON)
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE7];
		V_TEMP1=V_TEMP1&(~ARC_T7_TIMEONP);		//��򿪶�����־
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=V_TEMP1;
		
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		V_TEMP1=V_TEMP1&(~ARC_T3_TIMEON);		//���ж�����־
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
		
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP1=V_TEMP1&(~SYSFLAG_TIME);		//���ж�ʱ��־
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
		
		ADR_VAR[ADR_VAR_START+ADR_TIMEON] = 0;
		TmClk_SetState=0;
	}
	else if(TmClk_SetState==TmClk_SetState_TMOFF)
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE7];
		V_TEMP1=V_TEMP1&(~ARC_T7_TIMEOFFP);		//��򿪶�����־
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=V_TEMP1;
		
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		V_TEMP1=V_TEMP1&(~ARC_T3_TIMEOFF);		//���ж�����־
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
		
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP1=V_TEMP1&(~SYSFLAG_TIME);		//���ж�ʱ��־
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
		
		ADR_VAR[ADR_VAR_START+ADR_TIMEOFF] = 0;
		TmClk_SetState=0;
	}

	
}
/***********************************************************
-----------ȡ����ʱ��--------------------------------
***********************************************************/
void USE_TIMEON_CANCEL(u16 V_group_num,u8 key_val)
{
u8 V_TEMP1,V_TEMP2;

	ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEONCL;	//ȡ������
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	V_TEMP1=V_TEMP1&(~ARC_T3_TIMEON);
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
	if(V_TEMP1==0)
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2&(~SYSFLAG_TIME);		//clr ��ʱ��־
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	key_val=ARC_KEY_TIMECL;
/*	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
	V_TEMP1=V_TEMP1&TM_SET_MASK;
	if(V_TEMP1==CON_TM_MODE_FABSOLUTE)
	{	//��ʾΪ����ʱ��,����Ϊ���ʱ�䣨ʱ��
		ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=0;
		ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR]=0;
		ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN]=0;
	}
	else if(V_TEMP1!=CON_TM_MODE_ABSOLUTE)*/
//	{	//����Զ�ʱ��ʱ��
		ADR_VAR[ADR_VAR_START+ADR_TIMEON]=0;
//	}
	JUDGE_POWER_ONOFF(V_group_num);
	SET_VarFlag_SEND(key_val);
}
/**********************************************************
;-----------ȡ����ʱ��--------------------------------
***********************************************************/
void USE_TIMEOFF_CANCEL(u16 V_group_num,u8 key_val)
{
u8 V_TEMP1,V_TEMP2;

	ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFCL;
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	V_TEMP1=V_TEMP1&(~ARC_T3_TIMEOFF);		//ȡ������
	ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
	if(V_TEMP1==0)
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2&(~SYSFLAG_TIME);	//clr ��ʱ��־
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	key_val=ARC_KEY_TIMECL;
/*	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
	V_TEMP1=V_TEMP1&TM_SET_MASK;
	if(V_TEMP1==CON_TM_MODE_FABSOLUTE)
	{  //��ʾΪ����ʱ��,����Ϊ���ʱ�䣨ʱ��
	
		ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=0;
		ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR]=0;
		ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN]=0;
	}
	else if(V_TEMP1!=CON_TM_MODE_ABSOLUTE)*/
//	{	//����Զ�ʱ��ʱ��
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
		V_TEMP1=V_TEMP1|SYSFLAG_POWER;				//����
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
		V_TEMP1=V_TEMP1&(~SYSFLAG_POWER);				//�ػ�
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
	if((V_TEMP1&ARC_T3_TIMEOFF)!=0) USE_TIMEOFF_CANCEL(V_group_num,key_val);	//ȡ����ʱ��
	if((V_TEMP1&ARC_T3_TIMEON)!=0) USE_TIMEON_CANCEL(V_group_num,key_val);	//ȡ����ʱ��

	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	if(((V_TEMP1&F_DonotClrScreen)==0)&&((V_TEMP2&SYSFLAG_POWER)==0))
		VarFlag=VarFlag|VarFlag_USE_DISABLE;
	else	VarFlag=VarFlag&(~VarFlag_USE_DISABLE);

	key_val=ARC_KEY_POWER;
	SET_VarFlag_SEND(key_val);
}

/*===========================================================
;��ȡ�Զ�����ʪ ģʽ���¶ȷ�Χ
;input: ADR_MODE
;output:IR_DRV_vtmp0(��Сֵ),IR_DRV_vtmp1(���ֵ)
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
	
	V_tempmax=V_TEMP1>>4;	//���ֵ
	V_tempmin=V_TEMP1&0x0f;	//��Сֵ
			
	*VP_temp_max=V_tempmax;
	*VP_temp_min=V_tempmin;
	
}

/*===========================================================
;�¶ȼӴ���
;˵��:;1.ÿ��һ���¶ȼ�1,��������
;	  2.�ͷ�ģʽ�¶ȹ̶�Ϊ25��
;	  3.���䣬��ů �¶ȱ�����ֵ��Χ0~16��ʾ16~32���϶�
;	  4.�Զ�����ʪģʽ�����¶ȷ�Χ��"TEMP_MAX_TAB"���Ƶķ�Χ�ڵ���
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
					ADR_TMP_BAK=V_TEMP1;	//�����¶�
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
					ADR_TMP_BAK=ADR_VAR[ADR_VAR_START+ADR_TMP];	//�����¶�
				}
				
			}
			else
			{
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMP];
				if(V_TEMP2<16)	//32-16
				{
					V_TEMP2++;
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
					ADR_TMP_BAK=ADR_VAR[ADR_VAR_START+ADR_TMP];	//�����¶�
				}
			
			}
			SET_VarFlag_SEND(key_val);
			break;
			
		//������ʱ��/ʱ��
		case	TmClk_SetState_TMOFF:		//���ص���+
			//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
			//V_TEMP1=V_TEMP1&TM_MODE_MASK;
			//if(V_TEMP1==CON_TM_MODE_OPPOSITE)
			//{	//���ʱ��
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
				if(V_TEMP2<12) 
				{
					V_TEMP2++;
					ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
				}
				//return;
			//}
/*			else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
			{	//��ʾΪ����ʱ��,����ҲΪ����ʱ��
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
			{	//��ʾΪ����ʱ��,����Ϊ���ʱ��
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
				V_TEMP2++;
				if(V_TEMP2>=144)	//24*6=144
					V_TEMP2=0;
				ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
				
				//return;
			}
*/			
			break;
		case	TmClk_SetState_TMON:		//��������+
			//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
			//V_TEMP1=V_TEMP1&TM_MODE_MASK;
			//if(V_TEMP1==CON_TM_MODE_OPPOSITE)
			//{	//���ʱ��
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
				if(V_TEMP2<12) 
				{
					V_TEMP2++;
					ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
				}
				//return;
			//}
/*			else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
			{	//��ʾΪ����ʱ��,����ҲΪ����ʱ��
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
			{	//��ʾΪ����ʱ��,����Ϊ���ʱ��
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
;�¶ȼ�����
;˵��:1.ÿ��һ���¶ȼ�1,��������
;	  2.�ͷ�ģʽ�¶ȹ̶�Ϊ25��
;	  3.���䣬��ů �¶ȱ�����ֵ��Χ0~16��ʾ16~32���϶�
;	  4.�Զ�����ʪģʽ�����¶ȷ�Χ��"TEMP_MAX_TAB"���Ƶķ�Χ�ڵ���
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
					ADR_TMP_BAK=V_TEMP1;	//�����¶�
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
					ADR_TMP_BAK=ADR_VAR[ADR_VAR_START+ADR_TMP];	//�����¶�
				}
				
			}
			else
			{
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMP];
				if(V_TEMP2>0)
				{
					V_TEMP2--;
					ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
					ADR_TMP_BAK=ADR_VAR[ADR_VAR_START+ADR_TMP];	//�����¶�
				}
			
			}
			SET_VarFlag_SEND(key_val);
			break;
			
		//������ʱ��/ʱ��
		case	TmClk_SetState_TMOFF:		//���ص���-
			TmMin=0;
			//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
			//V_TEMP1=V_TEMP1&TM_MODE_MASK;
			//if(V_TEMP1==CON_TM_MODE_OPPOSITE)
			//{	//���ʱ��
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
				if(V_TEMP2>1) 
				{
					V_TEMP2--;
					ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
				}
				//return;
			//}
/*			else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
			{	//��ʾΪ����ʱ��,����ҲΪ����ʱ��
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
			{	//��ʾΪ����ʱ��,����Ϊ���ʱ��
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
		case	TmClk_SetState_TMON:		//��������-
			TmMin=0;
			//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
			//V_TEMP1=V_TEMP1&TM_MODE_MASK;
			//if(V_TEMP1==CON_TM_MODE_OPPOSITE)
			//{	//���ʱ��
				V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
				if(V_TEMP2>1) 
					V_TEMP2--;
				//else
				//	V_TEMP2=23;
					ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
	
				//return;
			//}
/*			else if(V_TEMP1==CON_TM_MODE_ABSOLUTE)
			{	//��ʾΪ����ʱ��,����ҲΪ����ʱ��
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
			{	//��ʾΪ����ʱ��,����Ϊ���ʱ��
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
;ģʽ����
;˵��:1.ÿ��һ��ģʽ��1,����ģʽѭ��
;	  2.ÿ��ģ���¶�ֵ,����ֵ�Ƿ�Ҫ��������??
;input:	ADR_MODE
;output:	ADR_TMP,ADR_MODE
;===========================================================*/
void USE_MODE_KEY_PRO(u16 V_group_num,u8 key_val)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3,V_Temperat_Min,V_Temperat_Max;

	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	
	ADR_VAR[ADR_VAR_START+ADR_TMP]=ADR_TMP_BAK;	//�ָ��趨�¶�
	ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ADR_FANLEV_BAK;
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	V_TEMP1=V_TEMP1&(~(SYSFLAG_HEAT+SYSFLAG_STRONG+SYSFLAG_ECONOMIC));	//ȡ���縨�ȣ�ǿ��������
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
			V_TEMP2=(V_Temperat_Min+V_Temperat_Max)/2;	//�м��¶�ֵ
			ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
		}
		
	}
	else
	{
		V_TEMP1++;
		ADR_VAR[ADR_VAR_START+ADR_MODE]=V_TEMP1;
		if(V_TEMP1==ARC_MODE_FAN)
		{	//�ͷ�ģʽ
			ADR_VAR[ADR_VAR_START+ADR_TMP]=25-16;	//20120901
			if(ADR_FANLEV_BAK==ARC_FANLEV_1)
				ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ARC_FANLEV_2;
			else
				ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ADR_FANLEV_BAK;
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP2=V_TEMP2&(~SYSFLAG_SLEEP);			//�ͷ� ģʽ��˯��
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;

		}
		else if(V_TEMP1==ARC_MODE_DHMD)
		{	//��ʪģʽ
			V_TEMP3=READ_SYS_CHARACTER(V_group_num);
			ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ARC_FANLEV_2;		//�ͷ���
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			if(((V_TEMP2&SYSFLAG_SLEEP)!=0)&&((V_TEMP3&F_SleepNHours)!=0))  ADR_VAR[ADR_VAR_START+ADR_TIMEOFF] = 0;
			V_TEMP2=V_TEMP2&(~SYSFLAG_SLEEP);			//��ʪ ģʽ��˯��
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
		
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
			READ_TEMP_BOUND(V_group_num,V_TEMP2,&V_Temperat_Min,&V_Temperat_Max);	//*/
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMP];
			if((V_TEMP2<V_Temperat_Min)||(V_TEMP2>V_Temperat_Max))
			{
				V_TEMP2=(V_Temperat_Min+V_Temperat_Max)/2;	//�м��¶�ֵ
				ADR_VAR[ADR_VAR_START+ADR_TMP]=V_TEMP2;
			}
			
			
		}
	}
	SET_VarFlag_SEND(key_val);
	//return;
}

/*===========================================================
;��������
;˵��:1.ÿ��һ�η���ֵ��1,���ַ���ѭ��
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
		{	//�ͷ�ģʽ	�͡��С���
			
			ADR_VAR[ADR_VAR_START+ADR_FANLEV]=ARC_FANLEV_2;
			ADR_FANLEV_BAK=ARC_FANLEV_2;
		}
	
	}
	SET_VarFlag_SEND(key_val);
}
	
/*===========================================================
;�ֶ�������
;˵��:1.ÿ��һ��ADR_MFANDIR,ADR_FANDIRֵ��1,���ַ���ѭ��
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
;�Զ�������
;˵��:1.ÿ��һ��ADR_AFANDIRֵ��1,���ַ���ѭ��,ADR_FANDIR����
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
;��ʱ�����ô���
;˵��:���������
;	1.��Զ�ʱ����С��λΪ1Сʱ����ʱ������ ADR_TIMEON (��λСʱ)��,ͬʱ���ݵ�ADR_TMON_10MINS(��λСʱ)��;
;	2.��ʾ���Զ�ʱ��ʵ�ʷ���ҲΪ���Զ�ʱ��ʱ������ ADR_TIMEON��Сʱ���� ADR_TMON_10MINS��10���ӣ���;
;	3.��ʾ���Զ�ʱ��ʵ�ʷ���Ϊ��Զ�ʱ��ʱ������  ADR_TMON_10MINS����λ10���ӣ���,ͬʱ���ݵ�ADR_TIMEON����λ10���ӣ���;
;===========================================================*/
/*----------------------------------------------------------------------
;Сʱ,���� ת10����,;��������ʱ��
;input:	VarTemp1 (����),VarTemp2(Сʱ),VarTemp3(��ǰ�Ķ�ʱ����ʱ�䵥λΪ10����)
;output:	V_Time_10Min(10����)
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
;��10����Ϊ��λ��ʱ���ֵ��ת��ΪСʱ������
;input:	ACC
;output:	u8 * V_Hour_point(Сʱֵ),u8 * V_Min_point(����ֵ)
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
;��Զ�ʱ�������ؿ�ͬʱ�跽ʽCON_TM_SET_TWO_PT��ȷ����ʱʱ�Ŀ��ػ�����
;��������ͬʱ�裬������ǰ�ػ���������ǰ����
;���趨���ػ������趨�ؿ���
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
			V_TEMP3=V_TEMP3&(~SYSFLAG_POWER);		//������ǰ���ػ�
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
		else
		{
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3|SYSFLAG_POWER;			//������ǰ������
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
	}
	else if(V_TEMP2==ARC_T3_TIMEOFF)
	{
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP3=V_TEMP3|SYSFLAG_POWER;			//������ǰ������
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
	}
	else
	{
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP3=V_TEMP3&(~SYSFLAG_POWER);		//������ǰ���ػ�
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
	}
	
}



/*-----------------------------------------------------------------------
;���Զ�ʱ�������ؿ�ͬʱ�跽ʽCON_TM_SET_TWO_PT��ȷ����ʱʱ�Ŀ��ػ�����
;��������ͬʱ�裬������ǰ�ػ���������ǰ����
;���趨���ػ������趨�ؿ���
;input: VarTemp0(TIMEON MIN),VarTemp1(TIMEON HOUR),VarTemp2(TIMEOFF MIN),VarTemp3(TIMEOFF HOUR)
;output: ADR_VAR_START+ADR_SYSFLAG
;-----------------------------------------------------------------------*/
//----------�����ʱ��תΪ����ʱ��---------------------------------
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
		{	//������ǰ������
			
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3|SYSFLAG_POWER;
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
		else
		{	//������ǰ���ػ�
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3&(~SYSFLAG_POWER);
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
	}
	else
	{
		if(V_TEMP2==ARC_T3_TIMEOFF)
		{	//������ǰ������
			
			V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP3=V_TEMP3|SYSFLAG_POWER;
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP3;
		}
		else
		{	//������ǰ���ػ�
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
		//����ʱ����������ö�ʱ;����������״̬�������ö����� 
	//if((TmClk_SetState==TmClk_SetState_ClkHr)||(TmClk_SetState==TmClk_SetState_ClkMin))  return;
	if(TmClk_SetState==TmClk_SetState_TMOFF) return;
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if((TmClk_SetState==TmClk_SetState_TMON)||((V_TEMP1&ARC_T3_TIMEON)==0))
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=READ_TIME_CHARACTER(V_group_num);
		V_TEMP2=V_TEMP2&TM_SET_MASK;

		if((V_TEMP2!=CON_TM_SET_ONE)&&(V_TEMP2!=CON_TM_SET_TWO)&&(V_TEMP2!=CON_TM_SET_TWO_PT))
		{	SET_VarFlag_SEND(key_val);}	//��ʱ��һ����������
		else
		{
			if((V_TEMP2==CON_TM_SET_ONE)&&((V_TEMP1&SYSFLAG_POWER)!=0))  return;	//20120905

			if(TmClk_SetState!=TmClk_SetState_TMON)
			{
				TmClk_SetState=TmClk_SetState_TMON;
				ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEONP;		//�򿪶���
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
				V_TEMP1=V_TEMP1|ARC_T3_TIMEON;				//�ж���
				ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
				
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
				V_TEMP1=V_TEMP1|SYSFLAG_TIME;				//�ж�ʱ
				ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
				
				ADR_VAR[ADR_VAR_START+ADR_TIMEON] = ARC_TIMEON_BAK;
				
/*
				V_TEMP1=READ_TIME_CHARACTER(V_group_num);
				V_TEMP1=V_TEMP1&TM_MODE_MASK;
				switch(V_TEMP1)
				{
					case CON_TM_MODE_OPPOSITE:	//��Զ�ʱ
					
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
						if(V_TEMP2>=24+1)
						{
							V_TEMP2=1;
							ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP2;
						}
						ADR_VAR[ADR_VAR_START+ADR_TIMEON]=V_TEMP2;
						break;
						
					case CON_TM_MODE_ABSOLUTE:	//���Զ�ʱ
						
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
						if(V_TEMP2>=6)
						{
							ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=0;
							V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							if(V_TEMP1>=24) 
							ADR_VAR[ADR_VAR_START+ADR_TIMEON]=0;
						}
						break;
						
					case CON_TM_MODE_FABSOLUTE:	//�پ��Զ�ʱ����ʾ���Է�����ԣ�

						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
						V_TEMP4=HourMin_Chang_Opt10MIN(V_TEMP1,V_TEMP2,V_TEMP3);
						ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP4;
						point1=&ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR];
						point2=&ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN];
						F10MIN_TO_HOUR_MIN(V_TEMP4,point1,point2);	//;��10����Ϊ��λ��ʱ���ֵ��ת��ΪСʱ������
						
						break;
					default:
						break;
						
				}
*/				
				
			}
			else	//ȷ�϶�ʱ
			{
				//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
				//V_TEMP1=V_TEMP1&TM_MODE_MASK;
				//switch(V_TEMP1)
				//{
				//	case CON_TM_MODE_OPPOSITE:	//��Զ�ʱ

						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
						if(V_TEMP1!=0)
						{
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
							if(((V_TEMP2&ARC_T3_TIMEOFF)==0)||(V_TEMP3!=V_TEMP4))	//�����Ͷ��ز�������һ����ʱ��
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							//	ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP1;		//���ݶ�ʱʱ��
								ARC_TIMEON_BAK = V_TEMP1;
								TmMin=0;
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
								TIME_OPPOSITE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2);	//���ػ�����
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}
						
						}
						//break;
						
/*					case CON_TM_MODE_ABSOLUTE:	//���Զ�ʱ
	
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
								TIME_ABSOLUTE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2,V_TEMP3,V_TEMP4);	//���Զ�ʱ���ػ�����
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}

						}
						break;
						
					case CON_TM_MODE_FABSOLUTE:	//�پ��Զ�ʱ����ʾ���Է�����ԣ�
					
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
						V_TEMP4=HourMin_Chang_Opt10MIN(V_TEMP1,V_TEMP2,V_TEMP3);
						ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=V_TEMP4;
						point1=&ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR];
						point2=&ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN];
						F10MIN_TO_HOUR_MIN(V_TEMP4,point1,point2);	//;��10����Ϊ��λ��ʱ���ֵ��ת��ΪСʱ������
					
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
						if(V_TEMP1!=0)
						{
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];	
							if(((V_TEMP2&ARC_T3_TIMEOFF)==0)||(V_TEMP3!=V_TEMP4))	//�����Ͷ��ز�������һ����ʱ��
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
								HEX_TO_DEC(V_TEMP1,&V_TEMP2,&V_TEMP3,&V_TEMP4);
								TmMin=V_TEMP2;
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
								TIME_OPPOSITE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2);	//���ػ�����
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
	{	//ȡ������
	
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEONCL;	//ȡ������
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		V_TEMP1=V_TEMP1&(~ARC_T3_TIMEON);
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
		if(V_TEMP1==0)
		{	
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP2=V_TEMP2&(~SYSFLAG_TIME);	//clr ��ʱ��־
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
		}
		key_val=ARC_KEY_TIMECL;	//ȡ����ʱ��
/*		V_TEMP1=READ_TIME_CHARACTER(V_group_num);
		V_TEMP1=V_TEMP1&TM_MODE_MASK;
		if(V_TEMP1==CON_TM_MODE_FABSOLUTE)
		{	//��ʾΪ����ʱ��,����Ϊ���ʱ�䣨ʱ��
			ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS]=0;
			ADR_VAR[ADR_VAR_START+ADR_TMON_DISHOUR]=0;
			ADR_VAR[ADR_VAR_START+ADR_TMON_DISMIN]=0;
		
		}
		else if(V_TEMP1==CON_TM_MODE_OPPOSITE)*/
			ADR_VAR[ADR_VAR_START+ADR_TIMEON]=0;		//����Զ�ʱ��ʱ��
		
		JUDGE_POWER_ONOFF(V_group_num);
		SET_VarFlag_SEND(key_val);
		
	}

}



/*===========================================================
;��ʱ�����ô���
;˵��:���������
;	1.��Զ�ʱ����С��λΪ1Сʱ����ʱ������ ADR_TIMEOFF (��λСʱ)��,ͬʱ���ݵ�ADR_TMOFF_10MINS(��λСʱ)��;
;	2.��ʾ���Զ�ʱ��ʵ�ʷ���ҲΪ���Զ�ʱ��ʱ������ ADR_TIMEOFF��Сʱ���� ADR_TMOFF_10MINS��10���ӣ���;
;	3.��ʾ���Զ�ʱ��ʵ�ʷ���Ϊ��Զ�ʱ��ʱ������  ADR_TMOFF_10MINS����λ10���ӣ���,ͬʱ���ݵ�ADR_TIMEOFF����λ10���ӣ���;
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
		if(V_TEMP1&SYSFLAG_SLEEP)	return;		//��������˯�߼�Сʱ��û����
	}

		//����ʱ����������ö�ʱ;����������״̬�������ö��ء� 
	if(TmClk_SetState==TmClk_SetState_TMON) return;
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if((TmClk_SetState==TmClk_SetState_TMOFF)||((V_TEMP1&ARC_T3_TIMEOFF)==0))
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=READ_TIME_CHARACTER(V_group_num);
		V_TEMP2=V_TEMP2&TM_SET_MASK;

		if((V_TEMP2!=CON_TM_SET_ONE)&&(V_TEMP2!=CON_TM_SET_TWO)&&(V_TEMP2!=CON_TM_SET_TWO_PT))
		{	SET_VarFlag_SEND(key_val);}	//��ʱ��һ����������
		else
		{
			if((V_TEMP2==CON_TM_SET_ONE)&&((V_TEMP1&SYSFLAG_POWER)==0))  return;		//20120905

			if(TmClk_SetState!=TmClk_SetState_TMOFF)
			{
				TmClk_SetState=TmClk_SetState_TMOFF;
				ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFP;		//�򿪶���
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
				V_TEMP1=V_TEMP1|ARC_T3_TIMEOFF;				//�ж���
				ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
				
				V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
				V_TEMP1=V_TEMP1|SYSFLAG_TIME;				//�ж�ʱ
				ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;

				ADR_VAR[ADR_VAR_START+ADR_TIMEOFF] = ARC_TIMEOFF_BAK;

/*
				V_TEMP1=READ_TIME_CHARACTER(V_group_num);
				V_TEMP1=V_TEMP1&TM_MODE_MASK;
				switch(V_TEMP1)
				{
					case CON_TM_MODE_OPPOSITE:	//��Զ�ʱ
					
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
						if(V_TEMP2>=24+1)
						{
							V_TEMP2=1;
							ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP2;
						}
						ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=V_TEMP2;
						break;
						
					case CON_TM_MODE_ABSOLUTE:	//���Զ�ʱ
						
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
						if(V_TEMP2>=6)
						{
							ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=0;
							V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
							if(V_TEMP1>=24) 
							ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;
						}
						break;
						
					case CON_TM_MODE_FABSOLUTE:	//�پ��Զ�ʱ����ʾ���Է�����ԣ�

						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
						V_TEMP4=HourMin_Chang_Opt10MIN(V_TEMP1,V_TEMP2,V_TEMP3);
						ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP4;
						point1=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR];
						point2=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN];
						F10MIN_TO_HOUR_MIN(V_TEMP4,point1,point2);	//;��10����Ϊ��λ��ʱ���ֵ��ת��ΪСʱ������
						
						break;
					default:
						break;
						
				}
			*/	
				
			}
			else	//ȷ�϶�ʱ
			{
				//V_TEMP1=READ_TIME_CHARACTER(V_group_num);
				//V_TEMP1=V_TEMP1&TM_MODE_MASK;
				//switch(V_TEMP1)
				//{
				//	case CON_TM_MODE_OPPOSITE:	//��Զ�ʱ

						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
						if(V_TEMP1!=0)
						{
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
							if(((V_TEMP2&ARC_T3_TIMEON)==0)||(V_TEMP3!=V_TEMP4))	//�����Ͷ��ز�������һ����ʱ��
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
								//ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP1;		//���ݶ�ʱʱ��
								ARC_TIMEOFF_BAK = V_TEMP1;
								TmMin=0;
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
								TIME_OPPOSITE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2);	//���ػ�����
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}
						
						}
						//break;
/*						
					case CON_TM_MODE_ABSOLUTE:	//���Զ�ʱ
	
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
								TIME_ABSOLUTE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2,V_TEMP3,V_TEMP4);	//���Զ�ʱ���ػ�����
								TmClk_SetState=0;
								SET_VarFlag_SEND(key_val);
							}

						}
						break;
						
					case CON_TM_MODE_FABSOLUTE:	//�پ��Զ�ʱ����ʾ���Է�����ԣ�
					
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
						V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
						V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
						V_TEMP4=HourMin_Chang_Opt10MIN(V_TEMP1,V_TEMP2,V_TEMP3);
						ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=V_TEMP4;
						point1=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR];
						point2=&ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN];
						F10MIN_TO_HOUR_MIN(V_TEMP4,point1,point2);	//;��10����Ϊ��λ��ʱ���ֵ��ת��ΪСʱ������
					
						V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
						if(V_TEMP1!=0)
						{
							V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
							V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
							V_TEMP4=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];	
							if(((V_TEMP2&ARC_T3_TIMEON)==0)||(V_TEMP3!=V_TEMP4))	//�����Ͷ��ز�������һ����ʱ��
							{
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
								HEX_TO_DEC(V_TEMP1,&V_TEMP2,&V_TEMP3,&V_TEMP4);
								TmMin=V_TEMP2;
								V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
								V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
								TIME_OPPOSITE_POWER_PRO(V_group_num,V_TEMP1,V_TEMP2);	//���ػ�����
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
	{	//ȡ������
	
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFCL;	//ȡ������
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
		V_TEMP1=V_TEMP1&(~ARC_T3_TIMEOFF);
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE3]=V_TEMP1;
		if(V_TEMP1==0)
		{	
			V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
			V_TEMP2=V_TEMP2&(~SYSFLAG_TIME);	//clr ��ʱ��־
			ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
		}
		key_val=ARC_KEY_TIMECL;	//ȡ����ʱ��
	/*	V_TEMP1=READ_TIME_CHARACTER(V_group_num);
		V_TEMP1=V_TEMP1&TM_MODE_MASK;
		if(V_TEMP1==CON_TM_MODE_FABSOLUTE)
		{	//��ʾΪ����ʱ��,����Ϊ���ʱ�䣨ʱ��
			ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS]=0;
			ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISHOUR]=0;
			ADR_VAR[ADR_VAR_START+ADR_TMOFF_DISMIN]=0;
		
		}
		else if(V_TEMP1==CON_TM_MODE_OPPOSITE)	*/
			ADR_VAR[ADR_VAR_START+ADR_TIMEOFF]=0;		//����Զ�ʱ��ʱ��

		JUDGE_POWER_ONOFF(V_group_num);
		SET_VarFlag_SEND(key_val);
		
	}

}


/*===========================================================
;˯�ߴ���
;˵��:1.��˯�߱�־��1,����0��˯��ʱ���붨ʱ��������ADR_TIMEOFF
;input:	ADR_SYSFLAG��ADR_TIMEOFF
;output:	ADR_SYSFLAG��ADR_TIMEOFF
;===========================================================*/
void USE_SLEEP_KEY_PRO(u16 V_group_num,u8 key_val)
{
u8   V_TEMP1,V_TEMP2,V_TEMP3;
u8	*point1=NULL,*point2=NULL,*point3=NULL;

	//if((SYSTEMFLAG&MULITCODE)!=0) return;
	if((VarFlag&VarFlag_USE_DISABLE)!=0) return;	
	if(TmClk_SetState==TmClk_SetState_TMOFF) return;	//���ö���ʱ����������˯��
//	if(VarState_KeyPro!=0) return;
//	VarState_KeyPro++;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
	V_TEMP1=V_TEMP1&(~SYSFLAG_STRONG);
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
	
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_MODE];
	
	if((V_TEMP1==ARC_MODE_DHMD)||(V_TEMP1==ARC_MODE_FAN)) return;	//��ʪ���ͷ�ģʽ��������˯��
	
	V_TEMP1=READ_SYS_CHARACTER(V_group_num);
	if((V_TEMP1&F_SleepNHours)==0)
	{
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=(V_TEMP2^SYSFLAG_SLEEP)&(~SYSFLAG_STRONG);	//����˯�ߣ���ǿ��
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	else
	{//˯�߿���ʱ��
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
			if(V_TEMP3>=11)	//���˯��10Сʱ
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
;�縨�ȴ���
;˵��:1.���縨�ȱ�־��1,����0
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
;ǿ������
;˵��:1.��ǿ����־��1,����0
;ֻ�����䣬����ģʽ ��Ч
;���ø�ǿ����ͬʱ����
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
	if((V_TEMP1&SYSFLAG_SLEEP)!=0)	 return;	//˯�߿��������ܿ�ǿ��
	
	V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_MODE];
	if((V_TEMP2==ARC_MODE_HEAT)||(V_TEMP2==ARC_MODE_COOL))
	{
		V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP1=(V_TEMP1^SYSFLAG_STRONG)&(~SYSFLAG_ECONOMIC);	//˯�߿��������ܿ�ǿ��
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
		SET_VarFlag_SEND(key_val);
	}
	
}

/*===========================================================
;�ƹ⴦��
;˵��:1.���ƹ��־��1,����0
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
;����/����/��������
;˵��:1.�������־��1,����0
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
;���ô���
;˵��:1.�����ñ�־��1,����0
;���ø�ǿ����ͬʱ����
;��ʪ�ͷ�ģʽ�����þ���
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
	V_TEMP1=(V_TEMP1^SYSFLAG_ECONOMIC)&(~SYSFLAG_STRONG);	//��ǿ��
	ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP1;
	
	SET_VarFlag_SEND(key_val);
}

/*===========================================================
;ʱ�Ӹ��£���ʱʱ�����(ʱ��1���Ӹ���1��)
;��ʱ״̬Ҳ������ı�,�򿪶�ʱ״̬�����˺����ı�Ϊ�ȴ���ʱʱ�䵽״̬
;input:	Clock_Sec, ADR_TIMEON,ADR_TIMEOFF,ADR_TM_MIN,ADR_TMON_10MINS,ADR_TMOFF_10MINS,ADR_SYSFLAG,ADR_TMSTATE7,ADR_TMSTATE3
;output: 	ADR_CKMIN,ADR_CKHOUR,ADR_TIMEON,ADR_TIMEOFF,ADR_TM_MIN,ADR_TMON_10MINS,ADR_TMOFF_10MINS,ADR_SYSFLAG,ADR_TMSTATE7,ADR_TMSTATE3
;===========================================================*/

/*---------------------------------------------------------
;��ʱ������ʱ
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
//	{	//���ʱ��
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
	{	//��ʾΪ����ʱ��,����ҲΪ����ʱ��
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEON];
		if(V_TEMP2!=V_TEMP3) return;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
		V_TEMP2=MIN_CHANGE_TAB[V_TEMP2];
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
		if(V_TEMP2!=V_TEMP3) return;
	}
	else
	{	//��ʾΪ����ʱ��,����Ϊ���ʱ��(ʱ���)
	
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
		F10MIN_TO_HOUR_MIN(V_TEMP2,point1,point2);	//;��10����Ϊ��λ��ʱ���ֵ��ת��ΪСʱ������
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMON_10MINS];
		if(V_TEMP2!=0) return;
		
	}*/
	//��ʱ��ʱ�䵽
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if(V_TEMP1&ARC_T3_TIMEOFF)
	{
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFW;		//�ȶ���
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2|SYSFLAG_POWER;		//����
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	else
	{
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=0;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=(V_TEMP2&(~SYSFLAG_TIME))|SYSFLAG_POWER;	//�嶨ʱ,����
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
;���ص���ʱ
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
//	{	//���ʱ��
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
	{	//��ʾΪ����ʱ��,����ҲΪ����ʱ��
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_CKHOUR];
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_TIMEOFF];
		if(V_TEMP2!=V_TEMP3) return;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
		V_TEMP2=MIN_CHANGE_TAB[V_TEMP2];
		V_TEMP3=ADR_VAR[ADR_VAR_START+ADR_CKMIN];
		if(V_TEMP2!=V_TEMP3) return;
	}
	else
	{	//��ʾΪ����ʱ��,����Ϊ���ʱ��(ʱ���)
	
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
		F10MIN_TO_HOUR_MIN(V_TEMP2,point1,point2);	//;��10����Ϊ��λ��ʱ���ֵ��ת��ΪСʱ������
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_TMOFF_10MINS];
		if(V_TEMP2!=0) return;
		
	}*/
	//��ʱ��ʱ�䵽
	V_TEMP1=ADR_VAR[ADR_VAR_START+ADR_TMSTATE3];
	if(V_TEMP1&ARC_T3_TIMEON)
	{
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=ARC_T7_TIMEOFFW;		//�ȶ���
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2&(~SYSFLAG_POWER);		//�ػ�
		ADR_VAR[ADR_VAR_START+ADR_SYSFLAG]=V_TEMP2;
	}
	else
	{
		ADR_VAR[ADR_VAR_START+ADR_TMSTATE7]=0;
		V_TEMP2=ADR_VAR[ADR_VAR_START+ADR_SYSFLAG];
		V_TEMP2=V_TEMP2&(~(SYSFLAG_TIME+SYSFLAG_POWER));	//�嶨ʱ,�ػ�
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
;����ʹ���ж�
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
;ȫ�ֱ�����ʼ��
;=================================================*/
//��һ��Ӧ�ó�ʼ�������û�ʹ�ù��󣬽���Щ��������flash�У��´δ�flash�ж�ȡ
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
//��������״̬������ʼ������
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
;ʹ�ô���
;���ܰ�����Ŀ��������

;============================================================*/
void ARC_USE_PRO(u16 V_group_num,u8 key_val)
{
u8	V_TEMP1;
u16 	characteristic_id;

	if(V_group_num >= EXT_FILE_DATA) characteristic_id = V_group_num;
	else characteristic_id = GROUP_CHANG_Array[V_group_num];
	
	if(key_val>=DAT_KEY_NUM_MAX) return;
	
	V_TEMP1=ARC_KEY_ENCHECK(characteristic_id,key_val);	//����ʹ���ж�
	if(V_TEMP1==0)  return;
	
	switch(key_val)
	{
		case	ARC_KEY_POWER:		//��Դ
			USE_POWER_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_TUP:		//�¶�+
			USE_TEMPUP_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_TDOWN:		//�¶�-
			USE_TEMPDN_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_MODE:		//ģʽ
			USE_MODE_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_FANLEV:		//����
			USE_FANLEV_KEY_PRO(key_val);
			break;
		case	ARC_KEY_AFANDIR:	//�Զ�����
			USE_AFANDIR_KEY_PRO(key_val);	//�Զ�����/����
			break;
		case	ARC_KEY_MFANDIR:	//�ֶ�����
			USE_MFANDIR_KEY_PRO(key_val);	//�ֶ�����/����
			break;
		case	ARC_KEY_TIMEON:		//����
			USE_TIMEON_KEY_PRO(characteristic_id,key_val);	//��ʱ��
			break;
		case	ARC_KEY_TIMEOFF:	//����
			USE_TIMEOFF_KEY_PRO(characteristic_id,key_val);
			break;
//		case	ARC_KEY_TIMECL:		//ȡ����ʱ
		
		case	ARC_KEY_SLEEP:		//˯��
			USE_SLEEP_KEY_PRO(characteristic_id,key_val);
			break;
		case	ARC_KEY_HEAT:		//�縨��
			USE_HEAT_KEY_PRO(key_val);
			break;
		case	ARC_KEY_STRONG:		//ǿ��
			ARC_KEY_STRONG_PRO(key_val);
			break;
		case	ARC_KEY_LIGHT:		//�ƹ�
			ARC_KEY_LIGHT_PRO(key_val);
			break;
		case	ARC_KEY_AIRCLEAR:	//����
			ARC_KEY_AIRCLEAR_PRO(key_val);
			break;
		case	ARC_KEY_ECONOMIC:	//����
			ARC_KEY_ECONOMIC_PRO(key_val);
			break;

		default:
			break;
	}
}


