
#ifndef _MT_PMIC_INFO_H_
#define _MT_PMIC_INFO_H_

#define PMIC6335_E1_CID_CODE    0x3510
#define PMIC6335_E2_CID_CODE    0x3520
#define PMIC6335_E3_CID_CODE    0x3530

#define PMICTAG                "[PMIC] "
extern unsigned int gPMICDbgLvl;

#define PMIC_LOG_DBG     4
#define PMIC_LOG_INFO    3
#define PMIC_LOG_NOT     2
#define PMIC_LOG_WARN    1
#define PMIC_LOG_ERR     0

#define PMICLOG(fmt, arg...) do { \
	if (gPMICDbgLvl >= PMIC_LOG_DBG) \
		pr_err(PMICTAG "%s: " fmt, __func__ , ##arg); \
} while (0)



#endif				/* _MT_PMIC_INFO_H_ */
