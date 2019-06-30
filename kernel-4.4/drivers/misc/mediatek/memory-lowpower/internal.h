#ifndef __MEMORY_LOWPOWER_INTERNAL_H
#define __MEMORY_LOWPOWER_INTERNAL_H

/* Memory Lowpower State & Action */
enum power_state {
	MLP_INIT,		/* Before memory-lowpower is initialized */
	/* action */
	MLP_ENTER_AGAIN,	/* Entering MLP again (valid at MLP_ENABLE) */
	MLP_ENTER,		/* Entering MLP */
	MLP_LEAVE,		/* Leaving MLP */
	/* state */
	MLP_ENABLE,		/* MLP is enabled */
	MLP_DISABLE,		/* MLP is disabled */
	MLP_NR_STATUS,
};

#define TEST_MEMORY_LOWPOWER_STATE(uname, lname) \
static inline int Mlps##uname(unsigned long *state) \
		{ return test_bit(MLP_##lname, state); }

#define SET_MEMORY_LOWPOWER_STATE(uname, lname)	\
static inline void SetMlps##uname(unsigned long *state) \
			{ set_bit(MLP_##lname, state); }

#define CLEAR_MEMORY_LOWPOWER_STATE(uname, lname) \
static inline void ClearMlps##uname(unsigned long *state) \
			{ clear_bit(MLP_##lname, state); }

TEST_MEMORY_LOWPOWER_STATE(Init, INIT)
TEST_MEMORY_LOWPOWER_STATE(Enter, ENTER)
TEST_MEMORY_LOWPOWER_STATE(Leave, LEAVE)
TEST_MEMORY_LOWPOWER_STATE(EnterAgain, ENTER_AGAIN)
TEST_MEMORY_LOWPOWER_STATE(Enable, ENABLE)
TEST_MEMORY_LOWPOWER_STATE(Disable, DISABLE)

SET_MEMORY_LOWPOWER_STATE(Init, INIT)
SET_MEMORY_LOWPOWER_STATE(Enter, ENTER)
SET_MEMORY_LOWPOWER_STATE(Leave, LEAVE)
SET_MEMORY_LOWPOWER_STATE(EnterAgain, ENTER_AGAIN)
SET_MEMORY_LOWPOWER_STATE(Enable, ENABLE)
SET_MEMORY_LOWPOWER_STATE(Disable, DISABLE)

CLEAR_MEMORY_LOWPOWER_STATE(Init, INIT)
CLEAR_MEMORY_LOWPOWER_STATE(Enter, ENTER)
CLEAR_MEMORY_LOWPOWER_STATE(Leave, LEAVE)
CLEAR_MEMORY_LOWPOWER_STATE(EnterAgain, ENTER_AGAIN)
CLEAR_MEMORY_LOWPOWER_STATE(Enable, ENABLE)
CLEAR_MEMORY_LOWPOWER_STATE(Disable, DISABLE)

#define IS_ACTION_ENTER(action)		(action == MLP_ENTER)
#define IS_ACTION_LEAVE(action)		(action == MLP_LEAVE)

/* Memory Lowpower Features & their operations */
enum power_level {
	MLP_LEVEL_DCS,
	MLP_LEVEL_PASR,
	NR_MLP_LEVEL,
};

typedef void (*get_range_t) (int, unsigned long *, unsigned long *);

/* Feature specific operations */
struct memory_lowpower_operation {
	struct list_head link;
	enum power_level level;
	/*
	 * Taking actions before entering this feature -
	 * callee needs to issue func to get the range "times" times
	 */
	int (*config)(int times, get_range_t func);
	/* Entering this feature */
	int (*enable)(void);
	/* Leaving this feature */
	int (*disable)(void);
	/* Taking actions after leaving this feature */
	int (*restore)(void);
};

struct memory_lowpower_statistics {
	u64 nr_acquire_memory;
	u64 nr_release_memory;
	u64 nr_full_acquire;
	u64 nr_partial_acquire;
	u64 nr_empty_acquire;
};


/* memory-lowpower-task APIs */
extern phys_addr_t memory_lowpower_get_grab_lastsize(void);
extern bool memory_lowpower_task_inited(void);
extern void register_memory_lowpower_operation(struct memory_lowpower_operation *handler);
extern void unregister_memory_lowpower_operation(struct memory_lowpower_operation *handler);

/* memory-lowpower APIs */
extern bool memory_lowpower_inited(void);
extern int get_memory_lowpower_cma(void);
extern int put_memory_lowpower_cma(void);
extern int get_memory_lowpower_cma_aligned(int count, unsigned int align, struct page **pages, bool last);
extern int put_memory_lowpower_cma_aligned(int count, struct page *pages, bool last);
extern int memory_lowpower_task_init(void);
extern phys_addr_t memory_lowpower_base(void);
extern phys_addr_t memory_lowpower_size(void);
extern void set_memory_lowpower_aligned(int aligned);

#endif /* __MEMORY_LOWPOWER_INTERNAL_H */
