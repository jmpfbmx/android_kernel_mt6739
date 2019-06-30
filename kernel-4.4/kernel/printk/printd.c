/*
 * @Aim: add printd module to implement SMART_LOG, 
 * which will fetch the kernel log and write it to emmc
 * @
 */


#include <linux/mutex.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/core.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/sdio.h>
#include <linux/dma-mapping.h>
#include <linux/spinlock.h>
//#include <mt-plat/partition.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/printd.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>
#include <linux/smart_info.h>
#include <linux/printd.h>
// private defines
#define PRINTD_WQ "ptintd_wq"
#define _printk(fmt, args...) printk(fmt, ##args)
#define TRACE_REGION "/dev/block/platform/bootdevice/by-name/smart"
#define KERNEL_LOG_BEGIN_OFFSET     KERNEL_LOG_OFFSET_ADDR
#define KERNEL_LOG_END_OFFSET       FEC_DATA_OFFSET_ADDR
#define LOG_BUF_LEN_1 0X200000
#define LOG_BUF_LEN_2 0x200000

#define BOOT_UNCOMPLETED_MARK 0x11111111  // when boot uncompleted, then set this flag, next boot log will be recorded
#define AUTO_CATCH_UNCOMPLETED_BOOT_LOG // when boot uncompleted, next boot log will be recorded

// enums
enum
{
	LOG_STATE_DISABLE,
	LOG_STATE_WORKING,
};

enum
{
	LOG_BUF_1,
	LOG_BUF_2,
};

// functions declare
//EXPORT_SYMBOL(printd_dump);
//EXPORT_SYMBOL(stop_printd);

// variables
static struct workqueue_struct *print_wq;
static struct delayed_work print_work;
static int  curr_log_state = LOG_STATE_WORKING; 
static u64 curr_log_addr = KERNEL_LOG_BEGIN_OFFSET;
static u64 log_end_addr = KERNEL_LOG_END_OFFSET;
static char log_buf_1[LOG_BUF_LEN_1];
static char log_buf_2[LOG_BUF_LEN_2];
static uint log_buf_1_pos = 0;
static uint log_buf_2_pos = 0;
static uint curr_buf = LOG_BUF_1;
static DEFINE_SPINLOCK(printd_lock);
static unsigned long printd_flag;
static int after_sale_flag = 0;

#ifdef AUTO_CATCH_UNCOMPLETED_BOOT_LOG
static struct workqueue_struct *clear_wq;
static struct delayed_work clear_work;
#define CLEAR_WQ "clear_wq"
#endif

/*
 * function: when boot completed, clear the BOOT_UNCOMPLETED_MARK if the flag exits.
 * @NOTE:
 */
#ifdef AUTO_CATCH_UNCOMPLETED_BOOT_LOG
static void clear_boot_uncompleted_mark(struct work_struct *work)
{
	int fd;
	int flag;
	mm_segment_t oldfs;
	// if boot completed, then clear the BOOT_UNCOMPLETED_MARK
	oldfs = get_fs();
	set_fs(KERNEL_DS);

	fd = sys_open(TRACE_REGION, O_RDWR, 0);
	if (fd >= 0) {
		sys_lseek(fd, 0, 0);
		sys_read(fd, (char *)&flag, 4);
		if (flag == BOOT_UNCOMPLETED_MARK) {
			// clear the BOOT_UNCOMPLETED_MARK
			printk("[SMART_LOG] now clear the BOOT_UNCOMPLETED_MARK!");
			flag = 0;
			sys_lseek(fd, 0, 0);
			sys_write(fd, (char *)&flag, 4);
		}
		else {
			printk("[SMART_LOG] not detect the BOOT_UNCOMPLETED_MARK!");
		}
		sys_close(fd);
	} else {
		printk("[SMART_LOG] can not open %s file fd:%d\n", TRACE_REGION, fd);
	}

	set_fs(oldfs);
}
#endif


/*
 * function: buffer log
 * @NOTE: double buffer, if one is being flushed to emmc, then buffer log to another
 */
void printd_dump (const char *buf, int len)
{
	spin_lock_irqsave(&printd_lock, printd_flag);

	if (curr_log_state != LOG_STATE_DISABLE) {
		if (curr_buf == LOG_BUF_1) {
			if (log_buf_1_pos + len <= LOG_BUF_LEN_1) {
				memcpy(log_buf_1 + log_buf_1_pos, buf, len);
				log_buf_1_pos += len;
			}
		} else if (curr_buf == LOG_BUF_2) {
			if (log_buf_2_pos + len <= LOG_BUF_LEN_2) {
				memcpy(log_buf_2 + log_buf_2_pos, buf, len);
				log_buf_2_pos += len;
			}
		}
	}

	spin_unlock_irqrestore(&printd_lock, printd_flag);
}
EXPORT_SYMBOL(printd_dump);

/*
 * function: write log to emmc
 * @Param:
 *      addr: offset address to the TRACE_REGION partition
 *      buf: pointer to the log buf
 *      len: length of log to write
 * @return:
 */
void log_write(u64 addr, char *buf, uint len)
{
	int fd;
	long written;
	mm_segment_t oldfs;

	oldfs = get_fs();
	set_fs(KERNEL_DS);

	fd = sys_open(TRACE_REGION, O_RDWR, 0);
	if (fd >= 0) {
		sys_lseek(fd, addr, 0);

		written = sys_write(fd, buf, len);
		if (written != len) {
			//printk("[SMART_LOG]log_write at addr %lld error, write %d bytes, only %ld bytes written!\n", addr, len, written);
		} else {
			//printk("[SMART_LOG]log_write at addr %lld successfully, write %d bytes, only %ld bytes written!\n", addr, len, written);
		}
        
		sys_close(fd);
	} else {
		printk("[SMART_LOG]log_write can not open %s file fd:%d\n", TRACE_REGION, fd);
	}

	set_fs(oldfs);
}

