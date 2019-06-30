
#ifndef __CCCI_PORT_H__
#define __CCCI_PORT_H__

enum {
	CRIT_USR_FS,
	CRIT_USR_MUXD,
	CRIT_USR_MDLOG,
	CRIT_USR_META,
	CRIT_USR_MDLOG_CTRL = CRIT_USR_META,
	CRIT_USR_MAX,
};

int ccci_port_init(int md_id);

void ccci_port_dump_status(int md_id);

void ccci_port_md_status_notify(int md_id, unsigned int state);

void ccci_port_queue_status_notify(int md_id, int hif_id, int qno, int dir, unsigned int state);

int ccci_port_recv_skb(int md_id, int hif_id, struct sk_buff *skb, unsigned int flag);

int ccci_port_check_critical_user(int md_id);
int ccci_port_get_critical_user(int md_id, unsigned int user_id);

int ccci_port_send_msg_to_md(int md_id, int ch, unsigned int msg,
	unsigned int resv, int blocking);

void ccci_port_set_traffic_flag(int md_id, unsigned int dump_flag);
#endif /* __CCCI_PORT_H__ */
