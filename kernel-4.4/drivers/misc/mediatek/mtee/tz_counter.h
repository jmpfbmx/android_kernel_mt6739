
#ifndef __KREE_INC_COUNTER
#define __KREE_INC_COUNTER

#ifdef CONFIG_ARM64
/* enable counter implementation */
#define ENABLE_INC_ONLY_COUNTER
#endif

int update_counter_thread(void *data);

#endif
