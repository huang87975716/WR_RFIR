/***************************************************************************
*****************************************************************************/
#ifndef _REMOTE_ARC_TAB_h
#define _REMOTE_ARC_TAB_h
/****************************constant define********************************/
#define	TBOF_Auto_Mode	0 
#define	TBOF_DHMD_Mode	1 
/****************************constant define********************************/
#define	EnK_AFANDIR	0x01		//�Զ����� 
#define	EnK_SLEEP	0x02		// ˯��   
#define	EnK_HEAT	0x04		// �縨�� 
#define	EnK_STRONG	0x08		// ǿ��   
#define	EnK_LIGHT	0x10		// �ƹ�   
#define	EnK_AIRCLEAR	0x20		// ����/����/���� 
#define	EnK_ECONOMIC	0x40		// ���� 
#define	EnK_MFANDIR	0x80		// �ֶ�����
/****************************constant define********************************/
#define	F_DonotClrScreen	0x10		//�ػ������� 
#define	F_DonotDisp	0x20		//ֻ��ʾʱ��/��ʱ/����ţ�����������ʾ����û������ң������ʾ��յ�����������һ�µ������ 
#define	F_SleepNHours	0x40		//˯�߿�����ٸ�Сʱ 
#define	F_FANDIR_LR	0x80		//���ҷ��� ��ʾ����  
/****************************constant define********************************/ 
#define	TM_MODE_MASK	0x3		//bit0~bit1		;��������ʶ��λ�ã�������ʱ���ù�������壩 
#define	CON_TM_MODE_OPPOSITE	0		//��Զ�ʱ  
#define	CON_TM_MODE_ABSOLUTE	01		//���Զ�ʱ  
#define	CON_TM_MODE_FABSOLUTE	02		//�پ��Զ�ʱ����ʾ���Է�����ԣ�
#define	F_NoTunoffTime	0x4		//���ػ���ȡ����ʱ   
#define	TM_SET_MASK	0x70		//bit4~bit7	;��������ʶ��λ�ã�������ʱ���ù�������壩
#define	CON_TM_SET_ONE	0*0x10		//�ػ��趨���������趨�� 
#define	CON_TM_SET_TWO	1*0x10		//���ػ��������ؿ�ͬʱ�� 
#define	CON_TM_SET_TWO_PT	2*0x10		//���ػ��������ؿ�ͬʱ��,������ǰȷ�����Զ�������������ǰȷ����ػ� 
#define	CON_TM_SET_SENDKEY	3*0x10		//��ʱ��һ���������� 
#define	F_Disable_TMON	0x80		//û�ж���   
#define	F_Disable_TMOFF	0x8		//û�ж���       

/********************�ⲿ������������************************************/
extern const u8  GROUP_CHANG_Array[];
extern const u8 TEMP_Bound_Array[][2];
extern const u8 SYS_CHARACTER_Array[];
extern const u8 KEY_ENABLE_Array[];
extern const u8 TIME_CHARACTER_Array[];
extern const u8 BIT_ADR_MASK_TAB[];

#endif
