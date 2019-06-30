

#include "vow_hw.h"
#include "scp_ipi.h"


unsigned int vow_check_scp_status(void)
{
	return is_scp_ready(SCP_A_ID);
}
