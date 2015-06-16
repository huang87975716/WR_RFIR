/***************************************************************************
*****************************************************************************/
#ifndef _REMOTE_ARC_RPO_H
#define _REMOTE_ARC_RPO_H
/*****************************************************************************/
/*====================================================================		
//====================================================================
//====================================================================
//	һ����˵��
//====================================================================
// һЩ����
// 1. ����������λ����Ŀǰ���ܳ���256Bit
// 2. �����������չ��16B
// 3. �������������Ϊ36B,����г�������Ҫ���⴦����
// 4. ��ʱʱ�������3�����ڹ��ܳ�����Ҫ������ʱ����ʱʱ�䵽Ҫ�嶨ʱ״̬������2����
//====================================================================
//	ARC DAT CONSTANT DEFINE
//====================================================================*/
/*DRV DATA ���������� ������*/
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
#define	CMD_DAT_JUDGE		0xF0	//�жϱ�־λ�����ձ�־λȡֵ
//====================================================================
#define	JUDGE_POWER_FLAG	0x10	//����
#define	JUDGE_SLEEP_FLAG	0x20	//˯��
#define	JUDGE_ELEHOT_FLAG	0x30	//����
#define	JUDGE_TIME_FLAG	0x40		//�ж������ж��أ��޶�ʱ
#define	CLOCK_TEN_MIN		0x50	//ʱ��ʮ����
#define	TIMEON_TEN_MIN		0x60	//����ʮ����
#define	TIMEOFF_TEN_MIN	0x70	//����ʮ����
//====================================================================
/*CMDFlag���������ò���*/
#define	CF_REV			0x01		//ȡ��/����Ĭ���ǵ������д˱�־����ʾ�ݼ���
#define	CF_SERIES		0x02		//����������ֻ������ֵ�����ֵ��
#define	CF_SER			0x02		//����(��)
#define	CF_16BITS		0x04		//��ʾ���ݱ�Ϊ16λ���ݣ�Ĭ��Ϊ8λ
#define	CF_VERR			0x0FF	//���Ʊ������ݳ�����Χ
//==============================================================
/*CMD_DAT_CHECK	����*/
#define	CT_SUM8			0x00		//���ֽ���ͣ�CMD_DAT_CHECK+CT_SUM8
#define	CT_SUM4			0x01		//��4λ���ߵͣ����
#define	CT_SUM4L		0x02		//����4λ���
#define	CT_SUM4H		0x03		//����4λ���
#define	CT_EOR8			0x04		//���ֽ����
#define	CT_EOR4			0x05		//��4λ���ߵͣ����
#define	CT_EOR4L		0x06		//����4λ���
#define	CT_EOR4H		0x07		//����4λ���
#define	CT_NUM0		0x08		//����0�ĸ���
//================================================================================================
/* �յ��������� ADR_VAR[]˵�� */
#define	ADR_VAR_START	0x00				//�յ������Ĵ��λ�ã���Ҫ���ݳ����޸ģ�
#define	ADR_MODE		0		//ģʽ------------���仯��Χ"0~4"��Ӧģʽ���"ģʽ����"����
#define	ADR_TMP		1		//�¶�------------���仯��Χ"0~16"��Ӧ�¶�"16~32"
#define	ADR_FANLEV		2		//����------------���仯��Χ"0~3"��Ӧ����"�Զ����ͣ��У���"
#define	ADR_FANDIR		3		//����------------�����Զ��������0�����ֶ��������1����1~8֮��仯���������Զ�������ֶ�����������ͬһλ�ã�
#define	ADR_AFANDIR		4		//�Զ�����----�����Զ�������ı䣨��0~8֮��仯�������ֶ���������䣨�����Զ�������ֶ����������ڲ�ͬλ�ã�
#define	ADR_MFANDIR	5		//�ֶ�����----�����ֶ�������ı䣨��0~8֮��仯�������Զ���������䣨�����Զ�������ֶ����������ڲ�ͬλ��,��ֻ��һ���������ʱ��
#define	ADR_CKHOUR		6		//ʱ��Сʱ
#define	ADR_TIMEON		7		//����ʱ�䣨Сʱ��----���仯��Χ"0~12"��Ӧ����ʱ��"0~12"Сʱ��ֻҪ���ڶ������������ڲ�ͬλ�õ�ʱ��
#define	ADR_TIMEOFF		8		//����ʱ�䣨Сʱ��----���仯��Χ"0~12"��Ӧ����ʱ��"0~12"Сʱ��ֻҪ���ڶ������������ڲ�ͬλ�õ�ʱ��
#define	ADR_KEYVAL		9		//��ֵ------------�����"��������"����,�ռ�:0FFH
#define	ADR_SYSFLAG	10		//���ܱ�־---�����"���ܱ�־"��������
#define	ADR_TMSTATE7	11		//��ʱ״̬��7�֣������"��ʱ״̬������7̬��"
#define	ADR_TMSTATE3	12		//��ʱ״̬��3�֣������"��ʱ״̬������3̬��"
//ADR_TM_MIN		EQU	13	//��ʱ����
#define	ADR_CKMIN		13		//ʱ�ӷ���
//#define	ADR_TMON_10MINS	14		//��ʱ������
//#define	ADR_TMOFF_10MINS	15		//��ʱ�ط���
//---------------------------------------------------------------------------------------------
//ƫ�Ƶ�ַ����15�ĵı�����ֻ�������� "CMD_DAT_VAR" ��ȡ
//#define	ADR_TMON_DISHOUR			16	//����ʱ���Сʱ
//#define	ADR_TMON_DISMIN			17	//����ʱ������
//#define	ADR_TMOFF_DISHOUR			18	//����ʱ���Сʱ
//#define	ADR_TMOFF_DISMIN			19	//����ʱ������
//ע�⣺�����ϵ���������"�յ�����"��Ҫ���³�ʼ��
//=========================================================================================
//... �������Բ���
//ADR_VAR
#define	ADR_VAR_LEN		14	//20		//�������ݻ�����������ADR_VAR�е���ʼλ��
/*ARC �¶ȳ���*/
#define	ARC_TMP_L		0		//����¶�Ϊ0+16���϶�
#define	ARC_TMP_H		16		//����¶�Ϊ16+16���϶�
/*ARC ģʽ���� ADR_MODE*/
#define	ARC_MODE_AUTO	0		//�Զ�
#define	ARC_MODE_COOL	1		//����
#define	ARC_MODE_DHMD	2		//��ʪ
#define	ARC_MODE_FAN	3		//�ͷ�
#define	ARC_MODE_HEAT	4		//����
/*ARC ADR_FANDIR������*/
#define	ARC_FANDIR_1	0		//�Զ� ???
#define	ARC_FANDIR_2	1		//�ֶ�1
#define	ARC_FANDIR_3	2		//�ֶ�2
#define	ARC_FANDIR_4	3		//�ֶ�3
#define	ARC_FANDIR_5	4		//�ֶ�4
#define	ARC_FANDIR_6	5		//�ֶ�5
#define	ARC_FANDIR_7	6		//�ֶ�6
#define	ARC_FANDIR_8	7		//�ֶ�7
#define	ARC_FANDIR_9	8		//�ֶ�8
#define	ARC_FANDIR_10	9		//�ֶ�9
/*ARC  ADR_AFANDIR �Զ�������*/
#define	ARC_AFANDIR_1	0		//�Զ�����1
#define	ARC_AFANDIR_2	1		//�Զ�����2
#define	ARC_AFANDIR_3	2		//�Զ�����3
#define	ARC_AFANDIR_4	3		//�Զ�����4
#define	ARC_AFANDIR_5	4		//�Զ�����5
#define	ARC_AFANDIR_6	5		//�Զ�����6
#define	ARC_AFANDIR_7	6		//�Զ�����7
#define	ARC_AFANDIR_8	7		//�Զ�����8
#define	ARC_AFANDIR_9	8		//�Զ�����9
#define	ARC_AFANDIR_10	9		//�Զ�����10
/*ARC  ADR_MFANDIR �ֶ�������*/
#define	ARC_MFANDIR_1	0		//�ֶ�����1
#define	ARC_MFANDIR_2	1		//�ֶ�����2
#define	ARC_MFANDIR_3	2		//�ֶ�����3
#define	ARC_MFANDIR_4	3		//�ֶ�����4
#define	ARC_MFANDIR_5	4		//�ֶ�����5
#define	ARC_MFANDIR_6	5		//�ֶ�����6
#define	ARC_MFANDIR_7	6		//�ֶ�����7
#define	ARC_MFANDIR_8	7		//�ֶ�����8
#define	ARC_MFANDIR_9	8		//�ֶ�����9
#define	ARC_MFANDIR_10	9		//�ֶ�����10