/*
 * @function:
 *     do real writting log to the smart partion
 * @Param:
 */
static void printd_func(struct work_struct *work)
{
	u64 next_addr = 0;
	bool is_restart = true;
	static bool system_is_ok = false;

	// check if "/dev/block/platform/bootdevice/by-name/smart" is ready
	if (!system_is_ok) {
		int fd;
		mm_segment_t oldfs;
		oldfs = get_fs();
		set_fs(KERNEL_DS);
		fd = sys_open(TRACE_REGION, O_RDWR, 0);
		if (fd < 0) {
			printk("[SMART_LOG] smart directory is not ready!\n");
			queue_delayed_work(print_wq, &print_work, 200);
			set_fs(oldfs);
			return;
		} else {
			printk("[SMART_LOG] smart directory ready now!\n");
			system_is_ok = true;
			sys_close(fd);
			set_fs(oldfs);
		}
	}

	// We don't need to check curr_log_state. We only need to check log_buf_1_pos and log_buf_2_pos.
	// This helps the log system flush the buffer to emmc when we call stop_printd() function.
	spin_lock_irqsave(&printd_lock, printd_flag);
	if (curr_buf == LOG_BUF_1) {
		if (log_buf_1_pos > 0) {
		curr_buf = LOG_BUF_2;
		log_buf_2_pos = 0;
		spin_unlock_irqrestore(&printd_lock, printd_flag);
		next_addr = curr_log_addr + log_buf_1_pos;
		if (next_addr > log_end_addr) {
			log_buf_1_pos = log_end_addr - curr_log_addr;
			curr_log_state = LOG_STATE_DISABLE;
			is_restart = false;
		} else if (next_addr == log_end_addr) {
			curr_log_state = LOG_STATE_DISABLE;
			is_restart = false;
		}
		log_write(curr_log_addr, log_buf_1, log_buf_1_pos);
		curr_log_addr = next_addr;                
		} else {
			spin_unlock_irqrestore(&printd_lock, printd_flag);
		}
	} else if (curr_buf == LOG_BUF_2) {
		if (log_buf_2_pos > 0) {
			curr_buf = LOG_BUF_1;
			log_buf_1_pos = 0;
			spin_unlock_irqrestore(&printd_lock, printd_flag);
			next_addr = curr_log_addr + log_buf_2_pos;
			if (next_addr > log_end_addr) {
				log_buf_2_pos = log_end_addr - curr_log_addr;
				curr_log_state = LOG_STATE_DISABLE;
				is_restart = false;
			} else if (next_addr == log_end_addr) {
				curr_log_state = LOG_STATE_DISABLE;
				is_restart = false;
			}
			log_write(curr_log_addr, log_buf_2, log_buf_2_pos);
			curr_log_addr = next_addr;             
		} else {
			spin_unlock_irqrestore(&printd_lock, printd_flag);
		}
	} else {
		spin_unlock_irqrestore(&printd_lock, printd_flag);
	}

	if (is_restart) {
		queue_delayed_work(print_wq, &print_work, 100);        
	}
}


/*
 * function: check command_line whether after sale mode or not
 */
static bool check_after_sale_mode(void)
{
	char *cmd = saved_command_line;
	char *p;
	p = strstr(cmd, "androidboot.after_sale_mode=");
	if (p == NULL) {
		// not found
		return false;
	}
	p += strlen("androidboot.after_sale_mode=");
	if (*p == '1') {
		return true;
	}
	else {
		return false;
	}
}


/*
 * stop writing log to emmc, called when boot up end.
 */
void stop_printd(void)
{
#ifdef AUTO_CATCH_UNCOMPLETED_BOOT_LOG
	printk("[SMART_LOG] now ready to clear BOOT_UNCOMPLETED_MARK!");
	queue_delayed_work(clear_wq, &clear_work, 10);
#endif

	_printk("[SMART_LOG] logger stop: %s\n", __func__);

	// disable smart log output
	curr_log_state = LOG_STATE_DISABLE;
}
EXPORT_SYMBOL(stop_printd);

/*
 * printd module init function
 */
static int __init printd_init(void)
{
	int ret = 0;

#ifdef AUTO_CATCH_UNCOMPLETED_BOOT_LOG
	printk("[SMART_LOG] init clear_boot_uncompleted_mark wq!!");
	clear_wq = create_singlethread_workqueue(CLEAR_WQ);
	if (!clear_wq)
		return -ENOMEM;
	INIT_DELAYED_WORK(&clear_work, clear_boot_uncompleted_mark);
#endif

	// now check after_sale_mode, if not, then return directly.
	if (!check_after_sale_mode()) {
		_printk("[SMART_LOG] not after sale mode!\n");
		after_sale_flag = 0;
	}
	else {
		after_sale_flag = 1;
	}

	_printk("[SMART_LOG] logger start: %s\n", __func__);

	if (after_sale_flag) {
		printk("[SMART_LOG] create print work!\n");
		print_wq = create_singlethread_workqueue(PRINTD_WQ);
		if (!print_wq) {
			return -ENOMEM;
		}

		INIT_DELAYED_WORK(&print_work, printd_func);
		queue_delayed_work(print_wq, &print_work, 200);
	}

	return ret;
}

/*
 * printd module exit function
 */
static void __exit printd_exit(void)
{
	_printk("[SMART_LOG] logger stop: %s\n", __func__);
	if (print_wq) {
		destroy_workqueue(print_wq);
	}
}

// module register
module_init(printd_init);
module_exit(printd_exit);
MODULE_LICENSE("GPL");

