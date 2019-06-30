
#ifndef _MTK_VCODEC_DRV_H_
#define _MTK_VCODEC_DRV_H_

#include <linux/platform_device.h>
#include <linux/videodev2.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/videobuf2-core.h>

#include "mtk_vcodec_util.h"

#define MTK_VCODEC_MAX_INSTANCES		11
#define MTK_VCODEC_MAX_ENCODER_INSTANCES	3

#define MTK_VCODEC_MAX_FRAME_SIZE	0x800000
#define MTK_VIDEO_MAX_FRAME		32
#define MTK_MAX_CTRLS			10

#define MTK_VCODEC_DRV_NAME			"mtk_vcodec_drv"
#define MTK_VCODEC_DEC_NAME		"mtk-vcodec-dec"
#define MTK_VCODEC_ENC_NAME		"mtk-vcodec-enc"

#define MTK_VENC_IRQ_STATUS_SPS          0x1
#define MTK_VENC_IRQ_STATUS_PPS          0x2
#define MTK_VENC_IRQ_STATUS_FRM          0x4
#define MTK_VENC_IRQ_STATUS_DRAM         0x8
#define MTK_VENC_IRQ_STATUS_PAUSE        0x10
#define MTK_VENC_IRQ_STATUS_SWITCH       0x20

#define MTK_VENC_IRQ_STATUS_OFFSET       0x05C
#define MTK_VENC_IRQ_ACK_OFFSET          0x060

#define MTK_VCODEC_MAX_PLANES		3

#define VDEC_HW_ACTIVE	0x10
#define VDEC_IRQ_CFG    0x11
#define VDEC_IRQ_CLR    0x10

#define VDEC_IRQ_CFG_REG	0xa4
#define NUM_MAX_ALLOC_CTX  4
#define MTK_V4L2_BENCHMARK 0
#define WAIT_INTR_TIMEOUT	1000

enum mtk_hw_reg_idx {
	VDEC_SYS,
	VDEC_MISC,
	VDEC_LD,
	VDEC_TOP,
	VDEC_CM,
	VDEC_AD,
	VDEC_AV,
	VDEC_PP,
	VDEC_HWD,
	VDEC_HWQ,
	VDEC_HWB,
	VDEC_HWG,
	NUM_MAX_VDEC_REG_BASE,
	VENC_SYS = NUM_MAX_VDEC_REG_BASE,
	VENC_LT_SYS,
	NUM_MAX_VCODEC_REG_BASE
};

enum mtk_instance_type {
	MTK_INST_DECODER		= 0,
	MTK_INST_ENCODER		= 1,
};

enum mtk_instance_state {
	MTK_STATE_FREE		= 0,
	MTK_STATE_INIT = 1,
	MTK_STATE_HEADER = 2,
	MTK_STATE_FLUSH = 3,
	MTK_STATE_RES_CHANGE = 4,
	MTK_STATE_ABORT = 5,
};

enum mtk_encode_param {
	MTK_ENCODE_PARAM_NONE = 0,
	MTK_ENCODE_PARAM_BITRATE = (1 << 0),
	MTK_ENCODE_PARAM_FRAMERATE = (1 << 1),
	MTK_ENCODE_PARAM_INTRA_PERIOD = (1 << 2),
	MTK_ENCODE_PARAM_FRAME_TYPE = (1 << 3),
	MTK_ENCODE_PARAM_SKIP_FRAME = (1 << 4),
};

enum mtk_fmt_type {
	MTK_FMT_DEC		= 0,
	MTK_FMT_ENC		= 1,
	MTK_FMT_FRAME		= 2,
};

struct mtk_video_fmt {
	char *name;
	u32 fourcc;
	enum mtk_fmt_type type;
	u32 num_planes;
};

struct mtk_codec_framesizes {
	u32 fourcc;
	struct	v4l2_frmsize_stepwise	stepwise;
};

enum mtk_q_type {
	MTK_Q_DATA_SRC		= 0,
	MTK_Q_DATA_DST		= 1,
};

struct mtk_q_data {
	unsigned int		width;
	unsigned int		height;
	enum v4l2_field		field;
	enum v4l2_colorspace	colorspace;
	unsigned int		bytesperline[MTK_VCODEC_MAX_PLANES];
	unsigned int		sizeimage[MTK_VCODEC_MAX_PLANES];
	struct mtk_video_fmt	*fmt;
};

struct mtk_enc_params {
	unsigned int	bitrate;
	unsigned int	num_b_frame;
	unsigned int	rc_frame;
	unsigned int	rc_mb;
	unsigned int	seq_hdr_mode;
	unsigned int	gop_size;
	unsigned int	framerate_num;
	unsigned int	framerate_denom;
	unsigned int	h264_max_qp;
	unsigned int	h264_profile;
	unsigned int	h264_level;
	unsigned int	force_intra;
	unsigned int	skip_frame;
};

