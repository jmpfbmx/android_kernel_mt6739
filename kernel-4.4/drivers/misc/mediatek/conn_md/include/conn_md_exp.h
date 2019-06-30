
#ifndef __CONN_MD_EXP_H_
#define __CONN_MD_EXP_H_

#include "port_ipc.h"		/*data structure is defined here, mediatek/kernel/drivers/eccci */
#include "ccci_ipc_task_ID.h"	/*IPC task id is defined here, mediatek/kernel/drivers/eccci */
#define uint32 unsigned int
#define uint8 unsigned char
#define uint16 unsigned short
#ifdef CHAR
#undef CHAR
#endif

typedef struct ipc_ilm conn_md_ipc_ilm_t;

enum CONN_MD_ERR_CODE {
	CONN_MD_ERR_NO_ERR = 0,
	CONN_MD_ERR_DEF_ERR = -1,
	CONN_MD_ERR_INVALID_PARAM = -2,
	CONN_MD_ERR_OTHERS = -4,
};

/*For IDC test*/
typedef int (*conn_md_msg_rx_cb) (struct ipc_ilm *ilm);

struct conn_md_bridge_ops {
	conn_md_msg_rx_cb rx_cb;
};

extern int mtk_conn_md_bridge_reg(uint32 u_id, struct conn_md_bridge_ops *p_ops);
extern int mtk_conn_md_bridge_unreg(uint32 u_id);
extern int mtk_conn_md_bridge_send_msg(struct ipc_ilm *ilm);

#endif /*__CONN_MD_EXP_H_*/
