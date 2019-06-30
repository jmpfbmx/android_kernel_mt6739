
#if defined(CONFIG_MTK_WATCHDOG_COMMON)
/* use common wd api */
#include <mt-plat/mtk_wd_api.h>
#else
#error "watchdog: common wd_api.h shall be used."
#endif
