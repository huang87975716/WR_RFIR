/***************************************************************************
*****************************************************************************/
#ifndef _REMOTE_URC_PRO_H
#define _REMOTE_URC_PRO_H
/*****************************************************************************/
//====================================================================
//	ARC DAT CONSTANT DEFINE
//=====================================================================
//�豸�Ŷ���
#define	C_AUX_DEVICE		0	//����
#define	C_DVB_DEVICE		1	//����DVB
#define	C_STB_DEVICE		2	//���������
#define	C_ATV_DEVICE		3	//����TV��������TV
#define	C_CTV_DEVICE		4	//����TV
#define	C_DVD_DEVICE		5	//����DVD
#define	C_ARC_DEVICE		6	//
//ÿ���豸���������������
#define	C_AUX_GROUP_MAX         501
#define	C_DVB_GROUP_MAX         372
#define	C_STB_GROUP_MAX         213
#define	C_ATV_GROUP_MAX         550
#define	C_CTV_GROUP_MAX         360
#define	C_DVD_GROUP_MAX         315
#define	C_ARC_GROUP_MAX         1000
//=======================================================================
//��������
typedef enum/*���ɸı��ö�ٵ�˳���ֵ*/
{
KEY_AUX_MUTE = 1,          //������
KEY_AUX_POWER,	       //��Դ��
KEY_AUX_MUNE,	       //�˵���
KEY_AUX_UP,	       //�ϼ�
KEY_AUX_Guide,	       //����
KEY_AUX_LEFT,	       //���
KEY_AUX_OK,	       //OK ��
KEY_AUX_RIGHT,	       //�Ҽ�
KEY_AUX_INFO,	       //
KEY_AUX_DOWN,	       //�¼�
KEY_AUX_EXIT,	       //�˳�
KEY_AUX_VOLUP,	       //�����Ӽ�
KEY_AUX_PREVCH,	       //
KEY_AUX_CHUP,	       //Ƶ���Ӽ�  
KEY_AUX_VOLDN,	       //��������
KEY_AUX_INPUT,	       //
KEY_AUX_CHDN,	       //Ƶ������
KEY_AUX_PGUP,	       //P+
KEY_AUX_PGDN,	       //P-
KEY_AUX_REW,	       //����
KEY_AUX_PLAY,	       //����
KEY_AUX_STOP,	       //ֹͣ��
KEY_AUX_FF,	       //���
KEY_AUX_1,	       //���ּ�1
KEY_AUX_2,	       //���ּ�2
KEY_AUX_3,	       //���ּ�3
KEY_AUX_4,	       //���ּ�4
KEY_AUX_5,	       //���ּ�5
KEY_AUX_6,	       //���ּ�6
KEY_AUX_7,	       //���ּ�7
KEY_AUX_8,	       //���ּ�8
KEY_AUX_9,	       //���ּ�9
KEY_AUX_ENTER,	       //   
KEY_AUX_0,	       //���ּ�0
KEY_AUX_kuohao,	       //(-)
KEY_AUX_MAX,
}KEY_AUX_ENUM;

typedef enum/*���ɸı��ö�ٵ�˳���ֵ*/
{
KEY_STB_MUTE = 1,          //������
KEY_STB_POWER,	       //��Դ��
KEY_STB_MUNE,	       //�˵���
KEY_STB_UP,	       //�ϼ�
KEY_STB_Guide,	       //����
KEY_STB_LEFT,	       //���
KEY_STB_OK,	       //OK ��
KEY_STB_RIGHT,	       //�Ҽ�
KEY_STB_INFO,	       //
KEY_STB_DOWN,	       //�¼�
KEY_STB_EXIT,	       //�˳�
KEY_STB_VOLUP,	       //�����Ӽ�
KEY_STB_PREVCH,	       //
KEY_STB_CHUP,	       //Ƶ���Ӽ�  
KEY_STB_VOLDN,	       //��������
KEY_STB_INPUT,	       //
KEY_STB_CHDN,	       //Ƶ������
KEY_STB_PGUP,	       //P+
KEY_STB_PGDN,	       //P-
KEY_STB_REW,	       //����
KEY_STB_PLAY,	       //����
KEY_STB_STOP,	       //ֹͣ��
KEY_STB_FF,	       //���
KEY_STB_1,	       //���ּ�1
KEY_STB_2,	       //���ּ�2
KEY_STB_3,	       //���ּ�3
KEY_STB_4,	       //���ּ�4
KEY_STB_5,	       //���ּ�5
KEY_STB_6,	       //���ּ�6
KEY_STB_7,	       //���ּ�7
KEY_STB_8,	       //���ּ�8
KEY_STB_9,	       //���ּ�9
KEY_STB_ENTER,	       //   
KEY_STB_0,	       //���ּ�0
KEY_STB_kuohao,	       //(-)
KEY_STB_MAX,
}KEY_STB_ENUM;