/*ARC ADR_FANLEV ��������*/
#define	ARC_FANLEV_1	0		//����1 �Զ�
#define	ARC_FANLEV_2	1		//����2 �ͷ�
#define	ARC_FANLEV_3	2		//����3 �з�
#define	ARC_FANLEV_4	3		//����4 �߷�

/*ARC ���ػ�����*/
#define	ARC_POWER_ON		0
#define	ARC_POWER_OFF		1

/*ARC (ADR_KEYVAL)��������*/
#define	DAT_KEY_NUM_MAX		0x10
#define	ARC_NO_KEY			0x0FF	//�ռ�
#define	ARC_KEY_POWER		0	//��Դ
#define	ARC_KEY_TUP			1	//�¶�+
#define	ARC_KEY_TDOWN		2	//�¶�-
#define	ARC_KEY_MODE		3	//ģʽ
#define	ARC_KEY_FANLEV		4	//����
#define	ARC_KEY_AFANDIR		5	//�Զ�����
#define	ARC_KEY_MFANDIR		6	//�ֶ�����
#define	ARC_KEY_TIMEON		7	//����
#define	ARC_KEY_TIMEOFF		8	//����
//ARC_KEY_TIME		EQU	7	//��ʱ����
#define	ARC_KEY_TIMECL		9	//ȡ����ʱ
#define	ARC_KEY_SLEEP		10	//˯��
#define	ARC_KEY_HEAT		11	//�縨��
#define	ARC_KEY_STRONG		12	//ǿ��
#define	ARC_KEY_LIGHT		13	//�ƹ�
#define	ARC_KEY_AIRCLEAR	14	//����
#define	ARC_KEY_ECONOMIC	15	//����
#define	ARC_KEY_CLOCK		0x10		//ʱ��

