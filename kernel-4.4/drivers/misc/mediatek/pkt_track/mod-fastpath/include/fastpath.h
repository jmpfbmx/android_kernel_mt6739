
#ifndef _FASTPATH_H
#define _FASTPATH_H

#include <linux/netdevice.h>

bool fastpath_is_support_dev(const char *dev_name);

int fastpath_in_nf(int iface, struct sk_buff *skb);
void fastpath_out_nf(int iface, struct sk_buff *skb, const struct net_device *out);

#endif
