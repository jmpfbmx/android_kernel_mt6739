
#ifndef AUDIO_IPI_MSG_QUEUE_H
#define AUDIO_IPI_MSG_QUEUE_H

#include <linux/types.h>



typedef struct ipi_msg_t ipi_msg_t;



typedef struct ipi_queue_handler_t {
	/* set void to prevent get/set attributes from outside */
	void *msg_queue; /* msg_queue_t */
} ipi_queue_handler_t;



ipi_queue_handler_t *create_ipi_queue_handler(const uint8_t task_scene);
void destroy_ipi_queue_handler(ipi_queue_handler_t *handler);

ipi_queue_handler_t *get_ipi_queue_handler(const uint8_t task_scene);

void disable_ipi_queue_handler(ipi_queue_handler_t *handler);

int flush_ipi_queue_handler(ipi_queue_handler_t *handler);

int send_message(ipi_queue_handler_t *handler, ipi_msg_t *p_ipi_msg);
int send_message_ack(ipi_queue_handler_t *handler, ipi_msg_t *p_ipi_msg_ack);

#endif /* end of AUDIO_IPI_MSG_QUEUE_H */


