
#ifndef __CMDQ_RECORD_H__
#define __CMDQ_RECORD_H__

#include <linux/types.h>
#include "cmdq_def.h"
#include "cmdq_core.h"

struct TaskStruct;
typedef uint64_t CMDQ_VARIABLE;

struct cmdqRecStruct {
	uint64_t engineFlag;
	int32_t scenario;
	uint32_t blockSize;	/* command size */
	void *pBuffer;
	uint32_t bufferSize;	/* allocated buffer size */
	struct TaskStruct *pRunningTask;	/* running task after flush() or startLoop() */
	enum CMDQ_HW_THREAD_PRIORITY_ENUM priority;	/* setting high priority. This implies Prefetch ENABLE. */
	bool finalized;		/* set to true after flush() or startLoop() */
	uint32_t prefetchCount;	/* maintenance prefetch instruction */

	struct cmdqSecDataStruct secData;	/* secure execution data */

	/* profile marker */
#ifdef CMDQ_PROFILE_MARKER_SUPPORT
	struct cmdqProfileMarkerStruct profileMarker;
#endif
};

/* typedef dma_addr_t cmdqBackupSlotHandle; */
#define cmdqBackupSlotHandle dma_addr_t

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

/* tablet use */
#ifdef CONFIG_MTK_CMDQ_TAB
	int32_t cmdq_op_write_reg_secure_mask(struct cmdqRecStruct *handle, uint32_t addr,
				enum CMDQ_SEC_ADDR_METADATA_TYPE type, uint32_t value, uint32_t mask);
	int32_t cmdqRecWriteSecureMask(struct cmdqRecStruct *handle, uint32_t addr,
				enum CMDQ_SEC_ADDR_METADATA_TYPE type, uint32_t value, uint32_t mask);
#endif

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

	int32_t cmdq_task_flush_async_callback(struct cmdqRecStruct *handle, CmdqAsyncFlushCB callback,
					  uint32_t userData);
	int32_t cmdqRecFlushAsyncCallback(struct cmdqRecStruct *handle, CmdqAsyncFlushCB callback,
					  uint32_t userData);

	int32_t cmdq_task_start_loop(struct cmdqRecStruct *handle);
	int32_t cmdqRecStartLoop(struct cmdqRecStruct *handle);

	int32_t cmdq_task_start_loop_callback(struct cmdqRecStruct *handle, CmdqInterruptCB loopCB,
		unsigned long loopData);
	int32_t cmdqRecStartLoopWithCallback(struct cmdqRecStruct *handle, CmdqInterruptCB loopCB,
		unsigned long loopData);

	int32_t cmdq_task_stop_loop(struct cmdqRecStruct *handle);
	int32_t cmdqRecStopLoop(struct cmdqRecStruct *handle);

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

/* tablet use */
#ifdef CONFIG_MTK_CMDQ_TAB
	int32_t cmdq_task_set_secure_mode(struct cmdqRecStruct *handle, enum CMDQ_DISP_MODE mode);
	int32_t cmdqRecSetSecureMode(struct cmdqRecStruct *handle, enum CMDQ_DISP_MODE mode);
#endif

#ifdef __cplusplus
}
#endif
#endif				/* __CMDQ_RECORD_H__ */
