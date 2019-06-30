#ifndef __MTK_NAND_PROC_H__
#define __MTK_NAND_PROC_H__


#define PROCNAME	"driver/nand"
extern int g_i4Interrupt;
extern bool DDR_INTERFACE;
extern int g_i4InterruptRdDMA;
extern int g_i4Homescreen;
extern int init_module_mem(void *buf, int size);
int mtk_nand_fs_init(void);

void mtk_nand_fs_exit(void);

#endif
