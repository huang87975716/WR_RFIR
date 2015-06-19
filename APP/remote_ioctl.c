#include "remote.h"

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
		}
	}
	else
	{
	}
	return send_state;
}
