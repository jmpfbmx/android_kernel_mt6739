
#ifndef __VOW_API_H__
#define __VOW_API_H__
int VowDrv_ChangeStatus(void);
int VowDrv_EnableHW(int status);
void VowDrv_SetDmicLowPower(bool enable);
void VowDrv_SetSmartDevice(bool enable);
void VowDrv_SetSmartDevice_GPIO(bool enable);
void VowDrv_SetPeriodicEnable(bool enable);
void VowDrv_SetMtkifType(unsigned int type);
#endif
