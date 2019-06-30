
#ifndef __PMIC_LBAT_SERVICE_H__
#define __PMIC_LBAT_SERVICE_H__

struct lbat_dbg_st {
	unsigned int dbg_id;
};

struct lbat_user {
	char name[30];
	unsigned int hv_thd;
	unsigned int lv1_thd;
	unsigned int lv2_thd;
	void (*callback)(unsigned int);
};

/* extern function */
extern int lbat_service_init(void);
extern int lbat_user_register(struct lbat_user *user, const char *name,
	unsigned int hv_thd, unsigned int lv1_thd, unsigned int lv2_thd, void (*callback)(unsigned int));
extern void lbat_min_en_setting(int en_val);
extern void lbat_max_en_setting(int en_val);
extern void lbat_suspend(void);
extern void lbat_resume(void);
extern void lbat_dump_reg(void);
extern int lbat_debug_init(struct dentry *debug_dir);

#endif	/* __PMIC_LBAT_SERVICE_H__ */
