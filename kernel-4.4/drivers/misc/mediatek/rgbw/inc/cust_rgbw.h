
#ifndef __CUST_RGBW_H__
#define __CUST_RGBW_H__

#include <linux/types.h>

#define C_CUST_I2C_ADDR_NUM 4

#define MAX_THRESHOLD_HIGH 0xffff
#define MIN_THRESHOLD_LOW 0x0

struct rgbw_hw {
	int i2c_num;                                    /*!< the i2c bus used by ALS/PS */
	int power_id;                                   /*!< the VDD power id of the als chip */
	int power_vol;                                  /*!< the VDD power voltage of the als chip */
	unsigned char   i2c_addr[C_CUST_I2C_ADDR_NUM];/*!< i2c address list, some chip will have multiple address */
	bool is_batch_supported;
};

struct rgbw_hw *get_rgbw_dts_func(const char *, struct rgbw_hw*);

#endif
