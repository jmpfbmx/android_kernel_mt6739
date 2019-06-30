/*
 * @Aim: add smart module to write some information to the smart partion
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
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/smart_info.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>


// private defines
#define TRACE_REGION "/dev/block/platform/bootdevice/by-name/smart"
#define SMART_INFO_WQ "smart_info_wq"
#define LAUCHER_TIME_WRITE_WQ "laucher_time_write_wq"
#define LAUCHER_TIME_WRITE_DELAY_TIME     30000  // 300s, 5min


// private types
struct write_request {
	u64 addr;
	char *buf;
	size_t size;
};

struct my_request_list {
	struct list_head list;
	struct write_request request;
};


// variables
static struct workqueue_struct *smart_info_wq;
static struct delayed_work smart_info_work;
static struct workqueue_struct *laucher_time_write_wq;
static struct delayed_work laucher_time_write_work;
static struct big_data_struct big_data;
static struct tool_data_struct tool_data;
static struct list_head head;
static DEFINE_SPINLOCK(request_lock);
static char* boot_mode = NULL;


// functions
static void laucher_time_write_func(struct work_struct *work);
static void log_write(u64 addr, char *buf, uint len);
static void smart_info_func(struct work_struct *work);
static void add_to_write_request_list(struct write_request* request);
static void lk_laucher_time_write(u8* laucher_time);

/*
 * @function:
 *     do real writting laucher time to the smart partion
 * @Param:
 */
