



#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/uaccess.h>
#include <linux/buffer_head.h>
#include "tz_fileio.h"

struct file *FILE_Open(const char *path, int flags, int mode)
{
	struct file *filp = NULL;
	mm_segment_t oldfs;
	int err = 0;

	oldfs = get_fs();
	set_fs(get_ds());
	filp = filp_open(path, flags, mode);
	set_fs(oldfs);
	if (IS_ERR(filp)) {
		err = PTR_ERR(filp);
		return NULL;
	}
	return filp;
}

int FILE_Read(struct file *file, unsigned char *data, unsigned int size,
		unsigned long long *offset)
{
	mm_segment_t oldfs;
	int ret;

	oldfs = get_fs();
	set_fs(get_ds());

	ret = vfs_read(file, data, size, offset);

	set_fs(oldfs);
	return ret;
}

int FILE_Write(struct file *file, unsigned char *data, unsigned int size,
		unsigned long long *offset)
{
	mm_segment_t oldfs;
	int ret;

	oldfs = get_fs();
	set_fs(get_ds());

	ret = vfs_write(file, data, size, offset);

	set_fs(oldfs);
	return ret;
}

int FILE_ReadData(const char *path, unsigned int u4Offset, char *pData,
			int i4Length)
{
	struct file *file = NULL;
	UINT64 u8Offset = (UINT64)u4Offset;

	file = FILE_Open(path, O_RDONLY, 0);
	if (!file)
		return -EFAULT;

	i4Length = FILE_Read(file, (void *)pData, i4Length, &u8Offset);
	filp_close(file, NULL);
	return i4Length;
}


int FILE_WriteData(const char *path, unsigned int u4Offset, char *pData,
			int i4Length)
{
	struct file *file = NULL;
	UINT64 u8Offset = (UINT64)u4Offset;

	file = FILE_Open(path, O_WRONLY|O_CREAT, 0644);
	if (!file)
		return -EFAULT;

	i4Length = FILE_Write(file, (void *)pData, i4Length, &u8Offset);
	filp_close(file, NULL);
	return i4Length;
}

