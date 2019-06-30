#include "linux/kernel.h"  
#include "linux/module.h"  
#include "linux/fs.h"  
#include "linux/init.h"  
#include "linux/types.h"  
#include "linux/errno.h"  
#include "linux/uaccess.h"  
#include "linux/kdev_t.h"  
#include <linux/device.h>
#include <linux/time.h>
#include <linux/limits.h>
#include <linux/slab.h>
#include <linux/sched.h>



static long file_lock_ioctl(struct file *fp, unsigned int cmd, unsigned long arg);
unsigned long gen_random_data(void);
unsigned long calc_magic_data(unsigned long arg);
void register_pthead_to_connect(pid_t tid);
int check_current_pthread_permission(pid_t tid);
pid_t get_current_tid(void);


static int device_num = 0;
static char* devName = "file_lock";
static struct class *file_lock_class;
static struct device *file_lock_dev;

//static unsigned long random_data;
//static pid_t permi_tid = -1;

#define FILE_LOCK_CMD_BASE              100
#define FILE_LOCK_CMD_GET_RANDOM        FILE_LOCK_CMD_BASE+1
#define FILE_LOCK_CMD_REGISTER_PID      FILE_LOCK_CMD_BASE+2
#define FILE_LOCK_CMD_HIDE_LOCK         FILE_LOCK_CMD_BASE+3
#define FILE_LOCK_CMD_FILE_LOCK         FILE_LOCK_CMD_BASE+4
#define FILE_LOCK_CMD_CHECK_STATE       FILE_LOCK_CMD_BASE+5

#define FILE_LOCK_PATH_START_POS         7
// PATH_MAX & NAME_MAX are defined by  linux/limits.h
#define FILE_LOCK_MAX_SOCKET_BUF_LEN				PATH_MAX+NAME_MAX+FILE_LOCK_PATH_START_POS  

static DEFINE_MUTEX(file_lock_mutex);

static unsigned long long code_buf[8] = {
    0x12d5d74e22c15f98, 
    0x4676a1b4dd9878ef,
    0x45631dbc5ee13756,
    0x14561ce156431abd,
    0xfedc146913213411,
    0x31256746bf4ac953,
    0xbfdd14613136a1ce,
    0x16431645646df1bd,
};

struct register_pthead_info {
    pid_t tid;
    unsigned long magic_data;
};

struct file_lock_control {
    char filename[PATH_MAX+NAME_MAX];
    unsigned char flag;
};

struct wil_file_lock_operations {
    int (*lock) (unsigned char, const char __user *);
    int (*hide) (int);
    int (*get_state) (unsigned char __user * , const char __user *);
    void (*register_pthread) (pid_t);
    int (*check_permission) (pid_t);
    unsigned long (*gen_random_data) (void);
    unsigned long (*calc_magic_data) (unsigned long);
    int (*show_hide_status) (void);

    unsigned long random_data;
    pid_t perm_tid;
};

static struct wil_file_lock_operations fl_ops = {
    .lock = file_lock_switch,
    .hide = lock_hide_switch,
    .get_state = get_file_state,
    .register_pthread = register_pthead_to_connect,
    .check_permission = check_current_pthread_permission,
    .gen_random_data = gen_random_data,
    .calc_magic_data = calc_magic_data,
    .show_hide_status = get_lockhide_flag,
};

static const struct file_operations f_ops = {
  .unlocked_ioctl = file_lock_ioctl,
}; 

void register_pthead_to_connect(pid_t tid)
{
    fl_ops.perm_tid = tid;
}

int check_current_pthread_permission(pid_t tid)
{
    //printk(KERN_ERR "hello current pid is %d\n", tid);
    return (fl_ops.perm_tid == tid ? 0 : -EINVAL);
}

unsigned long gen_random_data()
{
    struct timespec ts;
    ts = current_kernel_time();
    return ts.tv_sec*0x13bd/0xef98;
}

unsigned long calc_magic_data(unsigned long arg) 
{
    unsigned long new_buf[8];
    unsigned long m=0;
    int i;

	//As dividend, arg cannot be zero
	if (!(arg+1))
		arg+=2;
	else
		arg+=1;

    for(i=0; i<8; i++) {
        new_buf[i] = code_buf[i] ^ arg;
        m = m + new_buf[i] / arg + new_buf[i] % arg; 
    }

    return m;
}

pid_t get_current_tid(void)
{
    return task_pid_vnr(current);
}

static ssize_t hide_status_show(struct device *dev,
                                     struct device_attribute *attr, char *buf)
{
    if (fl_ops.show_hide_status)
	    return sprintf(buf, "%d\n", fl_ops.show_hide_status());
    else
        return sprintf(buf, "%s\n", "can not show hide status");
}

