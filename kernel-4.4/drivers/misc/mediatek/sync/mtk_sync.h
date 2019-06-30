
#ifndef _MTK_SYNC_H
#define _MTK_SYNC_H

#define TIMEOUT_NEVER   -1

/* --------------------------------------------------------------------------- */

#ifdef __KERNEL__

#include <linux/version.h>

#include <../drivers/staging/android/sw_sync.h>


struct fence_data {
	__u32 value;
	char name[32];
	__s32 fence;	/* fd of new fence */
};



struct sw_sync_timeline *timeline_create(const char *name);

void timeline_destroy(struct sw_sync_timeline *obj);

void timeline_inc(struct sw_sync_timeline *obj, u32 value);

int fence_create(struct sw_sync_timeline *obj, struct fence_data *data);

int fence_merge(char *const name, int fd1, int fd2);

#endif	/* __KERNEL __ */

#endif	/* _MTK_SYNC_H */
