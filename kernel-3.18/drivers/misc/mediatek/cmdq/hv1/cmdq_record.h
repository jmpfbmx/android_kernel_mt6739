
#ifndef __CMDQ_RECORD_H__
#define __CMDQ_RECORD_H__

#include <linux/types.h>
#include "cmdq_def.h"
#include "cmdq_core.h"

struct TaskStruct;

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
	cmdqSecDataStruct secData;	/* secure execution data */
};
typedef struct cmdqRecStruct *cmdqRecHandle;
typedef dma_addr_t cmdqBackupSlotHandle;

typedef void *CmdqRecLoopHandle;

#ifdef __cplusplus
extern "C" {
#endif

	int32_t cmdqRecCreate(enum CMDQ_SCENARIO_ENUM scenario, cmdqRecHandle *pHandle);

	int32_t cmdqRecReset(cmdqRecHandle handle);

	int32_t cmdqRecSetSecure(cmdqRecHandle handle, const bool isSecure);

	int32_t cmdqRecSecureEnableDAPC(cmdqRecHandle handle, const uint64_t engineFlag);

	int32_t cmdqRecSecureEnablePortSecurity(cmdqRecHandle handle, const uint64_t engineFlag);

	int32_t cmdqRecMark(cmdqRecHandle handle);

	int32_t cmdqRecEnablePrefetch(cmdqRecHandle handle);

	int32_t cmdqRecDisablePrefetch(cmdqRecHandle handle);

	int32_t cmdqRecWrite(cmdqRecHandle handle, uint32_t addr, uint32_t value, uint32_t mask);


	int32_t cmdqRecWriteSecure(cmdqRecHandle handle,
				   uint32_t addr,
				   CMDQ_SEC_ADDR_METADATA_TYPE type,
				   uint32_t baseHandle,
				   uint32_t offset, uint32_t size, uint32_t port);


	int32_t cmdqRecWriteSecureMask(cmdqRecHandle handle, uint32_t addr,
				       CMDQ_SEC_ADDR_METADATA_TYPE type,
				       uint32_t value, uint32_t mask);

	int32_t cmdqRecPoll(cmdqRecHandle handle, uint32_t addr, uint32_t value, uint32_t mask);

	int32_t cmdqRecWait(cmdqRecHandle handle, enum CMDQ_EVENT_ENUM event);

	int32_t cmdqRecWaitNoClear(cmdqRecHandle handle, enum CMDQ_EVENT_ENUM event);

	int32_t cmdqRecClearEventToken(cmdqRecHandle handle, enum CMDQ_EVENT_ENUM event);

	int32_t cmdqRecSetEventToken(cmdqRecHandle handle, enum CMDQ_EVENT_ENUM event);
	int32_t cmdqRecReadToDataRegister(cmdqRecHandle handle, uint32_t hwRegAddr,
					  enum CMDQ_DATA_REGISTER_ENUM dstDataReg);
	int32_t cmdqRecWriteFromDataRegister(cmdqRecHandle handle,
					     enum CMDQ_DATA_REGISTER_ENUM srcDataReg,
					     uint32_t hwRegAddr);


	int32_t cmdqBackupAllocateSlot(cmdqBackupSlotHandle *phBackupSlot, uint32_t slotCount);

	int32_t cmdqBackupReadSlot(cmdqBackupSlotHandle hBackupSlot, uint32_t slotIndex,
				   uint32_t *value);

	int32_t cmdqBackupWriteSlot(cmdqBackupSlotHandle hBackupSlot, uint32_t slotIndex,
				    uint32_t value);


	int32_t cmdqBackupFreeSlot(cmdqBackupSlotHandle hBackupSlot);


	int32_t cmdqRecBackupRegisterToSlot(cmdqRecHandle handle,
					    cmdqBackupSlotHandle hBackupSlot,
					    uint32_t slotIndex, uint32_t addr);
	int32_t cmdqRecBackupWriteRegisterFromSlot(cmdqRecHandle handle,
						   cmdqBackupSlotHandle hBackupSlot,
						   uint32_t slotIndex, uint32_t addr);

	int32_t cmdqRecBackupUpdateSlot(cmdqRecHandle handle,
					cmdqBackupSlotHandle hBackupSlot,
					uint32_t slotIndex, uint32_t value);

	int32_t cmdqRecFlush(cmdqRecHandle handle);


	int32_t cmdqRecFlushAndReadRegister(cmdqRecHandle handle, uint32_t regCount,
					    uint32_t *addrArray, uint32_t *valueArray);

	int32_t cmdqRecFlushAsync(cmdqRecHandle handle);

	int32_t cmdqRecFlushAsyncCallback(cmdqRecHandle handle, CmdqAsyncFlushCB callback,
					  uint32_t userData);

	int32_t cmdqRecStartLoop(cmdqRecHandle handle);

	int32_t cmdqRecStopLoop(cmdqRecHandle handle);

	int32_t cmdqRecGetInstructionCount(cmdqRecHandle handle);

	int32_t cmdqRecDumpCommand(cmdqRecHandle handle);

	void cmdqRecDestroy(cmdqRecHandle handle);

	int32_t cmdqRecSetSecureMode(cmdqRecHandle handle, enum CMDQ_DISP_MODE mode);

	int32_t cmdqRecWaitThreadIdleWithTimeout(int threadID, unsigned int retryCount);

#ifdef __cplusplus
}
#endif
#endif				/* __CMDQ_RECORD_H__ */
