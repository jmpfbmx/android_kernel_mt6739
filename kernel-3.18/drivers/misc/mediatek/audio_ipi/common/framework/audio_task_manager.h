
#ifndef AUDIO_TASK_MANAGER_H
#define AUDIO_TASK_MANAGER_H

#include <linux/types.h>

#include "audio_task.h"
#include "audio_ipi_message.h"




typedef void (*task_unloaded_t)(void);




void audio_task_manager_init(void);


void audio_task_manager_deinit(void);


int audio_task_register_callback(
	const task_scene_t task_scene,
	recv_message_t recv_message,
	task_unloaded_t task_unloaded_callback);


int audio_load_task(const task_scene_t task_scene);


int audio_send_ipi_msg(
	ipi_msg_t *p_ipi_msg,
	uint8_t task_scene, /* task_scene_t */
	uint8_t msg_layer, /* audio_ipi_msg_layer_t */
	audio_ipi_msg_data_t data_type,
	audio_ipi_msg_ack_t ack_type,
	uint16_t msg_id,
	uint32_t param1, /* payload/DMA => buf_len*/
	uint32_t param2,
	char    *data_buffer);


int audio_send_ipi_filled_msg(ipi_msg_t *p_ipi_msg);



#endif /* end of AUDIO_TASK_MANAGER_H */



