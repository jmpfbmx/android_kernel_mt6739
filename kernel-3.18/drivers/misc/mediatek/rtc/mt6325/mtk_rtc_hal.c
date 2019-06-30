#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/rtc.h>
#include <mach/upmu_hw.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/types.h>

#include <mach/irqs.h>
#include <mach/mtk_rtc_hal.h>
#include <mtk_rtc_hal_common.h>
#include <mach/mt_rtc_hw.h>
#include <mt_pmic_wrap.h>
#if defined CONFIG_MTK_KERNEL_POWER_OFF_CHARGING
#include <mt_boot.h>
#endif

#include <mt_gpio.h>

#define hal_rtc_xinfo(fmt, args...)		\
	pr_notice(fmt, ##args)

#define hal_rtc_xerror(fmt, args...)	\
	pr_err(fmt, ##args)

#define hal_rtc_xfatal(fmt, args...)	\
	pr_emerg(fmt, ##args)



u16 rtc_spare_reg[][3] = {
	{RTC_AL_HOU, 0x7f, 8},
	{RTC_PDN1, 0xf, 0},
	{RTC_PDN1, 0x3, 4},
	{RTC_PDN1, 0x1, 6},
	{RTC_PDN1, 0x1, 7},
	{RTC_PDN1, 0x1, 13},
	{RTC_PDN1, 0x1, 14},
	{RTC_PDN1, 0x1, 15},
	{RTC_PDN2, 0x1, 4},
	{RTC_PDN2, 0x3, 5},
	{RTC_PDN2, 0x1, 7},
	{RTC_PDN2, 0x1, 15},
	{RTC_SPAR0, 0x1, 6},
	{RTC_SPAR0, 0x1, 7}
};

void hal_rtc_set_abb_32k(u16 enable)
{
	hal_rtc_xinfo("ABB 32k not support\n");
}

u16 hal_rtc_get_gpio_32k_status(void)
{
	u16 con;

	con = rtc_read(RTC_CON);

	hal_rtc_xinfo("RTC_GPIO 32k status(RTC_CON=0x%x)\n", con);

	if (con & RTC_CON_F32KOB)
		return 0;
	else
		return 1;
}

void hal_rtc_set_gpio_32k_status(u16 user, bool enable)
{
	u16 con, pdn1;

	if (enable) {
		pdn1 = rtc_read(RTC_PDN1);
	} else {
		pdn1 = rtc_read(RTC_PDN1) & ~(1U << user);
		rtc_write(RTC_PDN1, pdn1);
		rtc_write_trigger();
	}

	con = rtc_read(RTC_CON);
	if (enable) {
		con &= ~RTC_CON_F32KOB;
	} else {
		if (!(pdn1 & RTC_GPIO_USER_MASK)) {	/* no users */
			con |= RTC_CON_F32KOB;
		}
	}
	rtc_write(RTC_CON, con);
	rtc_write_trigger();


	if (enable) {
		pdn1 |= (1U << user);
		rtc_write(RTC_PDN1, pdn1);
		rtc_write_trigger();
	}
	hal_rtc_xinfo("RTC_GPIO user %d enable = %d 32k (0x%x)\n", user, enable, pdn1);
}

void hal_rtc_bbpu_pwdn(void)
{
	u16 con;

	/* disable 32K export if there are no RTC_GPIO users */
	if (!(rtc_read(RTC_PDN1) & RTC_GPIO_USER_MASK)) {
		con = rtc_read(RTC_CON) | RTC_CON_F32KOB;
		rtc_write(RTC_CON, con);
		rtc_write_trigger();
	}
	rtc_bbpu_pwrdown(true);
}

void hal_rtc_get_pwron_alarm(struct rtc_time *tm, struct rtc_wkalrm *alm)
{
	u16 pdn1, pdn2;


	pdn1 = rtc_read(RTC_PDN1);
	pdn2 = rtc_read(RTC_PDN2);

	alm->enabled = (pdn1 & RTC_PDN1_PWRON_TIME ? (pdn2 & RTC_PDN2_PWRON_LOGO ? 3 : 2) : 0);
	alm->pending = !!(pdn2 & RTC_PDN2_PWRON_ALARM);	/* return Power-On Alarm bit */

	hal_rtc_get_alarm_time(tm);
}

bool hal_rtc_is_lp_irq(void)
{
	u16 irqsta;

	irqsta = rtc_read(RTC_IRQ_STA);	/* read clear */
	if (unlikely(!(irqsta & RTC_IRQ_STA_AL))) {
#ifndef USER_BUILD_KERNEL
		if (irqsta & RTC_IRQ_STA_LP)
			rtc_lp_exception();
#endif
		return true;
	}

	return false;
}

bool hal_rtc_is_pwron_alarm(struct rtc_time *nowtm, struct rtc_time *tm)
{
	u16 pdn1;

	pdn1 = rtc_read(RTC_PDN1);
	hal_rtc_xinfo("pdn1 = 0x%4x\n", pdn1);

	if (pdn1 & RTC_PDN1_PWRON_TIME) {	/* power-on time is available */

		hal_rtc_xinfo("pdn1 = 0x%4x\n", pdn1);
		hal_rtc_get_tick_time(nowtm);
		hal_rtc_xinfo("pdn1 = 0x%4x\n", pdn1);
		if (rtc_read(RTC_TC_SEC) < nowtm->tm_sec) {	/* SEC has carried */
			hal_rtc_get_tick_time(nowtm);
		}

		hal_rtc_get_pwron_alarm_time(tm);

		return true;
	}

	return false;
}

void hal_rtc_get_alarm(struct rtc_time *tm, struct rtc_wkalrm *alm)
{
	u16 irqen, pdn2;

	irqen = rtc_read(RTC_IRQ_EN);
	hal_rtc_get_alarm_time(tm);
	pdn2 = rtc_read(RTC_PDN2);
	alm->enabled = !!(irqen & RTC_IRQ_EN_AL);
	alm->pending = !!(pdn2 & RTC_PDN2_PWRON_ALARM);	/* return Power-On Alarm bit */
}

void hal_rtc_set_alarm(struct rtc_time *tm)
{
	u16 irqen;

	hal_rtc_set_alarm_time(tm);

	irqen = rtc_read(RTC_IRQ_EN) | RTC_IRQ_EN_ONESHOT_AL;
	rtc_write(RTC_IRQ_EN, irqen);
	rtc_write_trigger();
}

void hal_rtc_clear_alarm(struct rtc_time *tm)
{
	u16 irqsta, irqen, pdn2;

	irqen = rtc_read(RTC_IRQ_EN) & ~RTC_IRQ_EN_AL;
	pdn2 = rtc_read(RTC_PDN2) & ~RTC_PDN2_PWRON_ALARM;
	rtc_write(RTC_IRQ_EN, irqen);
	rtc_write(RTC_PDN2, pdn2);
	rtc_write_trigger();
	irqsta = rtc_read(RTC_IRQ_STA);	/* read clear */

	hal_rtc_set_alarm_time(tm);
}

void hal_rtc_set_lp_irq(void)
{
	u16 irqen;

#ifndef USER_BUILD_KERNEL
	irqen = rtc_read(RTC_IRQ_EN) | RTC_IRQ_EN_LP;
#else
	irqen = rtc_read(RTC_IRQ_EN) & ~RTC_IRQ_EN_LP;
#endif
	rtc_write(RTC_IRQ_EN, irqen);
	rtc_write_trigger();
}

void hal_rtc_save_pwron_time(bool enable, struct rtc_time *tm, bool logo)
{
	u16 pdn1, pdn2;

	hal_rtc_set_pwron_alarm_time(tm);

	if (logo)
		pdn2 = rtc_read(RTC_PDN2) | RTC_PDN2_PWRON_LOGO;
	else
		pdn2 = rtc_read(RTC_PDN2) & ~RTC_PDN2_PWRON_LOGO;

	rtc_write(RTC_PDN2, pdn2);

	if (enable)
		pdn1 = rtc_read(RTC_PDN1) | RTC_PDN1_PWRON_TIME;
	else
		pdn1 = rtc_read(RTC_PDN1) & ~RTC_PDN1_PWRON_TIME;
	rtc_write(RTC_PDN1, pdn1);
	rtc_write_trigger();
}