struct mtk_vcodec_pm {
	struct clk	*vdecpll;
	struct clk	*vdec_sel;
	struct clk	*venc;
	struct clk	*venclt;
	struct device	*larbvdec;
	struct device	*larbvenc;
	struct device	*dev;
	struct mtk_vcodec_dev *mtkdev;
};

struct vdec_pic_info {
	unsigned int pic_w;
	unsigned int pic_h;
	unsigned int buf_w;
	unsigned int buf_h;
	unsigned int y_bs_sz;
	unsigned int c_bs_sz;
	unsigned int y_len_sz;
	unsigned int c_len_sz;
};

struct mtk_vcodec_ctx {
	enum mtk_instance_type type;
	struct mtk_vcodec_dev *dev;
	struct v4l2_fh fh;
	struct v4l2_m2m_ctx *m2m_ctx;
	struct mtk_q_data q_data[2];
	int idx;
	enum mtk_instance_state state;
	enum mtk_encode_param param_change;

	struct mutex vb2_mutex;
	struct mtk_enc_params enc_params;

	struct vdec_common_if *dec_if;
	struct venc_common_if *enc_if;
	unsigned long drv_handle;

	struct vdec_pic_info picinfo;
	int dpb_count;

	int int_cond;
	int int_type;
	wait_queue_head_t queue;
	unsigned int irq_status;

	struct v4l2_ctrl_handler ctrl_hdl;
	struct v4l2_ctrl *ctrls[MTK_MAX_CTRLS];

	struct work_struct decode_work;
	struct work_struct encode_work;
	struct vdec_pic_info last_decoded_picinfo;

	int decoded_frame_cnt;
#if MTK_V4L2_BENCHMARK
	unsigned int total_enc_dec_cnt;
	unsigned int total_enc_dec_time;
	unsigned int total_enc_hdr_time;
	unsigned int total_enc_dec_init_time;

	unsigned int total_qbuf_out_time;
	unsigned int total_qbuf_cap_time;
	unsigned int total_qbuf_out_cnt;
	unsigned int total_qbuf_cap_cnt;
	unsigned int total_dqbuf_out_time;
	unsigned int total_dqbuf_cap_time;
	unsigned int total_dqbuf_out_cnt;
	unsigned int total_dqbuf_cap_cnt;
	unsigned int total_dqbuf_cnt;
	unsigned int total_expbuf_time;
#endif
};

struct mtk_vcodec_dev {
	struct v4l2_device	v4l2_dev;
	struct video_device	*vfd_dec;
	struct video_device	*vfd_enc;

	struct v4l2_m2m_dev	*m2m_dev_dec;
	struct v4l2_m2m_dev	*m2m_dev_enc;
	struct platform_device	*plat_dev;
	struct platform_device	*vpu_plat_dev;
	struct vb2_alloc_ctx	*alloc_ctx;
	struct mtk_vcodec_ctx	*ctx[MTK_VCODEC_MAX_INSTANCES];
	int curr_ctx;
	void __iomem		*reg_base[NUM_MAX_VCODEC_REG_BASE];

	unsigned long	instance_mask[BITS_TO_LONGS(MTK_VCODEC_MAX_INSTANCES)];
	int			num_instances;

	struct workqueue_struct *decode_workqueue;
	struct workqueue_struct *encode_workqueue;

	int			int_cond;
	int			int_type;
	struct mutex		dev_mutex;
	wait_queue_head_t	queue;

	int			dec_irq;
	int			enc_irq;
	int			enc_lt_irq;

	struct mutex		dec_mutex;
	struct mutex		enc_mutex;

	struct mtk_vcodec_pm	pm;
	unsigned int		dec_capability;
	unsigned int		enc_capability;
};

struct mtk_vcodec_ctrl {
	u32			id;
	enum v4l2_ctrl_type	type;
	u8			name[32];
	s32			minimum;
	s32			maximum;
	s32			step;
	u32			menu_skip_mask;
	s32			default_value;
	u8			is_volatile;
};

static inline struct mtk_vcodec_ctx *fh_to_ctx(struct v4l2_fh *fh)
{
	return container_of(fh, struct mtk_vcodec_ctx, fh);
}

static inline struct mtk_vcodec_ctx *ctrl_to_ctx(struct v4l2_ctrl *ctrl)
{
	return container_of(ctrl->handler, struct mtk_vcodec_ctx, ctrl_hdl);
}

extern const struct v4l2_ioctl_ops mtk_vdec_ioctl_ops;
extern const struct v4l2_m2m_ops mtk_vdec_m2m_ops;
extern const struct v4l2_ioctl_ops mtk_venc_ioctl_ops;
extern const struct v4l2_m2m_ops mtk_venc_m2m_ops;

#endif /* _MTK_VCODEC_DRV_H_ */
