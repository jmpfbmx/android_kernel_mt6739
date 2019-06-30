


#ifndef _SPEAKER_AMP_H
#define _SPEAKER_AMP_H

struct amp_i2c_control {
	int (*i2c_probe)(struct i2c_client *,
			 const struct i2c_device_id *);
	int (*i2c_remove)(struct i2c_client *);
	void (*i2c_shutdown)(struct i2c_client *);
};

enum speaker_amp_type {
	NOT_SMARTPA = 0,
	RICHTEK_RT5509,
	AMP_TYPE_NUM
};

int get_amp_index(void);

#endif

