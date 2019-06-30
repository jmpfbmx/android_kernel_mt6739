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

#include "convert_file_op.h"

#define EOF -1

int katoi(char *str)
{
    int res = 0, i; // Initialize result
  
    // Iterate through all characters of input string and
    // update result
    for (i = 0; str[i] != '\0'; ++i)
    {
        // adding a condition for space
        if(str[i] >=0x30 && str[i]<=0x39)
            res = res*10 + str[i] - '0';
    }       
  
    // return result.
    return res;
}

char * kstrtok(char * s, const char * delim, char ** lasts)
{
    const char * spanp;
    int c, sc;
    char * tok = NULL;

    /* s may be NULL */
    if (!(delim && lasts))
        return NULL;

    if (s == NULL && (s = *lasts) == NULL)
        return NULL;

    /*
     * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
cont:
    c = (int)(*s++);
    for (spanp = delim; (sc = *spanp++) != 0;) {
        if (c == sc)
            goto cont;
    }

    if (c == 0) {       /* no non-delimiter characters */
        *lasts = NULL;
        return NULL;
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for (;;) {
        c = (int)(*s++);
        spanp = delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *lasts = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}

int kfgetc(struct file *f)
{
    char c = 0;
//    ssize_t t;
    mm_segment_t oldfs;

    oldfs = get_fs();
    set_fs(get_ds());

    if (f->f_op && f->f_op->read) 
        f->f_op->read(f, &c, 1, &f->f_pos); 
    else 
        c = EOF;

    set_fs(oldfs);
    return c;
}

char * kgets(char *dst, int max, struct file *fp)
{
	int c = 0;
	char *p = NULL;

	/* get max bytes or upto a newline */
	for (p = dst, max--; max > 0; max--) {
		if ((c = kfgetc (fp)) == EOF)
			break;
		*p++ = c;
		if (c == '\n')
			break;
	}
	*p = 0;
	if (p == dst || c == EOF)
		return NULL;

	return (p);
}

struct file *file_open(const char *path, int flags, int rights, int *size)
{
    struct file *filp = NULL;
    mm_segment_t oldfs;
    struct inode *inode;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if (IS_ERR(filp))
    {
        err = PTR_ERR(filp);
        return NULL;
    }
#if LINUX_VERSION_CODE <= KERNEL_VERSION(3, 18, 0)
		inode = filp->f_dentry->d_inode;
#else
		inode = filp->f_path.dentry->d_inode;
#endif
        if(size != NULL)
		    *size = inode->i_size;

        //DBG(0, "%s: fsize = %d \n", __func__, size);

    return filp;
}

void file_close(struct file *file) 
{
    filp_close(file, NULL);
}

int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    DBG(0, "%s: size = %d \n", __func__, size);

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

int file_read(struct file *file, unsigned long long *offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;
    loff_t pos = 0;
       
    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, data, size, &pos);

    set_fs(oldfs);
    return ret;
}

int file_sync(struct file *file) 
{
    vfs_fsync(file, 0);
    return 0;
}

// int fprintf(struct file *file, const char *fmt, ...)
// {
//     va_list args;
//     int length;
//     char *buff = NULL;

//     // formatting strings to buffer .
//     va_start(args, fmt);
//     length = vsnprintf(buff, INT_MAX, fmt, args);
//     va_end(args);

//     printk("size of buff = %d", strlen(buff));

//     // writting the formatted buffer to the file.
//     file_write(file, 0, buff, strlen(buff));

//     return length;

// }

// double atof(const char *s)
// {
//     int i = 0, d = 0;
//     double res = 0, fact = 1;
//     //double base = 10.0f;

//     if(*s == '-')
//     {
//         s++;
//       //  fact = -1;
//     }

//     for(i = 0; *s; s++)
//     {
//         if(*s == '.')
//         {
//             i = 1;
//             continue;
//         }

//         d = *s - '0';
//         if(d >= 0 && d <= 9)
//         {
//             if(i)
//             {
//              //   fact = fact / base;
//             }
//             //res = res * base + (double)d;
//         }
//     }
 
//     return res * fact;
// }
