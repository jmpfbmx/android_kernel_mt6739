
#undef TRACE_SYSTEM
#define TRACE_SYSTEM ccci

#if !defined(_TRACE_EVENT_SAMPLE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_EVENT_SAMPLE_H

#include <linux/tracepoint.h>

TRACE_EVENT(port_net_rx,
	    TP_PROTO(unsigned int md_id, unsigned int queue_no, unsigned int ccci_ch, unsigned int rx_cb_time,
		     unsigned int rx_total_time), TP_ARGS(md_id, queue_no, ccci_ch, rx_cb_time, rx_total_time),
	    TP_STRUCT__entry(__field(unsigned int, md_id)
			     __field(unsigned int, queue_no)
			     __field(unsigned int, ccci_ch)
			     __field(unsigned int, rx_cb_time)
			     __field(unsigned int, rx_total_time)
	    ),
	    TP_fast_assign(__entry->md_id = md_id;
			   __entry->queue_no = queue_no;
			   __entry->ccci_ch = ccci_ch;
			   __entry->rx_cb_time = rx_cb_time; __entry->rx_total_time = rx_total_time;
	    ),
	    TP_printk("md%u	q%d	ch%u	%u	%u", __entry->md_id + 1, __entry->queue_no, __entry->ccci_ch,
		      __entry->rx_cb_time, __entry->rx_total_time)
	);

TRACE_EVENT(port_net_tx,
	    TP_PROTO(int md_id, unsigned int queue_no, unsigned int ccci_ch, unsigned int req_alloc_time,
		     unsigned int send_req_time, unsigned int tx_total_time), TP_ARGS(md_id, queue_no, ccci_ch,
										      req_alloc_time, send_req_time,
										      tx_total_time),
	    TP_STRUCT__entry(__field(unsigned int, md_id)
			     __field(unsigned int, queue_no)
			     __field(unsigned int, ccci_ch)
			     __field(unsigned int, req_alloc_time)
			     __field(unsigned int, send_req_time)
			     __field(unsigned int, tx_total_time)
	    ),
	    TP_fast_assign(__entry->md_id = md_id;
			   __entry->queue_no = queue_no;
			   __entry->ccci_ch = ccci_ch;
			   __entry->req_alloc_time = req_alloc_time;
			   __entry->send_req_time = send_req_time; __entry->tx_total_time = tx_total_time;
	    ),
	    TP_printk("md%d	q%u	ch%u	%u	%u	%u", __entry->md_id + 1, __entry->queue_no,
		      __entry->ccci_ch, __entry->req_alloc_time, __entry->send_req_time, __entry->tx_total_time)
	);

TRACE_EVENT(port_net_error,
	    TP_PROTO(int md_id, int queue_no, unsigned int ccci_ch, unsigned int error_no, unsigned int line_no),
	    TP_ARGS(md_id, queue_no, ccci_ch, error_no, line_no), TP_STRUCT__entry(__field(int, md_id)
										   __field(int, queue_no)
										   __field(int, ccci_ch)
										   __field(unsigned int, error_no)
										   __field(unsigned int, line_no)
	    ),
	    TP_fast_assign(__entry->md_id = md_id;
			   __entry->queue_no = queue_no;
			   __entry->ccci_ch = ccci_ch; __entry->error_no = error_no; __entry->line_no = line_no;
	    ),
	    TP_printk("md%d	q%d	ch=%d	err=%u	line_no=%u", (__entry->md_id + 1), __entry->queue_no,
		      __entry->ccci_ch, __entry->error_no, __entry->line_no)
	);
#endif

/***** NOTICE! The #if protection ends here. *****/

#undef TRACE_INCLUDE_PATH
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE port_net_events
#include <trace/define_trace.h>
