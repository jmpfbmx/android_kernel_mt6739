
#include <mt-plat/mtk_secure_api.h>

#include <mtk_spm.h>
#include <mtk_spm_idle.h>
#include <mtk_spm_internal.h>
#include <mtk_spm_sodi_cmdq.h>
#include <mtk_spm_vcore_dvfs.h>
#include <mtk_spm_resource_req_internal.h>


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


void spm_sodi_pre_process(struct pwr_ctrl *pwrctrl, u32 operation_cond)
{
	dvfsrc_md_scenario_update_to_fb(1);
}

void spm_sodi_post_process(void)
{
	dvfsrc_md_scenario_update_to_fb(0);
}

void spm_sodi3_pre_process(struct pwr_ctrl *pwrctrl, u32 operation_cond)
{
	dvfsrc_md_scenario_update_to_fb(1);
}

void spm_sodi3_post_process(void)
{
	dvfsrc_md_scenario_update_to_fb(0);
}

void spm_sodi_pcm_setup_before_wfi(
	u32 cpu, struct pcm_desc *pcmdesc, struct pwr_ctrl *pwrctrl, u32 operation_cond)
{
	unsigned int resource_usage;

	spm_sodi_pre_process(pwrctrl, operation_cond);

	__spm_sync_pcm_flags(pwrctrl);

	/* Get SPM resource request and update reg_spm_xxx_req */
	resource_usage = spm_get_resource_usage();

	mt_secure_call(MTK_SIP_KERNEL_SPM_SODI_ARGS,
		pwrctrl->pcm_flags, pwrctrl->pcm_flags1, resource_usage);
}

void spm_sodi3_pcm_setup_before_wfi(
	u32 cpu, struct pcm_desc *pcmdesc, struct pwr_ctrl *pwrctrl, u32 operation_cond)
{
	unsigned int resource_usage;

	spm_sodi3_pre_process(pwrctrl, operation_cond);

	__spm_sync_pcm_flags(pwrctrl);

	/* Get SPM resource request and update reg_spm_xxx_req */
	resource_usage = spm_get_resource_usage();

	mt_secure_call(MTK_SIP_KERNEL_SPM_SODI_ARGS,
		pwrctrl->pcm_flags, pwrctrl->pcm_flags1, resource_usage);
	mt_secure_call(MTK_SIP_KERNEL_SPM_PWR_CTRL_ARGS,
		SPM_PWR_CTRL_SODI3, PWR_WDT_DISABLE, pwrctrl->wdt_disable);
}

