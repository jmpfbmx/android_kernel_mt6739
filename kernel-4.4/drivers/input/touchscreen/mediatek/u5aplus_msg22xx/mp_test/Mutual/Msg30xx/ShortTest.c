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
 
#include "ShortTest.h"

extern int _gMsg30xxDeltaC[MAX_MUTUAL_NUM];
int _gMsg30xxSenseNum = 0;
int _gMsg30xxDriveNum = 0;
int _gMsg30xxGRNum = 0;
int _gMsg30xxGRSize = 0;
int _gMsg30xxGRtestPins[14] = {0};

u16 Msg30xxGRPins[14];
u16 Msg30xxtestPin_data[TEST_ITEM_NUM][MAX_MUTUAL_NUM];        //6:max subframe    14:max afe

u16 _gMsg30xxMuxMem_20_3E_0_Settings[16] = {0};
u16 _gMsg30xxMuxMem_20_3E_1_Settings[16] = {0};
u16 _gMsg30xxMuxMem_20_3E_2_Settings[16] = {0};
u16 _gMsg30xxMuxMem_20_3E_3_Settings[16] = {0};
u16 _gMsg30xxMuxMem_20_3E_4_Settings[16] = {0};
u16 _gMsg30xxMuxMem_20_3E_5_Settings[16] = {0};
u16 _gMsg30xxMuxMem_20_3E_6_Settings[16] = {0};

                        //-----------------0------4------8-----12-----16-----20-----24-----28-----32-----36-----40-----44-----48-----52-----56-----60,41pins
u16 MUX_MEM_3036_1_Settings[16] =   { 0x0001,0x0000,0x0020,0x0300,0x4000,0x0000,0x0050,0x0600,0x0000,0x0007,0x0080,0x0000,0x0009,0x0000,0x0000,0x0000 };
u16 MUX_MEM_3036_2_Settings[16] =   { 0x0010,0x0000,0x0200,0x3000,0x0000,0x0004,0x0500,0x0000,0x0006,0x0070,0x0800,0x0000,0x0000,0x0000,0x0000,0x0000 };
u16 MUX_MEM_3036_3_Settings[16] =   { 0x0100,0x0000,0x2000,0x0000,0x0003,0x0400,0x5000,0x0000,0x0060,0x0700,0x8000,0x0000,0x0000,0x0000,0x0000,0x0000 };
u16 MUX_MEM_3036_4_Settings[16] =   { 0x1000,0x0000,0x0000,0x0002,0x0030,0x4000,0x0000,0x0005,0x0600,0x7000,0x0000,0x0008,0x0000,0x0000,0x0000,0x0000 };
u16 MUX_MEM_3036_5_Settings[16] =   { 0x0000,0x1000,0x0000,0x0020,0x0300,0x0000,0x0004,0x0050,0x6000,0x0000,0x0007,0x8000,0x0000,0x0000,0x0000,0x0000 };
                        //-----------------0------4------8-----12-----16-----20-----24-----28-----32-----36-----40-----44-----48-----52-----56-----60,49pins
u16 MUX_MEM_3056_1_Settings[16] =   { 0x0001,0x0020,0x0300,0x4000,0x0000,0x0005,0x0060,0x0700,0x8000,0x0000,0x0009,0x00A0,0x0000,0x0000,0x0000,0x0000 };
u16 MUX_MEM_3056_2_Settings[16] =   { 0x0010,0x0200,0x3000,0x0000,0x0004,0x0050,0x0600,0x7000,0x0000,0x0008,0x0090,0x0A00,0x0000,0x0000,0x0000,0x0000 };
u16 MUX_MEM_3056_3_Settings[16] =   { 0x0100,0x2000,0x0000,0x0003,0x0040,0x0500,0x6000,0x0000,0x0007,0x0080,0x0900,0xA000,0x0000,0x0000,0x0000,0x0000 };
u16 MUX_MEM_3056_4_Settings[16] =   { 0x1000,0x0000,0x0002,0x0030,0x0400,0x5000,0x0000,0x0006,0x0070,0x0800,0x9000,0x0000,0x0000,0x0000,0x0000,0x0000 };
u16 MUX_MEM_3056_5_Settings[16] =   { 0x0000,0x0001,0x0020,0x0300,0x4000,0x0000,0x0005,0x0060,0x0700,0x8000,0x0000,0x0009,0x000A,0x0000,0x0000,0x0000 };

                        //--------------0--1--2--3--4--5--6--7--8--9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29
u16 sensepad_pin_mapping_3036[60] =   { 8, 9,10,11, 0, 0, 0,12, 0,13,14,15,16,17,18,19,20,21,22,23,24, 0,25,26,27,28,29,30,31,32,
                        //-------------30-31-32-33-34-35-36-37-38-39-40-41-42-43-44-45-46-47-48-49-50-51-52-53-54-55-56-57-58-59
                                       33, 0,34,35,36,37,38,39,40,41,42,43,44,45,46, 0, 0,47,48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                        //--------------0--1--2--3--4--5--6--7--8--9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29
u16 sensepad_pin_mapping_3056[60] =   { 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,
                        //-------------30-31-32-33-34-35-36-37-38-39-40-41-42-43-44-45-46-47-48-49-50-51-52-53-54-55-56-57-58-59
                                       38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void Msg30xxSetNoiseSensorMode(u8 nEnable)  //201703xx
{
    DBG(0, "*** %s() ***\n", __func__);

    if (nEnable == ENABLE)
    {
        //RegSet16BitValueByAddressMode(0x1523, BIT4, ADDRESS_MODE_16BIT);
        Msg30xxSetMutualCsubViaDBbus(0);
    }
}

s32 Msg30xxRead_mapping_to_ICpin_for_short_test(void)   //201703xx
{
    int i, j, nMappingItem, nRet = 0;
	u16 MUX_MEM_1_Settings[16] = {0};
    u16 MUX_MEM_2_Settings[16] = {0};
    u16 MUX_MEM_3_Settings[16] = {0};
    u16 MUX_MEM_4_Settings[16] = {0};
    u16 MUX_MEM_5_Settings[16] = {0};

    char *sSupportIC = ptMsg30xxMutualMpTest->UIConfig.sSupportIC;
    
    DBG(0, "*** %s() ***\n", __func__);

    if ((!strcmp(sSupportIC, "3036")) || (!strcmp(sSupportIC, "3040"))) {
			memcpy(MUX_MEM_1_Settings, MUX_MEM_3036_1_Settings, sizeof(_gMsg30xxMuxMem_20_3E_1_Settings));
			memcpy(MUX_MEM_2_Settings, MUX_MEM_3036_2_Settings, sizeof(_gMsg30xxMuxMem_20_3E_2_Settings));
			memcpy(MUX_MEM_3_Settings, MUX_MEM_3036_3_Settings, sizeof(_gMsg30xxMuxMem_20_3E_3_Settings));
			memcpy(MUX_MEM_4_Settings, MUX_MEM_3036_4_Settings, sizeof(_gMsg30xxMuxMem_20_3E_4_Settings));
			memcpy(MUX_MEM_5_Settings, MUX_MEM_3036_5_Settings, sizeof(_gMsg30xxMuxMem_20_3E_5_Settings));
    }
    else
    if (!strcmp(sSupportIC, "3056")) {
	    	memcpy(MUX_MEM_1_Settings, MUX_MEM_3056_1_Settings, sizeof(_gMsg30xxMuxMem_20_3E_1_Settings));
	        memcpy(MUX_MEM_2_Settings, MUX_MEM_3056_2_Settings, sizeof(_gMsg30xxMuxMem_20_3E_2_Settings));
	        memcpy(MUX_MEM_3_Settings, MUX_MEM_3056_3_Settings, sizeof(_gMsg30xxMuxMem_20_3E_3_Settings));
	        memcpy(MUX_MEM_4_Settings, MUX_MEM_3056_4_Settings, sizeof(_gMsg30xxMuxMem_20_3E_4_Settings));
	        memcpy(MUX_MEM_5_Settings, MUX_MEM_3056_5_Settings, sizeof(_gMsg30xxMuxMem_20_3E_5_Settings));
    }
    else
    	return nRet;

	for (nMappingItem = 0; nMappingItem < 6; nMappingItem++)
    {
		u16 testpin = 1;
        u16 index = 0;

        for (i = 0; i < sizeof(MUX_MEM_1_Settings) / sizeof(MUX_MEM_1_Settings[0]); i++)
        {
        	for (j = 0; j < 4; j++)
            {
        		if (nMappingItem == 0)
                {
        			if (((MUX_MEM_1_Settings[i] >> (4 * j)) & 0x0F) != 0)
                    {
        				Msg30xxtestPin_data[nMappingItem][index] = testpin;
                        index++;
                    }
                }
                else if (nMappingItem == 1)
                {
                	if (((MUX_MEM_2_Settings[i] >> (4 * j)) & 0x0F) != 0)
                    {
                		Msg30xxtestPin_data[nMappingItem][index] = testpin;
                		index++;
                    }
                }
                else if (nMappingItem == 2)
                {
                	if (((MUX_MEM_3_Settings[i] >> (4 * j)) & 0x0F) != 0)
                    {
                		Msg30xxtestPin_data[nMappingItem][index] = testpin;
                        index++;
                    }
                }
                else if (nMappingItem == 3)
                {
                	if (((MUX_MEM_4_Settings[i] >> (4 * j)) & 0x0F) != 0)
                    {
                		Msg30xxtestPin_data[nMappingItem][index] = testpin;
                		index++;
                    }
                }
                else if (nMappingItem == 4)
                {
                	if (((MUX_MEM_5_Settings[i] >> (4 * j)) & 0x0F) != 0)
                    {
                		Msg30xxtestPin_data[nMappingItem][index] = testpin;
                		index++;
                    }
                }
                testpin++;
            } // for (j = 0; j < 4; j++)
        } // for (i = 0; i < MUX_MEM_1_Settings.Length; i++)
    } // for (nMappingItem = 0; nMappingItem < 6; nMappingItem++)
	return 1;
}

void Msg30xxChangeANASetting_for_ICpin_short_test(char *sSupportIC) //201703xx
{
    int i, nMappingItem;

    DBG(0, "*** %s() ***\n", __func__);

	if ((!strcmp(ptMsg30xxMutualMpTest->UIConfig.sSupportIC, "3036")) || (!strcmp(ptMsg30xxMutualMpTest->UIConfig.sSupportIC, "3040"))) {
			memcpy(_gMsg30xxMuxMem_20_3E_1_Settings, MUX_MEM_3036_1_Settings, sizeof(_gMsg30xxMuxMem_20_3E_1_Settings));
			memcpy(_gMsg30xxMuxMem_20_3E_2_Settings, MUX_MEM_3036_2_Settings, sizeof(_gMsg30xxMuxMem_20_3E_2_Settings));
			memcpy(_gMsg30xxMuxMem_20_3E_3_Settings, MUX_MEM_3036_3_Settings, sizeof(_gMsg30xxMuxMem_20_3E_3_Settings));
			memcpy(_gMsg30xxMuxMem_20_3E_4_Settings, MUX_MEM_3036_4_Settings, sizeof(_gMsg30xxMuxMem_20_3E_4_Settings));
			memcpy(_gMsg30xxMuxMem_20_3E_5_Settings, MUX_MEM_3036_5_Settings, sizeof(_gMsg30xxMuxMem_20_3E_5_Settings));
	}
	else
	if (!strcmp(ptMsg30xxMutualMpTest->UIConfig.sSupportIC, "3056")) {
	    	memcpy(_gMsg30xxMuxMem_20_3E_1_Settings, MUX_MEM_3056_1_Settings, sizeof(_gMsg30xxMuxMem_20_3E_1_Settings));
	        memcpy(_gMsg30xxMuxMem_20_3E_2_Settings, MUX_MEM_3056_2_Settings, sizeof(_gMsg30xxMuxMem_20_3E_2_Settings));
	        memcpy(_gMsg30xxMuxMem_20_3E_3_Settings, MUX_MEM_3056_3_Settings, sizeof(_gMsg30xxMuxMem_20_3E_3_Settings));
	        memcpy(_gMsg30xxMuxMem_20_3E_4_Settings, MUX_MEM_3056_4_Settings, sizeof(_gMsg30xxMuxMem_20_3E_4_Settings));
	        memcpy(_gMsg30xxMuxMem_20_3E_5_Settings, MUX_MEM_3056_5_Settings, sizeof(_gMsg30xxMuxMem_20_3E_5_Settings));
	}

    for (nMappingItem = 0; nMappingItem < 6; nMappingItem++)
    {
        /// sensor mux sram read/write base address / write length
        RegSet16BitValueByAddressMode(0x2140, 0x00, ADDRESS_MODE_16BIT);    //select sram pad mux
        RegSet16BitValueByAddressMode(0x2148, 0x00, ADDRESS_MODE_16BIT);    //disable sensor mux cen overwrite
        RegSet16BitValueByAddressMode(0x2101, BIT0, ADDRESS_MODE_16BIT);    //reg_mem_soft_rst
        RegSet16BitValueByAddressMode(0x2101, 0x00, ADDRESS_MODE_16BIT);
        RegSet16BitValueByAddressMode(0x2142, 0x07, ADDRESS_MODE_16BIT);    //sensor mux sram write length
        RegSet16BitValueByAddressMode(0x2141, 0x07 * nMappingItem, ADDRESS_MODE_16BIT);
        RegSet16BitValueByAddressMode(0x2143, BIT0, ADDRESS_MODE_16BIT);    //sram write start

        for (i = 0; i < 7; i++)
        {
            if (nMappingItem == 0)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_1_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_1_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                DBG(0, "_gMsg30xxMuxMem_20_3E_1_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_1_Settings[%d] = 0x%04x\n", 2*i, _gMsg30xxMuxMem_20_3E_1_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_1_Settings[2 * i + 1]);
            }
            if (nMappingItem == 1)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_2_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_2_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                DBG(0, "_gMsg30xxMuxMem_20_3E_2_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_2_Settings[%d] = 0x%04x\n", 2*i, _gMsg30xxMuxMem_20_3E_2_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_2_Settings[2 * i + 1]);
            }
            if (nMappingItem == 2)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_3_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_3_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                DBG(0, "_gMsg30xxMuxMem_20_3E_3_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_3_Settings[%d] = 0x%04x\n", 2*i, _gMsg30xxMuxMem_20_3E_3_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_3_Settings[2 * i + 1]);
            }
            if (nMappingItem == 3)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_4_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_4_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                DBG(0, "_gMsg30xxMuxMem_20_3E_4_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_4_Settings[%d] = 0x%04x\n", 2*i, _gMsg30xxMuxMem_20_3E_4_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_4_Settings[2 * i + 1]);
            }
            if (nMappingItem == 4)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_5_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_5_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                DBG(0, "_gMsg30xxMuxMem_20_3E_5_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_5_Settings[%d] = 0x%04x\n", 2*i, _gMsg30xxMuxMem_20_3E_5_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_5_Settings[2 * i + 1]);
            }
            if (nMappingItem == 5)
            {
                memset(_gMsg30xxMuxMem_20_3E_0_Settings, 0, sizeof(_gMsg30xxMuxMem_20_3E_0_Settings));
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_0_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_0_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
            }
        }
    }
}

