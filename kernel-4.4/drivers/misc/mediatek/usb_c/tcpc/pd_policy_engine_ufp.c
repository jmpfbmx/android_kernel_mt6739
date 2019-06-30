
#include "inc/pd_core.h"
#include "inc/pd_dpm_core.h"
#include "inc/tcpci.h"
#include "inc/pd_policy_engine.h"


void pe_ufp_vdm_get_identity_entry(struct pd_port *pd_port)
{
	pd_dpm_ufp_request_id_info(pd_port);
}


void pe_ufp_vdm_get_svids_entry(struct pd_port *pd_port)
{
	pd_dpm_ufp_request_svid_info(pd_port);
}


void pe_ufp_vdm_get_modes_entry(struct pd_port *pd_port)
{
	pd_dpm_ufp_request_mode_info(pd_port);
}


void pe_ufp_vdm_evaluate_mode_entry_entry(
			struct pd_port *pd_port)
{
	pd_dpm_ufp_request_enter_mode(pd_port);
}


void pe_ufp_vdm_mode_exit_entry(struct pd_port *pd_port)
{
	pd_dpm_ufp_request_exit_mode(pd_port);
}


void pe_ufp_vdm_attention_request_entry(
	struct pd_port *pd_port)
{
	VDM_STATE_NORESP_CMD(pd_port);

	switch (pd_port->mode_svid) {
#ifdef CONFIG_USB_PD_ALT_MODE
	case USB_SID_DISPLAYPORT:
		pd_dpm_ufp_send_dp_attention(pd_port);
		break;
#endif
	default:
		pd_send_vdm_attention(pd_port,
			TCPC_TX_SOP, pd_port->mode_svid, pd_port->mode_obj_pos);
		break;
	}
}


#ifdef CONFIG_USB_PD_ALT_MODE

void pe_ufp_vdm_dp_status_update_entry(struct pd_port *pd_port)
{
	pd_dpm_ufp_request_dp_status(pd_port);
}

void pe_ufp_vdm_dp_configure_entry(struct pd_port *pd_port)
{
	pd_dpm_ufp_request_dp_config(pd_port);
}

#endif	/* CONFIG_USB_PD_ALT_MODE */


#ifdef CONFIG_USB_PD_CUSTOM_VDM

void pe_ufp_uvdm_recv_entry(struct pd_port *pd_port)
{
	pd_dpm_ufp_recv_uvdm(pd_port);
}

#endif	/* CONFIG_USB_PD_CUSTOM_VDM */