static DEVICE_ATTR(hideStatus, 0444, hide_status_show, NULL);

int init_module()
{
    int ret;
    ret = register_chrdev(0, devName, &f_ops);
    if (ret < 0)
    {
        printk("regist failure!\n"); 
        return ret;
    }
    else
    {
        device_num = ret;
        ret = 0;
    }


	file_lock_class = class_create(THIS_MODULE, "file_lock");
	if (IS_ERR(file_lock_class)) {
		ret = PTR_ERR(file_lock_class);
		goto out_chrdev;
	}

	file_lock_dev = device_create(file_lock_class, NULL, MKDEV(device_num, 0), NULL,
		      "file_lock");

    if( device_create_file(file_lock_dev, &dev_attr_hideStatus ) < 0 )
		pr_err( "Failed to create device file(%s)!\n", dev_attr_hideStatus.attr.name );

    fl_ops.random_data = fl_ops.gen_random_data();

    fl_ops.perm_tid = -1;

    goto out;

out_chrdev:
	unregister_chrdev(device_num, "file_lock");
out:
	return ret; 
}

void cleanup_module() 
{
    device_remove_file(file_lock_dev, &dev_attr_hideStatus);
    device_destroy(file_lock_class, MKDEV(device_num, 0));
	class_destroy(file_lock_class);
	unregister_chrdev(device_num, devName);
    printk("unregister it success!\n");
}  

static long file_lock_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
    unsigned long k_magic_data;
    struct register_pthead_info info_data;
    struct file_lock_control __user *argc;

    int ret = 0;
    void __user *argp = (void __user *)arg;
    int hide_flag = 1;

    mutex_lock(&file_lock_mutex);
    //printk(KERN_ERR "hello start file_lock_ioctl, cmd is %d\n", cmd);

    switch(cmd) {
    case FILE_LOCK_CMD_GET_RANDOM:
        if (copy_to_user(argp, &(fl_ops.random_data), sizeof(unsigned long))) {
            mutex_unlock(&file_lock_mutex);
            return -EFAULT;
        }
        break;

    case FILE_LOCK_CMD_REGISTER_PID:
        if (copy_from_user(&info_data, argp, sizeof(struct register_pthead_info))) {
            mutex_unlock(&file_lock_mutex);
            return -EFAULT;
        }

        k_magic_data = fl_ops.calc_magic_data(fl_ops.random_data);

        if (k_magic_data == info_data.magic_data) {
            fl_ops.register_pthread(info_data.tid);
            //printk(KERN_ERR "hello magic data is right, perm_tid:%d\n", fl_ops.perm_tid);
        }
        else {
            printk(KERN_ERR "hello magic data is wrong\n");
            ret = -EINVAL;
        }
        
        fl_ops.random_data = fl_ops.gen_random_data();  
        break;

    case FILE_LOCK_CMD_HIDE_LOCK:
        if (fl_ops.check_permission(get_current_tid()) < 0) {
            printk(KERN_ERR "hello this thread is NOT allowed to control file lock\n");
            mutex_unlock(&file_lock_mutex);
            return -EINVAL;
        }

        if (copy_from_user(&hide_flag, argp, sizeof(int))) {
            mutex_unlock(&file_lock_mutex);
            return -EFAULT;
        }
        
        ret = fl_ops.hide(hide_flag);
        break;

    case FILE_LOCK_CMD_FILE_LOCK:        
        if (fl_ops.check_permission(get_current_tid()) < 0) {
            printk(KERN_ERR "hello this thread is NOT allowed to control file lock\n");
            mutex_unlock(&file_lock_mutex);
            return -EINVAL;
        }
        argc =  (struct file_lock_control __user *)arg;
        ret = fl_ops.lock(argc->flag, argc->filename);
        break;

    case FILE_LOCK_CMD_CHECK_STATE:        
        if (fl_ops.check_permission(get_current_tid()) < 0) {
            printk(KERN_ERR "hello this thread is NOT allowed to control file lock\n");
            mutex_unlock(&file_lock_mutex);
            return -EINVAL;
        }
        argc =  (struct file_lock_control __user *)arg;
        ret = fl_ops.get_state(&argc->flag, argc->filename);
        break;
        
    default:
        printk(KERN_ERR "hello can not support this cmd\n");
        mutex_unlock(&file_lock_mutex);
        return -EINVAL;
    }

    mutex_unlock(&file_lock_mutex);
    return ret;
}


module_init(init_module);
module_exit(cleanup_module);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION(" driver for file lock");
MODULE_AUTHOR("Liu Jie <jie.liu.sz@tcl.com>");


