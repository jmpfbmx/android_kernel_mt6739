
#ifndef __MTK_SYS_TIMER_MBOX_H__
#define __MTK_SYS_TIMER_MBOX_H__

#include <sspm_ipi_mbox_layout.h>

#define SYS_TIMER_MBOX                      SHAREMBOX_NO_MCDI
#define SYS_TIMER_MBOX_OFFSET_BASE          SHAREMBOX_OFFSET_TIMESTAMP


#define SYS_TIMER_MBOX_TICK_H               (SYS_TIMER_MBOX_OFFSET_BASE + 0)
#define SYS_TIMER_MBOX_TICK_L               (SYS_TIMER_MBOX_OFFSET_BASE + 1)
#define SYS_TIMER_MBOX_TS_H                 (SYS_TIMER_MBOX_OFFSET_BASE + 2)
#define SYS_TIMER_MBOX_TS_L                 (SYS_TIMER_MBOX_OFFSET_BASE + 3)
#define SYS_TIMER_MBOX_DEBUG_TS_H           (SYS_TIMER_MBOX_OFFSET_BASE + 4)
#define SYS_TIMER_MBOX_DEBUG_TS_L           (SYS_TIMER_MBOX_OFFSET_BASE + 5)

#endif /* __MTK_SYS_TIMER_MBOX_H__ */

