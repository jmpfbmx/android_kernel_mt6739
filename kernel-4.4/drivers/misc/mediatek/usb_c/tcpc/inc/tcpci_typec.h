
#ifndef __LINUX_TCPCI_TYPEC_H
#define __LINUX_TCPCI_TYPEC_H
#include "tcpci.h"

struct tcpc_device;


extern int tcpc_typec_enter_lpm_again(struct tcpc_device *tcpc_dev);
extern int tcpc_typec_handle_cc_change(struct tcpc_device *tcpc_dev);

extern int tcpc_typec_handle_ps_change(
		struct tcpc_device *tcpc_dev, int vbus_level);

extern int tcpc_typec_handle_timeout(
		struct tcpc_device *tcpc_dev, uint32_t timer_id);

extern int tcpc_typec_handle_vsafe0v(struct tcpc_device *tcpc_dev);

extern int tcpc_typec_set_rp_level(struct tcpc_device *tcpc_dev, uint8_t res);

extern int tcpc_typec_error_recovery(struct tcpc_device *tcpc_dev);

extern int tcpc_typec_disable(struct tcpc_device *tcpc_dev);
extern int tcpc_typec_enable(struct tcpc_device *tcpc_dev);

extern int tcpc_typec_change_role(
	struct tcpc_device *tcpc_dev, uint8_t typec_role);

#ifdef CONFIG_USB_POWER_DELIVERY
extern int tcpc_typec_handle_pe_pr_swap(struct tcpc_device *tcpc_dev);
#endif /* CONFIG_USB_POWER_DELIVERY */

#ifdef CONFIG_TYPEC_CAP_ROLE_SWAP
extern int tcpc_typec_swap_role(struct tcpc_device *tcpc_dev);
#endif /* CONFIG_TYPEC_CAP_ROLE_SWAP */

#endif /* #ifndef __LINUX_TCPCI_TYPEC_H */
