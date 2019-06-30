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
 
#include "MutualCommon.h"

int ascii_to_hex(char ch)
{
	char ch_tmp;
	int hex_val = -1;

	ch_tmp = tolower(ch);

	if ((ch_tmp >= '0') && (ch_tmp <= '9')) {
		hex_val = ch_tmp - '0';
	}
	else if ((ch_tmp >= 'a') && (ch_tmp <= 'f')) {
		hex_val = ch_tmp - 'a' + 10;
	}

	return hex_val;
}

int str_to_hex(char *hex_str)
{
	int i, len;
	int hex_tmp, hex_val;

	len = strlen(hex_str);
	hex_val = 0;
	for (i=0; i<len;i++) {
		hex_tmp = ascii_to_hex(hex_str[i]);

		if (hex_tmp == -1) {
			return -1;
		}

		hex_val = (hex_val) * 16 + hex_tmp;
	}
	return hex_val;
}

#ifndef DISABLE_DOUBLE
u8 checkDoubleValueInRange(double nValue, double nMax, double nMin)
#else
u8 checkDoubleValueInRange(s32 nValue, s32 nMax, s32 nMin)
#endif
{
   	if (nValue <= nMax && nValue >= nMin)
   	{
   	   	return 1;
   	}
   	else
   	{
   	   	return 0;
   	}
}


u8 CheckValueInRange(s32 nValue, s32 nMax, s32 nMin)
{
   	if (nValue <= nMax && nValue >= nMin)
   	{
   	   	return 1;
   	}
   	else
   	{
   	   	return 0;
   	}
}