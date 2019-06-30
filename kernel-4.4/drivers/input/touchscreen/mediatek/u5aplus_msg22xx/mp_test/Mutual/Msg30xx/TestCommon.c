/*
 * Copyright (C) 2006-2017 ILITEK TECHNOLOGY CORP.
 *
 * Description: ILITEK I2C touchscreen driver for linux platform.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Dicky Chiang
 * Maintain: Luca Hsu, Tigers Huang
 */
 
#include "TestCommon.h"

extern u32 SLAVE_I2C_ID_DBBUS; //0x62 // for MSG28xx/MSG58xxA/ILI2117A/ILI2118A
extern u32 SLAVE_I2C_ID_DWI2C; //0x26 

void Msg30xxDBBusReEnter(void)
{
	u8 nParCmdSelUseCfg = 0x7F;
	u8 nParCmdAdByteEn1 = 0x51;
	u8 nPar_N_CmdUSetSelB0 = 0x80;
	u8 nPar_N_CmdUSetSelB1 = 0x82;
	u8 nPar_N_CmdSetSelB2  = 0x84;

	DBG(0, "*** %s() ***\n", __func__);

	IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdSelUseCfg, 1);
	IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdAdByteEn1, 1);
	IicWriteData(SLAVE_I2C_ID_DBBUS, &nPar_N_CmdUSetSelB0, 1);
	IicWriteData(SLAVE_I2C_ID_DBBUS, &nPar_N_CmdUSetSelB1, 1);
	IicWriteData(SLAVE_I2C_ID_DBBUS, &nPar_N_CmdSetSelB2, 1);
}

void Msg30xxDBBusReadDQMemStart(void)
{
    u8 nParCmdSelUseCfg = 0x7F;
    u8 nParCmdAdByteEn0 = 0x50;
    u8 nParCmdAdByteEn1 = 0x51;
    u8 nParCmdDaByteEn0 = 0x54;
    u8 nParCmdUSetSelB0 = 0x80;
    u8 nParCmdUSetSelB1 = 0x82;
    u8 nParCmdSetSelB2  = 0x85;
    u8 nParCmdIicUse    = 0x35;
    //u8 nParCmdWr        = 0x10;

    DBG(0, "*** %s() ***\n", __func__);

    RegSet16BitValueOff(0x0F50, BIT1);

    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdSelUseCfg, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdAdByteEn0, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdAdByteEn1, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdDaByteEn0, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdUSetSelB0, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdUSetSelB1, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdSetSelB2,  1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdIicUse,    1);
}

void Msg30xxDBBusReadDQMemEnd(void)
{
    u8 nParCmdNSelUseCfg = 0x7E;

    DBG(0, "*** %s() ***\n", __func__);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdNSelUseCfg, 1);
    RegSet16BitValueOn(0x0F50, BIT1);
}


void Msg30xxEnableAdcOneShot(void)
{
	DBG(0, "*** %s() ***\n", __func__);
    RegSet16BitValueOn(0x100a, BIT0);

    return;
}

void Msg30xxAnaChangeCDtime(u16 T1, u16 T2)
{
	DBG(0, "*** %s() ***\n", __func__);

    RegSet16BitValueByAddressMode(0x1013, T1, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x1018, T2, ADDRESS_MODE_16BIT);
}

