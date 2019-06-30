
#ifndef __FBT_BASE_H__
#define __FBT_BASE_H__

#include <linux/compiler.h>

#ifdef FBT_DEBUG
#define FBT_LOGI(...)	pr_debug("FBT:" __VA_ARGS__)
#else
#define FBT_LOGI(...)
#endif
#define FBT_LOGE(...)	pr_debug("FBT:" __VA_ARGS__)
#define FBT_CONTAINER_OF(ptr, type, member) ((type *)(((char *)ptr) - offsetof(type, member)))

unsigned long ged_copy_to_user(void __user *pvTo, const void *pvFrom, unsigned long ulBytes);

unsigned long ged_copy_from_user(void *pvTo, const void __user *pvFrom, unsigned long ulBytes);

void *fbt_alloc(int i32Size);

void *fbt_alloc_atomic(int i32Size);

void fbt_free(void *pvBuf, int i32Size);

long ged_get_pid(void);

unsigned long long ged_get_time(void);

#endif
