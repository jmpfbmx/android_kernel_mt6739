
#ifndef __HAL_BTIF_PUB_H_
#define __HAL_BTIF_PUB_H_

#include "plat_common.h"

/*Enum Defination*/
/*BTIF Mode Enum */
typedef enum _ENUM_BTIF_MODE_ {
	BTIF_MODE_PIO = 0,
	BTIF_MODE_DMA = BTIF_MODE_PIO + 1,
	BTIF_MODE_MAX,
} ENUM_BTIF_MODE;

P_MTK_BTIF_INFO_STR hal_btif_info_get(void);

#if 0				/*included in hal_btif_info_get */
P_MTK_BTIF_IRQ_STR hal_btif_get_irq(void);
#endif

#if !defined(CONFIG_MTK_CLKMGR)
int hal_btif_clk_get_and_prepare(struct platform_device *pdev);
int hal_btif_clk_unprepare(void);
#endif
int hal_btif_clk_ctrl(P_MTK_BTIF_INFO_STR p_btif, ENUM_CLOCK_CTRL flag);

int hal_btif_hw_init(P_MTK_BTIF_INFO_STR p_btif);

int hal_btif_rx_cb_reg(P_MTK_BTIF_INFO_STR p_btif_info,
		       btif_rx_buf_write rx_cb);

int hal_btif_loopback_ctrl(P_MTK_BTIF_INFO_STR p_btif, bool en);

int hal_btif_irq_handler(P_MTK_BTIF_INFO_STR p_btif,
			 unsigned char *p_buf, const unsigned int max_len);

int hal_btif_tx_mode_ctrl(P_MTK_BTIF_INFO_STR p_btif, ENUM_BTIF_MODE mode);

int hal_btif_rx_mode_ctrl(P_MTK_BTIF_INFO_STR p_btif, ENUM_BTIF_MODE mode);

int hal_btif_send_data(P_MTK_BTIF_INFO_STR p_btif,
		       const unsigned char *p_buf, const unsigned int buf_len);

int hal_btif_raise_wak_sig(P_MTK_BTIF_INFO_STR p_btif);

int hal_btif_dump_reg(P_MTK_BTIF_INFO_STR p_btif, ENUM_BTIF_REG_ID flag);

bool hal_btif_is_tx_complete(P_MTK_BTIF_INFO_STR p_btif);

bool hal_btif_is_tx_allow(P_MTK_BTIF_INFO_STR p_btif);

int hal_btif_pm_ops(P_MTK_BTIF_INFO_STR p_btif, MTK_BTIF_PM_OPID opid);

void mtk_btif_read_cpu_sw_rst_debug_plat(void);

#endif /*__HAL_BTIF_PUB_H_*/