s32 Msg30xxTriggerWaterProofOneShot(s16 * pResultData, int *nSize)
{
    u16 nAddr = 0x5000;
    u16 nAddrNextSF = 0x1A4;
    u16 nSF = 0, nAfeOpening = 0, nDriOpening = 0;
    u16 nMaxDataNumOfOneSF = 0;
    u16 nDriMode = 0;
    u16 i;
    u8 nRegData = 0;
    u8 nShotData[392] = {0};//13*15*2
    u16 nRegDataU16 = 0;

    DBG(0, "*** %s() ***\n", __func__);

    nRegData = RegGetLByteValue(0x130A);
    nSF = nRegData>>4;
    nAfeOpening = nRegData & 0x0f;

    if(nSF == 0)
    {
        return -1;
    }

    nRegData = RegGetLByteValue(0x100B);
    nDriMode = nRegData;

    nRegData = RegGetLByteValue(0x1312);
    nDriOpening = nRegData;

    DBG(0, "*** Msg30xx WaterProof Test# TriggerWaterProofOneShot nSF=%d, nAfeOpening=%d, nDriMode=%d, nDriOpening=%d. ***\n", nSF, nAfeOpening, nDriMode, nDriOpening);

    nMaxDataNumOfOneSF = nAfeOpening * nDriOpening;

    RegSet16BitValueOff(0x3D08, BIT8);      ///FIQ_E_FRAME_READY_MASK

    ///polling frame-ready interrupt status
    Msg30xxEnableAdcOneShot();

    while (0x0000 == (nRegDataU16 & BIT8))
    {
        nRegDataU16 = RegGet16BitValue(0x3D18);
    }

    RegSet16BitValueOn(0x3D08, BIT8);      ///FIQ_E_FRAME_READY_MASK
    RegSet16BitValueOn(0x3D08, BIT4);      ///FIQ_E_TIMER0_MASK

    if (ptMsg30xxMutualMpTest->Pattern_type == 1)// for short test
    {
        /// get ALL raw data
    	Msg30xxDBBusReadDQMemStart();
    	//RegGetXByteValue(nShotData, nAddr, 16);
        RegGetXBitValue(nAddr, nShotData, 16, MAX_I2C_TRANSACTION_LENGTH_LIMIT);
        Msg30xxDBBusReadDQMemEnd();

        for (i = 0; i < 8; i++)
        {
            pResultData[i] = (s16)(nShotData[2 * i] | nShotData[2 * i + 1] << 8);
        }
        *nSize = 8;
    }
    else if(ptMsg30xxMutualMpTest->Pattern_type == 3 || ptMsg30xxMutualMpTest->Pattern_type == 4)// for open test
    {
        if(nSF >4)
            nSF = 4;

        /// get ALL raw data, combine and handle datashift.
        for (i = 0; i < nSF; i++)
        {
        	Msg30xxDBBusReadDQMemStart();
            //RegGetXByteValue(nShotData, nAddr + i * nAddrNextSF, 16);
            RegGetXBitValue(nAddr + i * nAddrNextSF, nShotData, 16, MAX_I2C_TRANSACTION_LENGTH_LIMIT);
            Msg30xxDBBusReadDQMemEnd();

            pResultData[2 * i] = (s16)(nShotData[4 * i] | nShotData[4 * i + 1] << 8);
            pResultData[2 * i + 1] = (s16)(nShotData[4 * i + 2] | nShotData[4 * i + 3] << 8);
            DBG(0, "pResultData[%d] = %d, pResultData[%d] = %d\n", 2 * i, pResultData[2 * i], 2 * i + 1, pResultData[2 * i + 1]);
        }
        *nSize = 2 * nSF;
    }
    else
    {
    	*nSize = 0;
        return -1;
    }

    return 0;
}

