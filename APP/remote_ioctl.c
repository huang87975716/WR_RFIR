#include "remote.h"

/***************************************************************************/

type_ex_ctl	ex_ctl;
u16 current_group[7];	//��ǰ����
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
		//���ڲ�URC
			Remote_Uart_CmdTxURCIndex(remote_ctl_g->device,remote_ctl_g->group,remote_ctl_g->keyval);	//���ڲ�URC
			send_state = 1;
		}
		else
		{
			/*
			//���ڲ�ARC
			ADR_VAR[ADR_KEYVAL] = remote_ctl_g->keyval;
			ARC_USE_PRO(remote_ctl_g->group,remote_ctl_g->keyval);
			if(VarFlag&VarFlag_SEND)
			{
				Remote_Uart_CmdTxARCIndex(remote_ctl_g->group,remote_ctl_g->arc_state_var);	//���ڲ�ARC
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
// 			//��URC����ļ�����
// 			Remote_Uart_CmdTxURCFile(FGroup, (u8)remote_ctl_g->keyval);	//��URC����ļ�����
// 			send_state = 1;
// 		}
// 		else
// 		{
// 			//��ARC����ļ�����
// 			ADR_VAR[ADR_KEYVAL] = remote_ctl_c.keyval;
// 			ARC_USE_PRO(remote_ctl_g->group,remote_ctl_g->keyval);
// 			if(VarFlag&VarFlag_SEND)
// 			{
// 				Remote_Uart_CmdTxARCFile(FGroup,remote_ctl_g->arc_state_var);	//��ARC����ļ�����
// 				VarFlag = VarFlag - VarFlag_SEND;
// 				send_state = 2;
// 			}
// 			else	
// 			{
// 				updata_arc_disp = 1;
// 				ARC_DISPLAY_PRO(remote_ctl_g->group,Ar_Disp_DATA);	//������ʾbuf
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
		//���ڲ�URC
			Remote_Uart_CmdTxURCIndex(remote_ctl_g->device,remote_ctl_g->group,skeyval);	//���ڲ�URC
			//ex_ctl.Time_100m = 10;
			//ex_ctl.Time_100m_cnt = 0;
			ex_ctl.cmd = HB_IOCTL_SEND_Search;
			 ex_ctl.exit_en = 1;
		}
		else
		{
		//���ڲ�ARC
			ADR_VAR_INIT_Search(remote_ctl_g->group);	//ȫ�ֱ�����ʼ��
			Remote_Uart_CmdTxARCIndex(remote_ctl_g->group,remote_ctl_g->arc_state_var);	//���ڲ�ARC
			ARC_DISPLAY_PRO(remote_ctl_g->group,Ar_Disp_DATA);	//������ʾbuf
			updata_arc_disp = 1;
		//	ARC_CLK_TM_UPDATA(remote_ctl_g->group,ARC_clock.GetHour(),ARC_clock.GetMinute());		//���¶�ʱʱ��
		}
	}
	else
	{
		FGroup = (u8)(remote_ctl_g->group - EXT_FILE_DATA);
		
		if(remote_ctl_g->device != C_ARC_DEVICE)
		{
			//��URC����ļ�����
			Remote_Uart_CmdTxURCFile(FGroup, skeyval);	//��URC����ļ�����
			ex_ctl.Time_100m = 10;
			ex_ctl.Time_100m_cnt = 0;
			 ex_ctl.exit_en = 1;
		}
		else
		{
			//��ARC����ļ�����
			ADR_VAR_INIT_Search(remote_ctl_g->group);	//ȫ�ֱ�����ʼ��
			Remote_Uart_CmdTxARCFile(FGroup,remote_ctl_g->arc_state_var);	//��ARC����ļ�����
			ARC_DISPLAY_PRO(remote_ctl_g->group,Ar_Disp_DATA);	//������ʾbuf
			updata_arc_disp = 1;
		//	ARC_CLK_TM_UPDATA(remote_ctl_g->group,ARC_clock.GetHour(),ARC_clock.GetMinute());		//���¶�ʱʱ��
		}
		
	}

}
*/