/*ADR_SYSFLAG(���ܱ�־)��������*/
#define	SYSFLAG_POWER	0x01	//00000001B		//���ػ���־----������=1���ػ�=0
#define	ADSF_POWER		0				//���ػ���־λ��
#define	SYSFLAG_SLEEP	0x02	//00000010B		//˯�߱�־--------��˯�߿�=1��˯�߹�=0
#define	ADSF_SLEEP		1				//˯�߱�־λ��
#define	SYSFLAG_HEAT	0x04	//00000100B		//�縨�ȱ�־----���縨�ȿ�=1���縨�ȹ�=0
#define	ADSF_HEAT		2				//�縨�ȱ�־λ��
#define	SYSFLAG_TIME	0x08	//00001000B		//��ʱ��־-------�� �ж�ʱ= 1��û�ж�ʱ = 0
#define	ADSF_TIME		3				//��ʱ��־λ��
#define	SYSFLAG_STRONG	 0x10	//00010000B		//ǿ����־-------�� ǿ����= 1��ǿ���� = 0
#define	ADSF_STRONG		4				//ǿ����־λ��
#define	SYSFLAG_LIGHT	0x20	//00100000B		//�ƹ��־-------�� �ƹ⿪= 1���ƹ�� = 0
#define	ADSF_LIGHT		5				//�ƹ��־λ��
#define	SYSFLAG_AIRCLEAR	0x40	//01000000B		//����/����/������־-------�� ����/����/������= 1������/����/������ = 0
#define	ADSF_AIRCLEAR	6				//����/����/������־λ��
#define	SYSFLAG_ECONOMIC	0x80	//10000000B		//���ñ�־-------�� ���ÿ�= 1�����ù� = 0
#define	ADSF_ECONOMIC	7				//���ñ�־λ��


/*ARC ADR_TMSTATE7��ʱ״̬������7̬��*/
#define	ARC_T7_NOTIME		0	//û�ж�ʱ
#define	ARC_T7_TIMEONP		1	//�򿪶���
#define	ARC_T7_TIMEOFFP		2	//�򿪶���
#define	ARC_T7_TIMEONW		3	//�ȴ�����״̬
#define	ARC_T7_TIMEOFFW		4	//�ȴ�����״̬
#define	ARC_T7_TIMEONCL		5	//ȡ������
#define	ARC_T7_TIMEOFFCL	6	//ȡ������
/*ADR_TMSTATE3 ��ʱ״̬������3̬��*/
#define	ARC_T3_NOTIME		0		//û�ж�ʱ
#define	ARC_T3_TIMEON		1		//�ж���
#define	ARC_T3_TIMEOFF		2		//�ж���
#define	ARC_T3_TIMEONOFF	3		//ͬʱ�ж�������
//------------------------------------------------------------------------------
/*DAT PRO������*/
#define	READ_AGAIN		0x55	//���¶�����
#define	READ_NEXT		0x0		//����һ������
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
extern u8	ADR_VAR[ADR_VAR_LEN];		//���Ʊ�������
extern u8 	CMDFlag;		//����,����ӱ�־
extern u16	DP_Dat_Head,Wdata16;
extern u16	DP_Dat_Des_OFS;
extern u8 ADR_FANLEV_BAK,ADR_TMP_BAK;


/*********************variable define*********************************************/

/******************************************************************************/
/*=========================================================================
define const
==========================================================================*/


#endif