void Msg30xxChangeANASetting(void)  //201703xx
{
    int i, nMappingItem;

    DBG(0, "*** %s() ***\n", __func__);

    for (nMappingItem = 0; nMappingItem < 6; nMappingItem++)
    {
        /// sensor mux sram read/write base address / write length
        RegSet16BitValueByAddressMode(0x2140, 0x00, ADDRESS_MODE_16BIT);    //select sram pad mux
        RegSet16BitValueByAddressMode(0x2148, 0x00, ADDRESS_MODE_16BIT);    //disable sensor mux cen overwrite
        RegSet16BitValueByAddressMode(0x2101, BIT0, ADDRESS_MODE_16BIT);    //reg_mem_soft_rst
        RegSet16BitValueByAddressMode(0x2101, 0x00, ADDRESS_MODE_16BIT);
        RegSet16BitValueByAddressMode(0x2142, 0x07, ADDRESS_MODE_16BIT);    //sensor mux sram write length
        RegSet16BitValueByAddressMode(0x2141, 0x07 * nMappingItem, ADDRESS_MODE_16BIT);
        RegSet16BitValueByAddressMode(0x2143, BIT0, ADDRESS_MODE_16BIT);    //sram write start

        for (i = 0; i < 7; i++)
        {
            if (nMappingItem == 0)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_1_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_1_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                //DBG(0, "_gMsg30xxMuxMem_20_3E_1_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_1_Settings[%d] = 0x%04x", 2*i, _gMsg30xxMuxMem_20_3E_1_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_1_Settings[2 * i + 1]);
            }
            if (nMappingItem == 1)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_2_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_2_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                //DBG(0, "_gMsg30xxMuxMem_20_3E_2_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_2_Settings[%d] = 0x%04x", 2*i, _gMsg30xxMuxMem_20_3E_2_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_2_Settings[2 * i + 1]);
            }
            if (nMappingItem == 2)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_3_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_3_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                //DBG(0, "_gMsg30xxMuxMem_20_3E_3_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_3_Settings[%d] = 0x%04x", 2*i, _gMsg30xxMuxMem_20_3E_3_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_3_Settings[2 * i + 1]);
            }
            if (nMappingItem == 3)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_4_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_4_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                //DBG(0, "_gMsg30xxMuxMem_20_3E_4_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_4_Settings[%d] = 0x%04x", 2*i, _gMsg30xxMuxMem_20_3E_4_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_4_Settings[2 * i + 1]);
            }
            if (nMappingItem == 4)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_5_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_5_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                //DBG(0, "_gMsg30xxMuxMem_20_3E_5_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_5_Settings[%d] = 0x%04x", 2*i, _gMsg30xxMuxMem_20_3E_5_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_5_Settings[2 * i + 1]);
            }
            if (nMappingItem == 5)
            {
                RegSet16BitValueByAddressMode(0x2144, _gMsg30xxMuxMem_20_3E_6_Settings[2 * i], ADDRESS_MODE_16BIT);
                RegSet16BitValueByAddressMode(0x2145, _gMsg30xxMuxMem_20_3E_6_Settings[2 * i + 1], ADDRESS_MODE_16BIT);
                //DBG(0, "_gMsg30xxMuxMem_20_3E_6_Settings[%d] = 0x%04x, _gMsg30xxMuxMem_20_3E_6_Settings[%d] = 0x%04x", 2*i, _gMsg30xxMuxMem_20_3E_6_Settings[2 * i], 2*i+1, _gMsg30xxMuxMem_20_3E_6_Settings[2 * i + 1]);
            }
        }
    }
}

void Msg30xxAnaLoadSetting_for_ICpin_short_test(char *SupportIC)
{
    // Stop mcu
	StopMCU();
	Msg30xxChangeANASetting_for_ICpin_short_test(SupportIC);
}

void Msg30xxAnaLoadSetting(void)
{
    // Stop mcu
	StopMCU();
	Msg30xxChangeANASetting();
}

