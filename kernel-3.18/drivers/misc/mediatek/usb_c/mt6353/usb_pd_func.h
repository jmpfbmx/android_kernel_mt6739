

/* USB Power delivery module */

#ifndef _USB_PD_FUNC_H
#define _USB_PD_FUNC_H

extern int pd_build_request(int cnt, uint32_t *src_caps, uint32_t *rdo,
		     uint32_t *ma, uint32_t *mv, enum pd_request_type req_type);

extern int pd_is_max_request_allowed(void);

extern void pd_process_source_cap(struct typec_hba *hba, int cnt, uint32_t *src_caps);

extern void pd_set_max_voltage(unsigned mv);

extern unsigned pd_get_max_voltage(void);

extern int pd_is_valid_input_voltage(int mv);

extern int pd_check_requested_voltage(uint32_t rdo);

extern void pd_transition_voltage(int idx);

extern void pd_power_supply_reset(struct typec_hba *hba);

extern int pd_set_power_supply_ready(struct typec_hba *hba);

extern void pd_request_source_voltage(struct typec_hba *hba, int mv);

extern void pd_set_external_voltage_limit(struct typec_hba *hba, int mv);

extern void pd_set_input_current_limit(struct typec_hba *hba, uint32_t max_ma,
				uint32_t supply_voltage);

extern void typec_set_input_current_limit(struct typec_hba *hba, uint32_t max_ma,
				   uint32_t supply_voltage);

extern int pd_board_checks(void);

extern int pd_snk_is_vbus_provided(struct typec_hba *hba);

extern int pd_check_power_swap(struct typec_hba *hba);

extern int pd_check_data_swap(struct typec_hba *hba);


extern int pd_check_vconn_swap(struct typec_hba *hba);

extern void pd_check_pr_role(struct typec_hba *hba, int pr_role, int flags);

extern void pd_check_dr_role(struct typec_hba *hba, int dr_role, int flags);

extern int pd_charge_from_device(uint16_t vid, uint16_t pid);

extern void pd_execute_data_swap(struct typec_hba *hba, int data_role);

extern void pd_get_info(uint32_t *info_data);

extern int pd_custom_vdm(struct typec_hba *hba, int cnt, uint32_t *payload, uint32_t **rpayload);

extern int pd_svdm(struct typec_hba *hba, int cnt, uint32_t *payload, uint32_t **rpayload);

extern int pd_custom_flash_vdm(struct typec_hba *hba, int cnt, uint32_t *payload);

extern uint32_t pd_dfp_enter_mode(struct typec_hba *hba, uint16_t svid, int opos);

extern int pd_dfp_dp_get_pin_mode(struct typec_hba *hba, uint32_t status);

extern int pd_dfp_exit_mode(struct typec_hba *hba, uint16_t svid, int opos);

extern void pd_dfp_pe_init(struct typec_hba *hba);

extern uint16_t pd_get_identity_vid(struct typec_hba *hba);

extern uint16_t pd_get_identity_pid(struct typec_hba *hba);

int pd_dev_store_rw_hash(struct typec_hba *hba, uint16_t dev_id, uint32_t *rw_hash,
			 uint32_t ec_current_image);

int pd_fetch_acc_log_entry(struct typec_hba *hba);

void pd_log_recv_vdm(struct typec_hba *hba, int cnt, uint32_t *payload);

extern void pd_send_vdm(struct typec_hba *hba, uint32_t vid, int cmd, const uint32_t *data,
		 int count);

extern int pd_get_source_pdo(const uint32_t **src_pdo);

extern void pd_send_host_event(int mask);

extern int pd_alt_mode(struct typec_hba *hba, uint16_t svid);

extern void pd_send_hpd(struct typec_hba *hba, enum hpd_event hpd);

extern void pd_usb_billboard_deferred(void);
/* --- Physical layer functions : chip specific --- */

/* Packet preparation/retrieval */

extern void pd_init_dequeue(struct typec_hba *hba);

extern int pd_dequeue_bits(struct typec_hba *hba, int off, int len, uint32_t *val);

extern int pd_find_preamble(struct typec_hba *hba);

extern int pd_write_preamble(struct typec_hba *hba);

extern int pd_write_sym(struct typec_hba *hba, int bit_off, uint32_t val10);


extern int pd_write_last_edge(struct typec_hba *hba, int bit_off);

extern int encode_word(struct typec_hba *hba, int off, uint32_t val32);

extern int prepare_message(struct typec_hba *hba, uint16_t header, uint8_t cnt,
		    const uint32_t *data);

extern void pd_dump_packet(struct typec_hba *hba, const char *msg);

extern void pd_set_clock(struct typec_hba *hba, int freq);

/* TX/RX callbacks */

extern int pd_start_tx(struct typec_hba *hba, int polarity, int bit_len);

extern void pd_tx_set_circular_mode(struct typec_hba *hba);

extern void pd_tx_clear_circular_mode(struct typec_hba *hba);

extern void pd_tx_done(struct typec_hba *hba, int polarity);

extern int pd_rx_started(struct typec_hba *hba);

extern void pd_set_suspend(struct typec_hba *hba, int enable);

/* Callback when the hardware has detected an incoming packet */
extern void pd_rx_event(struct typec_hba *hba);
/* Start sampling the CC line for reception */
extern void pd_rx_start(struct typec_hba *hba);
/* Call when we are done reading a packet */
extern void pd_rx_complete(struct typec_hba *hba);

/* restart listening to the CC wire */
extern void pd_rx_enable_monitoring(struct typec_hba *hba);
/* stop listening to the CC wire during transmissions */
extern void pd_rx_disable_monitoring(struct typec_hba *hba);

/* get time since last RX edge interrupt */
extern uint64_t get_time_since_last_edge(struct typec_hba *hba);

extern void pd_hw_release(struct typec_hba *hba);

extern void pd_hw_init(struct typec_hba *hba, int role);

extern void pd_hw_init_rx(struct typec_hba *hba);

/* --- Protocol layer functions --- */

extern int pd_analyze_rx(struct typec_hba *hba, uint32_t *payload);

extern int pd_is_connected(struct typec_hba *hba);

extern void pd_execute_hard_reset(struct typec_hba *hba);

extern void pd_transmit_complete(struct typec_hba *hba, int status);

extern int pd_get_polarity(struct typec_hba *hba);

extern int pd_get_partner_data_swap_capable(struct typec_hba *hba);

extern void pd_request_power_swap(struct typec_hba *hba);

extern void pd_request_data_swap(struct typec_hba *hba);

extern void pd_comm_enable(struct typec_hba *hba, int enable);

extern void pd_ping_enable(struct typec_hba *hba, int enable);

/* Issue PD soft reset */
extern void pd_soft_reset(void);

/* Prepare PD communication for reset */
extern void pd_prepare_reset(void);

extern void pd_set_new_power_request(struct typec_hba *hba);

/* ----- Logging ----- */
#ifdef CONFIG_USB_PD_LOGGING
extern void pd_log_event(uint8_t type, uint8_t size_port,
		  uint16_t data, void *payload);

extern int pd_vdm_get_log_entry(uint32_t *payload);
#else  /* CONFIG_USB_PD_LOGGING */
static inline void pd_log_event(uint8_t type, uint8_t size_port,
				uint16_t data, void *payload) {}
static inline int pd_vdm_get_log_entry(uint32_t *payload) { return 0; }
#endif /* CONFIG_USB_PD_LOGGING */

#endif
