
#include <linux/kernel.h>
#include <linux/module.h>
#if defined(CONFIG_MTK_PMIC_CHIP_MT6353) && defined(CONFIG_MTK_MT6750TT)
unsigned int tFyTbl[6] = {0x0e, 0xa, 0x1d, 0x8, 0x0, 0x116};/* 1.807 */
unsigned int tSbTbl[6] = {0x0e, 0xa, 0x1d, 0x8, 0x0, 0x12c};/* 1.95 */
unsigned int *tbl;
unsigned int *get_turbo(unsigned int binLevel, unsigned int binLevelEng)
{
	if ((1 == binLevel) || (3 == binLevel)) {
		tbl = tFyTbl;
	} else if ((2 == binLevel) || (4 == binLevel)) {
		tbl = tFyTbl;
	} else {
		if ((2 == ((binLevelEng >> 4) & 0x07)) || (2 == ((binLevelEng >> 10) & 0x07)))
			tbl = tFyTbl;
		else
			tbl = tFyTbl;
	}
	return tbl;
}
#endif
