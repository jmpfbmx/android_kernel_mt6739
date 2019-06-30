#ifndef _MT_SOC_AFE_CONNECTION_H_
#define _MT_SOC_AFE_CONNECTION_H_

#include "AudDrv_Common.h"
#include "AudDrv_Def.h"
#include "AudDrv_Afe.h"
#include "AudDrv_Ana.h"
#include "AudDrv_Clk.h"

bool SetConnectionState(uint32 ConnectionState, uint32 Input, uint32 Output);
bool SetIntfConnectionFormat(uint32 ConnectionFormat, uint32 Aud_block);
bool SetIntfConnectionState(uint32 ConnectionState, uint32 Aud_block_In, uint32 Aud_block_Out);

#endif
