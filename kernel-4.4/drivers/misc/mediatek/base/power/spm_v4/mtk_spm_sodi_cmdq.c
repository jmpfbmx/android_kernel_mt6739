
#include <mtk_spm_sodi_cmdq.h>


void exit_pd_by_cmdq(struct cmdqRecStruct *handler)
{
	/* Switch to CG mode */
	cmdqRecWrite(handler, 0x100065F8, 0x9ce2, 0xffff);
	/* Polling ack */
	cmdqRecPoll(handler, 0x100065DC, 0x0, ~0);
}

void enter_pd_by_cmdq(struct cmdqRecStruct *handler)
{
	/* Switch to PD mode */
	cmdqRecWrite(handler, 0x100065F8, 0x9ce0, 0xffff);
}
