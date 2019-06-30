
#include <mtk_spm_internal.h>

void spm_dpidle_pre_process(unsigned int operation_cond, struct pwr_ctrl *pwrctrl)
{
	__spm_sync_pcm_flags(pwrctrl);
}

void spm_dpidle_post_process(void)
{
}

void spm_deepidle_chip_init(void)
{
}