typedef enum/*���ɸı��ö�ٵ�˳���ֵ*/
{
KEY_CTV_MUTE = 1,          //������             	
KEY_CTV_POWER,	       //��Դ��             	
KEY_CTV_MUNE,	       //�˵���             	
KEY_CTV_UP,	       //�ϼ�               	
KEY_CTV_Guide,	       //����               	
KEY_CTV_LEFT,	       //���               	
KEY_CTV_OK,	       //OK ��              	
KEY_CTV_RIGHT,	       //�Ҽ�               	
KEY_CTV_INFO,	       //                   	
KEY_CTV_DOWN,	       //�¼�               	
KEY_CTV_EXIT,	       //�˳�               	
KEY_CTV_VOLUP,	       //�����Ӽ�           	
KEY_CTV_PREVCH,	       //                   	
KEY_CTV_CHUP,	       //Ƶ���Ӽ�           	
KEY_CTV_VOLDN,	       //��������           	
KEY_CTV_INPUT,	       //                   	
KEY_CTV_CHDN,	       //Ƶ������           	
KEY_CTV_PGUP,	       //P+                 	
KEY_CTV_PGDN,	       //P-                 	
KEY_CTV_REW,	       //����               	
KEY_CTV_PLAY,	       //����               	
KEY_CTV_STOP,	       //ֹͣ��             	
KEY_CTV_FF,	       //���               	
KEY_CTV_1,	       //���ּ�1            	
KEY_CTV_2,	       //���ּ�2            	
KEY_CTV_3,	       //���ּ�3            	
KEY_CTV_4,	       //���ּ�4            	
KEY_CTV_5,	       //���ּ�5            	
KEY_CTV_6,	       //���ּ�6            	
KEY_CTV_7,	       //���ּ�7            	
KEY_CTV_8,	       //���ּ�8            	
KEY_CTV_9,	       //���ּ�9            	
KEY_CTV_ENTER,	       //                   	
KEY_CTV_0,	       //���ּ�0            	
KEY_CTV_kuohao,	       //(-) 
KEY_CTV_MAX,
}KEY_CTV_ENUM;

typedef enum/*���ɸı��ö�ٵ�˳���ֵ*/
{
KEY_DVD_MUTE = 1,          //������             	
KEY_DVD_POWER,	       //��Դ��             	
KEY_DVD_MUNE,	       //�˵���             	
KEY_DVD_UP,	       //�ϼ�               	
KEY_DVD_Guide,	       //����               	
KEY_DVD_LEFT,	       //���               	
KEY_DVD_OK,	       //OK ��              	
KEY_DVD_RIGHT,	       //�Ҽ�               	
KEY_DVD_INFO,	       //                   	
KEY_DVD_DOWN,	       //�¼�               	
KEY_DVD_EXIT,	       //�˳�               	
KEY_DVD_VOLUP,	       //�����Ӽ�           	
KEY_DVD_PREVCH,	       //                   	
KEY_DVD_CHUP,	       //Ƶ���Ӽ�           	
KEY_DVD_VOLDN,	       //��������           	
KEY_DVD_INPUT,	       //                   	
KEY_DVD_CHDN,	       //Ƶ������           	
KEY_DVD_PGUP,	       //P+                 	
KEY_DVD_PGDN,	       //P-                 	
KEY_DVD_REW,	       //����               	
KEY_DVD_PLAY,	       //����               	
KEY_DVD_STOP,	       //ֹͣ��             	
KEY_DVD_FF,	       //���               	
KEY_DVD_1,	       //���ּ�1            	
KEY_DVD_2,	       //���ּ�2            	
KEY_DVD_3,	       //���ּ�3            	
KEY_DVD_4,	       //���ּ�4            	
KEY_DVD_5,	       //���ּ�5            	
KEY_DVD_6,	       //���ּ�6            	
KEY_DVD_7,	       //���ּ�7            	
KEY_DVD_8,	       //���ּ�8            	
KEY_DVD_9,	       //���ּ�9            	
KEY_DVD_ENTER,	       //                   	
KEY_DVD_0,	       //���ּ�0            	
KEY_DVD_kuohao,	       //(-) 
KEY_DVD_MAX,
}KEY_DVD_ENUM;

