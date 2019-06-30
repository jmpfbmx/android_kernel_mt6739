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

extern u8 SLAVE_I2C_ID_DBBUS; //0x62 // for MSG21XX/MSG21XXA/MSG26XXM/MSG28XX
//u8 SLAVE_I2C_ID_DBBUS = (0xB2>>1); //0x59 // for MSG22XX
extern u8 SLAVE_I2C_ID_DWI2C; //0x26

void Msg28xxAnaChangeCDtime(u16 T1, u16 T2)
{
	DBG(0, "*** %s() ***\n", __func__);

    RegSet16BitValue(0x1026, T1);
    RegSet16BitValue(0x1030, T2);
}

void Msg28xxAnaSwReset(void)
{
    DBG(0, "*** %s() ***\n", __func__);

    /// reset ANA
    RegSet16BitValueOn(0x1002, (BIT0 | BIT1 | BIT2 | BIT3));     ///reg_tgen_soft_rst: 1 to reset
    RegSet16BitValueOff(0x1002, (BIT0 | BIT1 | BIT2 | BIT3));

    /// delay
    //usleep(20000);
    mdelay(20);
}

void Msg28xxEnableAdcOneShot(void)
{
	DBG(0, "*** %s() ***\n", __func__);
    RegSet16BitValueOn(0x100a, BIT0);

    return;
}

void Msg28xxDBBusReEnter(void)
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

void Msg28xxDBBusReadDQMemStart(void)
{    
    u8 nParCmdSelUseCfg = 0x7F;
    u8 nParCmdAdByteEn0 = 0x50;
    u8 nParCmdAdByteEn1 = 0x51;
    u8 nParCmdDaByteEn0 = 0x54;
    u8 nParCmdUSetSelB0 = 0x80;
    u8 nParCmdUSetSelB1 = 0x82;
    u8 nParCmdSetSelB2  = 0x85;
    u8 nParCmdIicUse    = 0x35;
    
    DBG(0, "*** %s() ***\n", __func__);

	RegSet16BitValueOff(0x0F50, BIT1);

    //u8 nParCmdWr        = 0x10;

    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdSelUseCfg, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdAdByteEn0, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdAdByteEn1, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdDaByteEn0, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdUSetSelB0, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdUSetSelB1, 1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdSetSelB2,  1);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdIicUse,    1);
}

void Msg28xxDBBusReadDQMemEnd(void)
{
    u8 nParCmdNSelUseCfg = 0x7E;

    DBG(0, "*** %s() ***\n", __func__);
    IicWriteData(SLAVE_I2C_ID_DBBUS, &nParCmdNSelUseCfg, 1);
    RegSet16BitValueOn(0x0F50, BIT1);
}

