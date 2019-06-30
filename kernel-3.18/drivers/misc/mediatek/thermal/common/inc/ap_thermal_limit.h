#ifndef __AP_THERMAL_LIMIT_H__
#define __AP_THERMAL_LIMIT_H__

struct apthermolmt_user {
	char *log;
	unsigned int cpu_limit;
	unsigned int gpu_limit;
	void *ptr;
};

extern
int apthermolmt_register_user
(struct apthermolmt_user *handle, char *log);

extern
int apthermolmt_unregister_user
(struct apthermolmt_user *handle);

extern
void apthermolmt_set_cpu_power_limit
(struct apthermolmt_user *handle, unsigned int limit);

extern
void apthermolmt_set_gpu_power_limit
(struct apthermolmt_user *handle, unsigned int limit);

extern
void apthermolmt_set_general_cpu_power_limit
(unsigned int limit);

extern
void apthermolmt_set_general_gpu_power_limit
(unsigned int limit);

extern
unsigned int apthermolmt_get_cpu_power_limit(void);

extern
unsigned int apthermolmt_get_gpu_power_limit(void);

#endif	/* __AP_THERMAL_LIMIT_H__ */
