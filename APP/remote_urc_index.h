#ifndef _URC_INDEX_H_
#define _URC_INDEX_H_
#include "remote.h"
/*********************************************************/
#define	urc_icgroup_max         999     //IC内部万用码组数目
#define	urc_exgroup_max         55	//外挂空调数据组数，对应“urc_exdata_index[]”
/*********************************************************/
extern const s32 urc_exdata_addr[];
extern const s32 Device_Default_Group[];
extern const u16 g_remote_DVB_info[32][38];
extern const u16 g_remote_ATV_info[166][65];
extern const u16 g_remote_AUX_info[228][49];
extern const u16 g_remote_DVD_info[187][18];
extern const u16 g_remote_STB_info[149][27];
extern const u16 g_remote_CTV_info[262][37];
extern const u16 g_remote_all_group[4][502];
extern const u16 g_remote_num_tran[1143];
/*********************************************************/
#endif

