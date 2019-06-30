
#ifndef TLSECMEM_H_
#define TLSECMEM_H_

#include "tci.h"

#include "secmem_plat.h"

#define CMD_SEC_MEM_ALLOC         1
#define CMD_SEC_MEM_REF           2
#define CMD_SEC_MEM_UNREF         3
#define CMD_SEC_MEM_ALLOC_TBL     4
#define CMD_SEC_MEM_UNREF_TBL     5
#define CMD_SEC_MEM_USAGE_DUMP    6
#define CMD_SEC_MEM_ENABLE        7
#define CMD_SEC_MEM_DISABLE       8
#define CMD_SEC_MEM_ALLOCATED     9
#define CMD_SEC_MEM_ALLOC_PA      10
#define CMD_SEC_MEM_REF_PA        11
#define CMD_SEC_MEM_UNREF_PA      12
#define CMD_SEC_MEM_ALLOC_ZERO    13

#define CMD_SEC_MEM_DUMP_INFO     255

#define MAX_NAME_SZ              (32)

#define EXIT_ERROR                  ((uint32_t)(-1))

struct tl_cmd_t {
	struct tciCommandHeader_t header;     /**< Command header */
	uint32_t                  len;        /**< Length of data to process or buffer */
	uint32_t                  respLen;    /**< Length of response buffer */
};

struct tl_rsp_t {
	struct tciResponseHeader_t header;     /**< Response header */
	uint32_t                   len;
};

struct tl_sender_info_t {
	uint8_t  name[MAX_NAME_SZ];
	uint32_t id;
};

struct tciMessage_t {
	union {
		struct tl_cmd_t     cmd_secmem;
		struct tl_rsp_t     rsp_secmem;
	};

#ifdef SECMEM_64BIT_PHYS_SUPPORT
	uint64_t    alignment;  /* IN */
	uint64_t    size;       /* IN */
	uint32_t    refcount;   /* INOUT */
	uint64_t    sec_handle; /* OUT */
#else
	uint32_t    alignment;  /* IN */
	uint32_t    size;       /* IN */
	uint32_t    refcount;   /* INOUT */
	uint32_t    sec_handle; /* OUT */
#endif /* !SECMEM_64BIT_PHYS_SUPPORT */

	uint32_t    ResultData;

	/* Debugging purpose */
	struct tl_sender_info_t sender;

};

#define TL_SECMEM_UUID {0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

#endif /* TLSECMEM_H_ */
