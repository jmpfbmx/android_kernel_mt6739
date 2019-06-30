
#ifndef __PMIC_ADB_DBG_H
#define __PMIC_ADB_DBG_H

#define adb_output_reg(reg) \
	seq_printf(s, "[PMIC Exception] " #reg " Reg[0x%x]=0x%x\n", reg, upmu_get_reg_value(reg))

#define kernel_output_reg(reg) \
	pr_notice("[PMIC Exception] " #reg " Reg[0x%x]=0x%x\n", reg, upmu_get_reg_value(reg))

#define both_output_reg(reg) \
	do { \
		seq_printf(s, "[PMIC Exception] " #reg " Reg[0x%x]=0x%x\n", \
			reg, upmu_get_reg_value(reg)); \
		pr_notice("[PMIC Exception] " #reg " Reg[0x%x]=0x%x\n", \
			reg, upmu_get_reg_value(reg)); \
	} while (0)


extern const struct file_operations pmic_dump_exception_operations;

extern void kernel_dump_exception_reg(void);
#endif /*--PMIC_ADB_DBG_H--*/
