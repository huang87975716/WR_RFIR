/***************************************************************************
*****************************************************************************/
#ifndef _REMOTE_URC_PRO_H
#define _REMOTE_URC_PRO_H
/*****************************************************************************/
//====================================================================
//	ARC DAT CONSTANT DEFINE
//=====================================================================
//设备号定义
#define	C_AUX_DEVICE		0	//国外
#define	C_DVB_DEVICE		1	//国内DVB
#define	C_STB_DEVICE		2	//国外机顶盒
#define	C_ATV_DEVICE		3	//国内TV，或东南亚TV
#define	C_CTV_DEVICE		4	//国外TV
#define	C_DVD_DEVICE		5	//国外DVD
#define	C_ARC_DEVICE		6	//
//每个设备的最大码组数定义
#define	C_AUX_GROUP_MAX         501
#define	C_DVB_GROUP_MAX         372
#define	C_STB_GROUP_MAX         213
#define	C_ATV_GROUP_MAX         550
#define	C_CTV_GROUP_MAX         360
#define	C_DVD_GROUP_MAX         315
#define	C_ARC_GROUP_MAX         1000
//=======================================================================
//按键定义
typedef enum/*不可改变此枚举的顺序和值*/
{
KEY_AUX_MUTE = 1,          //静音键
KEY_AUX_POWER,	       //电源键
KEY_AUX_MUNE,	       //菜单键
KEY_AUX_UP,	       //上键
KEY_AUX_Guide,	       //导视
KEY_AUX_LEFT,	       //左键
KEY_AUX_OK,	       //OK 键
KEY_AUX_RIGHT,	       //右键
KEY_AUX_INFO,	       //
KEY_AUX_DOWN,	       //下键
KEY_AUX_EXIT,	       //退出
KEY_AUX_VOLUP,	       //音量加键
KEY_AUX_PREVCH,	       //
KEY_AUX_CHUP,	       //频道加键  
KEY_AUX_VOLDN,	       //音量减键
KEY_AUX_INPUT,	       //
KEY_AUX_CHDN,	       //频道减键
KEY_AUX_PGUP,	       //P+
KEY_AUX_PGDN,	       //P-
KEY_AUX_REW,	       //快退
KEY_AUX_PLAY,	       //播放
KEY_AUX_STOP,	       //停止键
KEY_AUX_FF,	       //快进
KEY_AUX_1,	       //数字键1
KEY_AUX_2,	       //数字键2
KEY_AUX_3,	       //数字键3
KEY_AUX_4,	       //数字键4
KEY_AUX_5,	       //数字键5
KEY_AUX_6,	       //数字键6
KEY_AUX_7,	       //数字键7
KEY_AUX_8,	       //数字键8
KEY_AUX_9,	       //数字键9
KEY_AUX_ENTER,	       //   
KEY_AUX_0,	       //数字键0
KEY_AUX_kuohao,	       //(-)
KEY_AUX_MAX,
}KEY_AUX_ENUM;

typedef enum/*不可改变此枚举的顺序和值*/
{
KEY_STB_MUTE = 1,          //静音键
KEY_STB_POWER,	       //电源键
KEY_STB_MUNE,	       //菜单键
KEY_STB_UP,	       //上键
KEY_STB_Guide,	       //导视
KEY_STB_LEFT,	       //左键
KEY_STB_OK,	       //OK 键
KEY_STB_RIGHT,	       //右键
KEY_STB_INFO,	       //
KEY_STB_DOWN,	       //下键
KEY_STB_EXIT,	       //退出
KEY_STB_VOLUP,	       //音量加键
KEY_STB_PREVCH,	       //
KEY_STB_CHUP,	       //频道加键  
KEY_STB_VOLDN,	       //音量减键
KEY_STB_INPUT,	       //
KEY_STB_CHDN,	       //频道减键
KEY_STB_PGUP,	       //P+
KEY_STB_PGDN,	       //P-
KEY_STB_REW,	       //快退
KEY_STB_PLAY,	       //播放
KEY_STB_STOP,	       //停止键
KEY_STB_FF,	       //快进
KEY_STB_1,	       //数字键1
KEY_STB_2,	       //数字键2
KEY_STB_3,	       //数字键3
KEY_STB_4,	       //数字键4
KEY_STB_5,	       //数字键5
KEY_STB_6,	       //数字键6
KEY_STB_7,	       //数字键7
KEY_STB_8,	       //数字键8
KEY_STB_9,	       //数字键9
KEY_STB_ENTER,	       //   
KEY_STB_0,	       //数字键0
KEY_STB_kuohao,	       //(-)
KEY_STB_MAX,
}KEY_STB_ENUM;

