
#ifndef __CCU_MAILBOX_EXTIF__
#define __CCU_MAILBOX_EXTIF__

#include "ccu_ext_interface/ccu_ext_interface.h"

#define CCU_MAILBOX_QUEUE_SIZE 16	/*must be power of 2 for modulo operation take work*/

typedef struct _ccu_mailbox_t {
		volatile MUINT32 front;
		volatile MUINT32 rear;
		volatile ccu_msg_t queue[CCU_MAILBOX_QUEUE_SIZE];
} ccu_mailbox_t;

#endif
