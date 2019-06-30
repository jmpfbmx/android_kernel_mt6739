
#include <linux/bug.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/ptrace.h>
#include "mrdump_private.h"

void mrdump_save_current_backtrace(struct pt_regs *regs)
{
	asm volatile ("stmia %1, {r0 - r15}\n\t" "mrs %0, cpsr\n":"=r" (regs->uregs[16]) : "r"(regs) : "memory");
}
