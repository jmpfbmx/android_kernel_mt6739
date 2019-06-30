
#ifndef __RAM_CONSOLE_H__
#define __RAM_CONSOLE_H__
#ifdef CONFIG_MTPROF
extern int boot_finish;
#endif
#ifdef CONFIG_PSTORE
extern void pstore_bconsole_write(struct console *con, const char *s, unsigned c);
#endif
extern struct pstore_info *psinfo;
extern u32 scp_dump_pc(void);
extern u32 scp_dump_lr(void);
#endif
