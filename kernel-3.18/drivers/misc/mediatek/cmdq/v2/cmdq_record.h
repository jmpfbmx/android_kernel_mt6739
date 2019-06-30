
#ifndef __CMDQ_RECORD_H__
#define __CMDQ_RECORD_H__

#include <linux/types.h>
#include "cmdq_def.h"
#include "cmdq_core.h"

struct TaskStruct;
typedef uint64_t CMDQ_VARIABLE;

typedef struct cmdqRecStruct {
	uint64_t engineFlag;
	int32_t scenario;
	uint32_t blockSize;	/* command size */
	void *pBuffer;
	uint32_t bufferSize;	/* allocated buffer size */
	struct TaskStruct *pRunningTask;	/* running task after flush() or startLoop() */
	CMDQ_HW_THREAD_PRIORITY_ENUM priority;	/* setting high priority. This implies Prefetch ENABLE. */
	bool finalized;		/* set to true after flush() or startLoop() */
	uint32_t prefetchCount;	/* maintenance prefetch instruction */

	cmdqSecDataStruct secData;	/* secure execution data */

	/* profile marker */
#ifdef CMDQ_PROFILE_MARKER_SUPPORT
	cmdqProfileMarkerStruct profileMarker;
#endif
} cmdqRecStruct, *cmdqRecHandle;

typedef dma_addr_t cmdqBackupSlotHandle;

typedef void *CmdqRecLoopHandle;

