
#ifndef __PLAT_SRAM_FLAG_H__
#define __PLAT_SRAM_FLAG_H__

/* each flag is a word */
#define PLAT_FLAG0	0
#define PLAT_FLAG1	1
#define PLAT_FLAG2	2
#define PLAT_FLAG3	3

#define OFFSET_LASTPC_VALID			0
#define OFFSET_LASTPC_VALID_BEFORE_REBOOT	1
#define OFFSET_ETB_0				2

/* available multi-user ETB number (only count for ETB that supports multi-user) */
#define MAX_ETB_NUM		10
/* available user type is 0x0~0x7 */
#define MAX_ETB_USER_NUM	8


#define OFFSET_DFD_VALID                     0
#define OFFSET_DFD_VALID_BEFORE_REBOOT       1



int set_sram_flag_lastpc_valid(void);
int set_sram_flag_dfd_valid(void);
int set_sram_flag_etb_user(unsigned int utb_id, unsigned int user_id);
#endif
