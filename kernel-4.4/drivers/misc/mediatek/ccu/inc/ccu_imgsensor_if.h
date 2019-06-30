
#ifndef _CCU_IMGSENSOR_IF_H_
#define _CCU_IMGSENSOR_IF_H_

struct ccu_sensor_info {
	int32_t slave_addr;
	char *sensor_name_string;
};

void ccu_set_current_fps(int32_t sensorType, int32_t current_fps);
void ccu_set_sensor_info(int32_t sensorType, struct ccu_sensor_info *info);

#endif
