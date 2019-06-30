
#ifndef __DISP_LOG_H__
#define __DISP_LOG_H__
#include <mt-plat/aee.h>
#include <linux/printk.h>

#include "disp_recorder.h"
#include "disp_debug.h"
#include "disp_drv_platform.h"

#if defined(COMMON_DISP_LOG)
#include "mtkfb_debug.h"
#endif

#define ERROR_LEVEL   0
#define WARN_LEVEL    1
#define DEFAULT_LEVEL 3
#define DEBUG_LEVEL   5
#define DISP_IRQ_LEVEL     6


#define DISPERR(fmt, args...)                                          \
	do {                                                           \
		dprec_logger_pr(DPREC_LOGGER_ERROR, fmt, ##args);      \
		if ((g_mobilelog > 0) && (g_loglevel >= ERROR_LEVEL))    \
			pr_err("[DISP]"fmt, ##args);                   \
	} while (0)

#define DISPWRN(fmt, args...)                                          \
	do {                                                           \
		dprec_logger_pr(DPREC_LOGGER_DEBUG, fmt, ##args);      \
		if ((g_mobilelog > 0) && (g_loglevel >= WARN_LEVEL))     \
			pr_debug("[DISP]"fmt, ##args);                 \
	} while (0)

#define DISPMSG(fmt, args...)                                          \
	do {                                                           \
		dprec_logger_pr(DPREC_LOGGER_DEBUG, fmt, ##args);      \
		if ((g_mobilelog > 0) && (g_loglevel >= DEFAULT_LEVEL))  \
			pr_debug("[DISP]"fmt, ##args);                 \
	} while (0)

#define DISPFUNC()                                                     \
	do {                                                           \
		dprec_logger_pr(DPREC_LOGGER_DEBUG,                    \
			"func|%s\n", __func__);                        \
		if ((g_mobilelog > 0) && (g_loglevel >= DEBUG_LEVEL))  \
			pr_debug("[DISP]func|%s\n", __func__);         \
	} while (0)


#define DISPDBG(fmt, args...)                                          \
	do {                                                           \
		if (g_loglevel >= DEBUG_LEVEL)                         \
			dprec_logger_pr(DPREC_LOGGER_DEBUG,            \
					fmt, ##args);                  \
		if ((g_mobilelog > 0) && (g_loglevel >= DEBUG_LEVEL))  \
			pr_debug("[DISP]"fmt, ##args);                 \
	} while (0)

#define DISPIRQ(fmt, args...)                                          \
	do {                                                           \
		if (g_loglevel >= DISP_IRQ_LEVEL)                            \
			dprec_logger_pr(DPREC_LOGGER_DEBUG,            \
					fmt, ##args);                  \
		if ((g_mobilelog > 0) && (g_loglevel >= DISP_IRQ_LEVEL))      \
			pr_debug("[DISP]IRQ: "fmt, ##args);            \
	} while (0)

#define DISPRCD(fmt, args...)                                          \
	do {                                                           \
		if (g_rcdlevel > 0)                                     \
			dprec_logger_pr(DPREC_LOGGER_DEBUG,            \
				fmt, ##args);                          \
	} while (0)


#define DISPINFO(fmt, args...)                                         \
	do {                                                           \
		if (g_mobilelog)                                        \
			pr_debug("[DISP]"fmt, ##args);                 \
	} while (0)

#define DISPDMP(fmt, ...)                                              \
	do {                                                           \
		if (ddp_debug_analysis_to_buffer()) {                  \
			char log[512] = {'\0'};                        \
			scnprintf(log, 511, fmt, ##__VA_ARGS__);       \
			dprec_logger_dump(log);                        \
		} else {                                               \
			dprec_logger_pr(DPREC_LOGGER_DUMP,             \
					fmt, ##__VA_ARGS__);           \
			if (g_mobilelog)                                \
				pr_debug("[DISP]"fmt,                  \
					 ##__VA_ARGS__);               \
		}                                                      \
	} while (0)

#ifndef ASSERT
#define ASSERT(expr)					               \
	do {						               \
		if (expr)				               \
			break;				               \
		pr_err("[DISP]ASSERT FAILED %s, %d\n",	               \
			__FILE__, __LINE__); BUG();	               \
	} while (0)
#endif

#define DISPAEE(fmt, args...)                                          \
	do {                                                           \
		char str[200];                                         \
		snprintf(str, 199, "DISP:"fmt, ##args);                \
		DISPERR(fmt, ##args);                                  \
		aee_kernel_warning_api(__FILE__, __LINE__,             \
			DB_OPT_DEFAULT | DB_OPT_MMPROFILE_BUFFER,      \
			str, fmt, ##args);                             \
		pr_err("[DISP]"fmt, ##args);                           \
	} while (0)

#define DISPPR_ERROR(fmt, args...)                                     \
	do {                                                           \
		dprec_logger_pr(DPREC_LOGGER_ERROR,                    \
				fmt,                                   \
				##args);                               \
		pr_err("[DISP]ERROR:"fmt, ##args);	               \
	} while (0)

#define DISPPR_FENCE(fmt, args...)                                     \
	do {                                                           \
		dprec_logger_pr(DPREC_LOGGER_FENCE,                    \
				fmt,                                   \
				##args);                               \
		if (g_fencelog)                                         \
			pr_debug("[DISP]fence/"fmt, ##args);           \
	} while (0)

#endif /* __DISP_LOG_H__ */
