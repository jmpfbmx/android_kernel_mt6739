


#ifndef _AUDIO_CODEC_63xx_H
#define _AUDIO_CODEC_63xx_H

#define PMIC_HPIMP_DETECT

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
void OpenAnalogHeadphone(bool bEnable);
void OpenAnalogTrimHardware(bool bEnable);
void SetSdmLevel(unsigned int level);
void setOffsetTrimMux(unsigned int Mux);
void setOffsetTrimBufferGain(unsigned int gain);
void EnableTrimbuffer(bool benable);
void setHpGainZero(void);
void CalculateDCCompenForEachdB_L(void);
void CalculateDCCompenForEachdB_R(void);
void set_hp_impedance(int impedance);

/* headphone impedance detection function*/
int read_efuse_hp_impedance_current_calibration(void);
bool OpenHeadPhoneImpedanceSetting(bool bEnable);
void mtk_read_hp_detection_parameter(struct mtk_hpdet_param *hpdet_param);
int mtk_calculate_impedance_formula(int pcm_offset, int aux_diff);

void SetAnalogSuspend(bool bEnable);

bool hasHpDepopHw(void);
bool hasHp33Ohm(void);

/*mtk dpd function*/
void mtk_read_dpd_parameter(struct mtk_dpd_param *dpd_param);
#endif

