
#ifndef __MSSUSB_LINUX_SYSFS_H__
#define __MSSUSB_LINUX_SYSFS_H__


#ifdef CONFIG_SYSFS
int ssusb_sysfs_init(struct musb *musb);
void ssusb_sysfs_exit(struct musb *musb);
#else
static inline int ssusb_sysfs_init(struct musb *musb)
{
	return 0;
}

static inline void ssusb_sysfs_exit(struct musb *musb)
{
}
#endif

#endif				/*  __MUSB_LINUX_DEBUG_H__ */
