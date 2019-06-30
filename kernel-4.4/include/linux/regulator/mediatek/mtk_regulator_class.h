

#ifndef __LINUX_REGULATOR_MTK_REGULATOR_CLASS_H_
#define __LINUX_REGULATOR_MTK_REGULATOR_CLASS_H_

#include <linux/kernel.h>
#include <linux/device.h>

struct mtk_simple_regulator_device {
	struct device dev;
};

enum mtk_simple_regulator_property {
	MREG_PROP_SET_RAMP_DELAY = 0,
};

union mtk_simple_regulator_propval {
	int32_t intval;
	uint32_t uintval;
	const char *strval;
};

extern struct mtk_simple_regulator_device *
mtk_simple_regulator_device_register(const char *name, struct device *parent,
	void *drvdata);

extern void mtk_simple_regulator_device_unregister(
	struct mtk_simple_regulator_device *mreg_dev);

extern struct mtk_simple_regulator_device *mtk_simple_regulator_get_dev_by_name(
	const char *name);

#define to_mreg_device(obj) \
	container_of(obj, struct mtk_simple_regulator_device, dev)

#endif /* __LINUX_REGULATOR_MTK_REGULATOR_CLASS_H_ */
