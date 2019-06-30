
#ifndef __MTK_OCP_H__
#define __MTK_OCP_H__

#include <linux/kernel.h>


/* turn on/off OCP driver */
#define OCP_FEATURE_ENABLED	(0)

enum ocp_cluster {
	OCP_LL = 0,
	OCP_L,
	OCP_B,

	NR_OCP_CLUSTER,
};

/* OCP APIs */
extern int mt_ocp_set_target(enum ocp_cluster cluster, unsigned int target);
extern unsigned int mt_ocp_get_avgpwr(enum ocp_cluster cluster);
extern bool mt_ocp_get_status(enum ocp_cluster cluster);
extern unsigned int mt_ocp_get_mcusys_pwr(void);

#endif /* __MTK_OCP_H__ */

