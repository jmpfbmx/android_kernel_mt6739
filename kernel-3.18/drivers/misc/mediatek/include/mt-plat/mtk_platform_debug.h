
#ifndef __MTK_PLATFORM_DEBUG_H__
#define __MTK_PLATFORM_DEBUG_H__

#ifdef CONFIG_MTK_PLAT_SRAM_FLAG
/* plat_sram_flag */
extern int set_sram_flag_lastpc_valid(void);
extern int set_sram_flag_etb_user(unsigned int etb_id, unsigned int user_id);
#endif

#endif /* __MTK_PLATFORM_DEBUG_H__ */
