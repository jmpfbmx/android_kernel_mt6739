
#ifndef __CMDQ_RECORD_PRIVATE_H__
#define __CMDQ_RECORD_PRIVATE_H__

#include "cmdq_record.h"

#ifdef __cplusplus
extern "C" {
#endif

	int32_t cmdq_append_command(cmdqRecHandle handle, CMDQ_CODE_ENUM code, uint32_t arg_a,
				    uint32_t arg_b);
	int32_t cmdq_op_finalize_command(cmdqRecHandle handle, bool loop);

	int32_t cmdq_setup_sec_data_of_command_desc_by_rec_handle(cmdqCommandStruct *pDesc,
								      cmdqRecHandle handle);

	int32_t cmdq_rec_setup_profile_marker_data(cmdqCommandStruct *pDesc, cmdqRecHandle handle);

 #ifdef __cplusplus
}
#endif
#endif				/* __CMDQ_RECORD_PRIVATE_H__ */
