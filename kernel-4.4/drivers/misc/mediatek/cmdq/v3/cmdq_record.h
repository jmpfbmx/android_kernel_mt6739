
#ifndef __CMDQ_RECORD_H__
#define __CMDQ_RECORD_H__

#include <linux/types.h>
#include "cmdq_def.h"
#include "cmdq_core.h"

struct TaskStruct;

enum CMDQ_STACK_TYPE_ENUM {
	CMDQ_STACK_NULL = -1,
	CMDQ_STACK_TYPE_IF = 0,
	CMDQ_STACK_TYPE_ELSE = 1,
	CMDQ_STACK_TYPE_WHILE = 2,
	CMDQ_STACK_TYPE_BREAK = 3,
	CMDQ_STACK_TYPE_CONTINUE = 4,
	CMDQ_STACK_TYPE_DO_WHILE = 5,
};

#define CMDQ_DATA_BIT			(62)
#define CMDQ_BIT_VALUE			(0LL)
#define CMDQ_BIT_VAR			(1LL)
#define CMDQ_TASK_CPR_INITIAL_VALUE	(0)
#define CMDQ_REC_DEFAULT_PRIORITY	(100)
#define CMDQ_REC_MAX_PRIORITY		(0x7FFFFFFF)

struct cmdq_stack_node {
	uint32_t position;
	enum CMDQ_STACK_TYPE_ENUM stack_type;
	struct cmdq_stack_node *next;
};

struct cmdq_sub_function {
	bool is_subfunction;		/* [IN]true for subfunction */
	int32_t reference_cnt;
	uint32_t in_num;
	uint32_t out_num;
	CMDQ_VARIABLE *in_arg;
	CMDQ_VARIABLE *out_arg;
};

struct cmdqRecStruct {
	u64 engineFlag;
	s32 scenario;
	u32 blockSize;	/* command size */
	void *pBuffer;
	u32 bufferSize;	/* allocated buffer size */
	struct TaskStruct *pRunningTask;	/* running task after flush() or startLoop() */
	u32 priority;	/* task schedule priority. this is NOT HW thread priority. */
	bool finalized;		/* set to true after flush() or startLoop() */
	u32 prefetchCount;	/* maintenance prefetch instruction */

	struct cmdqSecDataStruct secData;	/* secure execution data */

	/* For v3 CPR use */
	struct cmdq_v3_replace_struct replace_instr;
	u8 local_var_num;
	struct cmdq_stack_node *if_stack_node;
	struct cmdq_stack_node *while_stack_node;
	CMDQ_VARIABLE arg_value;	/* temp data or poll value, wait_timeout start */
	CMDQ_VARIABLE arg_source;	/* poll source, wait_timeout event */
	CMDQ_VARIABLE arg_timeout;	/* wait_timeout timeout */

	/* profile marker */
	struct cmdqProfileMarkerStruct profileMarker;

	/* task property */
	void *prop_addr;
	u32 prop_size;
	struct CmdqRecExtend ext;
};

/* typedef dma_addr_t cmdqBackupSlotHandle; */
#define cmdqBackupSlotHandle dma_addr_t

/* typedef void *CmdqRecLoopHandle; */

