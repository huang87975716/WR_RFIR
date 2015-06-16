#include "remote.h"

/***************************************************************************/

type_ex_ctl	ex_ctl;
u16 current_group[7];	//当前码组
type_remote_ctl remote_ctl_c;
/***************************************************************************/

/************************************************************************************************************
*************************************************************************************************************/
int  keydn_general_pro(type_remote_ctl  *remote_ctl_g)
{
	int send_state=0;
	
	if(((remote_ctl_g->group)&0x8000)==0)
	{
		if(remote_ctl_g->device != C_ARC_DEVICE)
		{
		//发内部URC
			Remote_Uart_CmdTxURCIndex(remote_ctl_g->device,remote_ctl_g->group,remote_ctl_g->keyval);	//发内部URC
			send_state = 1;
		}
		else
		{
			/*
			//发内部ARC
			ADR_VAR[ADR_KEYVAL] = remote_ctl_g->keyval;
			ARC_USE_PRO(remote_ctl_g->group,remote_ctl_g->keyval);
			if(VarFlag&VarFlag_SEND)
			{
				Remote_Uart_CmdTxARCIndex(remote_ctl_g->group,remote_ctl_g->arc_state_var);	//发内部ARC
				VarFlag = VarFlag - VarFlag_SEND;
				send_state = 2;
			}
			*/
		}
	}
	else
	{
// 		FGroup = (u8)((remote_ctl_g->group)&0x7fff);
// 		
// 		if(remote_ctl_g->device != C_ARC_DEVICE)
// 		{
// 			//发URC外挂文件码组
// 			Remote_Uart_CmdTxURCFile(FGroup, (u8)remote_ctl_g->keyval);	//发URC外挂文件码组
// 			send_state = 1;
// 		}
// 		else
// 		{
// 			//发ARC外挂文件码组
// 			ADR_VAR[ADR_KEYVAL] = remote_ctl_c.keyval;
// 			ARC_USE_PRO(remote_ctl_g->group,remote_ctl_g->keyval);
// 			if(VarFlag&VarFlag_SEND)
// 			{
// 				Remote_Uart_CmdTxARCFile(FGroup,remote_ctl_g->arc_state_var);	//发ARC外挂文件码组
// 				VarFlag = VarFlag - VarFlag_SEND;
// 				send_state = 2;
// 			}
// 			else	
// 			{
// 				updata_arc_disp = 1;
// 				ARC_DISPLAY_PRO(remote_ctl_g->group,Ar_Disp_DATA);	//更新显示buf
// 			}
// 		}
		
	}
	return send_state;
}
/*
void  Search_Send_Pro(type_remote_ctl  *remote_ctl_g)
{
	u8 FGroup;
	u8  skeyval;

	skeyval = KeyValSearch[remote_ctl_g->device];
	
	if(remote_ctl_g->group < EXT_FILE_DATA)
	{
		if(remote_ctl_g->device != C_ARC_DEVICE)
		{
		//发内部URC
			Remote_Uart_CmdTxURCIndex(remote_ctl_g->device,remote_ctl_g->group,skeyval);	//发内部URC
			//ex_ctl.Time_100m = 10;
			//ex_ctl.Time_100m_cnt = 0;
			ex_ctl.cmd = HB_IOCTL_SEND_Search;
			 ex_ctl.exit_en = 1;
		}
		else
		{
		//发内部ARC
			ADR_VAR_INIT_Search(remote_ctl_g->group);	//全局变量初始化
			Remote_Uart_CmdTxARCIndex(remote_ctl_g->group,remote_ctl_g->arc_state_var);	//发内部ARC
			ARC_DISPLAY_PRO(remote_ctl_g->group,Ar_Disp_DATA);	//更新显示buf
			updata_arc_disp = 1;
		//	ARC_CLK_TM_UPDATA(remote_ctl_g->group,ARC_clock.GetHour(),ARC_clock.GetMinute());		//更新定时时间
		}
	}
	else
	{
		FGroup = (u8)(remote_ctl_g->group - EXT_FILE_DATA);
		
		if(remote_ctl_g->device != C_ARC_DEVICE)
		{
			//发URC外挂文件码组
			Remote_Uart_CmdTxURCFile(FGroup, skeyval);	//发URC外挂文件码组
			ex_ctl.Time_100m = 10;
			ex_ctl.Time_100m_cnt = 0;
			 ex_ctl.exit_en = 1;
		}
		else
		{
			//发ARC外挂文件码组
			ADR_VAR_INIT_Search(remote_ctl_g->group);	//全局变量初始化
			Remote_Uart_CmdTxARCFile(FGroup,remote_ctl_g->arc_state_var);	//发ARC外挂文件码组
			ARC_DISPLAY_PRO(remote_ctl_g->group,Ar_Disp_DATA);	//更新显示buf
			updata_arc_disp = 1;
		//	ARC_CLK_TM_UPDATA(remote_ctl_g->group,ARC_clock.GetHour(),ARC_clock.GetMinute());		//更新定时时间
		}
		
	}

}
*/
