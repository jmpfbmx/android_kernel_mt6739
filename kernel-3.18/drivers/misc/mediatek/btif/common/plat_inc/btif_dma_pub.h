
#ifndef __HAL_BTIFD_DMA_PUB_H_
#define __HAL_BTIFD_DMA_PUB_H_

#include <linux/dma-mapping.h>

#include "plat_common.h"

typedef enum _ENUM_DMA_CTRL_ {
	DMA_CTRL_DISABLE = 0,
	DMA_CTRL_ENABLE = DMA_CTRL_DISABLE + 1,
	DMA_CTRL_BOTH,
} ENUM_DMA_CTRL;

P_MTK_DMA_INFO_STR hal_btif_dma_info_get(ENUM_DMA_DIR dma_dir);

int hal_btif_dma_hw_init(P_MTK_DMA_INFO_STR p_dma_info);

int hal_btif_dma_clk_ctrl(P_MTK_DMA_INFO_STR p_dma_info, ENUM_CLOCK_CTRL flag);

int hal_btif_dma_ctrl(P_MTK_DMA_INFO_STR p_dma_info, ENUM_DMA_CTRL ctrl_id);

int hal_btif_dma_rx_cb_reg(P_MTK_DMA_INFO_STR p_dma_info,
			   dma_rx_buf_write rx_cb);

int hal_btif_vfifo_reset(P_MTK_DMA_INFO_STR p_dma_info);

int hal_tx_dma_irq_handler(P_MTK_DMA_INFO_STR p_dma_info);

int hal_dma_send_data(P_MTK_DMA_INFO_STR p_dma_info,
		      const unsigned char *p_buf, const unsigned int buf_len);

bool hal_dma_is_tx_complete(P_MTK_DMA_INFO_STR p_dma_info);

int hal_dma_get_ava_room(P_MTK_DMA_INFO_STR p_dma_info);

bool hal_dma_is_tx_allow(P_MTK_DMA_INFO_STR p_dma_info);

int hal_rx_dma_irq_handler(P_MTK_DMA_INFO_STR p_dma_info,
			   unsigned char *p_buf, const unsigned int max_len);

int hal_dma_dump_reg(P_MTK_DMA_INFO_STR p_dma_info, ENUM_BTIF_REG_ID flag);

int hal_dma_pm_ops(P_MTK_DMA_INFO_STR p_dma_info, MTK_BTIF_PM_OPID opid);

#endif /*__HAL_BTIFD_DMA_PUB_H_*/
