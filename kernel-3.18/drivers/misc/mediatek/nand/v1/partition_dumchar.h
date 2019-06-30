
#include <linux/module.h>
#if defined(CONFIG_MTK_TLC_NAND_SUPPORT)
#include "partition_define_tlc.h"
#else
#include "partition_define_mlc.h"
#endif

struct excel_info PartInfo[PART_MAX_COUNT];
EXPORT_SYMBOL(PartInfo);
