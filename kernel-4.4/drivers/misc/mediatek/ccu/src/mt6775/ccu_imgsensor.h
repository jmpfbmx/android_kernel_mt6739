
#ifndef _CCU_IMGSENSOR_H_
#define _CCU_IMGSENSOR_H_

void ccu_get_current_fps(int32_t *current_fps_list);

void ccu_get_sensor_i2c_slave_addr(int32_t *sensorI2cSlaveAddr);

void ccu_get_sensor_name(char **sensor_name);

#endif
