
#ifndef _AUDIO_HDMI_TYPE_H
#define _AUDIO_HDMI_TYPE_H

#include <linux/list.h>

enum HDMI_DISPLAY_TYPE {
	HDMI_DISPLAY_MHL = 0,
	HDMI_DISPLAY_SILMPORT = 1,
};

typedef struct {
	uint8 mHDMI_DisplayType;
	uint32 mI2Snum;
	uint32 mI2S_MCKDIV;
	uint32 mI2S_BCKDIV;

	uint32 mHDMI_Samplerate;
	uint32 mHDMI_Channels; /* channel number HDMI transmitted */
	uint32 mHDMI_Data_Lens;
	uint32 mTDM_Data_Lens;
	uint32 mClock_Data_Lens;
	uint32 mTDM_LRCK;
	uint32 msDATA_Channels; /* channel number per sdata */
	uint32 mMemIfFetchFormatPerSample;
	bool mSdata0;
	bool mSdata1;
	bool mSdata2;
	bool mSdata3;
} AudioHDMIFormat;


#endif