void Msg30xxreadSettings(void)
{
    int i, nSize;
    char str[512] = {0};

    DBG(0, "*** %s() ***\n", __func__);

    for (i = 1; i < 6; i++)
    {
    	nSize = 0;
    	if (i == 1)	{
    		//nSize = IniGetU16Array("SHORT_TEST_N1","MUX_MEM_20_3E", _gMsg30xxMuxMem_20_3E_1_Settings);
    		//DBG(0, "SHORT_TEST_N1");
    		//DebugShowArray2(_gMsg30xxMuxMem_20_3E_1_Settings, 16, 16, 16, 16);
			ms_getInidata("SHORT_TEST_N1","MUX_MEM_20_3E",str);
			nSize = ms_ini_split_u16_array(str, _gMsg30xxMuxMem_20_3E_1_Settings);
    		DBG(0, "SHORT_TEST_N1\n");
    		DebugShowArray2(_gMsg30xxMuxMem_20_3E_1_Settings, 16, 16, 16, 16);
    	}
        else if (i == 2) {
        	//nSize = IniGetU16Array("SHORT_TEST_N2","MUX_MEM_20_3E", _gMsg30xxMuxMem_20_3E_2_Settings);
        	//DBG(0, "SHORT_TEST_N2");
        	//DebugShowArray2(_gMsg30xxMuxMem_20_3E_2_Settings, 16, 16, 16, 16);
			ms_getInidata("SHORT_TEST_N2","MUX_MEM_20_3E",str);
			nSize = ms_ini_split_u16_array(str, _gMsg30xxMuxMem_20_3E_2_Settings);
        	DBG(0, "SHORT_TEST_N2\n");
        	DebugShowArray2(_gMsg30xxMuxMem_20_3E_2_Settings, 16, 16, 16, 16);
        }
        else if (i == 3) {
        	//nSize = IniGetU16Array("SHORT_TEST_S1","MUX_MEM_20_3E", _gMsg30xxMuxMem_20_3E_3_Settings);
        	//DBG(0, "SHORT_TEST_S1");
        	//DebugShowArray2(_gMsg30xxMuxMem_20_3E_3_Settings, 16, 16, 16, 16);
			ms_getInidata("SHORT_TEST_S1","MUX_MEM_20_3E",str);
			nSize = ms_ini_split_u16_array(str, _gMsg30xxMuxMem_20_3E_3_Settings);
        	DBG(0, "SHORT_TEST_S1\n");
        	DebugShowArray2(_gMsg30xxMuxMem_20_3E_3_Settings, 16, 16, 16, 16);
        }
        else if (i == 4) {
        	//nSize = IniGetU16Array("SHORT_TEST_S2","MUX_MEM_20_3E", _gMsg30xxMuxMem_20_3E_4_Settings);
        	//DBG(0, "SHORT_TEST_S2");
        	//DebugShowArray2(_gMsg30xxMuxMem_20_3E_4_Settings, 16, 16, 16, 16);
			ms_getInidata("SHORT_TEST_S2","MUX_MEM_20_3E",str);
			nSize = ms_ini_split_u16_array(str, _gMsg30xxMuxMem_20_3E_4_Settings);
        	DBG(0, "SHORT_TEST_S2\n");
        	DebugShowArray2(_gMsg30xxMuxMem_20_3E_4_Settings, 16, 16, 16, 16);
        }
        else if (i == 5) {
        	// if (IniGetU16Array("SHORT_TEST_N3","MUX_MEM_20_3E", _gMsg30xxMuxMem_20_3E_5_Settings)) { ///shortN-3
        	// 	//DBG(0, "SHORT_TEST_N3");
        	// 	//DebugShowArray2(_gMsg30xxMuxMem_20_3E_5_Settings, 16, 16, 16, 16);
        	// 	return;
        	// }
        	// if (IniGetU16Array("SHORT_TEST_S3","MUX_MEM_20_3E", _gMsg30xxMuxMem_20_3E_5_Settings)) { ///shortS-3
        	// 	//DBG(0, "SHORT_TEST_S3");
        	// 	//DebugShowArray2(_gMsg30xxMuxMem_20_3E_5_Settings, 16, 16, 16, 16);
        	// 	return;
        	// }
        	// if (IniGetU16Array("SHORT_TEST_GR","MUX_MEM_20_3E", _gMsg30xxMuxMem_20_3E_5_Settings)) {
        	// 	//DBG(0, "SHORT_TEST_GR");
        	// 	//DebugShowArray2(_gMsg30xxMuxMem_20_3E_5_Settings, 16, 16, 16, 16);
        	// 	return;
        	// }
			if(ms_getInidata("SHORT_TEST_N3","MUX_MEM_20_3E",str) != 0)
			{
				if (ms_ini_split_u16_array(str, _gMsg30xxMuxMem_20_3E_5_Settings)) { ///shortN-3
					DBG(0, "SHORT_TEST_N3\n");
					DebugShowArray2(_gMsg30xxMuxMem_20_3E_5_Settings, 16, 16, 16, 16);
					return;
				}
			}

			if(ms_getInidata("SHORT_TEST_S3","MUX_MEM_20_3E",str) != 0)
			{
				if (ms_ini_split_u16_array(str, _gMsg30xxMuxMem_20_3E_5_Settings)) { ///shortS-3
					DBG(0, "SHORT_TEST_S3\n");
					DebugShowArray2(_gMsg30xxMuxMem_20_3E_5_Settings, 16, 16, 16, 16);
					return;
				}
			}

			if(ms_getInidata("SHORT_TEST_GR","MUX_MEM_20_3E",str) != 0)
			{
				if (ms_ini_split_u16_array(str, _gMsg30xxMuxMem_20_3E_5_Settings)) {
					DBG(0, "SHORT_TEST_GR\n");
					DebugShowArray2(_gMsg30xxMuxMem_20_3E_5_Settings, 16, 16, 16, 16);
					DBG(0, "SHORT_TEST_GR end \n");
					return;
				}
			}            
        }
    }
}

s32 Msg30xxGetValueR(s32 * pTarget, u16 drv_mode) //201703xx
{
    s16 * pRawData = NULL;
    u16 nSenNumBak = 0;
    u16 nDrvNumBak = 0;
    u16 nShift = 0;
    s16 i, j;

    DBG(0, "*** %s() ***\n", __func__);

    pRawData = (s16 *)kcalloc(MAX_MUTUAL_NUM, sizeof(s16), GFP_KERNEL);

    if (Msg30xxGetMutualOneShotRawIIR(pRawData, &nSenNumBak, &nDrvNumBak, drv_mode) < 0)
    {
        printk("*** Msg30xx Short Test# GetMutualOneShotRawIIR failed! ***\n");
        return -1;
    }

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 14; j++)
        {
            nShift = (u16)(j + 14 * i);
            pTarget[nShift] = pRawData[j + 14 * i];
        }
    }

    kfree(pRawData);

    return 0;
}

u16 Msg30xxreadTestPins(u16 itemID, int *testPins)  //201703xx
{
    int nSize = 0, i;
    char str[512] = {0};

	DBG(0, "*** %s() ***\n", __func__);

    switch (itemID)
    {
    	case 0:
        case 10:
        	//nSize = IniGetIntArray("SHORT_TEST_N1", "TEST_PIN", testPins);
			ms_getInidata("SHORT_TEST_N1","TEST_PIN",str);
			nSize = ms_ini_split_int_array(str, testPins);
        	DBG(0, "SHORT_TEST_N1 nSize = %d\n", nSize);
            break;
        case 1:
        case 11:
        	//nSize = IniGetIntArray("SHORT_TEST_N2", "TEST_PIN", testPins);
			ms_getInidata("SHORT_TEST_N2","TEST_PIN",str);
			nSize = ms_ini_split_int_array(str, testPins);
        	DBG(0, "SHORT_TEST_N2 nSize = %d\n", nSize);
            break;
        case 2:
        case 12:
        	//nSize = IniGetIntArray("SHORT_TEST_S1", "TEST_PIN", testPins);
			ms_getInidata("SHORT_TEST_S1","TEST_PIN",str);
			nSize = ms_ini_split_int_array(str, testPins);
        	DBG(0, "SHORT_TEST_S1 nSize = %d\n", nSize);
            break;
        case 3:
        case 13:
        	//nSize = IniGetIntArray("SHORT_TEST_S2", "TEST_PIN", testPins);
			ms_getInidata("SHORT_TEST_S2","TEST_PIN",str);
			nSize = ms_ini_split_int_array(str, testPins);
        	DBG(0, "SHORT_TEST_S2 nSize = %d\n", nSize);
            break;

        case 4:
        case 14:
            // if (IniGetString("SHORT_TEST_N3", "MUX_MEM_20_3E") != 0) {
        	// 	nSize = IniGetIntArray("SHORT_TEST_N3", "TEST_PIN", testPins);
        	// 	DBG(0, "SHORT_TEST_N3 nSize = %d", nSize);
        	// }
            // else if (IniGetString("SHORT_TEST_S3", "MUX_MEM_20_3E") != 0) {
            // 	nSize = IniGetIntArray("SHORT_TEST_S3", "TEST_PIN", testPins);
            // 	DBG(0, "SHORT_TEST_S3 nSize = %d", nSize);
            // }
            // else if (IniGetString("SHORT_TEST_GR", "MUX_MEM_20_3E") != 0) {
            // 	if (ptMsg30xxMutualMpTest->sensorInfo.numGr == 0)
            // 		nSize = 0;
            // 	else
            // 		nSize = _gMsg30xxGRSize;

            // 	for (i = 0; i < sizeof(_gMsg30xxGRtestPins) / sizeof(_gMsg30xxGRtestPins[0]); i++)
            // 		testPins[i] = _gMsg30xxGRtestPins[i];
            // 	DBG(0, "SHORT_TEST_GR nSize = %d", nSize);
            // }
        	if (ms_getInidata("SHORT_TEST_N3", "MUX_MEM_20_3E", str) != 0) {
				ms_getInidata("SHORT_TEST_N3","TEST_PIN",str);
				nSize = ms_ini_split_int_array(str, testPins);
        		DBG(0, "SHORT_TEST_N3 nSize = %d\n", nSize);
        	}
            else if (ms_getInidata("SHORT_TEST_S3", "MUX_MEM_20_3E", str) != 0) {
				ms_getInidata("SHORT_TEST_S3","TEST_PIN",str);
				nSize = ms_ini_split_int_array(str, testPins);				
            	DBG(0, "SHORT_TEST_S3 nSize = %d\n", nSize);
            }
            else if (ms_getInidata("SHORT_TEST_GR", "MUX_MEM_20_3E", str) != 0) {
            	if (ptMsg30xxMutualMpTest->sensorInfo.numGr == 0)
            		nSize = 0;
            	else
            		nSize = _gMsg30xxGRSize;

            	for (i = 0; i < sizeof(_gMsg30xxGRtestPins) / sizeof(_gMsg30xxGRtestPins[0]); i++)
            		testPins[i] = _gMsg30xxGRtestPins[i];
            	DBG(0, "SHORT_TEST_GR nSize = %d\n", nSize);
            }
            break;

        default:
            return 0;
    }

    for (i = nSize; i < MAX_CHANNEL_NUM_30XX; i++) {
    	testPins[i] = PIN_NO_ERROR;
    }

    return nSize;
}

