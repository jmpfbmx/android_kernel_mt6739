
#ifndef _MT_PMIC_REGULATOR_H_
#define _MT_PMIC_REGULATOR_H_


#define PMIC_BUCK_GEN(_name, en, vol, min, max, step)	\
	{	\
		.desc = {	\
			.name = #_name,	\
			.n_voltages = ((max) - (min)) / (step) + 1,	\
			.min_uV = (min),	\
			.uV_step = (step),	\
		},	\
		.en_att = __ATTR(BUCK_##_name##_STATUS, 0664, show_BUCK_STATUS, store_BUCK_STATUS),	\
		.voltage_att = __ATTR(BUCK_##_name##_VOLTAGE, 0664, show_BUCK_VOLTAGE, store_BUCK_VOLTAGE),	\
		.init_data = {	\
			.constraints = {	\
				.valid_ops_mask = 9,	\
			},	\
		},	\
		.qi_en_reg = (en),	\
		.qi_vol_reg = (vol),	\
		.isUsedable = 0,	\
		.type = "BUCK",	\
	}

#endif				/* _MT_PMIC_REGULATOR_H_ */
