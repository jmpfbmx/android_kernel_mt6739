
#ifndef _AUDIO_CODEC_63xx_H
#define _AUDIO_CODEC_63xx_H

void audckbufEnable(bool enable);
void ClsqEnable(bool enable);
void Topck_Enable(bool enable);
void NvregEnable(bool enable);
void OpenClassAB(void);
void OpenAnalogHeadphone(bool bEnable);
void OpenAnalogHeadphoneSpeaker(bool bEnable);
void OpenAnalogTrimHardware(bool bEnable);
void SetSdmLevel(unsigned int level);
void setOffsetTrimMux(unsigned int Mux);
void setOffsetTrimBufferGain(unsigned int gain);
void EnableTrimbuffer(bool benable);
void SetHplTrimOffset(int Offset);
void SetHprTrimOffset(int Offset);
void SetHplSpkTrimOffset(int Offset);
void SetHprSpkTrimOffset(int Offset);
void setHpGainZero(void);
void setLineOutGainZero(void);
void SetAnalogSuspend(bool bEnable);
void OpenTrimBufferHardware(bool bEnable);

#endif