int Msg30xxreadGR_ICpin(void)
{
	int nSize = 0, i;
    char str[512] = {0};

    DBG(0, " *** %s ***\n", __func__);

	for (i = 0; i < sizeof(Msg30xxGRPins) / sizeof(u16); i++) {
		Msg30xxGRPins[i] = 0xFFFF;
	}

	//nSize = IniGetIntArray("SHORT_TEST_GR", "TEST_PIN", _gMsg30xxGRtestPins);
    ms_getInidata("SHORT_TEST_GR","TEST_PIN",str);
    nSize = ms_ini_split_int_array(str, _gMsg30xxGRtestPins);
	for (i = 0; i < 14; i++)
		DBG(0, "_gMsg30xxGRtestPins[%d] = %d\n", i, _gMsg30xxGRtestPins[i]);
	DBG(0, "nSize = %d\n", nSize);
	for (i = 0; i < nSize; i++) {
		Msg30xxGRPins[i] = _gMsg30xxGRtestPins[i];
		DBG(0, "Msg30xxGRPins[%d] = %d\n", i, Msg30xxGRPins[i]);
	}
	return nSize;
}

s32 Msg30xxShortTestJudge_ICpin(u16 nItemID, u16 *deltaC_result, s8 *TestFail, int nGRSize) //201703xx
{
	int nRet = 1, i, count_test_pin = 0, j;
	u16 GR_Id[14] = {0};

	int found_gr = 0, count = 0, BypassGR = 0;

	DBG(0, "*** %s() ***\n", __func__);

	if (!Msg30xxRead_mapping_to_ICpin_for_short_test())
	{
	    printk("Msg30xxRead_mapping_to_ICpin_for_short_test FAIL\n");
		return 0;
	}

	if (ptMsg30xxMutualMpTest->sensorInfo.numGr == 0)
		BypassGR = 1;

	if (nGRSize) {
		for (j = 0; j < sizeof(Msg30xxGRPins) / sizeof(u16); j++) {
			if (Msg30xxGRPins[j] == 0xFFFF)
				continue;
			for (i = 0; i < sizeof(Msg30xxtestPin_data) / sizeof(Msg30xxtestPin_data[0]); i++) {
				if ((Msg30xxtestPin_data[nItemID][i] != PIN_UN_USE) && (Msg30xxtestPin_data[nItemID][i] != 0)) {
					if (Msg30xxtestPin_data[nItemID][i] == (Msg30xxGRPins[j] + 1)) {
						DBG(0, "Msg30xxtestPin_data[%d][%d] = %d\n", nItemID, i, Msg30xxtestPin_data[nItemID][i]);
						GR_Id[count] = i;
				        found_gr = 1;
				        if (count < sizeof(GR_Id) / sizeof(u16))
				        	count++;
				    }
				}
				else
					break;
			}
		}
	}

	count = 0;
	for (i = nItemID * 14; i < (14 + 1) * nItemID; i++) {
		if ((found_gr == 1) && (i == GR_Id[count] + (nItemID * 14)) && (BypassGR == 1)) {
			_gMsg30xxDeltaC[i] = 1;
			if (count < sizeof(GR_Id) / sizeof(u16))
				count++;
		}
	}

	for (i = 0; i < sizeof(Msg30xxtestPin_data) / sizeof(Msg30xxtestPin_data[0]); i++) {
		if (Msg30xxtestPin_data[nItemID][i] != PIN_NO_ERROR)
			count_test_pin++;
	}

	for (i = 0; i < count_test_pin; i++) {
		if (0 == CheckValueInRange(_gMsg30xxDeltaC[i + nItemID * 14], ptMsg30xxMutualMpTest->sensorInfo.thrsICpin, -ptMsg30xxMutualMpTest->sensorInfo.thrsICpin))
		{
		    printk("CheckValueInRange(_gMsg30xxDeltaC[i=%d + nItemID=%d * 14] = %d, ptMsg30xxMutualMpTest->sensorInfo.thrsICpin = %d FAIL\n", i, nItemID, _gMsg30xxDeltaC[i + nItemID * 14], ptMsg30xxMutualMpTest->sensorInfo.thrsICpin);
			TestFail[nItemID] = 1;
			nRet = 0;
		}
	}

	return nRet;
}

s32 Msg30xxShortTestJudge(u8 nItemID, s8 *TestFail, u16 TestFail_check[][MAX_MUTUAL_NUM])   //201703xx
{
	int nRet = 1, i, count_test_pin = 0;
	int testPins[MAX_CHANNEL_NUM_30XX] = {0};

	DBG(0, "*** %s() ***\n", __func__);

	Msg30xxreadTestPins(nItemID, testPins);

	for (i = 0; i < sizeof(testPins) / sizeof(testPins[0]); i++) {
		if (testPins[i] != PIN_NO_ERROR)
			count_test_pin++;
	}

	for (i = 0; i < count_test_pin; i++) {
		TestFail_check[nItemID][i] = testPins[i];
		DBG(0, "testPins[%d]=%d\n", i, testPins[i]);
		if (0 == CheckValueInRange(_gMsg30xxDeltaC[i + nItemID * 14], ptMsg30xxMutualMpTest->sensorInfo.thrsShort, -ptMsg30xxMutualMpTest->sensorInfo.thrsShort))
	    {
	    	TestFail[nItemID] = 1;
	    	nRet = 0;
	    }
	}

	return nRet;
}

#ifdef DISABLE_DOUBLE
int Msg30xxCovertRValue(int dump_time, s32 deltaR) 
#else
double Msg30xxCovertRValue(double dump_time, s32 deltaR)    //201703xx
#endif
{
   	if (deltaR >= IIR_MAX)
   	   	return 0;
   	if (deltaR == 0)
   	    deltaR = 1;
#ifdef DISABLE_DOUBLE
    return (int)(73318 * dump_time) / (55 * (deltaR - 0));
#else
   	return (double)((3.09 - 1.3) * dump_time * 32768 * 1.25) / (50 * ((s32)deltaR - 0) * 1.1);
#endif

}