s32 Msg30xxTriggerMutualOneShot(s16 * pResultData, u16 * pSenNum, u16 * pDrvNum,u16 drv_mode)
{
    u8 nShotData[476] = {0};//14*17*2
    u32 nAddr = 0, nAddrNextSF = 0;
    u16 nSF = 0, nAfeOpening = 0, nDriOpening = 0;
    u16 nMaxDataNumOfOneSF = 0;
    u16 nDriMode = 0;
    u32 nDataShift = -1;
    u16 nRegData = 0, nSwcap = 0;
    s16 * pShotDataAll = NULL;
    u16 i, j, k;

    DBG(0, "*** %s() ***\n", __func__);

    DbBusIICUseBus();

    nRegData = RegGet16BitValueByAddressMode(0x1305, ADDRESS_MODE_16BIT);
    nSF = (nRegData & (BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13)) >> 8;
    nAfeOpening = nRegData & (BIT0 | BIT1 | BIT2 | BIT3);

    if (nSF == 0)
        return -1;

    nRegData = RegGet16BitValueByAddressMode(0x1005, ADDRESS_MODE_16BIT);
    nDriMode = ((nRegData & BIT9) >> 8);

    nRegData = RegGetLByteValue(0x100B);
    nDriMode = nRegData;

    nDriOpening = Msg30xxGet_Drv_opening();

    DBG(0, "*** Msg30xx MP Test# TriggerMutualOneShot nSF=%d, nAfeOpening=%d, drv_mode=%d, nDriOpening=%d. ***\n", nSF, nAfeOpening, drv_mode, nDriOpening);

    nMaxDataNumOfOneSF = nAfeOpening * nDriOpening;
    nAddrNextSF = nMaxDataNumOfOneSF * 2;
    nAddr = _gMsg30xxfout_base_addr;

    if (nMaxDataNumOfOneSF == 0)
    {
        printk("nMaxDataNumOfOneSF == 0");
        return -1;
    }

    RegSet16BitValueOff(0x3D08, BIT8);      ///FIQ_E_FRAME_READY_MASK
    RegMask16BitValue(0x0F28, BIT1, BIT1, ADDRESS_MODE_16BIT);

    nRegData = RegGet16BitValueByAddressMode(0x1100, ADDRESS_MODE_16BIT);
    nSwcap = (nRegData & BIT15);
    if (nSwcap)
    	Msg30xxEnableAdcOneShot();
    else   // sine mode
        RegMask16BitValue(0x1110, BIT1, BIT1, ADDRESS_MODE_16BIT);

    ///polling frame-ready interrupt status
    while (0x0000 == (nRegData & BIT8))
    {
        nRegData = RegGet16BitValue(0x3D18);
    }

    if (nAfeOpening % 2 == 0)
        nDataShift = -1;
    else
        nDataShift = 0;    //special case

    if ((nAddrNextSF % 4) != 0)
        nAddrNextSF = ((nAddrNextSF + 4) / 4) * 4;

    if (drv_mode == 2) // for short test
    {
        u32 data_length = nAddrNextSF;
        data_length += (nDataShift == 0) ? 2 : 0;
        pShotDataAll = (s16 *)kmalloc(sizeof(s16) * nSF * nMaxDataNumOfOneSF, GFP_KERNEL);

        /// get ALL raw data, combine and handle datashift.
        for (i = 0; i < nSF; i++)
        {
            memset(nShotData, 0, sizeof(nShotData));
            Msg30xxDBBusReadDQMemStart();
            //RegGetXByteValue(nShotData, nAddr + i * nAddrNextSF, data_length);
            RegGetXBitValue(nAddr + i * nAddrNextSF, nShotData, data_length, MAX_I2C_TRANSACTION_LENGTH_LIMIT);
            Msg30xxDBBusReadDQMemEnd();

            for (j = 0; j < nMaxDataNumOfOneSF; j++)
            {
                pShotDataAll[i*nMaxDataNumOfOneSF+j] = (s16)(nShotData[2 * j] | nShotData[2 * j + 1] << 8);

                if (nDataShift == 0 && j == (nMaxDataNumOfOneSF - 1))
                    pShotDataAll[i*nMaxDataNumOfOneSF+j] = (s16)(nShotData[2 * (j + 1)] | nShotData[2 * (j + 1) + 1] << 8);
                //DBG(0, "pShotDataAll[%d] = %d", i*nMaxDataNumOfOneSF+j, pShotDataAll[i*nMaxDataNumOfOneSF+j]);
            }
        }

        //problem here
        for (k = 0; k < nSF; k++)
        {
            for (i = k * nAfeOpening; i < nAfeOpening * (k + 1); i++) //Sen
            {
                pResultData[i] = pShotDataAll[k*nMaxDataNumOfOneSF + ((i - nAfeOpening * k) * nDriOpening)]; //resultData[Sen, Dri]
                //DBG(0, "pResultData[%d] = %d", i, pResultData[i]);
            }
        }

        *pSenNum = nSF * nAfeOpening;
        *pDrvNum = nDriOpening;
        kfree(pShotDataAll);
    }
    else // for open test
    {
        u16 frame_multiply = (_gMsg30xxTwoDACEnable == ENABLE) ? 2 : 1;
        u32 data_length = nAddrNextSF;
        data_length += (nDataShift == 0) ? 2 : 0;
        pShotDataAll = (s16 *)kmalloc(sizeof(s16) * nSF * frame_multiply * nMaxDataNumOfOneSF, GFP_KERNEL);

        /// get ALL raw data, combine and handle datashift.
        for (i = 0; i < (nSF * frame_multiply); i++)
        {
        	memset(nShotData, 0, sizeof(nShotData));
        	Msg30xxDBBusReadDQMemStart();
            //RegGetXByteValue(nShotData, nAddr + i * nAddrNextSF, data_length);
            RegGetXBitValue(nAddr + i * nAddrNextSF, nShotData, data_length, MAX_I2C_TRANSACTION_LENGTH_LIMIT);
            Msg30xxDBBusReadDQMemEnd();

            for (j = 0; j < nMaxDataNumOfOneSF; j++)
            {
                pShotDataAll[i*nMaxDataNumOfOneSF+j] = (s16)(nShotData[2 * j] | nShotData[2 * j + 1] << 8);

                if (nDataShift == 0 && j == (nMaxDataNumOfOneSF - 1))
                    pShotDataAll[i*nMaxDataNumOfOneSF+j] = (s16)(nShotData[2 * (j + 1)] | nShotData[2 * (j + 1) + 1] << 8);
                //DBG(0, "pShotDataAll[%d] = %d", i*nMaxDataNumOfOneSF+j, pShotDataAll[i*nMaxDataNumOfOneSF+j]);
            }
        }

        //problem here
        for (k = 0; k < (nSF * frame_multiply); k++)
        {
            for (i = k * nAfeOpening; i < nAfeOpening * (k + 1); i++) //Sen
            {
                for (j = 0; j < nDriOpening; j++) //Dri
                {
                    pResultData[i*MAX_CHANNEL_DRV+j] = pShotDataAll[k*nMaxDataNumOfOneSF + (j + (i - nAfeOpening * k) * nDriOpening)]; //resultData[Sen, Dri]
                }
            }
        }

        *pSenNum = nSF * nAfeOpening;
        *pDrvNum = nDriOpening;
        kfree(pShotDataAll);
    }
    RegSet16BitValueOn(0x3D08, BIT8);      ///FIQ_E_FRAME_READY_MASK
    RegSet16BitValueOn(0x3D08, BIT4);      ///FIQ_E_TIMER0_MASK
    RegMask16BitValue(0x3D0C, BIT8, BIT8, ADDRESS_MODE_16BIT);  //clear frame ready status
    return 0;
}

