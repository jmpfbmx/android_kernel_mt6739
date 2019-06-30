
#ifndef _MT_PMIC_INFO_H_
#define _MT_PMIC_INFO_H_

#define PMIC6353_E1_CID_CODE    0x5310
#define PMIC6353_E2_CID_CODE    0x5320
#define PMIC6353_E3_CID_CODE    0x5330

#define PMICTAG                "[PMIC] "
#ifdef PMIC_DEBUG
#define PMICDEB(fmt, arg...) pr_debug(PMICTAG "cpuid=%d, " fmt, raw_smp_processor_id(), ##arg)
#define PMICFUC(fmt, arg...) pr_debug(PMICTAG "cpuid=%d, %s\n", raw_smp_processor_id(), __func__)
#endif  /*-- defined PMIC_DEBUG --*/
#if defined PMIC_DEBUG_PR_DBG
#define PMICLOG(fmt, arg...)   pr_err(PMICTAG fmt, ##arg)
#else
#define PMICLOG(fmt, arg...)
#endif  /*-- defined PMIC_DEBUG_PR_DBG --*/
#define PMICERR(fmt, arg...)   pr_debug(PMICTAG "ERROR,line=%d " fmt, __LINE__, ##arg)
#define PMICREG(fmt, arg...)   pr_debug(PMICTAG fmt, ##arg)


#endif				/* _MT_PMIC_INFO_H_ */
