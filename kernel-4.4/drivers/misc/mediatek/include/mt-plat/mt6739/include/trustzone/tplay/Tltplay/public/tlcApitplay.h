
#ifndef TLTPLAY_H_
#define TLTPLAY_H_

#include "tci.h"

#define CMD_TPLAY_REQUEST   4
#define CMD_TPLAY_DUMP_PHY  5
#define CMD_TPLAY_TEST      6



#define EXIT_ERROR                  ((uint32_t)(-1))

struct tplay_cmd_t {
		tciCommandHeader_t  header;     /**< Command header */
		uint32_t            len;        /**< Length of data to process or buffer */
		uint32_t            respLen;    /**< Length of response buffer */
};

struct tplay_rsp_t {
		tciResponseHeader_t header;     /**< Response header */
		uint32_t            len;
};

struct tplay_tciMessage_t {
		union {
				tplay_cmd_t     cmd;
				tplay_rsp_t     rsp;
		};
		uint32_t    index;
		uint32_t    result;
		uint32_t    data_addr;
		uint32_t    data_len;
		uint32_t    seed_addr;
		uint32_t    seed_len;
		uint32_t    hacc_user;
		uint32_t    direction;
		uint32_t    tplay_handle_low_addr;
		uint32_t    tplay_handle_high_addr;
};

/* 05200000000000000000000000000000 */
#define TL_TPLAY_UUID {{0, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }

#endif /* TLFOO_H_ */