#ifdef __cplusplus
extern "C" {
#endif

	int32_t cmdq_task_create(enum CMDQ_SCENARIO_ENUM scenario, struct cmdqRecStruct **pHandle);
	int32_t cmdqRecCreate(enum CMDQ_SCENARIO_ENUM scenario, struct cmdqRecStruct **pHandle);

	int32_t cmdq_task_set_engine(struct cmdqRecStruct *handle, uint64_t engineFlag);
	int32_t cmdqRecSetEngine(struct cmdqRecStruct *handle, uint64_t engineFlag);

	int32_t cmdq_task_reset(struct cmdqRecStruct *handle);
	int32_t cmdqRecReset(struct cmdqRecStruct *handle);

	int32_t cmdq_task_set_secure(struct cmdqRecStruct *handle, const bool is_secure);
	int32_t cmdqRecSetSecure(struct cmdqRecStruct *handle, const bool is_secure);

	int32_t cmdq_task_is_secure(struct cmdqRecStruct *handle);
	int32_t cmdqRecIsSecure(struct cmdqRecStruct *handle);

	int32_t cmdq_task_secure_enable_dapc(struct cmdqRecStruct *handle, const uint64_t engineFlag);
	int32_t cmdqRecSecureEnableDAPC(struct cmdqRecStruct *handle, const uint64_t engineFlag);

	int32_t cmdq_task_secure_enable_port_security(struct cmdqRecStruct *handle, const uint64_t engineFlag);
	int32_t cmdqRecSecureEnablePortSecurity(struct cmdqRecStruct *handle, const uint64_t engineFlag);

	int32_t cmdqRecMark(struct cmdqRecStruct *handle);

	int32_t cmdqRecEnablePrefetch(struct cmdqRecStruct *handle);

	int32_t cmdqRecDisablePrefetch(struct cmdqRecStruct *handle);

	int32_t cmdq_op_write_reg(struct cmdqRecStruct *handle, uint32_t addr,
				   CMDQ_VARIABLE argument, uint32_t mask);
	int32_t cmdqRecWrite(struct cmdqRecStruct *handle, uint32_t addr, uint32_t value, uint32_t mask);

	int32_t cmdq_op_write_reg_secure(struct cmdqRecStruct *handle, uint32_t addr,
				   enum CMDQ_SEC_ADDR_METADATA_TYPE type, uint64_t baseHandle,
				   uint32_t offset, uint32_t size, uint32_t port);
	int32_t cmdqRecWriteSecure(struct cmdqRecStruct *handle,
				   uint32_t addr,
				   enum CMDQ_SEC_ADDR_METADATA_TYPE type,
				   uint64_t baseHandle,
				   uint32_t offset, uint32_t size, uint32_t port);

	int32_t cmdq_op_poll(struct cmdqRecStruct *handle, uint32_t addr, uint32_t value, uint32_t mask);
	int32_t cmdqRecPoll(struct cmdqRecStruct *handle, uint32_t addr, uint32_t value, uint32_t mask);

	int32_t cmdq_op_wait(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM event);
	int32_t cmdqRecWait(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM event);

	int32_t cmdq_op_wait_no_clear(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM event);
	int32_t cmdqRecWaitNoClear(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM event);

	int32_t cmdq_op_clear_event(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM event);
	int32_t cmdqRecClearEventToken(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM event);

	int32_t cmdq_op_set_event(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM event);
	int32_t cmdqRecSetEventToken(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM event);

	s32 cmdq_op_replace_overwrite_cpr(struct cmdqRecStruct *handle, u32 index,
		s32 new_arg_a, s32 new_arg_b, s32 new_arg_c);

	int32_t cmdq_op_read_to_data_register(struct cmdqRecStruct *handle, uint32_t hw_addr,
					  enum CMDQ_DATA_REGISTER_ENUM dst_data_reg);
	int32_t cmdqRecReadToDataRegister(struct cmdqRecStruct *handle, uint32_t hw_addr,
					  enum CMDQ_DATA_REGISTER_ENUM dst_data_reg);

	int32_t cmdq_op_write_from_data_register(struct cmdqRecStruct *handle,
					     enum CMDQ_DATA_REGISTER_ENUM src_data_reg, uint32_t hw_addr);
	int32_t cmdqRecWriteFromDataRegister(struct cmdqRecStruct *handle,
					     enum CMDQ_DATA_REGISTER_ENUM src_data_reg,
					     uint32_t hw_addr);


	int32_t cmdq_alloc_mem(cmdqBackupSlotHandle *p_h_backup_slot, uint32_t slotCount);
	int32_t cmdqBackupAllocateSlot(cmdqBackupSlotHandle *p_h_backup_slot, uint32_t slotCount);

	int32_t cmdq_cpu_read_mem(cmdqBackupSlotHandle h_backup_slot, uint32_t slot_index,
				   uint32_t *value);
	int32_t cmdqBackupReadSlot(cmdqBackupSlotHandle h_backup_slot, uint32_t slot_index,
				   uint32_t *value);

	int32_t cmdq_cpu_write_mem(cmdqBackupSlotHandle h_backup_slot, uint32_t slot_index,
					uint32_t value);
	int32_t cmdqBackupWriteSlot(cmdqBackupSlotHandle h_backup_slot, uint32_t slot_index,
				    uint32_t value);


	int32_t cmdq_free_mem(cmdqBackupSlotHandle h_backup_slot);
	int32_t cmdqBackupFreeSlot(cmdqBackupSlotHandle h_backup_slot);


	int32_t cmdq_op_read_reg_to_mem(struct cmdqRecStruct *handle,
					    cmdqBackupSlotHandle h_backup_slot,
					    uint32_t slot_index, uint32_t addr);
	int32_t cmdqRecBackupRegisterToSlot(struct cmdqRecStruct *handle,
					    cmdqBackupSlotHandle h_backup_slot,
					    uint32_t slot_index, uint32_t addr);

	int32_t cmdq_op_read_mem_to_reg(struct cmdqRecStruct *handle,
						   cmdqBackupSlotHandle h_backup_slot,
						   uint32_t slot_index, uint32_t addr);
	int32_t cmdqRecBackupWriteRegisterFromSlot(struct cmdqRecStruct *handle,
						   cmdqBackupSlotHandle h_backup_slot,
						   uint32_t slot_index, uint32_t addr);

	int32_t cmdq_op_write_mem(struct cmdqRecStruct *handle, cmdqBackupSlotHandle h_backup_slot,
						uint32_t slot_index, uint32_t value);
	int32_t cmdqRecBackupUpdateSlot(struct cmdqRecStruct *handle,
					cmdqBackupSlotHandle h_backup_slot,
					uint32_t slot_index, uint32_t value);

	int32_t cmdq_task_flush(struct cmdqRecStruct *handle);
	int32_t cmdqRecFlush(struct cmdqRecStruct *handle);

	int32_t cmdq_task_flush_and_read_register(struct cmdqRecStruct *handle, uint32_t regCount,
					    uint32_t *addrArray, uint32_t *valueArray);
	int32_t cmdqRecFlushAndReadRegister(struct cmdqRecStruct *handle, uint32_t regCount,
					    uint32_t *addrArray, uint32_t *valueArray);

	int32_t cmdq_task_flush_async(struct cmdqRecStruct *handle);
	int32_t cmdqRecFlushAsync(struct cmdqRecStruct *handle);

	int32_t cmdq_task_flush_async_callback(struct cmdqRecStruct *handle,
		CmdqAsyncFlushCB callback, u64 user_data);
	int32_t cmdqRecFlushAsyncCallback(struct cmdqRecStruct *handle,
		CmdqAsyncFlushCB callback, u64 user_data);

	int32_t cmdq_task_start_loop(struct cmdqRecStruct *handle);
	int32_t cmdqRecStartLoop(struct cmdqRecStruct *handle);

	int32_t cmdq_task_start_loop_callback(struct cmdqRecStruct *handle, CmdqInterruptCB loopCB,
		unsigned long loopData);
	int32_t cmdqRecStartLoopWithCallback(struct cmdqRecStruct *handle, CmdqInterruptCB loopCB,
		unsigned long loopData);

	s32 cmdq_task_start_loop_sram(struct cmdqRecStruct *handle, const char *SRAM_owner_name);

	int32_t cmdq_task_stop_loop(struct cmdqRecStruct *handle);
	int32_t cmdqRecStopLoop(struct cmdqRecStruct *handle);

	s32 cmdq_task_copy_to_sram(dma_addr_t pa_src, u32 sram_dest, size_t size);
	s32 cmdq_task_copy_from_sram(dma_addr_t pa_dest, u32 sram_src, size_t size);

	int32_t cmdq_task_get_instruction_count(struct cmdqRecStruct *handle);
	int32_t cmdqRecGetInstructionCount(struct cmdqRecStruct *handle);

	int32_t cmdq_op_profile_marker(struct cmdqRecStruct *handle, const char *tag);
	int32_t cmdqRecProfileMarker(struct cmdqRecStruct *handle, const char *tag);

	int32_t cmdq_task_dump_command(struct cmdqRecStruct *handle);
	int32_t cmdqRecDumpCommand(struct cmdqRecStruct *handle);

	int32_t cmdq_task_estimate_command_exec_time(const struct cmdqRecStruct *handle);
	int32_t cmdqRecEstimateCommandExecTime(const struct cmdqRecStruct *handle);

	int32_t cmdq_task_destroy(struct cmdqRecStruct *handle);
	void cmdqRecDestroy(struct cmdqRecStruct *handle);

	int32_t cmdq_op_set_nop(struct cmdqRecStruct *handle, uint32_t index);
	int32_t cmdqRecSetNOP(struct cmdqRecStruct *handle, uint32_t index);

	int32_t cmdq_task_query_offset(struct cmdqRecStruct *handle, uint32_t startIndex,
				   const enum CMDQ_CODE_ENUM opCode, enum CMDQ_EVENT_ENUM event);
	int32_t cmdqRecQueryOffset(struct cmdqRecStruct *handle, uint32_t startIndex,
				   const enum CMDQ_CODE_ENUM opCode, enum CMDQ_EVENT_ENUM event);

	int32_t cmdq_resource_acquire(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM resourceEvent);
	int32_t cmdqRecAcquireResource(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM resourceEvent);

	int32_t cmdq_resource_acquire_and_write(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM resourceEvent,
		uint32_t addr, uint32_t value, uint32_t mask);
	int32_t cmdqRecWriteForResource(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM resourceEvent,
		uint32_t addr, uint32_t value, uint32_t mask);

	int32_t cmdq_resource_release(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM resourceEvent);
	int32_t cmdqRecReleaseResource(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM resourceEvent);

	int32_t cmdq_resource_release_and_write(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM resourceEvent,
		uint32_t addr, uint32_t value, uint32_t mask);
	int32_t cmdqRecWriteAndReleaseResource(struct cmdqRecStruct *handle, enum CMDQ_EVENT_ENUM resourceEvent,
		uint32_t addr, uint32_t value, uint32_t mask);

	void cmdq_op_init_variable(CMDQ_VARIABLE *arg);

	void cmdq_op_init_global_cpr_variable(CMDQ_VARIABLE *arg, u32 cpr_offset);

	int32_t cmdq_op_assign(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out, CMDQ_VARIABLE arg_in);

	int32_t cmdq_op_add(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
				   CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

	int32_t cmdq_op_subtract(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
				   CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

	int32_t cmdq_op_multiply(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
				   CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

	int32_t cmdq_op_xor(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
				   CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

	int32_t cmdq_op_not(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out, CMDQ_VARIABLE arg_b);

	int32_t cmdq_op_or(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
				   CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

	int32_t cmdq_op_and(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
				   CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

	int32_t cmdq_op_left_shift(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
				   CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

	int32_t cmdq_op_right_shift(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
				   CMDQ_VARIABLE arg_b, CMDQ_VARIABLE arg_c);

	s32 cmdq_op_delay_us(struct cmdqRecStruct *handle, u32 delay_time);
	s32 cmdq_op_backup_CPR(struct cmdqRecStruct *handle, CMDQ_VARIABLE cpr,
		cmdqBackupSlotHandle h_backup_slot, uint32_t slot_index);
	s32 cmdq_op_backup_TPR(struct cmdqRecStruct *handle,
		cmdqBackupSlotHandle h_backup_slot, uint32_t slot_index);

	int32_t cmdq_op_if(struct cmdqRecStruct *handle, CMDQ_VARIABLE arg_b,
				   enum CMDQ_CONDITION_ENUM arg_condition, CMDQ_VARIABLE arg_c);

	int32_t cmdq_op_end_if(struct cmdqRecStruct *handle);

	int32_t cmdq_op_else(struct cmdqRecStruct *handle);

	int32_t cmdq_op_else_if(struct cmdqRecStruct *handle, CMDQ_VARIABLE arg_b,
				   enum CMDQ_CONDITION_ENUM arg_condition, CMDQ_VARIABLE arg_c);

	int32_t cmdq_op_while(struct cmdqRecStruct *handle, CMDQ_VARIABLE arg_b,
				   enum CMDQ_CONDITION_ENUM arg_condition, CMDQ_VARIABLE arg_c);

	s32 cmdq_op_continue(struct cmdqRecStruct *handle);

	s32 cmdq_op_break(struct cmdqRecStruct *handle);

	s32 cmdq_op_end_while(struct cmdqRecStruct *handle);

	s32 cmdq_op_do_while(struct cmdqRecStruct *handle);

	s32 cmdq_op_end_do_while(struct cmdqRecStruct *handle, CMDQ_VARIABLE arg_b,
		enum CMDQ_CONDITION_ENUM arg_condition, CMDQ_VARIABLE arg_c);


	s32 cmdq_op_wait_event_timeout(struct cmdqRecStruct *handle, CMDQ_VARIABLE *arg_out,
			enum CMDQ_EVENT_ENUM wait_event, u32 timeout_time);

	int32_t cmdq_op_read_reg(struct cmdqRecStruct *handle, uint32_t addr,
				   CMDQ_VARIABLE *arg_out, uint32_t mask);

	int32_t cmdq_op_read_mem(struct cmdqRecStruct *handle, cmdqBackupSlotHandle h_backup_slot,
				    uint32_t slot_index, CMDQ_VARIABLE *arg_out);
	s32 cmdq_task_update_property(struct cmdqRecStruct *handle, void *prop_addr, u32 prop_size);



#ifdef __cplusplus
}
#endif
#endif				/* __CMDQ_RECORD_H__ */