s32 Msg30xxGetMutualOneShotRawIIR(s16 * nResultData, u16 * pSenNum, u16 * pDrvNum, u16 drv_mode)
{
	DBG(0, "*** %s() ***\n", __func__);
    return Msg30xxTriggerMutualOneShot(nResultData, pSenNum, pDrvNum, drv_mode);
}

s32 Msg30xxtGetWaterProofOneShotRawIIR(s16 * pRawDataWP, int *nSize)
{
	DBG(0, "*** %s() ***\n", __func__);
    return Msg30xxTriggerWaterProofOneShot(pRawDataWP, nSize);
}

s32 Msg30xxCheckSwitchStatus(void)
{
    u16 nRegData = 0;
    int nTimeOut = 280;
    int nT = 0;

    DBG(0, "*** %s() ***\n", __func__);
    do
    {
        nRegData = RegGet16BitValueByAddressMode(0x1401, ADDRESS_MODE_16BIT);
        mdelay(20);
        nT++;
        if (nT > nTimeOut)
        {
            return -1;
        }
        DBG(0, "nT = %d, nRegData = 0x%04x\n", nT, nRegData);
    } while (nRegData != 0x7447);

    return 0;
}

s32 Msg30xxSwitchFwMode(u16 *nFMode, u16 *deep_standby) //201703xx
{
    u16 nRegData = 0;

    DBG(0, "*** %s() ***\n", __func__);

    StopMCU();
    mdelay(100);
    RegSet16BitValue(0X3C60, 0xAA55);    // disable watch dog

    RegSet16BitValue(0X3D08, 0xFFFF);   // clear interrupt status
    RegSet16BitValue(0X3D18, 0xFFFF);   // clear interrupt status

    RegSet16BitValue(0x1402, 0x7474);   // enter mp mode

    RegSet16BitValue(0x1E06, 0x0000);
    RegSet16BitValue(0x1E06, 0x0001);
    StartMCU();
    mdelay(300);

    if (Msg30xxCheckSwitchStatus()<0)
    {
        printk("*** Msg30xx MP Test# CheckSwitchStatus failed! ***\n");
        return -1;
    }

    //deep standby mode
    if (*deep_standby == 1)
    {
    	DBG(0, "*** Msg30xx MP Test# enter deep standby! ***\n");
    	RegSet16BitValue(0x1402, 0x6179);
    	DBG(0, "deep_standby_timeout = %d", ptMsg30xxMutualMpTest->deep_standby_timeout);
    	mdelay(ptMsg30xxMutualMpTest->deep_standby_timeout ); // depend on ini, default = 450ms

    	EnterDBBus();

    	if (Msg30xxCheckSwitchStatus()<0)
    	{
    		*deep_standby = 0;
    		printk("*** Msg30xx MP Test# enter deep standby FAILED! ***\n");
    		return -1;
    	}
    }

    _gMsg30xxScanMode = RegGet16BitValueByAddressMode(0x1361, ADDRESS_MODE_16BIT);
    _gMsg30xxCodeType = RegGet16BitValueByAddressMode(0x1362, ADDRESS_MODE_16BIT);
    _gMsg30xxTwoDACEnable = _gMsg30xxCodeType & 0x00FF;
    DBG(0, "nFMode = %x\n", *nFMode);

    switch (*nFMode)
    {
        case MUTUAL_MODE:
            RegSet16BitValue(0x1402, 0x5705);
            break;

        case MUTUAL_SINE:
            RegSet16BitValue(0x1402, 0x5706);
            break;

        case SELF:
            RegSet16BitValue(0x1402, 0x6278);
            break;

        case WATERPROOF:
            RegSet16BitValue(0x1402, 0x7992);
            break;

        case MUTUAL_SINGLE_DRIVE:
            RegSet16BitValue(0x1402, 0x0158);
            break;

        case SINE_PHASE:
            RegSet16BitValue(0x1402, 0xECFA);
            break;

        default:
            return -1;
    }
    if (Msg30xxCheckSwitchStatus()<0)
    {
        printk("*** Msg30xx MP Test# CheckSwitchStatus failed! ***\n");
        return -1;
    }

    switch (*nFMode)
    {
        case MUTUAL_MODE:
        case MUTUAL_SINE:
        case MUTUAL_KEY:
        case WATERPROOF:
        case MUTUAL_SINGLE_DRIVE:
        case MUTUAL_SINE_KEY:
            nRegData = RegGet16BitValueByAddressMode(0x1361, ADDRESS_MODE_16BIT);
            _gMsg30xxfout_base_addr = (u16)(nRegData << 2);
            DBG(0, "_gMsg30xxfout_base_addr = 0x%04x\n", _gMsg30xxfout_base_addr);
            break;
        default:
            return -1;
    }

    StopMCU();
    RegSet16BitValue(0x3D08, 0xFEFF);//open timer

    return 0;
}

