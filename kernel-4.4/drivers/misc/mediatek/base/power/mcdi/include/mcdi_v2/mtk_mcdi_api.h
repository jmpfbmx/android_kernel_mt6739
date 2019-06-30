
#ifndef __MTK_MCDI_API_H__
#define __MTK_MCDI_API_H__

unsigned int mcdi_get_curr_cpu_status(void);
void mcdi_heart_beat_log_dump(void);
bool mcdi_task_pause(bool paused);

#endif /* __MTK_MCDI_API_H__ */
