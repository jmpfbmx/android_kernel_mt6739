

#ifndef _MTK_JPEG_CORE_H
#define _MTK_JPEG_CORE_H

#include <linux/interrupt.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-fh.h>

#define MTK_JPEG_NAME		"mtk-jpeg"

#define MTK_JPEG_FMT_FLAG_DEC_OUTPUT	(1 << 0)
#define MTK_JPEG_FMT_FLAG_DEC_CAPTURE	(1 << 1)

#define MTK_JPEG_FMT_TYPE_OUTPUT	1
#define MTK_JPEG_FMT_TYPE_CAPTURE	2

#define MTK_JPEG_MIN_WIDTH	32
#define MTK_JPEG_MIN_HEIGHT	32
#define MTK_JPEG_MAX_WIDTH	8192
#define MTK_JPEG_MAX_HEIGHT	8192

#define MTK_JPEG_BENCHMARK	1

enum mtk_jpeg_ctx_state {
	MTK_JPEG_INIT = 0,
	MTK_JPEG_RUNNING,
	MTK_JPEG_SOURCE_CHANGE,
};

struct mtk_jpeg_dev {
	struct mutex		lock;
	struct mutex		dev_lock;
	spinlock_t		irq_lock;
	struct workqueue_struct	*workqueue;
	struct device		*dev;
	struct v4l2_device	v4l2_dev;
	struct v4l2_m2m_dev	*m2m_dev;
	void			*alloc_ctx;
	struct video_device	*dec_vdev;
	void __iomem		*dec_reg_base;
	struct clk		*clk_venc_jdec;
	struct clk		*clk_venc_jdec_smi;
	struct device		*larb;
};

struct mtk_jpeg_fmt {
	char	*name;
	u32	fourcc;
	int	depth;
	int	colplanes;
	int	h_align;
	int	v_align;
	u32	flags;
};

struct mtk_jpeg_q_data {
	struct mtk_jpeg_fmt	*fmt;
	u32			w;
	u32			h;
	u32			size;
};

struct mtk_jpeg_ctx {
	struct mtk_jpeg_dev		*jpeg;
	struct work_struct		work;
	struct completion		completion;
	struct mtk_jpeg_q_data		out_q;
	struct mtk_jpeg_q_data		cap_q;
	struct v4l2_fh			fh;
	u32				dec_irq_ret;
	enum mtk_jpeg_ctx_state		state;
#if MTK_JPEG_BENCHMARK
	struct timeval			jpeg_enc_dec_start;
	uint32_t			total_enc_dec_cnt;
	uint32_t			total_enc_dec_time;
	uint32_t			total_parse_cnt;
	uint32_t			total_parse_time;
#endif
};

#endif /* _MTK_JPEG_CORE_H */
