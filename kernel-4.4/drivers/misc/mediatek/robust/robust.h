


#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/platform_device.h>
#include <asm/atomic.h>
#include <asm/io.h>
#include <linux/sched.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/types.h>

/*----------------------------------------------------------------------------*/

#define ROBUST_DEV_NAME     "ROBUST"
/*----------------------------------------------------------------------------*/
#define ROBUST_TAG          "ROBUST"
#define ROBUST_FUN          pr_err(ROBUST_TAG "%s\n", __func__)

typedef struct robust_node_info{
	char*   dir;
	char*   node;
	ssize_t (*show)(struct device *, struct device_attribute *, char *);
	ssize_t (*store)(struct device*, struct device_attribute *, const char *, size_t);
}robust_node_info;

/********************************************/
 
