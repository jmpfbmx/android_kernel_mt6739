
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <uapi/asm-generic/fcntl.h>
#include <linux/err.h>
#include "ipanic.h"


struct file *expdb_open(void)
{
	static struct file *filp_expdb;

	/*we remove filp open here*/
	if (IS_ERR(filp_expdb))
		LOGD("open(%s) for aee failed (%ld)\n", AEE_EXPDB_PATH, PTR_ERR(filp_expdb));
	return filp_expdb;
}

ssize_t expdb_write(struct file *filp, const char *buf, size_t len, loff_t off)
{
	return kernel_write(filp, buf, len, off);
}

ssize_t expdb_read(struct file *filp, char *buf, size_t len, loff_t off)
{
	return kernel_read(filp, off, buf, len);
}

char *expdb_read_size(int off, int len)
{
	return NULL;
}
