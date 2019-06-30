
#ifndef _AUDIO_CODEC_SPEAKER_H
#define _AUDIO_CODEC_SPEAKER_H

void Speaker_ClassD_Open(uint32 in_trimming);
void Speaker_ClassD_close(uint32 in_trimming);
void Speaker_ClassAB_Open(void);
void Speaker_ClassAB_close(void);
void Speaker_ReveiverMode_Open(void);
void Speaker_ReveiverMode_close(void);
bool GetSpeakerOcFlag(void);

#endif
