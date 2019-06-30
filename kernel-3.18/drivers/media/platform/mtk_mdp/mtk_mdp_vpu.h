
#ifndef __MTK_MDP_VPU_H__
#define __MTK_MDP_VPU_H__

#include <linux/dma-direction.h>
#include "mtk_mdp_ipi.h"

struct mtk_mdp_vpu {
	struct platform_device		*pdev;
	unsigned int			failure;
	void				*shmem_va;
	struct mdp_process_param	*param;
};

int mtk_mdp_vpu_register(struct mtk_mdp_vpu *vpu, struct platform_device *pdev);
int mtk_mdp_vpu_init(struct mtk_mdp_vpu *vpu);
int mtk_mdp_vpu_deinit(struct mtk_mdp_vpu *vpu);
int mtk_mdp_vpu_process(struct mtk_mdp_vpu *vpu);

#endif /* __MTK_MDP_VPU_H__ */