typedef enum/*���ɸı��ö�ٵ�˳���ֵ*/
{
KEY_DVB_MUTE = 1,          //������             	
KEY_DVB_POWER,	       //��Դ��             	
KEY_DVB_MUNE,	       //�˵���             	
KEY_DVB_UP,	       //�ϼ�               	
KEY_DVB_RETURN,	       //����              	
KEY_DVB_LEFT,	       //���               	
KEY_DVB_OK,	       //OK ��              	
KEY_DVB_RIGHT,	       //�Ҽ�               	
KEY_DVB_jiemubiao,     //��Ŀ��                	
KEY_DVB_DOWN,	       //�¼�               	
KEY_DVB_EXIT,	       //�˳�               	
KEY_DVB_VOLUP,	       //�����Ӽ�           	
KEY_DVB_shengdao,      //����               	
KEY_DVB_CHUP,	       //Ƶ���Ӽ�           	
KEY_DVB_VOLDN,	       //��������           	
KEY_DVB_zixun,	       //��Ѷ              	
KEY_DVB_CHDN,	       //Ƶ������           	
KEY_DVB_PGUP,	       //�Ϸ�ҳ                	
KEY_DVB_PGDN,	       //�·�ҳ  
KEY_DVB_RED,	       //�� 
KEY_DVB_GREEN,	       //�� 
KEY_DVB_YELLOW,        //�� 
KEY_DVB_BLUE,	       //��             	              	
KEY_DVB_1,	       //���ּ�1            	
KEY_DVB_2,	       //���ּ�2            	
KEY_DVB_3,	       //���ּ�3            	
KEY_DVB_4,	       //���ּ�4            	
KEY_DVB_5,	       //���ּ�5            	
KEY_DVB_6,	       //���ּ�6            	
KEY_DVB_7,	       //���ּ�7            	
KEY_DVB_8,	       //���ּ�8            	
KEY_DVB_9,	       //���ּ�9            	
KEY_DVB_favor,	       //ϲ��                  	
KEY_DVB_0,	       //���ּ�0            	
KEY_DVB_INFO,	       //��Ϣ
KEY_DVB_MAX,
}KEY_DVB_ENUM;

typedef enum/*���ɸı��ö�ٵ�˳���ֵ*/
{
KEY_ATV_MUTE = 1,          //������             	
KEY_ATV_POWER,	       //��Դ��             	
KEY_ATV_MUNE,	       //�˵���             	
KEY_ATV_UP,	       //�ϼ�               	
KEY_ATV_AVTV,	       //              	
KEY_ATV_LEFT,	       //���               	
KEY_ATV_OK,	       //OK ��              	
KEY_ATV_RIGHT,	       //�Ҽ�               	
KEY_ATV_pingxian,      //����               	
KEY_ATV_DOWN,	       //�¼�               	
KEY_ATV_wangfan,       //����               	
KEY_ATV_VOLUP,	       //�����Ӽ�           	
KEY_ATV_zhishi,        //��ʽ             	
KEY_ATV_CHUP,	       //Ƶ���Ӽ�           	
KEY_ATV_VOLDN,	       //��������           	
KEY_ATV_zhengchang,    //����            	
KEY_ATV_CHDN,	       //Ƶ������           	
KEY_ATV_liyin,	       //����                	
KEY_ATV_banyin,	       //����  
KEY_ATV_PIP,	       //���л�
KEY_ATV_SLEEP,	       //˯��          	              	
KEY_ATV_1,	       //���ּ�1            	
KEY_ATV_2,	       //���ּ�2            	
KEY_ATV_3,	       //���ּ�3            	
KEY_ATV_4,	       //���ּ�4            	
KEY_ATV_5,	       //���ּ�5            	
KEY_ATV_6,	       //���ּ�6            	
KEY_ATV_7,	       //���ּ�7            	
KEY_ATV_8,	       //���ּ�8            	
KEY_ATV_9,	       //���ּ�9            	
KEY_ATV_qiehuan,       //-/--               	
KEY_ATV_0,	       //���ּ�0            	
KEY_ATV_bili,	       //16:09
KEY_ATV_MAX,
}KEY_ATV_ENUM;

/*=========================================================================
ARC DAT PUBLIC FUNCTION & VAR
==========================================================================*/

/*=========================================================================
variable announce
==========================================================================*/

/*=========================================================================
variable define
==========================================================================*/

/********************************************************************************/
#endif