typedef enum/*不可改变此枚举的顺序和值*/
{
KEY_CTV_MUTE = 1,          //静音键             	
KEY_CTV_POWER,	       //电源键             	
KEY_CTV_MUNE,	       //菜单键             	
KEY_CTV_UP,	       //上键               	
KEY_CTV_Guide,	       //导视               	
KEY_CTV_LEFT,	       //左键               	
KEY_CTV_OK,	       //OK 键              	
KEY_CTV_RIGHT,	       //右键               	
KEY_CTV_INFO,	       //                   	
KEY_CTV_DOWN,	       //下键               	
KEY_CTV_EXIT,	       //退出               	
KEY_CTV_VOLUP,	       //音量加键           	
KEY_CTV_PREVCH,	       //                   	
KEY_CTV_CHUP,	       //频道加键           	
KEY_CTV_VOLDN,	       //音量减键           	
KEY_CTV_INPUT,	       //                   	
KEY_CTV_CHDN,	       //频道减键           	
KEY_CTV_PGUP,	       //P+                 	
KEY_CTV_PGDN,	       //P-                 	
KEY_CTV_REW,	       //快退               	
KEY_CTV_PLAY,	       //播放               	
KEY_CTV_STOP,	       //停止键             	
KEY_CTV_FF,	       //快进               	
KEY_CTV_1,	       //数字键1            	
KEY_CTV_2,	       //数字键2            	
KEY_CTV_3,	       //数字键3            	
KEY_CTV_4,	       //数字键4            	
KEY_CTV_5,	       //数字键5            	
KEY_CTV_6,	       //数字键6            	
KEY_CTV_7,	       //数字键7            	
KEY_CTV_8,	       //数字键8            	
KEY_CTV_9,	       //数字键9            	
KEY_CTV_ENTER,	       //                   	
KEY_CTV_0,	       //数字键0            	
KEY_CTV_kuohao,	       //(-) 
KEY_CTV_MAX,
}KEY_CTV_ENUM;

typedef enum/*不可改变此枚举的顺序和值*/
{
KEY_DVD_MUTE = 1,          //静音键             	
KEY_DVD_POWER,	       //电源键             	
KEY_DVD_MUNE,	       //菜单键             	
KEY_DVD_UP,	       //上键               	
KEY_DVD_Guide,	       //导视               	
KEY_DVD_LEFT,	       //左键               	
KEY_DVD_OK,	       //OK 键              	
KEY_DVD_RIGHT,	       //右键               	
KEY_DVD_INFO,	       //                   	
KEY_DVD_DOWN,	       //下键               	
KEY_DVD_EXIT,	       //退出               	
KEY_DVD_VOLUP,	       //音量加键           	
KEY_DVD_PREVCH,	       //                   	
KEY_DVD_CHUP,	       //频道加键           	
KEY_DVD_VOLDN,	       //音量减键           	
KEY_DVD_INPUT,	       //                   	
KEY_DVD_CHDN,	       //频道减键           	
KEY_DVD_PGUP,	       //P+                 	
KEY_DVD_PGDN,	       //P-                 	
KEY_DVD_REW,	       //快退               	
KEY_DVD_PLAY,	       //播放               	
KEY_DVD_STOP,	       //停止键             	
KEY_DVD_FF,	       //快进               	
KEY_DVD_1,	       //数字键1            	
KEY_DVD_2,	       //数字键2            	
KEY_DVD_3,	       //数字键3            	
KEY_DVD_4,	       //数字键4            	
KEY_DVD_5,	       //数字键5            	
KEY_DVD_6,	       //数字键6            	
KEY_DVD_7,	       //数字键7            	
KEY_DVD_8,	       //数字键8            	
KEY_DVD_9,	       //数字键9            	
KEY_DVD_ENTER,	       //                   	
KEY_DVD_0,	       //数字键0            	
KEY_DVD_kuohao,	       //(-) 
KEY_DVD_MAX,
}KEY_DVD_ENUM;

