

#ifndef __KREE_MEM_H__
#define __KREE_MEM_H__

#if defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT) || defined(CONFIG_TRUSTY)

#include <gz/tz_cross/trustzone.h>
#include <gz/tz_cross/gz_version.h>
#include "system.h"

#define KREE_SESSION_HANDLE_NULL	((KREE_SESSION_HANDLE)0)
#define KREE_SESSION_HANDLE_FAIL	((KREE_SESSION_HANDLE)-1)


/* Shared memory handle define */
typedef uint32_t KREE_SHAREDMEM_HANDLE;

/* Secure memory handle define */
typedef uint32_t KREE_SECUREMEM_HANDLE;

/* Secure chunk memory handle define */
typedef uint32_t KREE_SECURECM_HANDLE;

/* Release Secure chunk memory handle define */
typedef uint32_t KREE_RELEASECM_HANDLE;

struct kree_shared_mem_param {
	void *buffer;
	uint32_t size;
	void *mapAry;
};
#define KREE_SHAREDMEM_PARAM struct kree_shared_mem_param

struct KREE_SHM_RUNLENGTH_ENTRY {
	uint32_t high;	/* (uint64_t) start PA address = high | low */
	uint32_t low;
	uint32_t size;
};

struct KREE_SHM_RUNLENGTH_LIST {
	struct KREE_SHM_RUNLENGTH_ENTRY entry;
	struct KREE_SHM_RUNLENGTH_LIST *next;
};


/* map_p: 0 = no remap, 1 = remap */
TZ_RESULT kree_register_sharedmem(KREE_SESSION_HANDLE session,
		KREE_SHAREDMEM_HANDLE *mem_handle, void *start,
		uint32_t size, void *map_p);

TZ_RESULT kree_unregister_sharedmem(KREE_SESSION_HANDLE session,
					KREE_SHAREDMEM_HANDLE mem_handle);


TZ_RESULT KREE_RegisterSharedmem(KREE_SESSION_HANDLE session,
		KREE_SHAREDMEM_HANDLE *shm_handle, KREE_SHAREDMEM_PARAM *param);

TZ_RESULT KREE_UnregisterSharedmem(KREE_SESSION_HANDLE session,
					KREE_SHAREDMEM_HANDLE shm_handle);


TZ_RESULT KREE_AllocSecuremem(KREE_SESSION_HANDLE session,
	KREE_SECUREMEM_HANDLE *mem_handle, uint32_t alignment, uint32_t size);

TZ_RESULT KREE_ZallocSecurememWithTag(KREE_SESSION_HANDLE session,
	KREE_SECUREMEM_HANDLE *mem_handle, uint32_t alignment, uint32_t size);

TZ_RESULT KREE_ReferenceSecuremem(KREE_SESSION_HANDLE session,
					KREE_SECUREMEM_HANDLE mem_handle);

TZ_RESULT KREE_UnreferenceSecuremem(KREE_SESSION_HANDLE session,
					KREE_SECUREMEM_HANDLE mem_handle);

#if (GZ_API_MAIN_VERSION > 2)


TZ_RESULT KREE_ReferenceSecurechunkmem(KREE_SESSION_HANDLE session,
					KREE_SECURECM_HANDLE cm_handle);

TZ_RESULT KREE_UnreferenceSecurechunkmem(KREE_SESSION_HANDLE session,
					 KREE_SECURECM_HANDLE cm_handle);

TZ_RESULT KREE_ReadSecurechunkmem(KREE_SESSION_HANDLE session,
				uint32_t offset, uint32_t size, void *buffer);

TZ_RESULT KREE_WriteSecurechunkmem(KREE_SESSION_HANDLE session,
			uint32_t offset, uint32_t size, void *buffer);

TZ_RESULT KREE_GetSecurechunkReleaseSize(KREE_SESSION_HANDLE session,
					uint32_t *size);
#endif /* end of chunk mem API */
#endif				/* CONFIG_MTK_IN_HOUSE_TEE_SUPPORT || CONFIG_TRUSTY*/
#endif				/* __KREE_MEM_H__ */