void Msg30xxAnaSwReset(void)    //301703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    /// reset ANA
    RegSet16BitValueOn(0x1002, (BIT0 | BIT1 | BIT2 | BIT3));     ///reg_tgen_soft_rst: 1 to reset
    RegSet16BitValueOff(0x1002, (BIT0 | BIT1 | BIT2 | BIT3));

    /// delay
    mdelay(20);
}

void Msg30xxAnaFixPrs(u16 option)   //201703xx
{
//    u16 regData = 0;

    DBG(0, "*** %s() ***\n", __func__);

    StopMCU();
    RegMask16BitValue(0x1004, BIT1 | BIT2, option << 1, ADDRESS_MODE_16BIT);
}

s32 Msg30xxEnterMpMode()    //201703xx
{
	DBG(0, "*** %s() ***\n", __func__);
	StopMCU();
	mdelay(100);
	RegSet16BitValue(0X3C60, 0xAA55);    // disable watch dog
	RegSet16BitValue(0X3D08, 0xFFFF);   // clear interrupt status
	RegSet16BitValue(0X3D18, 0xFFFF);   // clear interrupt status

	RegSet16BitValue(0x1402, 0x7474);   // enter mp mode

	RegSet16BitValue(0x1E06, 0x0000);
	RegSet16BitValue(0x1E06, 0x0001);
	StartMCU();
	mdelay(300);

	if (Msg30xxCheckSwitchStatus()<0)
	{
		printk("*** Msg30xx MP Test# CheckSwitchStatus failed! ***\n");
		return -1;
	}
	_gMsg30xxScanMode = RegGet16BitValueByAddressMode(0x1361, ADDRESS_MODE_16BIT);
 	_gMsg30xxCodeType = RegGet16BitValueByAddressMode(0x1362, ADDRESS_MODE_16BIT);
    _gMsg30xxTwoDACEnable = _gMsg30xxCodeType & 0x00FF;
    DBG(0, "_gMsg30xxScanMode = 0x%04X, _gMsg30xxCodeType = 0x%04X, _gMsg30xxTwoDACEnable = 0x%04X\n", _gMsg30xxScanMode, _gMsg30xxCodeType, _gMsg30xxTwoDACEnable);
	switch (ptMsg30xxMutualMpTest->Open_mode )
    {
        case 0:
            RegSet16BitValue(0x1402, MUTUAL_MODE);
            break;
        case 1:
            RegSet16BitValue(0x1402, MUTUAL_SINE);
            break;
    }
    if (Msg30xxCheckSwitchStatus()<0)
    {
        printk("*** Msg30xx MP Mp multi-drive # CheckSwitchStatus failed! ***\n");
        return -1;
    }
    
    return 0;
}

u16 Msg30xxGet_Drv_opening(void)    //201703xx
{
   u16 dri_opening = 0;
   u16 nRegData = 0;

   DBG(0, "*** %s() ***\n", __func__);

   nRegData = RegGet16BitValueByAddressMode(0x1306, ADDRESS_MODE_16BIT);
   dri_opening = nRegData & (BIT0 | BIT1 | BIT2 | BIT3 | BIT4);

   return dri_opening;
}

void Msg30xxScanDacSetting(u16 nTwoDAC) //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    Msg30xxTgenOvwrDACEnable(ENABLE, eDAC_0);

    if (nTwoDAC == ENABLE)
    {
        Msg30xxTgenOvwrDACEnable(ENABLE, eDAC_1);
    }
    else
    {
        Msg30xxTgenOvwrDACEnable(DISABLE, eDAC_1);
    }
}

void Msg30xxTgenOvwrDACEnable(u16 nEnable, u16 eDACSelect)  //201703xx
{
    //BIT0 : DAC enable or not
    //BIT1 : DAC LDO enable or not
    u16 nAddr = 0;

    DBG(0, "*** %s() ***\n", __func__);

    if (eDACSelect == eDAC_0)
    {
        nAddr = 0x1537;
    }
    else
    {
        nAddr = 0x153F;
    }

    if (nEnable == ENABLE)
    {
        RegMask16BitValue(nAddr, BIT0 | BIT1, BIT0 |BIT1, ADDRESS_MODE_16BIT);
    }
    else
    {
        RegMask16BitValue(nAddr, BIT0 | BIT1, 0, ADDRESS_MODE_16BIT);
    }
}

void Msg30xxTgenOvwrRegEn(u16 nEnable)  //201703xx
{
    //reg_en_afe_refbuf : bit0
    //reg_en_bg : bit1
    //reg_en_csub_refbuf : bit2
    //reg_en_lvdrvg_refbuf : bit3
    //reg_en_lvdrvm_refbuf : bit4
    //reg_en_lvdrvp_refbuf : bit5
    //reg_en_ref : bit6

    DBG(0, "*** %s() ***\n", __func__);

    if (nEnable == ENABLE)
    {
        RegMask16BitValue(0x152A, BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6, BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6, ADDRESS_MODE_16BIT);
    }
    else
    {
        RegMask16BitValue(0x152A, BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6, 0, ADDRESS_MODE_16BIT);
    }
}

