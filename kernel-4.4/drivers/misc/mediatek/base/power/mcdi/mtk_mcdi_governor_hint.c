
#include <linux/kernel.h>
#include <linux/spinlock.h>

#include "mtk_mcdi_governor_hint.h"

static DEFINE_SPINLOCK(system_idle_hint_spin_lock);

static unsigned int system_idle_hint;

unsigned int system_idle_hint_result_raw(void)
{
	unsigned long flags = 0;
	unsigned int hint = 0;

	spin_lock_irqsave(&system_idle_hint_spin_lock, flags);

	hint = system_idle_hint;

	spin_unlock_irqrestore(&system_idle_hint_spin_lock, flags);

	return hint;
}

bool system_idle_hint_result(void)
{
	return (system_idle_hint_result_raw() != 0);
}

bool system_idle_hint_request(unsigned int id, bool value)
{
	unsigned long flags = 0;

	if (!(id >= 0 && id < NF_SYSTEM_IDLE_HINT))
		return false;

	spin_lock_irqsave(&system_idle_hint_spin_lock, flags);

	if (value == true)
		system_idle_hint |= (1 << id);
	else
		system_idle_hint &= ~(1 << id);

	spin_unlock_irqrestore(&system_idle_hint_spin_lock, flags);

	return true;
}

