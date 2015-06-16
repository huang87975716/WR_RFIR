#ifndef _ARC_INDEX_H_
#define _ARC_INDEX_H_
/*********************************************************/
#define	arc_icgroup_max         999     //IC内部空调码组数目
#define	arc_exgroup_max         15	//外挂空调数据组数，对应“arc_exdata_index[]”
#define arc_brand_max           31      //空调品牌数目，对应“g_remote_arc_info[40][48]”
/*********************************************************/
extern const int arc_exdata_addr[];
extern const u16 g_remote_arc_info[41][49];




#endif
