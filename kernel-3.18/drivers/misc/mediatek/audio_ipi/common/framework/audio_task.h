
#ifndef AUDIO_TASK_H
#define AUDIO_TASK_H

#include <linux/types.h>

enum {
	TASK_SCENE_PHONE_CALL = 0,
	TASK_SCENE_VOICE_ULTRASOUND,
	TASK_SCENE_PLAYBACK_MP3,
	TASK_SCENE_RECORD,
	TASK_SCENE_VOIP,
	TASK_SCENE_SPEAKER_PROTECTION,
	TASK_SCENE_VOW,
	TASK_SCENE_SIZE, /* the size of tasks */
	TASK_SCENE_CONTROLLER = 0xFF
};

typedef uint8_t task_scene_t;


#endif /* end of AUDIO_TASK_H */

