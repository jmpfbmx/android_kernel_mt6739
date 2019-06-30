
#ifndef AUDIO_MESSENGER_IPI_H
#define AUDIO_MESSENGER_IPI_H

#include <linux/types.h>

#include "audio_ipi_message.h"





void audio_messenger_ipi_init(void);

void audio_reg_recv_message(uint8_t task_scene, recv_message_t recv_message);



int audio_send_ipi_msg(
	struct ipi_msg_t *p_ipi_msg,
	uint8_t task_scene, /* task_scene_t */
	uint8_t msg_layer, /* audio_ipi_msg_layer_t */
	uint8_t data_type, /* audio_ipi_msg_data_t */
	uint8_t ack_type, /* audio_ipi_msg_ack_t */
	uint16_t msg_id,
	uint32_t param1, /* payload/DMA => buf_len*/
	uint32_t param2,
	char    *data_buffer);

int send_message_to_scp(const struct ipi_msg_t *p_ipi_msg);




#endif /* end of AUDIO_MESSENGER_IPI_H */

