
#ifndef __PORT_SMEM_H__
#define __PORT_SMEM_H__

#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <mt-plat/mtk_ccci_common.h>
#include "ccci_config.h"
#include "ccci_core.h"
#include "ccci_modem.h"
#include "port_t.h"

enum {
	TYPE_RAW = 0,
	TYPE_CCB,
};

enum {
	CCB_USER_INVALID = 0,
	CCB_USER_OK,
	CCB_USER_ERR,
};

struct ccci_ccb_ctrl {
	unsigned char *ctrl_addr_phy;
	unsigned char *ctrl_addr_vir;
};

struct ccci_smem_port {
	enum SMEM_USER_ID user_id;
	unsigned char type;
	unsigned short core_id;

	unsigned char state;
	unsigned int wakeup;
	unsigned char wk_cnt;
	phys_addr_t addr_phy;
	unsigned char *addr_vir;
	unsigned int length;
	struct port_t *port;
	wait_queue_head_t rx_wq;
	int ccb_ctrl_offset;
	struct hrtimer notify_timer;
	spinlock_t write_lock;
};
#endif
