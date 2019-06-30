
#ifndef _MT_PMIC_INFO_H_
#define _MT_PMIC_INFO_H_

#define PMIC6335_E1_CID_CODE    0x3510
#define PMIC6335_E2_CID_CODE    0x3520
#define PMIC6335_E3_CID_CODE    0x3530

#ifdef CONFIG_MTK_TINYSYS_SSPM_SUPPORT
#define IPIMB
#endif

extern unsigned int pmic_ipi_test_code(void);

#define PMICTAG                "[PMIC] "
extern unsigned int gPMICDbgLvl;

#define PMIC_LOG_DBG     4
#define PMIC_LOG_INFO    3
#define PMIC_LOG_NOT     2
#define PMIC_LOG_WARN    1
#define PMIC_LOG_ERR     0

#define PMICLOG(fmt, arg...) do { \
	if (gPMICDbgLvl >= PMIC_LOG_DBG) \
		pr_notice(PMICTAG "%s: " fmt, __func__, ##arg); \
} while (0)

/* MT6335 Export API */
extern unsigned int pmic_scp_set_vcore(unsigned int);
extern unsigned int pmic_scp_set_vsram_vcore(unsigned int);
extern unsigned int enable_vsram_vcore_hw_tracking(unsigned int en);
extern unsigned int enable_vimvo_lp_mode(unsigned int);

#endif				/* _MT_PMIC_INFO_H_ */
