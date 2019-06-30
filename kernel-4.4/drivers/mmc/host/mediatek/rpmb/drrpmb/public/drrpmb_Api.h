

#ifndef __DRRPMBAPI_H__
#define __DRRPMBAPI_H__

#include "dci.h"


#define DCI_RPMB_CMD_READ_DATA      1
#define DCI_RPMB_CMD_GET_WCNT       2
#define DCI_RPMB_CMD_WRITE_DATA     3
#define DCI_RPMB_CMD_PROGRAM_KEY    4


/*... add more command ids when needed */

struct cmd_t {
	struct dciCommandHeader_t header;     /**< Command header */
	uint32_t            len;        /**< Length of data to process */
};

struct rsp_t {
	struct dciResponseHeader_t header;     /**< Response header */
	uint32_t            len;
};


#define RPMB_MULTI_BLOCK_ACCESS 1

#if RPMB_MULTI_BLOCK_ACCESS
#define MAX_RPMB_TRANSFER_BLK (16)
#define MAX_RPMB_REQUEST_SIZE (512*MAX_RPMB_TRANSFER_BLK) /* 8KB(16blks) per requests. */
#else
#define MAX_RPMB_TRANSFER_BLK (1)
#define MAX_RPMB_REQUEST_SIZE (512*MAX_RPMB_TRANSFER_BLK) /* 512B(1blks) per requests. */
#endif

struct rpmb_req_t {
	uint8_t frame[MAX_RPMB_REQUEST_SIZE];
	uint32_t frameLen;
	uint16_t type;
	uint16_t addr;
	uint16_t blks;
	uint16_t result;
};

struct dciMessage_t {
	union {
		struct cmd_t  command;
		struct rsp_t  response;
	};

	struct rpmb_req_t request;

};

#define RPMB_UUID { { 7, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }

#endif