void Msg30xxTgenOvwrInverterMode(u16 nEnable, u16 nTwoDAC)  //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    if (nEnable == ENABLE)
    {
        RegMask16BitValue(0x1532, BIT0 , BIT0, ADDRESS_MODE_16BIT);
        if (nTwoDAC == ENABLE)
        {
            RegMask16BitValue(0x153A, BIT0 , BIT0, ADDRESS_MODE_16BIT);
        }
        else
        {
            RegMask16BitValue(0x153A, BIT0 , 0, ADDRESS_MODE_16BIT);
        }
    }
    else
    {
        RegMask16BitValue(0x1532, BIT0 , 0, ADDRESS_MODE_16BIT);
        RegMask16BitValue(0x153A, BIT0 , 0, ADDRESS_MODE_16BIT);
    }
}

void Msg30xxTgenOvwrDRVLvBufCfbSetting(u16 nEnable, u16 nTwoDAC)    //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    if (nEnable == ENABLE)
    {
        RegMask16BitValue(0x1534, BIT0 | BIT1 | BIT4 | BIT5, BIT1 | BIT5, ADDRESS_MODE_16BIT);
        if (nTwoDAC == ENABLE)
        {
            RegMask16BitValue(0x153C, BIT0 | BIT1 | BIT4 | BIT5, BIT1 | BIT5, ADDRESS_MODE_16BIT);
        }
        else
        {
            RegMask16BitValue(0x153C, BIT0 | BIT1 | BIT4 | BIT5, 0, ADDRESS_MODE_16BIT);
        }
    }
    else
    {
        RegMask16BitValue(0x1534, BIT0 | BIT1 | BIT4 | BIT5, BIT0 | BIT1 | BIT4 | BIT5, ADDRESS_MODE_16BIT);
        if (nTwoDAC == ENABLE)
        {
            RegMask16BitValue(0x153C, BIT0 | BIT1 | BIT4 | BIT5, BIT0 | BIT1 | BIT4 | BIT5, ADDRESS_MODE_16BIT);
        }
        else
        {
            RegMask16BitValue(0x153C, BIT0 | BIT1 | BIT4 | BIT5, 0, ADDRESS_MODE_16BIT);
        }
    }
}

void Msg30xxTgenOvwrDRVLvBufGainSetting(u16 nEnable, u16 nTwoDAC)   //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    if (nEnable == ENABLE)
    {
        RegMask16BitValue(0x1534, BIT8 | BIT9| BIT12 | BIT13, 0, ADDRESS_MODE_16BIT);

        if (nTwoDAC == ENABLE)
        {
            RegMask16BitValue(0x153C, BIT8 | BIT9| BIT12 | BIT13, 0, ADDRESS_MODE_16BIT);
        }
        else
        {
            RegMask16BitValue(0x153C, BIT9| BIT13, BIT9 | BIT13, ADDRESS_MODE_16BIT);
        }
    }
    else
    {
        RegMask16BitValue(0x1534, BIT9| BIT13, BIT9 | BIT13, ADDRESS_MODE_16BIT);
        RegMask16BitValue(0x153C, BIT9| BIT13, BIT9 | BIT13, ADDRESS_MODE_16BIT);
    }
}

void Msg30xxTgenOvwrDRVHvCompSetting(u16 nEnable)   //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    if (nEnable == ENABLE)
    {
        RegMask16BitValue(0x1531, BIT8 | BIT10, BIT8 | BIT10, ADDRESS_MODE_16BIT);
    }
    else
    {
        RegMask16BitValue(0x1531, BIT8 | BIT10, 0, ADDRESS_MODE_16BIT);
    }
}

void Msg30xxTgenOvwrROSSelect(u16 nEnable, u16 nTwoDAC) //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    if (nEnable == ENABLE)
    {
        RegMask16BitValue(0x1531, BIT6 | BIT7, 0, ADDRESS_MODE_16BIT);
        RegMask16BitValue(0x1531, BIT14 | BIT15, 0, ADDRESS_MODE_16BIT);

        if (nTwoDAC == ENABLE)
        {
            RegMask16BitValue(0x1539, BIT6 | BIT7, 0, ADDRESS_MODE_16BIT);
            RegMask16BitValue(0x1539, BIT14 | BIT15, 0, ADDRESS_MODE_16BIT);
        }
    }
    else
    {
        RegMask16BitValue(0x1531, BIT6 | BIT7, BIT6 | BIT7, ADDRESS_MODE_16BIT);
        RegMask16BitValue(0x1531, BIT14 | BIT15, BIT14 | BIT15, ADDRESS_MODE_16BIT);

        if (nTwoDAC == ENABLE)
        {
            RegMask16BitValue(0x1539, BIT6 | BIT7, BIT6 | BIT7, ADDRESS_MODE_16BIT);
            RegMask16BitValue(0x1539, BIT14 | BIT15, BIT14 | BIT15, ADDRESS_MODE_16BIT);
        }
    }
}

