
#ifndef __MT_DCM_H__
#define __MT_DCM_H__

#define DCM_OFF (0)
#define DCM_ON	(1)

typedef enum {
	MCUSYS_DCM_OFF = DCM_OFF,
	MCUSYS_DCM_ON = DCM_ON,
} ENUM_MCUSYS_DCM;


void mt_dcm_disable(void);
void mt_dcm_restore(void);

int sync_dcm_set_cci_freq(unsigned int cci);
int sync_dcm_set_mp0_freq(unsigned int mp0);
int sync_dcm_set_mp1_freq(unsigned int mp1);

/* MCSI-A */
int sync_dcm_set_mcsi_a_freq(unsigned int mcsi_a);

#endif /* #ifndef __MT_DCM_H__ */

