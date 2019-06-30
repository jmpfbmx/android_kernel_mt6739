
#ifndef _MT6799_CCU_REG_H_
#define _MT6799_CCU_REG_H_

#include <sync_write.h>
#include "ccu_sw_ver.h"
#include "CCU_A_c_header.h"

/*For CCU_A_c_header_v2*/
#define ccu_read_reg(base, regName)       (((PCCU_A_REGS)base)->regName.Raw)
#define ccu_write_reg(base, regName, val)  (((PCCU_A_REGS)base)->regName.Raw = val)
#define ccu_read_reg_bit(base, regName, fieldNmae)       (((PCCU_A_REGS)base)->regName.Bits.fieldNmae)
#define ccu_write_reg_bit(base, regName, fieldNmae, val)  (((PCCU_A_REGS)base)->regName.Bits.fieldNmae = val)
/*#endif*/


#define CCU_SET_BIT(reg, bit)   ((*(volatile unsigned int *)(reg)) |= (unsigned int)(1 << (bit)))
#define CCU_CLR_BIT(reg, bit)   ((*(volatile unsigned int *)(reg)) &= ~((unsigned int)(1 << (bit))))

extern uint64_t ccu_base;

#endif