void Msg30xxTgenOvwrDRVHvSetting(u16 nEnable)   //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    if (nEnable == DISABLE) //Hv
    {
        RegMask16BitValue(0x1530, BIT0 | BIT1, BIT0 | BIT1, ADDRESS_MODE_16BIT);
    }
    else
    {
        RegMask16BitValue(0x1530, BIT0 | BIT1, 0, ADDRESS_MODE_16BIT);
    }
}

void Msg30xxTgenOvwrChargePumpSetting(u16 nEnable_charge_pump)  //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    if (nEnable_charge_pump == DISABLE)
    {
        RegMask16BitValue(0x1433, BIT0 | BIT1, 0, ADDRESS_MODE_16BIT);
        RegMask16BitValue(0x1540, BIT10, 0, ADDRESS_MODE_16BIT);
        RegMask16BitValue(0x152F, BIT8, BIT8, ADDRESS_MODE_16BIT);
    }
    else
    {
        RegMask16BitValue(0x1433, BIT0 | BIT1, BIT1, ADDRESS_MODE_16BIT);
        RegMask16BitValue(0x1540, BIT10, BIT10, ADDRESS_MODE_16BIT);
        RegMask16BitValue(0x152F, BIT8, 0, ADDRESS_MODE_16BIT);
    }
}

void Msg30xxAnaEnableChargePump(u16 nEnable_charge_pump)    //201703xx
{

    DBG(0, "*** %s() ***\n", __func__);

    Msg30xxTgenOvwrChargePumpSetting(nEnable_charge_pump);
}

void Msg30xxGain_Setting(void)   //201703xx
{
    u8 Cfb = ptMsg30xxMutualMpTest->Open_test_cfb;
    u16 decode_out = 0, dri_opening = 0, regdata = 0;

    DBG(0, "*** %s() ***\n", __func__);

    regdata = RegGet16BitValueByAddressMode(0x1306, ADDRESS_MODE_16BIT);    //get dri num
    dri_opening = regdata & (BIT0 | BIT1 | BIT2 | BIT3 | BIT4);

    RegSet16BitValueByAddressMode(0x1019, 0x083f, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x101a, 0x0029, ADDRESS_MODE_16BIT);  //post idle time in sub0
    RegSet16BitValueByAddressMode(0x101b, 0x0029, ADDRESS_MODE_16BIT);  //post idle time in sub1
    RegSet16BitValueByAddressMode(0x101c, 0x0029, ADDRESS_MODE_16BIT);  //post idle time in sub2
    RegSet16BitValueByAddressMode(0x101d, 0x0029, ADDRESS_MODE_16BIT);  //post idle time in sub3

    // fw default setting
    Msg30xxScanSetSampleNum(26);
    RegSet16BitValueByAddressMode(0x136B, 0x9D89, ADDRESS_MODE_16BIT);

    ///
    switch (dri_opening)
    {
        case 1:
            decode_out = 0;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 12:
        case 13:
        case 14:
        case 16:
            decode_out = 2;
            break;
        case 11:
        case 15:
        case 17:
            decode_out = 3;
            break;
    }
    // set shift decode out
    RegMask16BitValue(0x130D, BIT0 | BIT1 | BIT2 | BIT3, decode_out, ADDRESS_MODE_16BIT);
    // set shift all coef
    RegMask16BitValue(0x130D, BIT12 | BIT13 | BIT14 | BIT15, 0, ADDRESS_MODE_16BIT);
    // set shift fir out
    RegMask16BitValue(0x130D, BIT4 | BIT5 | BIT6 | BIT7, BIT6, ADDRESS_MODE_16BIT);

    /// all AFE Cfb use by autosettings.ini setting, defalt (50p)
    RegSet16BitValueByAddressMode(0x1504, 0x3FFF, ADDRESS_MODE_16BIT);  // all 14 AFE Cfb: SW control

    switch (Cfb)
    {
        case 0:
        case 2:
            Msg30xxSetCfb(_Msg30xx30p);
            break;
        case 1:
            Msg30xxSetCfb(_Msg30xx25p);
            break;
        case 3:
            Msg30xxSetCfb(_Msg30xx35p);
            break;
        case 4:
            Msg30xxSetCfb(_Msg30xx45p);
            break;
        case 5:
            Msg30xxSetCfb(_Msg30xx50p);
            break;
    }
    ///ADC: AFE Gain bypass
    RegSet16BitValueByAddressMode(0x1230, 0x3FFF, ADDRESS_MODE_16BIT);
}

