
#ifndef _FPSGO_MINI_TOP_H_
#define _FPSGO_MINI_TOP_H_

#include <linux/rbtree.h>
#include <linux/list.h>

struct tid_util {
	pid_t tid;
	int util;
};

struct minitop_work {
	pid_t tid[NR_CPUS];
	int util[NR_CPUS];
	struct work_struct work;
	struct list_head link;
};

#define MINITOP_SCHED		1
#define MINITOP_FTEH		2

struct minitop_rec {
	pid_t tid;
	struct rb_node node;

	u64 init_runtime;
	u64 init_timestamp;
	u64 ratio;
	u32 life;
	int debnc;
	int ever;

	int source;
	int debnc_fteh; /* irrelative to ceiling switch */
};

int __init minitop_init(void);
void __exit minitop_exit(void);

extern void (*fpsgo_sched_nominate_fp)(pid_t *, int *);

#endif
