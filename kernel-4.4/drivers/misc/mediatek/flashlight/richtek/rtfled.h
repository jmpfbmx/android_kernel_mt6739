
#ifndef LINUX_LEDS_RTFLED_H
#define LINUX_LEDS_RTFLED_H
#include "rt-flashlight.h"

struct rt_fled_dev;
struct rt_fled_hal {
int (*rt_hal_fled_init)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_suspend)
		(struct rt_fled_dev *fled_dev, pm_message_t state);
int (*rt_hal_fled_resume)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_set_mode)
		(struct rt_fled_dev *fled_dev, enum flashlight_mode mode);
int (*rt_hal_fled_get_mode)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_strobe)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_torch_current_list)
				(struct rt_fled_dev *fled_dev, int selector);
int (*rt_hal_fled_strobe_current_list)
				(struct rt_fled_dev *fled_dev, int selector);
int (*rt_hal_fled_timeout_level_list)
				(struct rt_fled_dev *fled_dev, int selector);
int (*rt_hal_fled_lv_protection_list)
			(struct rt_fled_dev *fled_dev, int selector);
int (*rt_hal_fled_strobe_timeout_list)
				(struct rt_fled_dev *fled_dev, int selector);
	/* method to set, optional */
int (*rt_hal_fled_set_torch_current)
	(struct rt_fled_dev *fled_dev, int min_uA, int max_uA, int *selector);
int (*rt_hal_fled_set_strobe_current)
	(struct rt_fled_dev *fled_dev, int min_uA, int max_uA, int *selector);
int (*rt_hal_fled_set_timeout_level)
	(struct rt_fled_dev *fled_dev, int min_uA, int max_uA, int *selector);
int (*rt_hal_fled_set_lv_protection)
	(struct rt_fled_dev *fled_dev, int min_mV, int max_mV, int *selector);
int (*rt_hal_fled_set_strobe_timeout)
	(struct rt_fled_dev *fled_dev, int min_ms, int max_ms, int *selector);
	/* method to set */
int (*rt_hal_fled_set_torch_current_sel)
				(struct rt_fled_dev *fled_dev, int selector);
int (*rt_hal_fled_set_strobe_current_sel)
				(struct rt_fled_dev *fled_dev, int selector);
int (*rt_hal_fled_set_timeout_level_sel)
				(struct rt_fled_dev *fled_dev, int selector);
int (*rt_hal_fled_set_lv_protection_sel)
				(struct rt_fled_dev *fled_dev, int selector);
int (*rt_hal_fled_set_strobe_timeout_sel)
				(struct rt_fled_dev *fled_dev, int selector);
	/* method to get */
int (*rt_hal_fled_get_torch_current_sel)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_get_strobe_current_sel)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_get_timeout_level_sel)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_get_lv_protection_sel)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_get_strobe_timeout_sel)(struct rt_fled_dev *fled_dev);
	/* method to get, optional*/
int (*rt_hal_fled_get_torch_current)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_get_strobe_current)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_get_timeout_level)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_get_lv_protection)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_get_strobe_timeout)(struct rt_fled_dev *fled_dev);
int (*rt_hal_fled_get_is_ready)(struct rt_fled_dev *fled_dev);
	/* PM shutdown, optional */
void (*rt_hal_fled_shutdown)(struct rt_fled_dev *fled_dev);
};
struct rt_fled_dev {
	struct rt_fled_hal *hal;
	struct flashlight_device *flashlight_dev;
	const struct flashlight_properties *init_props;
	char *name;
	char *chip_name;
};
struct rt_fled_dev *rt_fled_get_dev(const char *name);
#endif /*LINUX_LEDS_RTFLED_H*/
