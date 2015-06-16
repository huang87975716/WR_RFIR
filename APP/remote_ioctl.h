
/*****************************************************************************/
#ifndef	 _REMOTE_IOCTL_H
#define 	_REMOTE_IOCTL_H
/*****************************************************************************/
//#include "remote.h"
/*********************************************************************/
#define  SEND_CODE  	      0x01			//�����з��ͷ�������
#define 	SEND_END  		0x11		//�������

#define 	LENTIMEOUT		0x80
#define  LEN_DOING  	      0x01
#define  LEN_OK  	      0x02
#define  LEN_ERROR	0x03
/********************************************************************************/
enum
{
	HB_IOCTL_INIT = 0,		//��ʼ����Ӧ�ó����ʱ�����ô˺���
	HB_IOCTL_TIMER100,		//��ʱ��100ms��ʱ�����ô˺���
	HB_IOCTL_TIMER500,		//RTC��ʱ��500ms
	HB_IOCTL_CREATE_IRDevice,	//�½������豸���ղغ����豸ʱ���ã�
	//������Ӧ�����豸 ����
	//TV(�豸����)��Ʒ�ƣ�����������ѧϰ����
	//ARC:�յ���ر���
	HB_IOCTL_Delete_Device,		//ɾ�������豸��ɾ���ղؼ�¼ʱ���ã�
	HB_IOCTL_KeyDn,				//�豸�������£����ô˺�������
	HB_IOCTL_KeyUp,
		//����ݵ�ǰ�豸ֵ�������豸�ţ����ö�Ӧ�ļ��ж�Ӧ���ݣ������������
	HB_IOCTL_Change_Device,   // �����豸���ͣ�
		//Ӧ�ò�ı��豸���ͣ������豸��Ʒ�ƣ���������ʱ����Ҫ���ô˺������º���ϵͳ�ڲ�����
	HB_IOCTL_IRRX_BEGIN,	//�������HB1682 ������ѧϰ�� �����ѯѧϰ�Ƿ���ɣ��ڶ�ʱ���д���;
	HB_IOCTL_ARC_IRRX,		//�����յ���ѧϰ
	HB_IOCTL_IRRX_END,      // ��ѧϰ�����У��û�����ȡ��ѧϰ����Ӧ�ò���Ҫ���ô˺�����ֹѧϰ��
	HB_IOCTL_Send_LenCode,	//����URCѧϰ������
	HB_IOCTL_Send_ArcLenCode,	//����ARCѧϰ��
	HB_IOCTL_RX_ICVER,		//��ȡIC ����汾
	HB_IOCTL_RX_PRO,		//���ڽ������ݴ���
	HB_IOCTL_SEND_Search,	//��������
};	//ev_ioctl_e



typedef struct
{
	//u8  searchflg;	//����״̬��־	!=0,��ʾ������
	u8  device;		//�豸��
	u16 group;		//����ţ���group>=0x8000ʱ����ʾ����ļ�����
	u8  keyval;		//��ֵ
	u8  *arc_state_var;	//ָ��յ�������ֻ��device = C_ARC_DEVICE  ʱ��Ч
}type_remote_ctl;


typedef struct
{
	u8  exit_en;		
	u8	cmd;
	s16  Time_100m;		//
	s16  Time_100m_cnt;	//
}type_ex_ctl;

typedef struct
{
	s16  group_aux;	//����AUX
	s16	 group_dvb;	//����DVB
	s16  group_stb;	//���������
	s16  group_atv;	//����TV��������TV
	s16  group_ctv;	//����TV
	s16	 group_dvd;	//����DVD
	s16  group_arc;	//�յ�
}type_CUR_group;

typedef struct
{
	u16  arc_group;		
	u8	clk_hour;
	u8    clk_min;		//
}type_updatatime;

/********************************************************************
��������
*********************************************************************/

int HB_ioctl(int cmd, void* arg);





#endif



