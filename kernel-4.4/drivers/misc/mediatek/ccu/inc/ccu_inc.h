
#ifndef __CCU_INC_H__
#define __CCU_INC_H__

struct ccu_sensor_info {
	int32_t slave_addr;
	char *sensor_name_string;
};

void ccu_set_current_fps(int32_t current_fps);
void ccu_set_sensor_info(int32_t sensorType, struct ccu_sensor_info *info);

#endif
