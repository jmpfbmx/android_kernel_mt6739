
#undef TRACE_SYSTEM
#define TRACE_SYSTEM ccci

#if !defined(_TRACE_EVENT_SAMPLE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_EVENT_SAMPLE_H

#include <linux/tracepoint.h>


TRACE_EVENT(cldma_irq,
	TP_PROTO(unsigned int irq_type, unsigned int queue_mask),
	TP_ARGS(irq_type, queue_mask), TP_STRUCT__entry(__field(unsigned int, irq_type)
		__field(unsigned int, queue_mask)
	),
	TP_fast_assign(__entry->irq_type = irq_type; __entry->queue_mask = queue_mask;
	),
	TP_printk("%s	%u", ((__entry->irq_type == 0) ? "TX" : "RX"), __entry->queue_mask)
	);

TRACE_EVENT(cldma_rx,
	TP_PROTO(int queue_no, int ccci_ch, unsigned long long req_alloc_time, unsigned long long port_recv_time,
		unsigned long long skb_alloc_time, unsigned long long rx_cost_time, unsigned long long rx_bytes),
	TP_ARGS(queue_no, ccci_ch, req_alloc_time, port_recv_time, skb_alloc_time, rx_cost_time, rx_bytes),
	TP_STRUCT__entry(__field(int, queue_no)
		__field(int, ccci_ch)
		__field(unsigned long long, req_alloc_time)
		__field(unsigned long long, port_recv_time)
		__field(unsigned long long, skb_alloc_time)
		__field(unsigned long long, rx_cost_time)
		__field(unsigned long long, rx_bytes)
	),
	TP_fast_assign(__entry->queue_no = queue_no;
		__entry->ccci_ch = ccci_ch;
		__entry->req_alloc_time = req_alloc_time;
		__entry->port_recv_time = port_recv_time;
		__entry->skb_alloc_time = skb_alloc_time;
		__entry->rx_cost_time = rx_cost_time; __entry->rx_bytes = rx_bytes;
	),
	TP_printk("q%d	ch%d	%llu	%llu	%llu	%llu	%llu", __entry->queue_no, __entry->ccci_ch,
		__entry->req_alloc_time, __entry->port_recv_time, __entry->skb_alloc_time,
		__entry->rx_cost_time, __entry->rx_bytes)
	);

TRACE_EVENT(cldma_rx_done,
	TP_PROTO(int queue_no, unsigned long long rx_interal, unsigned long long total_time,
		unsigned int total_count, unsigned long long rx_bytes, unsigned long long sample_time,
		unsigned long long sample_rxbytes),
	TP_ARGS(queue_no, rx_interal, total_time, total_count,
		rx_bytes, sample_time, sample_rxbytes),
	TP_STRUCT__entry(__field(int, queue_no)
		__field(unsigned long long, rx_interal)
		__field(unsigned long long, total_time)
		__field(unsigned int, total_count)
		__field(unsigned long long, rx_bytes)
		__field(unsigned long long, sample_time)
		__field(unsigned long long, sample_rxbytes)
	),
	TP_fast_assign(__entry->queue_no = queue_no;
		__entry->rx_interal = rx_interal;
		__entry->total_time = total_time;
		__entry->total_count = total_count;
		__entry->rx_bytes = rx_bytes;
		__entry->sample_time = sample_time; __entry->sample_rxbytes = sample_rxbytes;
	),
	TP_printk("q%d	%llu	%llu	%u	%llu	%llu	%llu", __entry->queue_no, __entry->rx_interal,
		__entry->total_time, __entry->total_count, __entry->rx_bytes,
		__entry->sample_time, __entry->sample_rxbytes)
	);

TRACE_EVENT(cldma_tx,
	TP_PROTO(int queue_no, int ccci_ch, unsigned int free_slot, unsigned long long tx_interal,
		unsigned long long tx_cost_time, unsigned long long tx_bytes, unsigned long long sample_time,
		unsigned long long sample_txbytes),
	TP_ARGS(queue_no, ccci_ch, free_slot, tx_interal, tx_cost_time,
		tx_bytes, sample_time, sample_txbytes),
	TP_STRUCT__entry(__field(int, queue_no)
		__field(int, ccci_ch)
		__field(unsigned int, free_slot)
		__field(unsigned long long, tx_interal)
		__field(unsigned long long, tx_cost_time)
		__field(unsigned long long, tx_bytes)
		__field(unsigned long long, sample_time)
		__field(unsigned long long, sample_txbytes)
	),
	TP_fast_assign(__entry->queue_no = queue_no;
		__entry->ccci_ch = ccci_ch;
		__entry->free_slot = free_slot;
		__entry->tx_interal = tx_interal;
		__entry->tx_cost_time = tx_cost_time;
		__entry->tx_bytes = tx_bytes;
		__entry->sample_time = sample_time; __entry->sample_txbytes = sample_txbytes;
	),
	TP_printk("q%u	ch%d	%u	%llu	%llu	%llu	%llu	%llu", __entry->queue_no,
		__entry->ccci_ch, __entry->free_slot, __entry->tx_interal,
		__entry->tx_cost_time, __entry->tx_bytes, __entry->sample_time, __entry->sample_txbytes)
	);

TRACE_EVENT(cldma_tx_done,
	TP_PROTO(int queue_no, unsigned long long tx_interal, unsigned long long total_time,
		unsigned int total_count), TP_ARGS(queue_no, tx_interal, total_time, total_count),
	TP_STRUCT__entry(__field(int, queue_no)
		__field(unsigned long long, tx_interal)
		__field(unsigned long long, total_time)
		__field(unsigned int, total_count)
	),
	TP_fast_assign(__entry->queue_no = queue_no;
		__entry->total_time = tx_interal;
		__entry->total_time = total_time; __entry->total_count = total_count;
	),
	TP_printk("q%d	%llu	%llu	%u", __entry->queue_no, __entry->tx_interal, __entry->total_time,
		__entry->total_count)
	);

TRACE_EVENT(cldma_error,
	TP_PROTO(int queue_no, unsigned int ccci_ch, unsigned int error_no, unsigned int line_no),
	TP_ARGS(queue_no, ccci_ch, error_no, line_no),
	TP_STRUCT__entry(__field(int, queue_no)
		__field(int, ccci_ch)
		__field(unsigned int, error_no)
		__field(unsigned int, line_no)
	),
	TP_fast_assign(__entry->queue_no = queue_no;
		__entry->ccci_ch = ccci_ch; __entry->error_no = error_no; __entry->line_no = line_no;
	),
	TP_printk("q%d	ch=%d	err=%u	line_no=%u", __entry->queue_no, __entry->ccci_ch, __entry->error_no,
		__entry->line_no)
	);

TRACE_EVENT(ccci_skb_rx,
	TP_PROTO(unsigned long long *dl_delay),
	TP_ARGS(dl_delay),
	TP_STRUCT__entry(
		__array(unsigned long long,	dl_delay, 8)
	),

	TP_fast_assign(
		memcpy(__entry->dl_delay, dl_delay, 8*sizeof(unsigned long long));
	),

	TP_printk("	%llu	%llu	%llu	%llu	%llu	%llu	%llu	%llu",
		__entry->dl_delay[0], __entry->dl_delay[1], __entry->dl_delay[2], __entry->dl_delay[3],
		__entry->dl_delay[4], __entry->dl_delay[5], __entry->dl_delay[6], __entry->dl_delay[7])
);
#endif

/***** NOTICE! The #if protection ends here. *****/

#undef TRACE_INCLUDE_PATH
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE modem_cldma_events
#include <trace/define_trace.h>