typedef enum/*不可改变此枚举的顺序和值*/
{
KEY_DVB_MUTE = 1,          //静音键             	
KEY_DVB_POWER,	       //电源键             	
KEY_DVB_MUNE,	       //菜单键             	
KEY_DVB_UP,	       //上键               	
KEY_DVB_RETURN,	       //返回              	
KEY_DVB_LEFT,	       //左键               	
KEY_DVB_OK,	       //OK 键              	
KEY_DVB_RIGHT,	       //右键               	
KEY_DVB_jiemubiao,     //节目表                	
KEY_DVB_DOWN,	       //下键               	
KEY_DVB_EXIT,	       //退出               	
KEY_DVB_VOLUP,	       //音量加键           	
KEY_DVB_shengdao,      //声道               	
KEY_DVB_CHUP,	       //频道加键           	
KEY_DVB_VOLDN,	       //音量减键           	
KEY_DVB_zixun,	       //资讯              	
KEY_DVB_CHDN,	       //频道减键           	
KEY_DVB_PGUP,	       //上翻页                	
KEY_DVB_PGDN,	       //下翻页  
KEY_DVB_RED,	       //红 
KEY_DVB_GREEN,	       //绿 
KEY_DVB_YELLOW,        //黄 
KEY_DVB_BLUE,	       //蓝             	              	
KEY_DVB_1,	       //数字键1            	
KEY_DVB_2,	       //数字键2            	
KEY_DVB_3,	       //数字键3            	
KEY_DVB_4,	       //数字键4            	
KEY_DVB_5,	       //数字键5            	
KEY_DVB_6,	       //数字键6            	
KEY_DVB_7,	       //数字键7            	
KEY_DVB_8,	       //数字键8            	
KEY_DVB_9,	       //数字键9            	
KEY_DVB_favor,	       //喜爱                  	
KEY_DVB_0,	       //数字键0            	
KEY_DVB_INFO,	       //信息
KEY_DVB_MAX,
}KEY_DVB_ENUM;

typedef enum/*不可改变此枚举的顺序和值*/
{
KEY_ATV_MUTE = 1,          //静音键             	
KEY_ATV_POWER,	       //电源键             	
KEY_ATV_MUNE,	       //菜单键             	
KEY_ATV_UP,	       //上键               	
KEY_ATV_AVTV,	       //              	
KEY_ATV_LEFT,	       //左键               	
KEY_ATV_OK,	       //OK 键              	
KEY_ATV_RIGHT,	       //右键               	
KEY_ATV_pingxian,      //屏显               	
KEY_ATV_DOWN,	       //下键               	
KEY_ATV_wangfan,       //往返               	
KEY_ATV_VOLUP,	       //音量加键           	
KEY_ATV_zhishi,        //制式             	
KEY_ATV_CHUP,	       //频道加键           	
KEY_ATV_VOLDN,	       //音量减键           	
KEY_ATV_zhengchang,    //正常            	
KEY_ATV_CHDN,	       //频道减键           	
KEY_ATV_liyin,	       //丽音                	
KEY_ATV_banyin,	       //伴音  
KEY_ATV_PIP,	       //画中画
KEY_ATV_SLEEP,	       //睡眠          	              	
KEY_ATV_1,	       //数字键1            	
KEY_ATV_2,	       //数字键2            	
KEY_ATV_3,	       //数字键3            	
KEY_ATV_4,	       //数字键4            	
KEY_ATV_5,	       //数字键5            	
KEY_ATV_6,	       //数字键6            	
KEY_ATV_7,	       //数字键7            	
KEY_ATV_8,	       //数字键8            	
KEY_ATV_9,	       //数字键9            	
KEY_ATV_qiehuan,       //-/--               	
KEY_ATV_0,	       //数字键0            	
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
