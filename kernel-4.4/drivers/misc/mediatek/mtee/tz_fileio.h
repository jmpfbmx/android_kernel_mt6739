
#ifndef DRIVER_FILE_PR_IO_H
#define DRIVER_FILE_PR_IO_H
#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/uaccess.h>
#include <linux/buffer_head.h>
#define UINT64 unsigned long long
#define INT64 long long
struct file *FILE_Open(const char *path, int flags, int mode);

int FILE_Read(struct file *file, unsigned char *data, unsigned int size,
		unsigned long long *offset);

int FILE_Write(struct file *file, unsigned char *data, unsigned int size,
		unsigned long long *offset);

int FILE_ReadData(const char *path, unsigned int u4Offset, char *pData,
			int i4Length);


int FILE_WriteData(const char *path, unsigned int u4Offset, char *pData,
			int i4Length);
#endif