#ifdef __cplusplus
extern "C" {
#endif

	int32_t cmdq_task_create(CMDQ_SCENARIO_ENUM scenario, cmdqRecHandle *pHandle);
	int32_t cmdqRecCreate(CMDQ_SCENARIO_ENUM scenario, cmdqRecHandle *pHandle);

	int32_t cmdq_task_set_engine(cmdqRecHandle handle, uint64_t engineFlag);
	int32_t cmdqRecSetEngine(cmdqRecHandle handle, uint64_t engineFlag);

	int32_t cmdq_task_reset(cmdqRecHandle handle);
	int32_t cmdqRecReset(cmdqRecHandle handle);

	int32_t cmdq_task_set_secure(cmdqRecHandle handle, const bool is_secure);
	int32_t cmdqRecSetSecure(cmdqRecHandle handle, const bool is_secure);

	int32_t cmdq_task_is_secure(cmdqRecHandle handle);
	int32_t cmdqRecIsSecure(cmdqRecHandle handle);

	int32_t cmdq_task_secure_enable_dapc(cmdqRecHandle handle, const uint64_t engineFlag);
	int32_t cmdqRecSecureEnableDAPC(cmdqRecHandle handle, const uint64_t engineFlag);

	int32_t cmdq_task_secure_enable_port_security(cmdqRecHandle handle, const uint64_t engineFlag);
	int32_t cmdqRecSecureEnablePortSecurity(cmdqRecHandle handle, const uint64_t engineFlag);

	int32_t cmdqRecMark(cmdqRecHandle handle);

	int32_t cmdqRecEnablePrefetch(cmdqRecHandle handle);

	int32_t cmdqRecDisablePrefetch(cmdqRecHandle handle);

	int32_t cmdq_op_write_reg(cmdqRecHandle handle, uint32_t addr,
				   CMDQ_VARIABLE argument, uint32_t mask);
	int32_t cmdqRecWrite(cmdqRecHandle handle, uint32_t addr, uint32_t value, uint32_t mask);

	int32_t cmdq_op_write_reg_secure(cmdqRecHandle handle, uint32_t addr,
				   CMDQ_SEC_ADDR_METADATA_TYPE type, uint64_t baseHandle,
				   uint32_t offset, uint32_t size, uint32_t port);
	int32_t cmdqRecWriteSecure(cmdqRecHandle handle,
				   uint32_t addr,
				   CMDQ_SEC_ADDR_METADATA_TYPE type,
				   uint64_t baseHandle,
				   uint32_t offset, uint32_t size, uint32_t port);

/* tablet use */
#ifdef CONFIG_MTK_CMDQ_TAB
	int32_t cmdq_op_write_reg_secure_mask(cmdqRecHandle handle, uint32_t addr,
				CMDQ_SEC_ADDR_METADATA_TYPE type, uint32_t value, uint32_t mask);
	int32_t cmdqRecWriteSecureMask(cmdqRecHandle handle, uint32_t addr,
				CMDQ_SEC_ADDR_METADATA_TYPE type, uint32_t value, uint32_t mask);
#endif

	int32_t cmdq_op_poll(cmdqRecHandle handle, uint32_t addr, uint32_t value, uint32_t mask);
	int32_t cmdqRecPoll(cmdqRecHandle handle, uint32_t addr, uint32_t value, uint32_t mask);

	int32_t cmdq_op_wait(cmdqRecHandle handle, CMDQ_EVENT_ENUM event);
	int32_t cmdqRecWait(cmdqRecHandle handle, CMDQ_EVENT_ENUM event);

	int32_t cmdq_op_wait_no_clear(cmdqRecHandle handle, CMDQ_EVENT_ENUM event);
	int32_t cmdqRecWaitNoClear(cmdqRecHandle handle, CMDQ_EVENT_ENUM event);

	int32_t cmdq_op_clear_event(cmdqRecHandle handle, CMDQ_EVENT_ENUM event);
	int32_t cmdqRecClearEventToken(cmdqRecHandle handle, CMDQ_EVENT_ENUM event);

	int32_t cmdq_op_set_event(cmdqRecHandle handle, CMDQ_EVENT_ENUM event);
	int32_t cmdqRecSetEventToken(cmdqRecHandle handle, CMDQ_EVENT_ENUM event);

	int32_t cmdq_op_read_to_data_register(cmdqRecHandle handle, uint32_t hw_addr,
					  CMDQ_DATA_REGISTER_ENUM dst_data_reg);
	int32_t cmdqRecReadToDataRegister(cmdqRecHandle handle, uint32_t hw_addr,
					  CMDQ_DATA_REGISTER_ENUM dst_data_reg);

	int32_t cmdq_op_write_from_data_register(cmdqRecHandle handle,
					     CMDQ_DATA_REGISTER_ENUM src_data_reg, uint32_t hw_addr);
	int32_t cmdqRecWriteFromDataRegister(cmdqRecHandle handle,
					     CMDQ_DATA_REGISTER_ENUM src_data_reg,
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


	int32_t cmdq_op_read_reg_to_mem(cmdqRecHandle handle,
					    cmdqBackupSlotHandle h_backup_slot,
					    uint32_t slot_index, uint32_t addr);
	int32_t cmdqRecBackupRegisterToSlot(cmdqRecHandle handle,
					    cmdqBackupSlotHandle h_backup_slot,
					    uint32_t slot_index, uint32_t addr);

	int32_t cmdq_op_read_mem_to_reg(cmdqRecHandle handle,
						   cmdqBackupSlotHandle h_backup_slot,
						   uint32_t slot_index, uint32_t addr);
	int32_t cmdqRecBackupWriteRegisterFromSlot(cmdqRecHandle handle,
						   cmdqBackupSlotHandle h_backup_slot,
						   uint32_t slot_index, uint32_t addr);

	int32_t cmdq_op_write_mem(cmdqRecHandle handle, cmdqBackupSlotHandle h_backup_slot,
						uint32_t slot_index, uint32_t value);
	int32_t cmdqRecBackupUpdateSlot(cmdqRecHandle handle,
					cmdqBackupSlotHandle h_backup_slot,
					uint32_t slot_index, uint32_t value);

	int32_t cmdq_task_flush(cmdqRecHandle handle);
	int32_t cmdqRecFlush(cmdqRecHandle handle);

	int32_t cmdq_task_flush_and_read_register(cmdqRecHandle handle, uint32_t regCount,
					    uint32_t *addrArray, uint32_t *valueArray);
	int32_t cmdqRecFlushAndReadRegister(cmdqRecHandle handle, uint32_t regCount,
					    uint32_t *addrArray, uint32_t *valueArray);

	int32_t cmdq_task_flush_async(cmdqRecHandle handle);
	int32_t cmdqRecFlushAsync(cmdqRecHandle handle);

	int32_t cmdq_task_flush_async_callback(cmdqRecHandle handle, CmdqAsyncFlushCB callback,
					  uint32_t userData);
	int32_t cmdqRecFlushAsyncCallback(cmdqRecHandle handle, CmdqAsyncFlushCB callback,
					  uint32_t userData);

	int32_t cmdq_task_start_loop(cmdqRecHandle handle);
	int32_t cmdqRecStartLoop(cmdqRecHandle handle);

	int32_t cmdq_task_start_loop_callback(cmdqRecHandle handle, CmdqInterruptCB loopCB, unsigned long loopData);
	int32_t cmdqRecStartLoopWithCallback(cmdqRecHandle handle, CmdqInterruptCB loopCB, unsigned long loopData);

	int32_t cmdq_task_stop_loop(cmdqRecHandle handle);
	int32_t cmdqRecStopLoop(cmdqRecHandle handle);

	int32_t cmdq_task_get_instruction_count(cmdqRecHandle handle);
	int32_t cmdqRecGetInstructionCount(cmdqRecHandle handle);

	int32_t cmdq_op_profile_marker(cmdqRecHandle handle, const char *tag);
	int32_t cmdqRecProfileMarker(cmdqRecHandle handle, const char *tag);

	int32_t cmdq_task_dump_command(cmdqRecHandle handle);
	int32_t cmdqRecDumpCommand(cmdqRecHandle handle);

	int32_t cmdq_task_estimate_command_exec_time(const cmdqRecHandle handle);
	int32_t cmdqRecEstimateCommandExecTime(const cmdqRecHandle handle);

	int32_t cmdq_task_destroy(cmdqRecHandle handle);
	void cmdqRecDestroy(cmdqRecHandle handle);

	int32_t cmdq_op_set_nop(cmdqRecHandle handle, uint32_t index);
	int32_t cmdqRecSetNOP(cmdqRecHandle handle, uint32_t index);

	int32_t cmdq_task_query_offset(cmdqRecHandle handle, uint32_t startIndex,
				   const CMDQ_CODE_ENUM opCode, CMDQ_EVENT_ENUM event);
	int32_t cmdqRecQueryOffset(cmdqRecHandle handle, uint32_t startIndex,
				   const CMDQ_CODE_ENUM opCode, CMDQ_EVENT_ENUM event);

	int32_t cmdq_resource_acquire(cmdqRecHandle handle, CMDQ_EVENT_ENUM resourceEvent);
	int32_t cmdqRecAcquireResource(cmdqRecHandle handle, CMDQ_EVENT_ENUM resourceEvent);

	int32_t cmdq_resource_acquire_and_write(cmdqRecHandle handle, CMDQ_EVENT_ENUM resourceEvent,
		uint32_t addr, uint32_t value, uint32_t mask);
	int32_t cmdqRecWriteForResource(cmdqRecHandle handle, CMDQ_EVENT_ENUM resourceEvent,
		uint32_t addr, uint32_t value, uint32_t mask);

	int32_t cmdq_resource_release(cmdqRecHandle handle, CMDQ_EVENT_ENUM resourceEvent);
	int32_t cmdqRecReleaseResource(cmdqRecHandle handle, CMDQ_EVENT_ENUM resourceEvent);

	int32_t cmdq_resource_release_and_write(cmdqRecHandle handle, CMDQ_EVENT_ENUM resourceEvent,
		uint32_t addr, uint32_t value, uint32_t mask);
	int32_t cmdqRecWriteAndReleaseResource(cmdqRecHandle handle, CMDQ_EVENT_ENUM resourceEvent,
		uint32_t addr, uint32_t value, uint32_t mask);

/* tablet use */
#ifdef CONFIG_MTK_CMDQ_TAB
	int32_t cmdq_task_set_secure_mode(cmdqRecHandle handle, enum CMDQ_DISP_MODE mode);
	int32_t cmdqRecSetSecureMode(cmdqRecHandle handle, enum CMDQ_DISP_MODE mode);
#endif

#ifdef __cplusplus
}
#endif
#endif				/* __CMDQ_RECORD_H__ */
