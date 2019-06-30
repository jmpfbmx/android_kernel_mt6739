
#ifndef _AUDIO_CODEC_63xx_H
#define _AUDIO_CODEC_63xx_H

/* Headphone Impedance Detection */
struct mtk_hpdet_param {
	int auxadc_upper_bound;
	int dc_Step;
	int dc_Phase0;
	int dc_Phase1;
	int dc_Phase2;
	int resistance_first_threshold;
	int resistance_second_threshold;
};

/* DPD parameter */
struct mtk_dpd_param {
	int efuse_on;
	int a2_lch;
	int a3_lch;
	int a2_rch;
	int a3_rch;
};

void audckbufEnable(bool enable);

void SetAnalogSuspend(bool bEnable);

/* mtk audio interface calibration function */
void mtkaif_calibration_set_loopback(bool enable);
void mtkaif_calibration_set_phase(int mode1);

/*mtk dpd function*/
void mtk_read_dpd_parameter(struct mtk_dpd_param *dpd_param);


/* API for ACCDET to reset input precharge */
void mtk_audio_reset_input_precharge(void);

#endif

