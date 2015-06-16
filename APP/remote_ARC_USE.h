/***************************************************************************
========================================================================
Company	:	HBG      Copyright (c) 2011
Author		:	Kalvin				
��Ȩ��Ϣ	:	Copyright (c) 2011 HBG Hi Tech	
File		: 	ARC_DRV.h	
VER		:	1.0.0		
Development	: 	VC++ 6.0	
Data		:	2011-7-29
Customer	:	�Ʋ���			
-------------------------History list-----------------------------------
*****************************************************************************/
#ifndef _REMOTE_ARC_USE_H
#define _REMOTE_ARC_USE_H
//===========================================================
// DEFINE  CONSTANT 
//===========================================================
#define  CON_MODE_MAX			4		//���ģʽֵ��0~4��5��ģʽ
//#define  CON_TEMP_AUTOMODE		25-16		//�Զ�ģʽ�¶�ֵ
//#define  CON_TIME_MAX			12		//��ʱ�ʱ��

//#define  CON_ADR_Group_MAX		1000-1		//ʵ��������

//-----------------------------------------------------------//
//VarFlag ��־
//#define  VarFlag_KEYNOP		0x01	//00000001B	//û�а������±�־
//#define  VarFlag_KEYPRO		0x02	//00000010B	//�а������±�־
#define  VarFlag_DISP_L		0x04	//00000100B	//��ʾ�ƹ���Ϣ��־
#define  VarFlag_USE_DISABLE	0x08	//00001000B	//����ʧЧ��־,�������־λΪ1ʱ���¶ȣ�ģʽ�����򣬷������ܵ��ڣ�Ҳ����ʾ��ֻ��ʾʱ�䣬����
#define  VarFlag_SEARCH		0x10	//00010000B	//����״̬��ʾ
#define  VarFlag_SEND		0x20	//00100000B	//�����־
#define  VarFlag_FLASH		0x40	//01000000B	//500 MS 
//#define  LIE_FLAG		0x80	//10000000B	//�����ݱ�־
//TmClk_SetState	��ʱ����״̬
#define  TmClk_Seting_NONE			0		//û�н������ö�ʱ��ʱ��״̬
#define  TmClk_SetState_TMOFF			01		//���붨������״̬
#define  TmClk_SetState_TMON			02		//���붨������״̬
//#define  TmClk_SetState_ClkMin			03		//����ʱ�ӷ�������״̬
//#define  TmClk_SetState_ClkHr			04		//����ʱ��Сʱ����״̬
//�ⲿBIN�����ļ�����λ��
#define  FILE_ARC_KEYEN_OFS		0x10
#define  FILE_ARC_SYSCHAT_OFS		0x11
#define  FILE_ARC_TIMECHAT_OFS		0x12
#define  FILE_ARC_TEMPBD_OFS		0x13
//=============================================================
// DEFINE  VARIABLE
//=============================================================
//u8 VarState_KeyPro;			//��������״̬��û�а�������ʱҪ��0        ��0��û�д���

extern u8 	VarFlag;
extern u8	ADR_TMP_BAK;		//�¶ȱ���
extern u8	ADR_FANLEV_BAK;		//��������
extern u8	TmClk_SetState;		//��ʱ��ʱ�ӣ�����״̬
extern u8	TmMin;			//��ʱ�ķ��Ӽ�����
//=============================================================
// DEFINE  const
//=============================================================
extern const unsigned char MIN_CHANGE_TAB[7];
extern const u8 KEY_CHOICE_TAB[8];
/********************************************************************
��������
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






