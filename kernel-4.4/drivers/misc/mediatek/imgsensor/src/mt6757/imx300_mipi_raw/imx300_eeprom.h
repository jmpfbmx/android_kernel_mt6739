
#ifndef _IMX300_EEPROM_H
#define _IMX300_EEPROM_H

extern int iReadRegI2C(u8 *a_pSendData, u16 a_sizeSendData, u8 *a_pRecvData, u16 a_sizeRecvData,
		       u16 i2cId);

#endif
