
#include "mtk_cpuidle.h"
#include "mtk_spm.h"

char *irq_match[] = {
	"mediatek,kp",
	"mediatek,mt6763-consys",
	"mediatek,mdcldma",
	""
};

unsigned int irq_nr[IRQ_NR_MAX];

int wake_src_irq[] = {
	WAKE_SRC_R12_KP_IRQ_B,
	WAKE_SRC_R12_CONN2AP_SPM_WAKEUP_B,
	WAKE_SRC_R12_MD1_WDT_B,
	0
};

int irq_offset[] = {
	0,
	1,
	3,
	0
};

