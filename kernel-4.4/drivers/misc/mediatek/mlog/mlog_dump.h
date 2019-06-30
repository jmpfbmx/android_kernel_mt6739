
#ifndef _MLOG_DUMP_H
#define _MLOG_DUMP_H

extern void mlog_init_procfs(void);
extern int mlog_print_fmt(struct seq_file *m);
extern void mlog_doopen(void);
extern int mlog_doread(char __user *buf, size_t len);
extern int mlog_unread(void);

extern int dmlog_open(struct inode *inode, struct file *file);
extern int dmlog_release(struct inode *inode, struct file *file);
extern ssize_t dmlog_read(struct file *file, char __user *buf, size_t len, loff_t *ppos);

extern wait_queue_head_t mlog_wait;

#endif
