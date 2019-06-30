
#undef TRACE_SYSTEM
#define TRACE_SYSTEM ccci

#if !defined(_TRACE_EVENT_SAMPLE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_EVENT_SAMPLE_H

#include <linux/tracepoint.h>

TRACE_EVENT(ccci_bm,
	    TP_PROTO(unsigned int bm_pool1_size, unsigned int bm_pool2_size), TP_ARGS(bm_pool1_size, bm_pool2_size),
	    TP_STRUCT__entry(__field(unsigned int, bm_pool1_size)
			     __field(unsigned int, bm_pool2_size)
		),
	    TP_fast_assign(__entry->bm_pool1_size = bm_pool1_size;
			   __entry->bm_pool2_size = bm_pool2_size;),
	    TP_printk("%u,%u", __entry->bm_pool1_size, __entry->bm_pool2_size)
	);
#endif

/***** NOTICE! The #if protection ends here. *****/

#undef TRACE_INCLUDE_PATH
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE ccci_events
#include <trace/define_trace.h>
