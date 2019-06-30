#ifndef __MTK_HOOKS_H__
#define __MTK_HOOKS_H__

/* platform-dependent hook functions */
int arm_undefinstr_retry(struct pt_regs *regs, unsigned int instr);

#endif /* __MTK_HOOKS_H__ */
