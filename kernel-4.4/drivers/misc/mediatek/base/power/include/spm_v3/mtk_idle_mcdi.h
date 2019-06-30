
#ifndef __MTK_IDLE_MCDI_H__
#define __MTK_IDLE_MCDI_H__

/* IDLE_TYPE is used for idle_switch in mt_idle.c */
enum {
	IDLE_TYPE_DP = 0,
	IDLE_TYPE_SO3,
	IDLE_TYPE_SO,
	IDLE_TYPE_MC,
	IDLE_TYPE_MCSO,
	IDLE_TYPE_SL,
	IDLE_TYPE_RG,
	NR_TYPES,
};

int mtk_idle_select(int cpu);
#endif /* __MTK_IDLE_MCDI_H__ */