void Msg30xxScanShiftCodeInitRxCode(u16 codeLength, u16 scantag)
{
    u16 i;
    u16 RegData = 0;

    u16 RxShiftCodeCoef_m4[] =      {0x1000, 0x1000, 0x1000, 0x3000, 0x1000, 0x1000, 0x1000, 0x1000,    // codelength 4
                                     0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m5[] =      {0x0e8b, 0x02e8, 0x1a2e, 0x2ba2, 0x08ba, 0x1000, 0x1000, 0x1000,    // codelength 5
                                     0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m6[] =      {0x3b6d, 0x1249, 0x16db, 0x2492, 0x0db6, 0x0924, 0x1000, 0x1000,    // codelength 6
                                     0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m7[] =      {0x1000, 0x1000, 0x1000, 0x0000, 0x0000, 0x1000, 0x0000, 0x1000,    // codelength 7
                                     0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m8[]  =     {0x071c, 0x1555, 0x071c, 0x071c, 0x38e3, 0x1555, 0x38e3, 0x0e38,    // codelength 8
                                     0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m9[]  =     {0x0dc7, 0x0c72, 0x0ea4, 0x3e67, 0x3e1f, 0x109d, 0x3cf5, 0x07b7,    // codelength 9
                                     0x0550, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m10[] =     {0x064f, 0x13d4, 0x0736, 0x02b4, 0x38c9, 0x13d4, 0x39b0, 0x0ad1,    // codelength 10
                                     0x0000, 0x0ad1, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m11[] =     {0x1555, 0x1555, 0x1555, 0x0000, 0x1555, 0x1555, 0x0000, 0x1555,    // codelength 11
                                     0x0000, 0x0000, 0x0000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m12[] =     {0x06c6, 0x1046, 0x00c0, 0x036a, 0x06c6, 0x0fb2, 0x3939, 0x0d43,    // codelength 12
                                     0x3f3f, 0x066d, 0x3939, 0x08eb, 0x1000, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m13[] =     {0x1127, 0x0d8b, 0x02a3, 0x370a, 0x0b88, 0x117a, 0x3ed5, 0x02a8,    // codelength 13
                                     0x3fb8, 0x09f0, 0x0179, 0x3da6, 0x0053, 0x1000, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m14[] =     {0x05c1, 0x114f, 0x02a9, 0x3c4f, 0x3ff6, 0x10b5, 0x01fe, 0x0e1f,    // codelength 14
                                     0x381b, 0x022f, 0x3fec, 0x0ae5, 0x3d98, 0x0676, 0x1000, 0x1000};
    u16 RxShiftCodeCoef_m15[] =     {0x1000, 0x1000, 0x1000, 0x1000, 0x0000, 0x0000, 0x0000, 0x1000,    // codelength 15
                                     0x0000, 0x0000, 0x1000, 0x1000, 0x0000, 0x1000, 0x0000, 0x1000};
    u16 RxShiftCodeCoef_m16[] =     {0x0b65, 0x3f86, 0x121f, 0x3cb5, 0x0870, 0x3901, 0x052d, 0x079c,    // codelength 16
                                     0x3bcb, 0x057f, 0x3ccc, 0x11d6, 0x383f, 0x0dc9, 0x3d9f, 0x046d};
    u16 RxShiftCodeCoef_m17[] =     {0x0d27, 0x1175, 0x0f91, 0x0ccc, 0x3a71, 0x02b4, 0x01dd, 0x10aa,    // codelength 17
                                     0x0180, 0x3691, 0x0be3, 0x0c41, 0x024b, 0x1343, 0x3dad, 0x0b0c};

    if (codeLength == 17)
        RegSet16BitValueByAddressMode(0x132F, 0x06d6, ADDRESS_MODE_16BIT);
    else
        RegSet16BitValueByAddressMode(0x132F, 0x1000, ADDRESS_MODE_16BIT);          //reg_shift_code_coef_16

    if (scantag == MULTI)
    {
        u16 coefMatrix[16] = {0};
        switch (codeLength)
        {
            case 4:
                memcpy(coefMatrix, RxShiftCodeCoef_m4, sizeof(coefMatrix));
                break;
            case 5:
                memcpy(coefMatrix, RxShiftCodeCoef_m5, sizeof(coefMatrix));
                break;
            case 6:
                memcpy(coefMatrix, RxShiftCodeCoef_m6, sizeof(coefMatrix));
                break;
            case 7:
                memcpy(coefMatrix, RxShiftCodeCoef_m7, sizeof(coefMatrix));
                break;
            case 8:
                memcpy(coefMatrix, RxShiftCodeCoef_m8, sizeof(coefMatrix));
                break;
            case 9:
                memcpy(coefMatrix, RxShiftCodeCoef_m9, sizeof(coefMatrix));
                break;
            case 10:
                memcpy(coefMatrix, RxShiftCodeCoef_m10, sizeof(coefMatrix));
                break;
            case 11:
                memcpy(coefMatrix, RxShiftCodeCoef_m11, sizeof(coefMatrix));
                break;
            case 12:
                memcpy(coefMatrix, RxShiftCodeCoef_m12, sizeof(coefMatrix));
                break;
            case 13:
                memcpy(coefMatrix, RxShiftCodeCoef_m13, sizeof(coefMatrix));
                break;
            case 14:
                memcpy(coefMatrix, RxShiftCodeCoef_m14, sizeof(coefMatrix));
                break;
            case 15:
                memcpy(coefMatrix, RxShiftCodeCoef_m15, sizeof(coefMatrix));
                break;
            case 16:
                memcpy(coefMatrix, RxShiftCodeCoef_m16, sizeof(coefMatrix));
                break;
            case 17:
                memcpy(coefMatrix, RxShiftCodeCoef_m17, sizeof(coefMatrix));
                break;
        }

        for (i = 0; i < 16; i++)
        {
            RegData = coefMatrix[i];
            RegSet16BitValueByAddressMode(0x1330 + i, RegData, ADDRESS_MODE_16BIT);
        }
    }
    else if (scantag == SINGLE)
    {
        for (i = 0; i < 16; i++)
        {
            if (i < (codeLength - 1))
                RegData = 0;
            else
                RegData = 0x1000;
            RegSet16BitValueByAddressMode(0x1330 + i, RegData, ADDRESS_MODE_16BIT);
        }
    }
}

void Msg30xxPrepareAna_for_short_test(void)
{
    u16 SH_ChT = (u16)ptMsg30xxMutualMpTest->SHORT_Charge;
    u16 SH_DuT1 = (u16)ptMsg30xxMutualMpTest->SHORT_Dump1;
    u16 scantag = SINGLE;

	DBG(0, "*** %s() ***\n", __func__);

	// Stop mcu
	StopMCU();

	//set Subframe = 6 ; Sensor = 14
	Msg30xxScanSetSensorNum(MAX_AFE_NUM_30XX);
    Msg30xxScanSetSubframeNum(6);

    // single drive decodeout should be set to 0 => 1
    RegMask16BitValue(0x130D, BIT0 | BIT1 | BIT2 | BIT3 , 0, ADDRESS_MODE_16BIT);
    /// set shift all coef
    RegMask16BitValue(0x130D, BIT12 | BIT13 | BIT14 | BIT15, 0, ADDRESS_MODE_16BIT);
    /// adc analog+digital pipe delay, 60= 14 AFE. need advise, cayenne = 13 afe, camaro = 14 afe
    RegSet16BitValueByAddressMode(0x1029, 0x60, ADDRESS_MODE_16BIT);
    ///trim: Fout 52M &  1.2V
    RegSet16BitValueByAddressMode(0x1410, 0xA55A, ADDRESS_MODE_16BIT);//password
    RegSet16BitValueByAddressMode(0x1414, 0xA55A, ADDRESS_MODE_16BIT);//password
    RegSet16BitValueByAddressMode(0x1411, 0x877C, ADDRESS_MODE_16BIT);//go
    /// reg_drv0_sel_mode : mutual mode
    Msg30xxTgenOvwrInverterMode(DISABLE, DISABLE);
    /// reg_dac0_en, reg_dac0_1p2ldo_en
    RegSet16BitValueByAddressMode(0x1537, BIT0 | BIT1, ADDRESS_MODE_16BIT);
    /// reg_dac_clk_force_on = DAC clock always enabled
    RegSet16BitValueByAddressMode(0x2000, BIT0 | BIT1 | BIT2 | BIT3 | BIT4, ADDRESS_MODE_16BIT);
    Msg30xxSetNoiseSensorMode(ENABLE);
    Msg30xxAnaFixPrs(3);    //prs1
    if (SH_ChT == 0 || SH_DuT1 == 0)
    {
        SH_ChT = 0x007E;
        SH_DuT1 = 0x000C;
    }
    Msg30xxAnaChangeCDtime(SH_ChT, SH_DuT1);
    /// all AFE Cfb use by autosettings.ini setting, defalt (50p)
    RegSet16BitValueByAddressMode(0x1504, 0x3FFF, ADDRESS_MODE_16BIT);// all AFE Cfb: SW control
    Msg30xxSetCfb(_Msg30xx50p); // all AFE Cfb use defalt (50p)

    /// reg_afe_icmp disable
    RegSet16BitValueByAddressMode(0x1529, 0x0000, ADDRESS_MODE_16BIT);
    Msg30xxAnaEnableChargePump(ENABLE);
    ///ADC: AFE Gain bypass
    RegSet16BitValueByAddressMode(0x1230, 0x3FFF, ADDRESS_MODE_16BIT);
    ///reg_chip_swap_pn_en
    Msg30xxSetChipSwapEnable(DISABLE);
    ///reg_adc_desp_invert disable
    Msg30xxAdcDespInverterEnable(DISABLE);
    //RegSet16BitValueByAddressMode(0x1012, 0x0680, ADDRESS_MODE_16BIT); // same as Cobra
    RegSet16BitValueByAddressMode(0x1022, 0x0000, ADDRESS_MODE_16BIT); // Cobra : 4T, Camaro : 1T
    RegSet16BitValueByAddressMode(0x110A, 0x0104, ADDRESS_MODE_16BIT); // default value
    RegSet16BitValueByAddressMode(0x1310, 0x093F, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x1311, 0x093F, ADDRESS_MODE_16BIT);

    RegSet16BitValueByAddressMode(0x1019, 0x083f, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x101a, 0x0000, ADDRESS_MODE_16BIT);  //post idle time in sub0
    RegSet16BitValueByAddressMode(0x101b, 0x0000, ADDRESS_MODE_16BIT);  //post idle time in sub1
    RegSet16BitValueByAddressMode(0x101c, 0x0000, ADDRESS_MODE_16BIT);  //post idle time in sub2
    RegSet16BitValueByAddressMode(0x101d, 0x0000, ADDRESS_MODE_16BIT);  //post idle time in sub3
    // fix shift code
    Msg30xxScanShiftCodeInitRxCode( Msg30xxGet_Drv_opening(), scantag); // set shift code by drive mode
    // re-set sample and coefficient
    //20170111, if short test fout is floating huge, then change sample number try this to keep fout static state.
    Msg30xxScanSetSampleNum(0x20);
    RegSet16BitValueByAddressMode(0x136B, 0x8000, ADDRESS_MODE_16BIT);
    /// set shift fir out -> (1) multi-drive shift code : 1/8 fout (fw : 1/16 -> mp : 1/128) (2) single-drive shift code : 1/16 fout (fw : 1 -> mp : 1/16)
    if (scantag == SINGLE)
        RegMask16BitValue(0x130D, BIT4 | BIT5 | BIT6 | BIT7, BIT6, ADDRESS_MODE_16BIT);  // 2 dac fw -> mp setting
    else
        RegMask16BitValue(0x130D, BIT4 | BIT5 | BIT6 | BIT7, BIT4 | BIT5 | BIT6, ADDRESS_MODE_16BIT);

    Msg30xxTgenOvwrDRVLvBufGainSetting(ENABLE, DISABLE);
    Msg30xxTgenOvwrDRVLvBufCfbSetting(ENABLE, DISABLE);
}

void Msg30xxpatchFWAnaSetting_for_shorttest(void)
{
	int i = 0;

	DBG(0, "*** %s() ***\n", __func__);

    // overwrite sensor PAD , restore to default state
    for (i = 0; i < 8; i++)
    	RegSet16BitValueByAddressMode(0x1e33 + i, 0x0000, ADDRESS_MODE_16BIT);
    // overwrite PAD gpio , restore to default state
    RegSet16BitValueByAddressMode(0x1e30, 0x000f, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x1e31, 0x0000, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x1e32, 0xffff, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x1e3b, 0xffff, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x1e3c, 0xffff, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x1e3d, 0x003f, ADDRESS_MODE_16BIT);
    for (i = 0; i < 20; i++)
    	RegSet16BitValueByAddressMode(0x2110 + i, 0x0000, ADDRESS_MODE_16BIT);
    for (i = 0; i < 16; i++)
       	RegSet16BitValueByAddressMode(0x2160 + i, 0x0000, ADDRESS_MODE_16BIT);
    //post idle for
    RegSet16BitValueByAddressMode(0x101a, 0x0028, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x101b, 0x0028, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x101c, 0x0028, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x101d, 0x0028, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x101e, 0x0028, ADDRESS_MODE_16BIT);
}

s32 Msg30xxICPinShort(void)
{
	//int deltaC[MAX_MUTUAL_NUM] = {0};
    int *deltaC = NULL;
    s16 i;
    u16 drv_mode = 2;

    deltaC = (int *)kzalloc(sizeof(int) * 1904, GFP_KERNEL);

    DBG(0, "*** %s() ***\n", __func__);

    Msg30xxPrepareAna_for_short_test();
    Msg30xxSetSensorPADState(POS_PULSE);
    //DAC0 dati overwrite
    RegSet16BitValueByAddressMode(0x1506, 0x0066, ADDRESS_MODE_16BIT);  //bit0-7 : dac0,  bit8-15 : dac1 //AFE:1.3v for test
    RegSet16BitValueByAddressMode(0x1507, BIT0, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x1542, 0xCAFE, ADDRESS_MODE_16BIT);
    Msg30xxAnaLoadSetting_for_ICpin_short_test(ptMsg30xxMutualMpTest->UIConfig.sSupportIC);
    Msg30xxAnaSwReset();
    //Msg30xxpatchFWAnaSetting_for_shorttest();
    memset(_gMsg30xxDeltaC, 0, sizeof(_gMsg30xxDeltaC));
    if (Msg30xxGetValueR(_gMsg30xxDeltaC, drv_mode)<0) {
        printk("*** Msg30xx IC Pin Short Test# GetValueR failed! ***\n");
        return -1;
    }

    DBG(0, "*** Msg30xx IC Pin Short Test# GetValueR 1.3v! ***\n");
    DebugShowArray2(_gMsg30xxDeltaC, 70, -32, 10, 14);

     ///DAC overwrite
    RegSet16BitValueByAddressMode(0x1506, 0x00FF, ADDRESS_MODE_16BIT);  //bit0-7 : dac0,  bit8-15 : dac1 //AFE:3.09v for test
    if (Msg30xxGetValueR(deltaC, drv_mode)<0) {
    	printk("*** Msg30xx Short Test# GetValueR failed! ***\n");
        return -1;
    }

    DBG(0, "*** Msg30xx IC Pin Short Test# GetValueR 3.09v! ***\n");
    DebugShowArray2(deltaC, 70, -32, 10, 14);
    for (i = 0; i < 70; i++) // 14 AFE * 5 subframe
    {
    	if ((abs(deltaC[i]) < IIR_MAX) && (abs(_gMsg30xxDeltaC[i]) < IIR_MAX))
    		_gMsg30xxDeltaC[i] = deltaC[i] - _gMsg30xxDeltaC[i];
    	else {
    		if (abs(deltaC[i]) > abs(_gMsg30xxDeltaC[i]))
    			_gMsg30xxDeltaC[i] = deltaC[i];
    	}

    	if (abs(_gMsg30xxDeltaC[i]) >= (IIR_MAX))
            _gMsg30xxDeltaC[i] = 0x7FFF;
        else
            _gMsg30xxDeltaC[i] = abs(_gMsg30xxDeltaC[i]);
    }

    DBG(0, "*** Msg30xx IC Pin Short Test# GetValueR 3.09v - 1.3v ! ***");
    DebugShowArray2(_gMsg30xxDeltaC, 70, -32, 10, 14);
    kfree(deltaC);
    return 0;
}

s32 Msg30xxItoShort(void)
{
    int *deltaC = NULL;
	//int deltaC[MAX_MUTUAL_NUM] = {0};
    s16 i;
    u16 drv_mode = 2;

    deltaC = (int *)kzalloc(sizeof(int) * MAX_MUTUAL_NUM, GFP_KERNEL);

    DBG(0, "*** %s() ***\n", __func__);

    Msg30xxPrepareAna_for_short_test();
    Msg30xxSetSensorPADState(POS_PULSE);
    //DAC0 dati overwrite
    RegSet16BitValueByAddressMode(0x1506, 0x0066, ADDRESS_MODE_16BIT);  //bit0-7 : dac0,  bit8-15 : dac1 //AFE:1.3v for test
    RegSet16BitValueByAddressMode(0x1507, BIT0, ADDRESS_MODE_16BIT);
    RegSet16BitValueByAddressMode(0x1542, 0xCAFE, ADDRESS_MODE_16BIT);
    Msg30xxAnaLoadSetting();
    Msg30xxAnaSwReset();
    //Msg30xxpatchFWAnaSetting_for_shorttest();
    memset(_gMsg30xxDeltaC, 0, sizeof(_gMsg30xxDeltaC));
    if (Msg30xxGetValueR(_gMsg30xxDeltaC, drv_mode)<0) {
        printk("*** Msg30xx Ito Short Test# GetValueR failed! ***\n");
        return -1;
    }

    DBG(0, "*** Msg30xx Ito Short Test# GetValueR 1.3v! ***\n");
    DebugShowArray2(_gMsg30xxDeltaC, 70, -32, 10, 14);

    ///DAC overwrite
    RegSet16BitValueByAddressMode(0x1506, 0x00FF, ADDRESS_MODE_16BIT);  //bit0-7 : dac0,  bit8-15 : dac1 //AFE:3.09v for test
    if (Msg30xxGetValueR(deltaC, drv_mode)<0) {
        printk("*** Msg30xx Short Test# GetValueR failed! ***\n");
        return -1;
    }

    DBG(0, "*** Msg30xx Ito Short Test# GetValueR 3.09v! ***\n");
    DebugShowArray2(deltaC, 70, -32, 10, 14);
    for (i = 0; i < 70; i++) // 14 AFE * 5 subframe
    {
        if ((abs(deltaC[i]) < IIR_MAX) && (abs(_gMsg30xxDeltaC[i]) < IIR_MAX))
            _gMsg30xxDeltaC[i] = deltaC[i] - _gMsg30xxDeltaC[i];
        else {
            if (abs(deltaC[i]) > abs(_gMsg30xxDeltaC[i]))
                _gMsg30xxDeltaC[i] = deltaC[i];
        }

        if (abs(_gMsg30xxDeltaC[i]) >= (IIR_MAX))
            _gMsg30xxDeltaC[i] = 0x7FFF;
        else
            _gMsg30xxDeltaC[i] = abs(_gMsg30xxDeltaC[i]);
    }

    DBG(0, "*** Msg30xx Ito Short Test# GetValueR 3.09v - 1.3v ! ***");
    DebugShowArray2(_gMsg30xxDeltaC, 70, -32, 10, 14);
    kfree(deltaC);
    return 0;
}

#ifdef DISABLE_DOUBLE
s32 Msg30xxICpin_short_test_result_prepare(int thrs, int *senseR)
#else
s32 Msg30xxICpin_short_test_result_prepare(double thrs, double *senseR)
#endif
{
	int count = 0, i, nRet = 0;

	u16 pinTBL[MAX_CHANNEL_NUM_30XX];

	DBG(0, "*** %s() ***\n", __func__);

    if ((!strcmp(ptMsg30xxMutualMpTest->UIConfig.sSupportIC, "3036")) || (!strcmp(ptMsg30xxMutualMpTest->UIConfig.sSupportIC, "3040"))) {
        memcpy(pinTBL, sensepad_pin_mapping_3036, sizeof(pinTBL));
    }
    else
    if (!strcmp(ptMsg30xxMutualMpTest->UIConfig.sSupportIC, "3056")) {
        memcpy(pinTBL, sensepad_pin_mapping_3056, sizeof(pinTBL));
    }

    for (i = 0; i < MAX_CHANNEL_NUM_30XX; i++) {
    	ptMsg30xxMutualMpTestResult->pICPinChannel[i] = 0;
    	ptMsg30xxMutualMpTestResult->pICPinShortFailChannel[i] = 0;
    	if (pinTBL[i] != 0) {
    		ptMsg30xxMutualMpTestResult->pICPinChannel[i] = (int)pinTBL[i];
    		ptMsg30xxMutualMpTestResult->pICPinShortRData[i] = senseR[i];
    		DBG(0, "pICPinChannel[%d] = %d, IC Pin P[%d] = %.2dM, deltaC = %d\n", i, ptMsg30xxMutualMpTestResult->pICPinChannel[i], pinTBL[i], senseR[i], ptMsg30xxMutualMpTestResult->pICPinShortResultData[i]);
    		if (senseR[i] < thrs) {
    			ptMsg30xxMutualMpTestResult->pICPinShortFailChannel[i] = (int)pinTBL[i];
                DBG(0, "IC Pin Fail P%d = %.2dM\n", pinTBL[i], senseR[i]);
                count++;
                nRet = -1;
            }
        }
    } // for (int i = 0; i < senseR.Length; i++)
    return nRet;
}

s32 Msg30xxICPinShortTest(void) //201703xx
{
	 s16 i = 0, j = 0;
	 int nGRSize = 0;
#ifdef DISABLE_DOUBLE
     int *senseR = (int *)kmalloc(MAX_CHANNEL_NUM_30XX * sizeof(int), GFP_KERNEL);
	 int thrs = 0, dump_time;
#else
	 double *senseR = (double *)malloc(MAX_CHANNEL_NUM_30XX * sizeof(double));
	 double thrs = 0.0f, dump_time;
#endif

	 s8 normalTestFail[TEST_ITEM_NUM] = {0};    //0:golden    1:ratio
	 int count_test_pin = 0;

	 u16 nTestItemLoop = 6;
	 u16 nTestItem = 0;
	 s32 nRetVal = 0;

	 u8 time = 0;
	 u16 deep_standby = ptMsg30xxMutualMpTest->deep_standby;
     u16 fmode;

	 DBG(0, "*** %s() ***\n", __func__);
	 DrvDisableFingerTouchReport();

_retry_icpin_short:

	DrvTouchDeviceHwReset();

	EnterDBBus();
	mdelay(100);

	// Start mcu
	StartMCU();
	fmode = MUTUAL_SINGLE_DRIVE;
	 if(Msg30xxSwitchFwMode(&fmode, &deep_standby) < 0)
	 {
		 printk("*** Msg30xx Open Test# SwitchFwMode failed! ***\n");
		 time++;
		 if (time < 10)
			goto _retry_icpin_short;
		 else {
			ptMsg30xxMutualMpTestResult->pCheck_Fail[2] = 1;	// ic pin short fail
			nRetVal = -1;
		   	goto ITO_TEST_END;
		 }
	 }

	 // Stop mcu
	 StopMCU();

	 for (i = 0; i < MAX_CHANNEL_NUM_30XX; i++) {
		 senseR[i] = PIN_NO_ERROR;
	 }
#ifdef DISABLE_DOUBLE
     dump_time = (int)((ptMsg30xxMutualMpTest->SHORT_Dump1 + 1) * 4 * 2) / 52;
#else
	 dump_time = (double)((ptMsg30xxMutualMpTest->SHORT_Dump1 + 1) * 4 * 2) / 51.66;
#endif

	 thrs = Msg30xxCovertRValue(dump_time, ptMsg30xxMutualMpTest->sensorInfo.thrsICpin);

	 for (i = 0; i < 6; i++)  {//max 6 subframe
		 for (j = 0; j < 14; j++) {// max 14 AFE
			 if ((i * 14 + j) < MAX_CHANNEL_NUM_30XX)	// prevent heap corruption detected
				 ptMsg30xxMutualMpTestResult->pICPinShortFailChannel[i * 14 + j] = (int)PIN_UN_USE;
			 Msg30xxtestPin_data[i][j] = PIN_UN_USE;
		 }
	 }

	 //N1_ShortTest
	 if(Msg30xxICPinShort() < 0)
	 {
	     printk("*** Msg30xx Short Test# Get DeltaC failed! ***\n");
	     ptMsg30xxMutualMpTestResult->pCheck_Fail[2] = 1;	// ic pin short fail
	     nRetVal = -1;
	     goto ITO_TEST_END;
	 }
	 nGRSize = _gMsg30xxGRSize; //Msg30xxreadGR_ICpin();
	 for (nTestItem = 0; nTestItem < nTestItemLoop; nTestItem++) {
		if (!Msg30xxShortTestJudge_ICpin(nTestItem, (u16 *)_gMsg30xxDeltaC, normalTestFail, nGRSize)) {
		    printk("Msg30xxShortTestJudge_ICpin FAIL\n");
			ptMsg30xxMutualMpTestResult->pCheck_Fail[2] = 1;	// ic pin short fail
			nRetVal = -1;
			//goto ITO_TEST_END;
	    }

	    count_test_pin = 0;

	    for (i = 0; i < 14; i++) {
	    	if (Msg30xxtestPin_data[nTestItem][i] != PIN_UN_USE)
	    	{
	    		//printk("normalTestFail_check[%d, %d] = %d", nTestItem, i, Msg30xxtestPin_data[nTestItem][i]);
	    		count_test_pin++;
	    	}
	    }
	    if ((nTestItem >= 0) && (nTestItem < 6))
	    {
	    	for (i = 0; i < count_test_pin; i++) {
	    		if ((Msg30xxtestPin_data[nTestItem][i] > 0) && (Msg30xxtestPin_data[nTestItem][i] <= MAX_CHANNEL_NUM_30XX)) {
	    			senseR[Msg30xxtestPin_data[nTestItem][i] - 1] = Msg30xxCovertRValue(dump_time, _gMsg30xxDeltaC[i + nTestItem * 14]);
	    			ptMsg30xxMutualMpTestResult->pICPinShortResultData[Msg30xxtestPin_data[nTestItem][i] - 1] = _gMsg30xxDeltaC[i + nTestItem * 14];
	                DBG(0, "senseR[%d]={%.2d}, _gMsg30xxDeltaC[{%d}]={%d}\n", Msg30xxtestPin_data[nTestItem][i] - 1, senseR[Msg30xxtestPin_data[nTestItem][i] - 1], i + nTestItem * 14, _gMsg30xxDeltaC[i + nTestItem * 14]);
	    		}
	            else {
	            	nRetVal = -1;
	            	ptMsg30xxMutualMpTestResult->pCheck_Fail[2] = 1;	// ic pin short fail
	            	//goto ITO_TEST_END;
	            }
	    	}
	    } // if ((nTestItem > 0) && (nTestItem < 6))
	 }

	 if (Msg30xxICpin_short_test_result_prepare(thrs, senseR) < 0) {
	     DBG(0, "Msg30xxICpin_short_test_result_prepare\n");
		 ptMsg30xxMutualMpTestResult->pCheck_Fail[2] = 1;	// ic pin short fail
		 nRetVal = -1;
	 }

	 ExitDBBus();

ITO_TEST_END:

	 DrvTouchDeviceHwReset();
	 mdelay(300);
	 DrvEnableFingerTouchReport();
	 kfree(senseR);
	 return nRetVal;
}

s8 normalTestFail_30xx[TEST_ITEM_NUM] = {0};//0:golden    1:ratio
u16 normalTestFail_check_30xx[TEST_ITEM_NUM][MAX_MUTUAL_NUM];//6:max subframe    14:max afe

s32 Msg30xxItoShortTest(void)
{
	 s16 i = 0, j = 0;

#ifdef DISABLE_DOUBLE
	 int *senseR = (int *)kmalloc(_gMsg30xxSenseNum * sizeof(int), GFP_KERNEL);
	 int *driveR = (int *)kmalloc(_gMsg30xxSenseNum * sizeof(int), GFP_KERNEL);
	 int *GRR = (int *)kmalloc(_gMsg30xxSenseNum * sizeof(int), GFP_KERNEL);
	 int thrs = 0, dump_time;
#else
	 double *senseR = (double *)malloc(_gMsg30xxSenseNum * sizeof(double));
	 double *driveR = (double *)malloc(_gMsg30xxDriveNum * sizeof(double));
	 double *GRR = (double *)malloc(_gMsg30xxGRNum * sizeof(double));
	 double thrs = 0.0f, dump_time;
#endif

	 u16 Temp_20_3E_Settings[16] = {0};

	 int count_test_pin = 0;

	 u16 nTestItemLoop = 6 - 1;
	 u16 nTestItem = 0;
	 u32 nRetVal = 0;

	 u8 time = 0;
	 u16 deep_standby = ptMsg30xxMutualMpTest->deep_standby;
     u16 fmode;
     char str[512] = {0};

	 DBG(0, "*** %s() ***\n", __func__);
	 DrvDisableFingerTouchReport();

_retry_short:

    DrvTouchDeviceHwReset();

	EnterDBBus();
	mdelay(100);

	// Start mcu
	StartMCU();
	fmode = MUTUAL_SINGLE_DRIVE;
     if(Msg30xxSwitchFwMode(&fmode, &deep_standby) < 0)
     {
         printk("*** Msg30xx Ito Short Test# SwitchFwMode failed! ***\n");
         time++;
         if (time < 10)
            goto _retry_short;
         else {
            nRetVal = -1;
            goto ITO_TEST_END;
         }
     }

     // Stop mcu
     StopMCU();

#ifdef DISABLE_DOUBLE
    dump_time = (int)((ptMsg30xxMutualMpTest->SHORT_Dump1 + 1) * 4 * 2) / 52;
#else
    dump_time = (double)((ptMsg30xxMutualMpTest->SHORT_Dump1 + 1) * 4 * 2) / 51.66;   
#endif
     thrs = Msg30xxCovertRValue(dump_time, ptMsg30xxMutualMpTest->sensorInfo.thrsShort);

     for (i = 0; i < _gMsg30xxSenseNum; i++) senseR[i] = thrs;
     for (i = 0; i < _gMsg30xxDriveNum; i++) driveR[i] = thrs;
     for (i = 0; i < _gMsg30xxGRNum; i++) GRR[i] = thrs;

     for (i = 1; i < 6; i++)  {//max 6 subframe
         for (j = 0; j < 14; j++) {// max 14 AFE
             if ((i * 14 + j) < MAX_CHANNEL_NUM_30XX)
                 ptMsg30xxMutualMpTestResult->pShortFailChannel[i * 14 + j] = (u32)PIN_UN_USE;
             normalTestFail_check_30xx[i][j] = PIN_UN_USE;
         }
     }
     //N1_ShortTest
     if(Msg30xxItoShort() < 0)
     {
         printk("*** Msg30xx Ito Short Test# Get DeltaC failed! ***\n");
         nRetVal = -1;
         goto ITO_TEST_END;
     }

     for (nTestItem = 0; nTestItem < nTestItemLoop; nTestItem++) {
        if (!Msg30xxShortTestJudge(nTestItem, normalTestFail_30xx, normalTestFail_check_30xx)) {
            printk("*** Msg30xx Ito Short Test# ShortTestJudge failed! ***\n");
            nRetVal = -1;
            //goto ITO_TEST_END;
        }

        count_test_pin = 0;

        for (i = 0; i < 14; i++) {
            //printk("normalTestFail_check[%d][%d] = %x", nTestItem, i, normalTestFail_check[nTestItem][i]);
            if (normalTestFail_check_30xx[nTestItem][i] != PIN_UN_USE)
            {
                count_test_pin++;
            }
        }

        DBG(0, "nTestItem = %d, count_test_pin = %d\n", nTestItem, count_test_pin);

        memset(Temp_20_3E_Settings, 0, sizeof(Temp_20_3E_Settings));
        ms_getInidata("SHORT_TEST_N3", "MUX_MEM_20_3E", str);


        if (nTestItem == 0 || nTestItem == 1 || (nTestItem == 5 && (ms_ini_split_u16_array(str, Temp_20_3E_Settings)))) {
            DBG(0, "SHORT_TEST_N3");
            for (i = 0; i < count_test_pin; i++) {
                for (j = 0; j < _gMsg30xxSenseNum; j++) {
                    if (normalTestFail_check_30xx[nTestItem][i] == ptMsg30xxMutualMpTest->PAD2Sense[j]) {
                        senseR[j] = Msg30xxCovertRValue(dump_time, _gMsg30xxDeltaC[i + nTestItem  * 14]);
                        DBG(0, "senseR[%d] = %.2d, _gMsg30xxDeltaC[%d] = %d\n", j, senseR[j], i + nTestItem * 14, _gMsg30xxDeltaC[i + nTestItem * 14]);
                        ptMsg30xxMutualMpTestResult->pShortRData[j] = senseR[j];
                        ptMsg30xxMutualMpTestResult->pShortResultData[j] = _gMsg30xxDeltaC[i + nTestItem * 14];
                        if (senseR[j] >= 10)
                            ptMsg30xxMutualMpTestResult->pShortRData[j] = 10;

                        if (0 == CheckValueInRange(_gMsg30xxDeltaC[i + nTestItem  * 14], ptMsg30xxMutualMpTest->sensorInfo.thrsShort, -ptMsg30xxMutualMpTest->sensorInfo.thrsShort))
                        {
                            ptMsg30xxMutualMpTestResult->pShortFailChannel[j] = (u32)normalTestFail_check_30xx[nTestItem][i];
                            DBG(0, "Ito Short senseR, count_test_pin = %d, normalTestFail_check_30xx[%d][%d] = %d, pShortFailChannel[%d] = %d, _gMsg30xxDeltaC[%d] = %d\n", count_test_pin, nTestItem, i, normalTestFail_check_30xx[nTestItem][i], j, ptMsg30xxMutualMpTestResult->pShortFailChannel[j], i + nTestItem * 14, _gMsg30xxDeltaC[i + nTestItem * 14]);
                        }
                    } // if (normalTestFail_check[nTestItem, i]....
                } // for (j = 0; j < sizeof(sens...
            } // for (i = 0; i < count_test_pin; i++)
        } // if (nTestItem == 1 || nTe....

        memset(Temp_20_3E_Settings, 0, sizeof(Temp_20_3E_Settings));
        ms_getInidata("SHORT_TEST_S3", "MUX_MEM_20_3E", str);

        if (nTestItem == 2 || nTestItem == 3 || (nTestItem == 4 && (ms_ini_split_u16_array(str, Temp_20_3E_Settings)))) {
            DBG(0, "SHORT_TEST_S3\n");
            for (i = 0; i < count_test_pin; i++) {
                for (j = 0; j < _gMsg30xxDriveNum; j++) {
                    if (normalTestFail_check_30xx[nTestItem][i] == ptMsg30xxMutualMpTest->PAD2Drive[j]) {
                        driveR[j] = Msg30xxCovertRValue(dump_time, _gMsg30xxDeltaC[i + nTestItem * 14]);
                        DBG(0, "driveR[%d] = %.2d, _gMsg30xxDeltaC[%d] = %d\n", j, driveR[j], i + nTestItem * 14, _gMsg30xxDeltaC[i + nTestItem * 14]);
                        ptMsg30xxMutualMpTestResult->pShortRData[_gMsg30xxSenseNum + j] = driveR[j];
                        ptMsg30xxMutualMpTestResult->pShortResultData[_gMsg30xxSenseNum + j] = _gMsg30xxDeltaC[i + nTestItem * 14];
                        if (driveR[j] >= 10)
                            ptMsg30xxMutualMpTestResult->pShortRData[_gMsg30xxSenseNum + j] = 10;
                        if (0 == CheckValueInRange(_gMsg30xxDeltaC[i + nTestItem * 14], ptMsg30xxMutualMpTest->sensorInfo.thrsShort, -ptMsg30xxMutualMpTest->sensorInfo.thrsShort))
                        {
                            ptMsg30xxMutualMpTestResult->pShortFailChannel[_gMsg30xxSenseNum + j] = (u32)normalTestFail_check_30xx[nTestItem][i];
                            DBG(0, "Ito Short driveR, count_test_pin = %d, normalTestFail_check_30xx[%d][%d] = %d, pShortFailChannel[%d] = %d, _gMsg30xxDeltaC[%d] = %d\n", count_test_pin, nTestItem, i, normalTestFail_check_30xx[nTestItem][i], _gMsg30xxSenseNum+j, ptMsg30xxMutualMpTestResult->pShortFailChannel[_gMsg30xxSenseNum+j], i + nTestItem * 14, _gMsg30xxDeltaC[i + nTestItem * 14]);
                        }
                    } // if (normalTestFail_check[nTestItem, i] ....
                } // for (j = 0; j < sizeof(driveR....
            } // for (i = 0; i < count_test_pin; i++)
        } // if (nTestItem == 3 || nTe....

        memset(Temp_20_3E_Settings, 0, sizeof(Temp_20_3E_Settings));
        ms_getInidata("SHORT_TEST_GR", "MUX_MEM_20_3E", str);

        if (nTestItem == 4 && (ms_ini_split_u16_array(str, Temp_20_3E_Settings))) {
            for (i = 0; i < count_test_pin; i++) {
                for (j = 0; j < _gMsg30xxGRNum; j++) {
                    DBG(0, "normalTestFail_check_30xx[nTestItem = %d][i = %d] = %d, ptMsg30xxMutualMpTest->PAD2GR[j = %d] = %d\n", nTestItem, i, normalTestFail_check_30xx[nTestItem][i], j, ptMsg30xxMutualMpTest->PAD2GR[j]);
                    if (normalTestFail_check_30xx[nTestItem][i] == ptMsg30xxMutualMpTest->PAD2GR[j]) {
                        GRR[j] = Msg30xxCovertRValue(dump_time, _gMsg30xxDeltaC[i + nTestItem * 14]);
                        ptMsg30xxMutualMpTestResult->pShortRData[_gMsg30xxSenseNum + _gMsg30xxDriveNum + j] = GRR[j];
                        ptMsg30xxMutualMpTestResult->pShortResultData[_gMsg30xxSenseNum + _gMsg30xxDriveNum + j] = _gMsg30xxDeltaC[i + nTestItem * 14];
                        if (GRR[j] >= 10)
                            ptMsg30xxMutualMpTestResult->pShortRData[_gMsg30xxSenseNum + _gMsg30xxDriveNum + j] = 10.0;
                        if (0 == CheckValueInRange(_gMsg30xxDeltaC[i + nTestItem * 14], ptMsg30xxMutualMpTest->sensorInfo.thrsShort, -ptMsg30xxMutualMpTest->sensorInfo.thrsShort))
                        {
                            ptMsg30xxMutualMpTestResult->pShortFailChannel[_gMsg30xxSenseNum + _gMsg30xxDriveNum + j] = (u32)normalTestFail_check_30xx[nTestItem][i];
                            DBG(0, "Ito Short GRR, count_test_pin = %d, normalTestFail_check_30xx[%d][%d] = %d, pShortFailChannel[%d] = %d, _gMsg30xxDeltaC[%d] = %d\n", count_test_pin, nTestItem, i, normalTestFail_check_30xx[nTestItem][i], _gMsg30xxSenseNum+j, ptMsg30xxMutualMpTestResult->pShortFailChannel[_gMsg30xxSenseNum+j], i + nTestItem * 14, _gMsg30xxDeltaC[i + nTestItem * 14]);
                        }
                    } // if (normalTestFail_check[nTestItem, i]
                } // for (j = 0; j < sizeof(GRR)
            } // for (i = 0; i < count_test_pin; i++)
        } // if (nTestItem == 4 &&

        if (normalTestFail_30xx[nTestItem]) {
            ptMsg30xxMutualMpTestResult->pCheck_Fail[3] = normalTestFail_30xx[nTestItem];	// ito short fail
            nRetVal = -1;
        }
     }

     ExitDBBus();

ITO_TEST_END:

     DrvTouchDeviceHwReset();
     mdelay(300);;
     DrvEnableFingerTouchReport();
     kfree(senseR);
     kfree(driveR);
     kfree(GRR);
     return nRetVal;
}

s32 Msg30xxShortTestEntry(void)
{
    u32 nRetVal1 = 0, nRetVal2 = 0;

    DBG(0, "*** %s() ***\n", __func__);

    Msg30xxreadSettings();
    _gMsg30xxSenseNum = ptMsg30xxMutualMpTest->sensorInfo.numSen;
    _gMsg30xxDriveNum = ptMsg30xxMutualMpTest->sensorInfo.numDrv;
    _gMsg30xxGRNum = ptMsg30xxMutualMpTest->sensorInfo.numGr;
    DBG(0, "_gMsg30xxSenseNum = %d, _gMsg30xxDriveNum = %d, _gMsg30xxGRNum = %d\n", _gMsg30xxSenseNum, _gMsg30xxDriveNum, _gMsg30xxGRNum);
    _gMsg30xxGRSize = Msg30xxreadGR_ICpin();	//must execute once only , or it will return invalid buffer data (IniGetIntArray("SHORT_TEST_GR", "TEST_PIN", testPins))
    nRetVal1 = Msg30xxItoShortTest();
    nRetVal2 = Msg30xxICPinShortTest();

    DBG(0, "nRetVal1 = %d, nRetVal2 = %d\n", nRetVal1, nRetVal2);

    if ((nRetVal1 == -1) || (nRetVal2 == -1))
    	return -1;
    else
    	return 0;
}

int Msg30xxShortTest(void)
{
	int nRetVal = 0;
	int nRet = 0;

    DBG(0, "*** %s() ***\n", __func__);

	nRetVal = Msg30xxShortTestEntry();
	if (nRetVal == 0){
		nRet = ITO_TEST_OK; //PASS
	    printk("Msg30xx Short Test# MP test success\n");
	}else{
	    if(nRetVal == -1){
	    	printk("Msg30xx Short Test# MP test fail\n");
	    	nRet = ITO_TEST_FAIL;
	    }
	    else if (nRetVal == -2){
	    	nRet = ITO_TEST_GET_TP_TYPE_ERROR;
	    }
	    else{
	        nRet = ITO_TEST_UNDEFINED_ERROR;
	    }

	    printk("Msg30xx Short# MP test failed\n");
	}

	return nRet;
}
