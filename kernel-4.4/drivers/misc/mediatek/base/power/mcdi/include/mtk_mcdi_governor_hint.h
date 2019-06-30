
#ifndef __MTK_MCDI_GOVERNOR_HINT_H__
#define __MTK_MCDI_GOVERNOR_HINT_H__

enum {
	SYSTEM_IDLE_HINT_USER_MCDI_TEST = 0,
	SYSTEM_IDLE_HINT_USER_BLUE_TOOTH,
	SYSTEM_IDLE_HINT_USER_AUDIO,
	NF_SYSTEM_IDLE_HINT
};

unsigned int system_idle_hint_result_raw(void);
bool system_idle_hint_result(void);
bool system_idle_hint_request(unsigned int id, bool value);

#endif /* __MTK_MCDI_GOVERNOR_HINT_H__ */
