
/*****************************************************************************/
#ifndef	 _REMOTE_IOCTL_H
#define 	_REMOTE_IOCTL_H
/*****************************************************************************/
//#include "remote.h"
/*********************************************************************/
#define  SEND_CODE  	      0x01			//按键有发送发码命令
#define 	SEND_END  		0x11		//发码结束

#define 	LENTIMEOUT		0x80
#define  LEN_DOING  	      0x01
#define  LEN_OK  	      0x02
#define  LEN_ERROR	0x03
/********************************************************************************/
enum
{
	HB_IOCTL_INIT = 0,		//初始化，应用程序打开时，调用此函数
	HB_IOCTL_TIMER100,		//定时器100ms到时，调用此函数
	HB_IOCTL_TIMER500,		//RTC定时器500ms
	HB_IOCTL_CREATE_IRDevice,	//新建红外设备（收藏红外设备时调用）
	//建立对应红外设备 数据
	//TV(设备类型)，品牌，码组索引，学习数据
	//ARC:空调相关变量
	HB_IOCTL_Delete_Device,		//删除红外设备（删除收藏记录时调用）
	HB_IOCTL_KeyDn,				//设备按键按下，调用此函数发码
	HB_IOCTL_KeyUp,
		//序根据当前设备值，红外设备号，调用对应文件中对应数据，查表或运算后发码
	HB_IOCTL_Change_Device,   // 更改设备类型，
		//应用层改变设备类型，红外设备，品牌，或者码组时，需要调用此函数更新红外系统内部变量
	HB_IOCTL_IRRX_BEGIN,	//程序发码给HB1682 ，启动学习； 具体查询学习是否完成，在定时器中处理;
	HB_IOCTL_ARC_IRRX,		//启动空调码学习
	HB_IOCTL_IRRX_END,      // 在学习过程中，用户可以取消学习，则应用层需要调用此函数终止学习；
	HB_IOCTL_Send_LenCode,	//发送URC学习的数据
	HB_IOCTL_Send_ArcLenCode,	//发送ARC学习码
	HB_IOCTL_RX_ICVER,		//读取IC 程序版本
	HB_IOCTL_RX_PRO,		//串口接收数据处理
	HB_IOCTL_SEND_Search,	//搜索发码
};	//ev_ioctl_e



typedef struct
{
	//u8  searchflg;	//搜索状态标志	!=0,表示进搜索
	u8  device;		//设备号
	u16 group;		//码组号，当group>=0x8000时，表示外挂文件码组
	u8  keyval;		//键值
	u8  *arc_state_var;	//指向空调变量，只有device = C_ARC_DEVICE  时有效
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
	s16  group_aux;	//国外AUX
	s16	 group_dvb;	//国内DVB
	s16  group_stb;	//国外机顶盒
	s16  group_atv;	//国内TV，或东南亚TV
	s16  group_ctv;	//国外TV
	s16	 group_dvd;	//国外DVD
	s16  group_arc;	//空调
}type_CUR_group;

typedef struct
{
	u16  arc_group;		
	u8	clk_hour;
	u8    clk_min;		//
}type_updatatime;

/********************************************************************
函数定义
*********************************************************************/

int HB_ioctl(int cmd, void* arg);





#endif



