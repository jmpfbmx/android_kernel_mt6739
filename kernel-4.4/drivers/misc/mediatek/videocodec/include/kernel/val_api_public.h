
#ifndef _VAL_API_PUBLIC_H_
#define _VAL_API_PUBLIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "val_types_public.h"


VAL_UINT32_T eVideoInitMVA(VAL_VOID_T **a_pvHandle);


VAL_UINT32_T eVideoAllocMVA(
	VAL_VOID_T *a_pvHandle,
	VAL_UINT32_T a_u4Va,
	VAL_UINT32_T *ap_u4Pa,
	VAL_UINT32_T a_u4Size,
	VAL_VCODEC_M4U_BUFFER_CONFIG_T * a_pvM4uConfig
);

VAL_UINT32_T eVideoFreeMVA(
	VAL_VOID_T *a_pvHandle,
	VAL_UINT32_T a_u4Va,
	VAL_UINT32_T a_u4Pa,
	VAL_UINT32_T a_u4Size,
	VAL_VCODEC_M4U_BUFFER_CONFIG_T * a_pvM4uConfig
);


VAL_UINT32_T eVideoDeInitMVA(VAL_VOID_T *a_pvHandle);


VAL_INT32_T eVideoGetM4UModuleID(VAL_UINT32_T u4MemType);


VAL_RESULT_T eVideoAtoi(VAL_ATOI_T *a_prParam, VAL_UINT32_T a_u4ParamSize);


VAL_RESULT_T eVideoStrStr(VAL_STRSTR_T *a_prParam, VAL_UINT32_T a_u4ParamSize);


VAL_RESULT_T eVideoFlushCache(VAL_MEMORY_T *a_prParam, VAL_UINT32_T a_u4ParamSize, VAL_UINT32_T optype);


VAL_RESULT_T eVideoInvalidateCache(VAL_MEMORY_T *a_prParam, VAL_UINT32_T a_u4ParamSize, VAL_UINT32_T optype);


VAL_RESULT_T eVideoMemAlloc(VAL_MEMORY_T *a_prParam, VAL_UINT32_T a_u4ParamSize);


VAL_RESULT_T eVideoMemFree(VAL_MEMORY_T *a_prParam, VAL_UINT32_T a_u4ParamSize);


VAL_RESULT_T eVideoMemSet(
	VAL_MEMORY_T *a_prParam,
	VAL_UINT32_T a_u4ParamSize,
	VAL_INT32_T a_u4Value,
	VAL_UINT32_T a_u4Size
);


VAL_RESULT_T eVideoMemCpy(
	VAL_MEMORY_T *a_prParamDst,
	VAL_UINT32_T a_u4ParamDstSize,
	VAL_MEMORY_T *a_prParamSrc,
	VAL_UINT32_T a_u4ParamSrcSize,
	VAL_UINT32_T a_u4Size
);


VAL_RESULT_T eVideoMemCmp(
	VAL_MEMORY_T *a_prParamSrc1,
	VAL_UINT32_T a_u4ParamSrc1Size,
	VAL_MEMORY_T *a_prParamSrc2,
	VAL_UINT32_T a_u4ParamSrc2Size,
	VAL_UINT32_T a_u4Size
);


VAL_RESULT_T WaitISR(VAL_ISR_T *a_prParam, VAL_UINT32_T a_u4ParamSize);


VAL_RESULT_T eVideoLockHW(VAL_HW_LOCK_T *a_prParam, VAL_UINT32_T  a_u4ParamSize);


VAL_RESULT_T eVideoUnLockHW(VAL_HW_LOCK_T *a_prParam, VAL_UINT32_T  a_u4ParamSize);


VAL_RESULT_T eVideoGetTimeOfDay(VAL_TIME_T *a_prParam, VAL_UINT32_T a_u4ParamSize);


VAL_RESULT_T eHalEMICtrlForRecordSize(VAL_RECORD_SIZE_T *a_prDrvRecordSize);


VAL_RESULT_T eVideoVcodecSetThreadID(VAL_VCODEC_THREAD_ID_T *a_prThreadID);


VAL_RESULT_T eVideoGetParam(VAL_GET_TYPE_T a_eType, VAL_VOID_T *a_pvInParam, VAL_VOID_T *a_pvOutParam);

VAL_RESULT_T eVideoSetParam(VAL_SET_TYPE_T a_eType, VAL_VOID_T *a_pvInParam, VAL_VOID_T *a_pvOutParam);

VAL_RESULT_T eVideoE3TCMPowerON(VAL_UINT32_T a_u4E3TCMClk);
VAL_RESULT_T eVideoE3TCMPowerOFF(VAL_UINT32_T a_u4E3TCMClk);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _VAL_API_PUBLIC_H_ */
