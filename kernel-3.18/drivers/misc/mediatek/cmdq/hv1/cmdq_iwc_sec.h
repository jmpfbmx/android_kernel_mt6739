
#ifndef _CMDQ_IWC_SEC_H_
#define  _CMDQ_IWC_SEC_H_
#include "cmdqSecTl_Api.h"
#include "tz_cross/trustzone.h"
#include "tz_cross/ta_mem.h"
#include "trustzone/kree/system.h"
#include "trustzone/kree/mem.h"


typedef enum {
	IWC_INIT = 0,
	IWC_MOBICORE_OPENED = 1,
	IWC_WSM_ALLOCATED = 2,
	IWC_SES_OPENED = 3,
	IWC_SES_MSG_PACKAGED = 4,
	IWC_SES_TRANSACTED = 5,
	IWC_SES_ON_TRANSACTED = 6,
	IWC_END_OF_ENUM = 7,
} CMDQ_IWC_STATE_ENUM;
typedef struct cmdqSecContextStruct {
	struct list_head listEntry;

	/* basic info */
	uint32_t tgid;		/* tgid of procexx context */
	uint32_t referCount;	/* reference count for open cmdq device node */

	/* iwc state */
	CMDQ_IWC_STATE_ENUM state;

	/* iwc information */
	void *iwcMessage;	/* message buffer */
#if defined(CMDQ_SECURE_PATH_SUPPORT)
	KREE_SESSION_HANDLE sessionHandle;
	KREE_SESSION_HANDLE memSessionHandle;
#endif
} cmdqSecContextStruct, *cmdqSecContextHandle;
typedef struct cmdqSecSharedMemoryStruct {
	void *pVABase;		/* virtual address of command buffer */
	dma_addr_t MVABase;	/* physical address of command buffer */
	uint32_t size;		/* buffer size */
	cmdqSecContextHandle handle;	/* for alloc path */
	KREE_SHAREDMEM_HANDLE cmdq_share_cookie_handle;
} cmdqSecSharedMemoryStruct, *cmdqSecSharedMemoryHandle;
#endif				/* end of  _CMDQ_IWC_SEC_H_ */
