

#include "audio_ipi_platform.h"
#include "scp_ipi.h"

unsigned int audio_ipi_check_scp_status(void)
{
	return is_scp_ready(SCP_A_ID);
}

unsigned int get_audio_ipi_scp_location(void)
{
	return SCP_A_ID;
}

