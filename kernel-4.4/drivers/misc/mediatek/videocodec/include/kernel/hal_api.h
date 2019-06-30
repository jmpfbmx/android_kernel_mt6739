
#ifndef _HAL_API_H_
#define _HAL_API_H_
#include "val_types_public.h"
#include "hal_types_private.h"

#ifdef __cplusplus
extern "C" {
#endif


VAL_RESULT_T eHalInit(VAL_HANDLE_T *a_phHalHandle, HAL_CODEC_TYPE_T a_eHalCodecType);


VAL_RESULT_T eHalDeInit(VAL_HANDLE_T *a_phHalHandle);


VAL_ULONG_T eHalGetMMAP(VAL_HANDLE_T *a_hHalHandle, VAL_UINT32_T RegAddr);


VAL_RESULT_T eHalCmdProc(
	VAL_HANDLE_T *a_hHalHandle,
	HAL_CMD_T a_eHalCmd,
	VAL_VOID_T *a_pvInParam,
	VAL_VOID_T *a_pvOutParam
);



#ifdef __cplusplus
}
#endif

#endif /* #ifndef _HAL_API_H_ */
