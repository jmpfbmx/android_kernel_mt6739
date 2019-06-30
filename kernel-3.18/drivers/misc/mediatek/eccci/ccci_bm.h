
#ifndef __CCCI_BM_H__
#define __CCCI_BM_H__
#include "ccci_config.h"
#include "ccci_core.h"

#define SKB_4K (CCCI_MTU+128)	/* user MTU+CCCI_H+extra(ex. ccci_fsd's OP_ID), for genral packet */
#define SKB_1_5K (CCCI_NET_MTU+16)	/* net MTU+CCCI_H, for network packet */
#define SKB_16 16		/* for struct ccci_header */
#define NET_RX_BUF SKB_4K

#ifdef NET_SKBUFF_DATA_USES_OFFSET
#define skb_size(x) ((x)->end)
#define skb_data_size(x) ((x)->head + (x)->end - (x)->data)
#else
#define skb_size(x) ((x)->end - (x)->head)
#define skb_data_size(x) ((x)->end - (x)->data)
#endif

struct ccci_skb_queue {
	unsigned int magic_header;
	struct sk_buff_head skb_list;
	unsigned int max_len;
	struct work_struct reload_work;
	unsigned char pre_filled;
	unsigned int max_history;
	unsigned int max_occupied;
	unsigned int enq_count;
	unsigned int deq_count;
	unsigned int magic_footer;
};

struct sk_buff *ccci_alloc_skb(int size, unsigned char from_pool, unsigned char blocking);
void ccci_free_skb(struct sk_buff *skb);

struct sk_buff *ccci_skb_dequeue(struct ccci_skb_queue *queue);
void ccci_skb_enqueue(struct ccci_skb_queue *queue, struct sk_buff *newsk);
void ccci_skb_queue_init(struct ccci_skb_queue *queue, unsigned int skb_size, unsigned int max_len,
	char fill_now);
void ccci_dump_skb_pool_usage(int md_id);

void ccci_mem_dump(int md_id, void *start_addr, int len);
void ccci_cmpt_mem_dump(int md_id, void *start_addr, int len);

#endif				/* __CCCI_BM_H__ */