void Msg30xxSetSensorPADState(u16 state)
{
    u16 value = 0;
    int i;

    DBG(0, "*** %s() ***\n", __func__);

    for (i = 0; i < 8; i++)
        value |= (u16)(state << (i * 2));

    for (i = 0; i < 7; i++)
    {
        if (i == 6)
            value = (u16)(state & (BIT0 | BIT1));
        RegSet16BitValueByAddressMode(0x1514 + i, value, ADDRESS_MODE_16BIT);
    }

    for (i = 0; i < 4; i++)
    {
        if (i == 3)
            value = 0x0001;
        else
            value = 0xFFFF;
        RegSet16BitValueByAddressMode(0x1510 + i, value, ADDRESS_MODE_16BIT);
    }
}

void Msg30xxScanSetSensorNum(u16 nSensorNum)    //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);
    RegMask16BitValue(0x1305, BIT0 | BIT1 | BIT2 | BIT3, nSensorNum, ADDRESS_MODE_16BIT);
    RegMask16BitValue(0x1102, BIT0 | BIT1 | BIT2 | BIT3, nSensorNum - 1, ADDRESS_MODE_16BIT);
    RegMask16BitValue(0x1006, BIT0 | BIT1 | BIT2 | BIT3, nSensorNum - 1, ADDRESS_MODE_16BIT);
    RegMask16BitValue(0x1017, BIT11 | BIT12 | BIT13 | BIT14 | BIT15, (u16)(0x0C - abs(MAX_AFE_NUM_30XX - nSensorNum)) << 11, ADDRESS_MODE_16BIT);
}

void Msg30xxScanSetSubframeNum(u16 nSubframeNum)
{
    DBG(0, "*** %s() ***\n", __func__);
    RegMask16BitValue(0x1305, BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13, nSubframeNum << 8, ADDRESS_MODE_16BIT);  // bit8-13=SF_NUM,bit0-3=SENSOR_NUM
    RegMask16BitValue(0x1101, BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5, nSubframeNum, ADDRESS_MODE_16BIT);   // bit0-5=6,6 subframe per scan
    RegMask16BitValue(0x100B, BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6, nSubframeNum << 1, ADDRESS_MODE_16BIT);  // bit1-6= subframe
}

void Msg30xxScanSetSampleNum(u16 nSampleNum)
{
    DBG(0, "*** %s() ***\n", __func__);
    RegMask16BitValue(0x1304, 0x07FF, nSampleNum, ADDRESS_MODE_16BIT);
    RegMask16BitValue(0x1103, 0x1FFF, nSampleNum, ADDRESS_MODE_16BIT);
    RegMask16BitValue(0x100D, 0x07FF, nSampleNum, ADDRESS_MODE_16BIT);
    RegMask16BitValue(0x1B04, 0x07FF, nSampleNum, ADDRESS_MODE_16BIT);
}

void Msg30xxSetChipSwapEnable(u16 nSwapEnable)
{
    DBG(0, "*** %s() ***\n", __func__);
    if (nSwapEnable == ENABLE)
        RegMask16BitValue(0x111F, BIT2, BIT2, ADDRESS_MODE_16BIT);
    else
        RegMask16BitValue(0x111F, BIT2, 0, ADDRESS_MODE_16BIT);
}

void Msg30xxAdcDespInverterEnable(u16 nEnable)
{
    DBG(0, "*** %s() ***\n", __func__);
    if (nEnable == ENABLE)
        RegMask16BitValue(0x1210, BIT13, BIT13, ADDRESS_MODE_16BIT);
    else
        RegMask16BitValue(0x1210, BIT13, 0, ADDRESS_MODE_16BIT);
}

void Msg30xxSetMutualCsubViaDBbus(s16 nCSub)    //201703xx
{
    u8 nBaseLen = 6;
    u16 nFilter = 0x3F;
    u16 nLastFilter = 0x000F;   // 6x 14 % 16
    u8 nBasePattern = 0;
    u8 nPattern;
    u16 n16BitsPattern;
    u16 nCSub16Bits[6] = {0};
    int i;

    DBG(0, "*** %s() ***\n", __func__);

    for(i = 0; i < 6; i++)
    {
        if(i == 0)
            nPattern = nBasePattern;    //Patn => Pattern

        n16BitsPattern = (u16) ((nPattern & 0xF) << nBaseLen * 2) | (nPattern << nBaseLen) | nPattern;

        if(i == 5)
        {
            nCSub16Bits[i] = (u16)(n16BitsPattern & nLastFilter);
        }
        else
        {
            nCSub16Bits[i] = n16BitsPattern;
        }
        nPattern = (u8)((n16BitsPattern >> 4) & nFilter);
    }

    RegSet16BitValueByAddressMode(0x211F, 0x3FFF, ADDRESS_MODE_16BIT);

    for (i = 0; i < 6; i++)
    {
        RegSet16BitValueByAddressMode(0x2148 + i, nCSub16Bits[i], ADDRESS_MODE_16BIT);
        RegSet16BitValueByAddressMode(0x214A + i, nCSub16Bits[i], ADDRESS_MODE_16BIT);
    }
}