static void laucher_time_write_func(struct work_struct *work)
{
	struct timex txc;
	struct rtc_time tm;
	u8 laucher_time[20] = {0};

	do_gettimeofday(&(txc.time));
	rtc_time_to_tm(txc.time.tv_sec,&tm);
	printk("[BIG_LOG]UTC time :%04d-%02d-%02d %02d:%02d:%02d\n",tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	sprintf(laucher_time, "%04d-%02d-%02d %02d:%02d:%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	lk_laucher_time_write(laucher_time);
}

/*
 * function: write log to emmc
 * @Param:
 *      addr: offset address to the TRACE_REGION partition
 *      buf: pointer to the log buf
 *      len: length of log to write
 * @return:
 */
static void log_write(u64 addr, char *buf, uint len)
{
	int fd;
	long written;
	mm_segment_t oldfs;

	oldfs = get_fs();
	set_fs(KERNEL_DS);

	fd = sys_open(TRACE_REGION, O_RDWR, 0);
	if(fd >= 0) {
		sys_lseek(fd, addr, 0);

		written = sys_write(fd, buf, len);
		if (written != len) {
			//printk("[BIG_LOG]log_write at addr %lld error, write %d bytes, only %ld bytes written!\n", addr, len, written);
		} else {
			//printk("[BIG_LOG]log_write at addr %lld successfully, write %d bytes, only %ld bytes written!\n", addr, len, written);
		}

		sys_close(fd);
	} else {
		printk("[BIG_LOG]log_write can not open %s file\n", TRACE_REGION);
	}

	set_fs(oldfs);
}

/*
 * @function:
 *     do real writting log to the smart partion
 * @Param:
 */
static void smart_info_func(struct work_struct *work)
{
	struct list_head *p;
	struct my_request_list *entry;
	struct my_request_list re;
	static bool system_is_ok = false;

	// check if "/dev/block/platform/bootdevice/by-name/smart" is ready
	if (!system_is_ok) {
		int fd;
		mm_segment_t oldfs;
		oldfs = get_fs();
		set_fs(KERNEL_DS);
		fd = sys_open(TRACE_REGION, O_RDWR, 0);
		if (fd < 0) {
			printk("[BIG_LOG] smart directory is not ready!\n");
			queue_delayed_work(smart_info_wq, &smart_info_work, 200);
			set_fs(oldfs);
			return;
		} else {
			printk("[BIG_LOG] smart directory ready now!\n");
			system_is_ok = true;
			sys_close(fd);
			set_fs(oldfs);
		}
	}

	while(1) {
		spin_lock(&request_lock);
		if (!list_empty(&head)) {
			p = head.next;
			entry = list_entry(p, struct my_request_list, list);
			if (p == NULL || entry == NULL) {
				printk("[BIG_LOG] p == null or entry == null\n");
 			}
			memcpy(&re, entry, sizeof(struct my_request_list));
			list_del(p);
			kfree(entry);
			spin_unlock(&request_lock);
			log_write(re.request.addr, re.request.buf, re.request.size);
		}
		else {
			spin_unlock(&request_lock);
			break;
		}
	}


	queue_delayed_work(smart_info_wq, &smart_info_work, 100);

}


/*
 * get boot mode from device tree
 */
static int __init dt_get_boot_mode(unsigned long node, const char *uname, int depth, void *data)
{
	if (!of_flat_dt_is_compatible(node, "android,firmware"))
		return 0;
	boot_mode = (char*)of_get_flat_dt_prop(node, "mode", NULL);
	printk("[BIG_LOG] boot mode found: %s\n", boot_mode);
	return 1;
}

/*
 * printd module init function
 */
static int __init smart_info_init(void)
{
	int ret = 0;

	// init write request list
	INIT_LIST_HEAD(&head);

	// get boot mode
	of_scan_flat_dt(dt_get_boot_mode, NULL);

	smart_info_wq = create_singlethread_workqueue(SMART_INFO_WQ);
	if (!smart_info_wq) {
		return -ENOMEM;
	}

	INIT_DELAYED_WORK(&smart_info_work, smart_info_func);
	queue_delayed_work(smart_info_wq, &smart_info_work, 200);

	return ret;
}


/*
 * printd module exit function
 */
static void __exit smart_info_exit(void)
{
	printk("[BIG_LOG] logger stop: %s\n", __func__);
	if (smart_info_wq) {
		destroy_workqueue(smart_info_wq);
	}
}

/*
 * @function:
 *     add a write request to the list, and the system will write it to the smart partion at a proper time
 *
 */
static void add_to_write_request_list(struct write_request* request)
{
	struct my_request_list *p_list = (struct my_request_list*)kmalloc(sizeof(struct my_request_list), GFP_KERNEL);

	if (!p_list) {
		printk("[BIG_LOG] kmalloc fail!\n");
		return;
	}

	p_list->request.addr = request->addr;
	p_list->request.buf = request->buf;
	p_list->request.size = request->size;
	spin_lock(&request_lock);
	list_add_tail(&(p_list->list), &head);
	spin_unlock(&request_lock);
	//test
	//printk("[BIG_LOG] add to list ok!\n");
}

/*
 * @function:
 *     The smart part records the lk start time and laucher start time during the last 20 boot-ups.
 *     This function updates the latest one, and certainly the oldest one will be replaced.
 * @Note:
 *     this function doesn't write the string to the smart part directly, it will
 *     buffer the string, and then write it at a proper time.
 * @Param:
 *     laucher_time: laucher start time, a string like "2017-05-23 10:00:00"
 */
static void lk_laucher_time_write(u8* laucher_time)
{
	struct write_request request;
	if (laucher_time) {
		size_t size = min((size_t)20, strlen(laucher_time));
		memcpy(big_data.lk_laucher_time[0].laucher_start_time, laucher_time, size);
		request.addr = LK_LAUCHER_TIME_RECORD_ADDR+20;
		request.buf = big_data.lk_laucher_time[0].laucher_start_time;
		request.size = 20;

		add_to_write_request_list(&request);
	}
}

/*
 * @function:
 *     when it's detected that somebody is trying to write something to the system part,
 *     record the information to the smart part.
 * @Note:
 *     this function doesn't write the string to the smart part directly, it will
 *     buffer the string, and then write it at a proper time.
 * @Param:
 *     source: pointer to the string which will be written to the smart part.
 *     size: the length of the string, shouldn't be larger than 128
 */
void system_abnormal_write(u8 *source, size_t size)
{
	struct write_request request;
	size_t write_size = min(size, (size_t)128);

	// save buf
	memcpy(big_data.system_abnormal_write_info.warn_info, source, write_size);

	request.addr = SYSTEM_ABNORMAL_WRITE_ADDR;
	request.buf = big_data.system_abnormal_write_info.warn_info;
	request.size = 128;

	add_to_write_request_list(&request);

	printk("[BIG_LOG]system abnormal write end!\n");
}

/*
 * @function:
 *     record the time when fec decoding, fec failed or fec reparing happens, only
 *     saves the latest time
 * @Note:
 *     this function doesn't write the string to the smart part directly, it will
 *     buffer the string, and then write it at a proper time.
 * @Param:
 *     decode_time: a string like "2017-05-23 10:00:00", set it to NULL if you don't want to update it.
 *     failed_time: a string like "2017-05-23 10:00:00", set it to NULL if you don't want to update it.
 *     repair_time: a string like "2017-05-23 10:00:00", set it to NULL if you don't want to update it.
 */
void fec_time_write(u8* decode_time, u8* failed_time, u8* repair_time)
{
	struct write_request request;
	if (decode_time) {
		size_t size = min((size_t)20, strlen(decode_time));
		memcpy(big_data.fec_record.fec_decode_time, decode_time, size);
		request.addr = FEC_TIME_RECORD_ADDR;
		request.buf = big_data.fec_record.fec_decode_time;
		request.size = 20;

		add_to_write_request_list(&request);
	}
	if (failed_time) {
		size_t size = min((size_t)20, strlen(failed_time));
		memcpy(big_data.fec_record.fec_failed_time, failed_time, size);
		request.addr = FEC_TIME_RECORD_ADDR+20;
		request.buf = big_data.fec_record.fec_failed_time;
		request.size = 20;

		add_to_write_request_list(&request);
	}
	if (repair_time) {
		size_t size = min((size_t)20, strlen(repair_time));
		memcpy(big_data.fec_record.fec_repair_time, repair_time, size);
		request.addr = FEC_TIME_RECORD_ADDR+40;
		request.buf = big_data.fec_record.fec_repair_time;
		request.size = 20;

		add_to_write_request_list(&request);
	}
}


/*
 * delay some time to write laucher time to the smart partion
 */
void write_laucher_time(void)
{
	// now delay some time to do write laucher time work
	// write laucher time only in normal boot mode
	if (strcmp(boot_mode, "normal") == 0) {
		printk("[BIG_LOG] normal boot!\n");

		laucher_time_write_wq = create_singlethread_workqueue(LAUCHER_TIME_WRITE_WQ);
		if (!laucher_time_write_wq) {
			printk("[BIG_LOG] create write wq error!\n");
			return;
		}

		INIT_DELAYED_WORK(&laucher_time_write_work, laucher_time_write_func);
		queue_delayed_work(laucher_time_write_wq, &laucher_time_write_work, LAUCHER_TIME_WRITE_DELAY_TIME);
	} else {
 		printk("[BIG_LOG] not normal boot!\n");
	}
}

/*
 * record life_time to the smart partition
 */
void write_life_time(int life_time)
{
	struct write_request request;
	tool_data.life_time_info.emmc_life_time = life_time;
	request.addr = LIFE_TIME_INFO_ADDR;
	request.buf = (char *)&(tool_data.life_time_info.emmc_life_time);
	request.size = 4;

	add_to_write_request_list(&request);
	//printk("[BIG_LOG] write life time!\n");
}

// module register
module_init(smart_info_init);
module_exit(smart_info_exit);
MODULE_LICENSE("GPL");