s32 Msg28xxTriggerWaterProofOneShot(s16 * pResultData, int *nSize)
{
    u16 nAddr = 0x5000, nAddrNextSF = 0x1A4;
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

    DBG(0, "*** Msg28xx WaterProof Test# TriggerWaterProofOneShot nSF=%d, nAfeOpening=%d, nDriMode=%d, nDriOpening=%d. ***\n", nSF, nAfeOpening, nDriMode, nDriOpening);

    nMaxDataNumOfOneSF = nAfeOpening * nDriOpening;

    RegSet16BitValueOff(0x3D08, BIT8);      ///FIQ_E_FRAME_READY_MASK

    ///polling frame-ready interrupt status
    Msg28xxEnableAdcOneShot();

    while (0x0000 == (nRegDataU16 & BIT8))
    {
        nRegDataU16 = RegGet16BitValue(0x3D18);
    }

    RegSet16BitValueOn(0x3D08, BIT8);      ///FIQ_E_FRAME_READY_MASK
    RegSet16BitValueOn(0x3D08, BIT4);      ///FIQ_E_TIMER0_MASK

    if (ptMutualMpTest->Pattern_type == 1)// for short test
    {
        /// get ALL raw data
    	Msg28xxDBBusReadDQMemStart();
    	//RegGetXByteValue(nAddr, nShotData, 16);
        RegGetXBitValue(nAddr, nShotData, 16, MAX_I2C_TRANSACTION_LENGTH_LIMIT);
        Msg28xxDBBusReadDQMemEnd();

        for (i = 0; i < 8; i++)
        {
            pResultData[i] = (s16)(nShotData[2 * i] | nShotData[2 * i + 1] << 8);
        }
        *nSize = 8;
    }
    else if(ptMutualMpTest->Pattern_type == 3 || ptMutualMpTest->Pattern_type == 4)// for open test
    {
        if(nSF >4)
            nSF = 4;

        /// get ALL raw data, combine and handle datashift.
        for (i = 0; i < nSF; i++)
        {
        	Msg28xxDBBusReadDQMemStart();
            //RegGetXByteValue(nAddr + i * nAddrNextSF, nShotData, 16);
            RegGetXBitValue(nAddr + i * nAddrNextSF, nShotData, 16, MAX_I2C_TRANSACTION_LENGTH_LIMIT);
            Msg28xxDBBusReadDQMemEnd();

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

s32 Msg28xxTriggerMutualOneShot(s16 * pResultData, u16 * pSenNum, u16 * pDrvNum)
{
    int nAddr = 0x5000, nAddrNextSF = 0x1A4;
    u16 nSF = 0, nAfeOpening = 0, nDriOpening = 0;
    u16 nMaxDataNumOfOneSF = 0;
    u16 nDriMode = 0;
    int nDataShift = -1;
    u16 i, j, k;
    u8 nRegData = 0;
    u8 nShotData[392] = {0};//13*15*2
    u16 nRegDataU16 = 0;
    u16 regData = 0, nSwcap = 0;
    s16 * pShotDataAll = NULL;

    DBG(0, "*** %s() ***\n", __func__);

    DbBusIICUseBus();

    nRegData = RegGetLByteValue(0x130A);
    nSF = nRegData>>4;
    nAfeOpening = nRegData & 0x0f;

    if (nSF == 0)
    {
        return -1;
    }

    nRegData = RegGetLByteValue(0x100B);
    nDriMode = nRegData;
    //DEBUG("nDriMode nRegData = %x", nDriMode);

    nRegData = RegGetLByteValue(0x1312);
    nDriOpening = nRegData;

    DBG(0, "*** Msg28xx MP Test# TriggerMutualOneShot nSF=%d, nAfeOpening=%d, nDriMode=%d, nDriOpening=%d. ***\n", nSF, nAfeOpening, nDriMode, nDriOpening);

    nMaxDataNumOfOneSF = nAfeOpening * nDriOpening;

    pShotDataAll = (s16 *)kmalloc(sizeof(s16) * nSF * nMaxDataNumOfOneSF, GFP_KERNEL);

    RegSet16BitValueOff(0x3D08, BIT8);      ///FIQ_E_FRAME_READY_MASK

    regData = RegGet16BitValueByAddressMode(0x1301, ADDRESS_MODE_16BIT);
    nSwcap = (regData & 0x0800);
    if (nSwcap)
    	Msg28xxEnableAdcOneShot();
    else  { // sine mode
    	RegMask16BitValue(0x1E2A, BIT0, 0x0000, ADDRESS_MODE_16BIT);
    	RegMask16BitValue(0x1110, BIT1, BIT1, ADDRESS_MODE_16BIT);
    	RegMask16BitValue(0x1E2A, BIT0, BIT0, ADDRESS_MODE_16BIT);
    }

    mdelay(500);
    ///polling frame-ready interrupt status
    while (0x0000 == (nRegDataU16 & BIT8))
    {
        nRegDataU16 = RegGet16BitValue(0x3D18);
    }


    if (nDriMode == 2) // for short test
    {
        if (nAfeOpening % 2 == 0)
            nDataShift = -1;
        else
            nDataShift = 0;    //special case
        //s16 nShortResultData[nSF][nAfeOpening];

        /// get ALL raw data
        for (i = 0; i < nSF; i++)
        {
        	memset(nShotData, 0, sizeof(nShotData));
        	Msg28xxDBBusReadDQMemStart();
            //RegGetXByteValue(nShotData, nAddr + i * nAddrNextSF, 28);
            RegGetXBitValue(nAddr + i * nAddrNextSF, nShotData, 28, MAX_I2C_TRANSACTION_LENGTH_LIMIT);
            Msg28xxDBBusReadDQMemEnd();

            for (j = 0; j < nAfeOpening; j++)
            {
                pResultData[i*nAfeOpening+j] = (s16)(nShotData[2 * j] | nShotData[2 * j + 1] << 8);

                DBG(0, "nDriMode == 2, pResultData[%d] = %d \n", i*nAfeOpening+j, pResultData[i*nAfeOpening+j]);

                if (nDataShift == 0 && (j == nAfeOpening-1))
                {
                    pResultData[i*nAfeOpening+j] = (s16)(nShotData[2 * (j + 1)] | nShotData[2 * (j + 1) + 1] << 8);
                }
            }
        }

        *pSenNum = nSF;
        *pDrvNum = nAfeOpening;
    }
    else // for open test
    {
        //s16 nOpenResultData[nSF * nAfeOpening][nDriOpening];

        if (nAfeOpening % 2 == 0 || nDriOpening % 2 == 0)
            nDataShift = -1;
        else
            nDataShift = 0;    //special case

        /// get ALL raw data, combine and handle datashift.
        for (i = 0; i < nSF; i++)
        {
        	memset(nShotData, 0, sizeof(nShotData));
        	Msg28xxDBBusReadDQMemStart();
            //RegGetXByteValue(nShotData, nAddr + i * nAddrNextSF, 392);
            RegGetXBitValue(nAddr + i * nAddrNextSF, nShotData, 392, MAX_I2C_TRANSACTION_LENGTH_LIMIT);
            Msg28xxDBBusReadDQMemEnd();

            for (j = 0; j < nMaxDataNumOfOneSF; j++)
            {
                pShotDataAll[i*nMaxDataNumOfOneSF+j] = (s16)(nShotData[2 * j] | nShotData[2 * j + 1] << 8);

                if (nDataShift == 0 && j == (nMaxDataNumOfOneSF - 1))
                    pShotDataAll[i*nMaxDataNumOfOneSF+j] = (s16)(nShotData[2 * (j + 1)] | nShotData[2 * (j + 1) + 1] << 8);
            }
        }

        //problem here
        for (k = 0; k < nSF; k++)
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
    }
    RegSet16BitValueOn(0x3D08, BIT8);      ///FIQ_E_FRAME_READY_MASK
    RegSet16BitValueOn(0x3D08, BIT4);      ///FIQ_E_TIMER0_MASK

    kfree(pShotDataAll);

    return 0;
}

s32 Msg28xxtGetWaterProofOneShotRawIIR(s16 * pRawDataWP, int *nSize)
{
	DBG(0, "*** %s() ***\n", __func__);
    return Msg28xxTriggerWaterProofOneShot(pRawDataWP, nSize);
}


s32 Msg28xxGetMutualOneShotRawIIR(s16 * nResultData, u16 * pSenNum, u16 * pDrvNum)
{
	DBG(0, "*** %s() ***\n", __func__);
    return Msg28xxTriggerMutualOneShot(nResultData, pSenNum, pDrvNum);
}