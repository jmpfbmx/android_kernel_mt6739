
static int __init gz_init(void);
static void __exit gz_exit(void);

static int gz_dev_open(struct inode *inode, struct file *filp);
static int gz_dev_release(struct inode *inode, struct file *filp);
static ssize_t gz_dev_read(struct file *filp, char __user *buf, size_t count, loff_t *pos);
static ssize_t gz_dev_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos);

static long gz_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);
static long gz_compat_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);


