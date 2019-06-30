
#include <mt-plat/mtk_devinfo.h>
#include <mt-plat/mtk_platform_debug.h>
#include "../dfd.h"
#include <linux/printk.h>

static unsigned int is_kibop;

unsigned int check_dfd_support(void)
{
	unsigned int segment;

	is_kibop = 0;
	segment = (get_devinfo_with_index(30) & 0x000000E0) >> 5;
	if (segment == 0x3 || segment == 0x7) {
		is_kibop = 1;
		pr_notice("[dfd] Kibo+ enable DFD\n");
		return 1;
	}

	pr_notice("[dfd] Kibo disable DFD,0x%x\n", segment);
	return 0;

}
