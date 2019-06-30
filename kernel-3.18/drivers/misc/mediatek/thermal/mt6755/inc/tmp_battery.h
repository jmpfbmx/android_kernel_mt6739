
#include <charging.h>
/* Extern two API functions from battery driver to limit max charging current. */
extern int get_bat_charging_current_level(void);

extern int set_bat_charging_current_limit(int current_limit);
extern CHARGER_TYPE mt_get_charger_type(void);

extern int read_tbat_value(void);

