

#ifndef __KREE_H__
#define __KREE_H__

#if defined(CONFIG_MTK_IN_HOUSE_TEE_SUPPORT) || defined(CONFIG_TRUSTY)

#include "trustzone/tz_cross/trustzone.h"

/* / KREE session handle type. */
typedef uint32_t KREE_SESSION_HANDLE;

typedef uint32_t KREE_SHAREDMEM_HANDLE;


/* Session Management */
TZ_RESULT KREE_CreateSession(const char *ta_uuid, KREE_SESSION_HANDLE *pHandle);

TZ_RESULT KREE_CreateSessionWithTag(const char *ta_uuid,
				KREE_SESSION_HANDLE *pHandle, const char *tag);

TZ_RESULT KREE_CloseSession(KREE_SESSION_HANDLE handle);


TZ_RESULT KREE_TeeServiceCall(KREE_SESSION_HANDLE handle, uint32_t command,
			      uint32_t paramTypes, union MTEEC_PARAM param[4]);

#endif	/* CONFIG_MTK_IN_HOUSE_TEE_SUPPORT || CONFIG_TRUSTY */
#endif	/* __KREE_H__ */
