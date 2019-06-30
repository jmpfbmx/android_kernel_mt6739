
#ifndef _XHCI_MTK_H_
#define _XHCI_MTK_H_

#include "xhci.h"

#define XHCI_MTK_MAX_ESIT	64

struct mu3h_sch_bw_info {
	u32 bus_bw[XHCI_MTK_MAX_ESIT];
	struct list_head bw_ep_list;
};

struct mu3h_sch_ep_info {
	u32 esit;
	u32 num_budget_microframes;
	u32 bw_cost_per_microframe;
	struct list_head endpoint;
	void *ep;
	/*
	 * mtk xHCI scheduling information put into reserved DWs
	 * in ep context
	 */
	u32 offset;
	u32 repeat;
	u32 pkts;
	u32 cs_count;
	u32 burst_mode;
};

#define MU3C_U3_PORT_MAX 4
#define MU3C_U2_PORT_MAX 5

struct mu3c_ippc_regs {
	__le32 ip_pw_ctr0;
	__le32 ip_pw_ctr1;
	__le32 ip_pw_ctr2;
	__le32 ip_pw_ctr3;
	__le32 ip_pw_sts1;
	__le32 ip_pw_sts2;
	__le32 reserved0[3];
	__le32 ip_xhci_cap;
	__le32 reserved1[2];
	__le64 u3_ctrl_p[MU3C_U3_PORT_MAX];
	__le64 u2_ctrl_p[MU3C_U2_PORT_MAX];
	__le32 reserved2;
	__le32 u2_phy_pll;
	__le32 reserved3[33]; /* 0x80 ~ 0xff */
};

struct xhci_hcd_mtk {
	struct device *dev;
	struct usb_hcd *hcd;
	struct mu3h_sch_bw_info *sch_array;
	struct mu3c_ippc_regs __iomem *ippc_regs;
	int num_u2_ports;
	int num_u3_ports;
	struct regulator *vusb33;
	struct regulator *vbus;
	struct clk *sys_clk;	/* sys and mac clock */
	struct clk *wk_deb_p0;	/* port0's wakeup debounce clock */
	struct clk *wk_deb_p1;
	struct regmap *pericfg;
	struct phy **phys;
	int num_phys;
	int wakeup_src;
	bool lpm_support;
};

static inline struct xhci_hcd_mtk *hcd_to_mtk(struct usb_hcd *hcd)
{
	return dev_get_drvdata(hcd->self.controller);
}

#if IS_ENABLED(CONFIG_USB_XHCI_MTK)
int xhci_mtk_sch_init(struct xhci_hcd_mtk *mtk);
void xhci_mtk_sch_exit(struct xhci_hcd_mtk *mtk);
int xhci_mtk_add_ep_quirk(struct usb_hcd *hcd, struct usb_device *udev,
		struct usb_host_endpoint *ep);
void xhci_mtk_drop_ep_quirk(struct usb_hcd *hcd, struct usb_device *udev,
		struct usb_host_endpoint *ep);
extern void mtk_xhci_wakelock_lock(void);
extern void mtk_xhci_wakelock_unlock(void);
extern void enable_ipsleep_wakeup(void);
#else
static inline int xhci_mtk_add_ep_quirk(struct usb_hcd *hcd,
	struct usb_device *udev, struct usb_host_endpoint *ep)
{
	return 0;
}

static inline void xhci_mtk_drop_ep_quirk(struct usb_hcd *hcd,
	struct usb_device *udev, struct usb_host_endpoint *ep)
{
}

#if defined(CONFIG_USB_XHCI_MTK)
int xhci_mtk_register_plat(void);
void xhci_mtk_unregister_plat(void);
#endif

#endif

#ifdef CONFIG_MTK_UAC_POWER_SAVING
enum xhci_mtk_sram_id {
	/* memmory interfrace */
	XHCI_EVENTRING = 0,
	XHCI_EPTX,
	XHCI_EPRX,
	XHCI_DCBAA,
	XHCI_ERST,
	XHCI_SRAM_BLOCK_NUM
};
enum xhci_mtk_sram_state {
	STATE_UNINIT = 0,
	STATE_INIT,
	STATE_USE,
	STATE_NOMEM,
};

struct xhci_mtk_sram_block {
	dma_addr_t msram_phys_addr;
	void *msram_virt_addr;
	unsigned int mlength;
	enum xhci_mtk_sram_state state;
};
enum usb_data_id {
	USB_AUDIO_DATA_OUT_EP = 0,
	USB_AUDIO_DATA_IN_EP,
	USB_AUDIO_DATA_SYNC_EP,
	USB_AUDIO_DATA_BLOCK_NUM,
};

extern int mtk_audio_request_sram(dma_addr_t *phys_addr, unsigned char **virt_addr,
	unsigned int length, void *user);
extern void mtk_audio_free_sram(void *user);
extern int xhci_mtk_init_sram(struct xhci_hcd *xhci);
extern int xhci_mtk_deinit_sram(struct xhci_hcd *xhci);
extern int xhci_mtk_allocate_sram(int id, dma_addr_t *sram_phys_addr,
	unsigned char **msram_virt_addr);
extern int xhci_mtk_free_sram(int id);
extern void *mtk_usb_alloc_sram(int id, size_t size, dma_addr_t *dma);
extern void mtk_usb_free_sram(int id);
extern void xhci_mtk_allow_sleep(unsigned int sleep_ms);
#endif

#endif		/* _XHCI_MTK_H_ */
