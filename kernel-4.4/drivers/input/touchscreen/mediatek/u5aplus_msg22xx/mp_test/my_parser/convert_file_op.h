/*
 * Copyright (C) 2006-2017 ILITEK TECHNOLOGY CORP.
 *
 * Description: ILITEK I2C touchscreen driver for linux platform.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Dicky Chiang
 * Maintain: Luca Hsu, Tigers Huang
 */
 
#ifndef __CONVERT_FILE_OP_H
#define __CONVERT_FILE_OP_H

#include "../MpCommon.h"

extern int fprintf(struct file *file, const char *fmt, ...);
extern char * kgets(char *dst, int max, struct file *fp);
extern int katoi(char *str);

extern int file_sync(struct file *file);
extern int file_read(struct file *file, unsigned long long *offset, unsigned char *data, unsigned int size);
extern int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size);
extern struct file *file_open(const char *path, int flags, int rights, int *size);
extern void file_close(struct file *file);

#endif