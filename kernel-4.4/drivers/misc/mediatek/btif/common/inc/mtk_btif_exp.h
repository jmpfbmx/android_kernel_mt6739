
#ifndef _MTK_BTIF_EXP_H_
#define _MTK_BTIF_EXP_H_

/*--------------marco defination---------------*/
#define BTIF_MAX_LEN_PER_PKT 2048
#define BTIF_RXD_BE_BLOCKED_DETECT 1
/*--------------Enum Defination---------------*/
typedef enum _ENUM_BTIF_DPIDLE_ {
	BTIF_DPIDLE_DISABLE = 0,
	BTIF_DPIDLE_ENABLE = BTIF_DPIDLE_DISABLE + 1,
	BTIF_DPIDLE_MAX,
} ENUM_BTIF_DPIDLE_CTRL;

typedef enum _ENUM_BTIF_LPBK_MODE_ {
	BTIF_LPBK_DISABLE = 0,
	BTIF_LPBK_ENABLE = BTIF_LPBK_DISABLE + 1,
	BTIF_LPBK_MAX,
} ENUM_BTIF_LPBK_MODE;

typedef enum _ENUM_BTIF_DBG_ID_ {
	BTIF_DISABLE_LOGGER = 0,
	BTIF_ENABLE_LOGGER = BTIF_DISABLE_LOGGER + 1,
	BTIF_DUMP_LOG = BTIF_ENABLE_LOGGER + 1,
	BTIF_CLR_LOG = BTIF_DUMP_LOG + 1,
	BTIF_DUMP_BTIF_REG = BTIF_CLR_LOG + 1,
	BTIF_ENABLE_RT_LOG = BTIF_DUMP_BTIF_REG + 1,
	BTIF_DISABLE_RT_LOG = BTIF_ENABLE_RT_LOG + 1,
	BTIF_DUMP_BTIF_IRQ = BTIF_DISABLE_RT_LOG + 1,
	BTIF_DBG_MAX,
} ENUM_BTIF_DBG_ID;

typedef enum _ENUM_BTIF_OP_ERROR_CODE_ {
	E_BTIF_AGAIN = 0,
	E_BTIF_FAIL = -1,
	E_BTIF_BAD_POINTER = -2,
	E_BTIF_NO_SPACE = -3,
	E_BTIF_INTR = -4,
	E_BTIF_INVAL_PARAM = -5,
	E_BTIF_ALREADY_OPEN = -6,
	E_BTIF_NOT_OPEN = -7,
	E_BTIF_INVAL_STATE = -8,
} ENUM_BTIF_OP_ERROR_CODE;

/*--------------End of Enum Defination---------------*/

/*--------------Type Definition---------------*/

typedef int (*MTK_WCN_BTIF_RX_CB) (const unsigned char *p_buf,
				   unsigned int len);

/*--------------End of Type Definition---------------*/

/*--------------Normal Mode API declearation---------------*/

int mtk_wcn_btif_open(char *p_owner, unsigned long *p_id);

int mtk_wcn_btif_close(unsigned long u_id);

int mtk_wcn_btif_write(unsigned long u_id,
		       const unsigned char *p_buf, unsigned int len);

int mtk_wcn_btif_read(unsigned long u_id,
		      unsigned char *p_buf, unsigned int max_len);

int mtk_wcn_btif_dpidle_ctrl(unsigned long u_id, ENUM_BTIF_DPIDLE_CTRL en_flag);

int mtk_wcn_btif_rx_cb_register(unsigned long u_id, MTK_WCN_BTIF_RX_CB rx_cb);

int mtk_wcn_btif_wakeup_consys(unsigned long u_id);

/*--------------End of Normal Mode API declearation----------------*/

/*--------------Debug Purpose API declearation----------------*/

int mtk_wcn_btif_loopback_ctrl(unsigned long u_id, ENUM_BTIF_LPBK_MODE enable);

int mtk_wcn_btif_dbg_ctrl(unsigned long u_id, ENUM_BTIF_DBG_ID flag);
/*-----------End of Debug Purpose API declearation------------*/

bool mtk_wcn_btif_parser_wmt_evt(unsigned long u_id,
	const char *sub_str, unsigned int str_len);

int mtk_btif_exp_open_test(void);
int mtk_btif_exp_close_test(void);
int mtk_btif_exp_write_test(void);
int mtk_btif_exp_suspend_test(void);
int mtk_btif_exp_resume_test(void);
int mtk_btif_exp_enter_dpidle_test(void);
int mtk_btif_exp_exit_dpidle_test(void);
int mtk_btif_exp_write_stress_test(unsigned int length, unsigned int loop);
int mtk_btif_exp_log_debug_test(int flag);
int mtk_btif_exp_restore_noirq_test(void);
int btif_wakeup_consys_no_id(void);
int mtk_btif_exp_clock_ctrl(int en);
#if BTIF_RXD_BE_BLOCKED_DETECT
int mtk_btif_rxd_be_blocked_flag_get(void);
#endif
void mtk_btif_read_cpu_sw_rst_debug_exp(void);
#endif /*_MTK_BTIF_EXP_H_*/
