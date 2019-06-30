
#ifndef _AUDIO_HDMI_TDM_TYPE_H
#define _AUDIO_HDMI_TDM_TYPE_H

#include <linux/list.h>

#define _ANX7805_SLIMPORT_CHANNEL 2
#define _ANX7625_SLIMPORT_CHANNEL 8

#define HDMI_USE_CHANNELS_MIN     1
#define HDMI_USE_CHANNELS_MAX     8

void mtk_Hdmi_Configuration_Init(void *hdmi_stream_format);

void mtk_Hdmi_Clock_Set(void *hdmi_stream_format);

void mtk_Hdmi_Configuration_Set(void *hdmi_stream_format, int rate,
			int channels, snd_pcm_format_t format, int displaytype);

void mtk_Hdmi_Set_Interconnection(void *hdmi_stream_format);

void mtk_Hdmi_Set_Sdata(void *hdmi_stream_format);

void SetHDMIClockInverse(void);

void SetHDMIDebugEnable(bool enable);

void SetHDMIDumpReg(void);

bool SetHDMIMCLK(void);

bool SetHDMIBCLK(void);

unsigned int GetHDMIApLLSource(void);

bool SetHDMIApLL(unsigned int ApllSource);

bool SetHDMIdatalength(unsigned int length);

bool SetHDMIsamplerate(unsigned int samplerate);

bool SetHDMIChannels(unsigned int Channels);

bool SetHDMIEnable(bool bEnable);

bool SetHDMIConnection(unsigned int ConnectionState, unsigned int Input, unsigned int Output);

bool SetTDMLrckWidth(unsigned int cycles);

bool SetTDMbckcycle(unsigned int cycles);

bool SetTDMChannelsSdata(unsigned int channels);

bool SetTDMDatalength(unsigned int length);

bool SetTDMI2Smode(unsigned int mode);

bool SetTDMLrckInverse(bool enable);

bool SetTDMBckInverse(bool enable);

bool SetTDMEnable(bool enable);

#endif
