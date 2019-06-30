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
 
#include <stdlib.h>
#include "IniLookup.h"
#include "WaterProofTest.h"

s32 Msg30xxgetDeltaC_WP(s32 *pDeltaC, int *nSize)
{
	DEBUG("*** %s() ***\n", __func__);
	int i = 0;
	s16 pRawData[8] = {0};

    if (Msg30xxtGetWaterProofOneShotRawIIR(pRawData, nSize) == -1)
    	return 0;

    for (i = 0; i < 8; i++)
    	pDeltaC[i] = (s32)pRawData[i];

    return 1;
}

s32 Msg30xxWaterProof(s32 *pDeltaC, int *nSize)
{
	DEBUG("*** %s() ***\n", __func__);
	Msg30xxAnaSwReset();

    if (!Msg30xxgetDeltaC_WP(pDeltaC, nSize))
    	return 0;

    return 1;
}

s32 Msg30xxWaterProof_test_judge(s32 *pDeltaC_water, int *nSize)
{
	DEBUG("*** %s() ***\n", __func__);
	int i, nRet = 1;

    for (i = 0; i < *nSize; i++)
    {
    	if (abs(pDeltaC_water[i]) > ptMutualMpTest->sensorInfo.thrsWater)
    		nRet = 0;
    	ptMutualMpTestResult->pWaterProofResultData = abs(pDeltaC_water[i]);
    }
    return nRet;
}

s32 Msg30xxWaterProof_test()
{
	s32 nRetVal = 0;
	u8 time = 0;
	u16 deep_standby = ptMutualMpTest->deep_standby;
	u32 wp_support = 0;
	s32 thrsWP = ptMutualMpTest->sensorInfo.thrsWater;

	int deltaC_water_result[8] = {0}, nSize = 0;

	DEBUG("*** %s() ***\n", __func__);
	DisableFingerTouch();

_retry_waterproof:

	TouchDeviceResetHw();

	EnterDBBus();
	usleep(100000);

	// Start mcu
	StartMCU();

	u16 fmode = WATERPROOF_MODE;
	 if(Msg30xxSwitchFwMode(&fmode, &deep_standby) < 0)
	 {
		 DEBUG("*** Msg30xx Water Proof# SwitchFwMode failed! ***\n");
		 nRetVal = -1;
		 time++;
		 if (time < 10)
			goto _retry_waterproof;
		 else
		   	goto ITO_TEST_END;
	 }

	 StopMCU();

	 wp_support=RegGet16BitValue(0x1402);

     if (wp_support == 0x00008bbd)//// if polling 0X8BBD, means the FW is NOT supporting WP.
     {
    	 DEBUG("Water Proof Test Fail : No supporting the function.");
         nRetVal = -1;
         goto ITO_TEST_END;
     }

     if (!Msg30xxWaterProof(deltaC_water_result, &nSize))
     {
    	 DEBUG("Water Proof Test Fail : (Get Data)");
    	 nRetVal = -1;
    	 goto ITO_TEST_END;
     }

     if (!Msg30xxWaterProof_test_judge(deltaC_water_result, &nSize))
    	 nRetVal = -1;

ITO_TEST_END:

	TouchDeviceResetHw();
    usleep(300000);
    EnableFingerTouch();

	return nRetVal;

}

s32 Msg30xxWaterProofTestEntry(void)
{
    u32 nRetVal= 0;

    DEBUG("*** %s() ***\n", __func__);

    nRetVal = Msg30xxWaterProof_test();

    if (nRetVal == -1)
    	return -1;
    else
    	return 0;
}

int Msg30xxWaterProofTest()
{
	DEBUG("*** %s() ***\n", __func__);

	int nRetVal = 0;
	int nRet = 0;

	nRetVal = Msg30xxWaterProofTestEntry();
	if (nRetVal == 0){
		nRet = ITO_TEST_OK; //PASS
	    DEBUG("Msg30xx Water Proof Test# MP test success\n");
	}else{
	    if(nRetVal == -1){
	    	DEBUG("Msg30xx Water Proof Test# MP test fail\n");
	    	nRet = ITO_TEST_FAIL;
	    }
	    else if (nRetVal == -2){
	    	nRet = ITO_TEST_GET_TP_TYPE_ERROR;
	    }
	    else{
	        nRet = ITO_TEST_UNDEFINED_ERROR;
	    }
	    DEBUG("Msg30xx Water Proof # MP test failed\n");
	}

	return nRet;
}
