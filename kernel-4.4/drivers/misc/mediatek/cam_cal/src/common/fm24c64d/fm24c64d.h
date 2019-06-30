#ifndef __FM24C64D_H
#define __FM24C64D_H
#include <linux/i2c.h>

unsigned int fm24c64d_selective_read_region(struct i2c_client *client, unsigned int addr,
	unsigned char *data, unsigned int size);


#endif /* __CAM_CAL_H */

