#include <linux/ptrace.h>
#include <asm/pgtable.h>

int arm_undefinstr_retry(struct pt_regs *regs, unsigned int instr)
{
	return 1;
}
