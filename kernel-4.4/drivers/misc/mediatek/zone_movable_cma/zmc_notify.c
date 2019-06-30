#define pr_fmt(fmt) "zone_movable_cma: " fmt
#include <linux/notifier.h>

static BLOCKING_NOTIFIER_HEAD(zmc_notifier_list);

int zmc_register_client(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&zmc_notifier_list, nb);
}

int zmc_unregister_client(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&zmc_notifier_list, nb);
}

int zmc_notifier_call_chain(unsigned long val, void *v)
{
	return blocking_notifier_call_chain(&zmc_notifier_list, val, v);
}
