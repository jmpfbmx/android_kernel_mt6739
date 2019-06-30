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
 
#ifndef __MY_PARSER_H__
#define __MY_PARSER_H__

#include "../MpCommon.h"
#include "convert_file_op.h"

// This parser accpets the size of 600*512 to store values from ini file.
// It would be doubled in kernel as key and value seprately.
// Strongly suggest that do not modify the size unless you know how much
// size of your kernel stack is.
#define PARSER_MAX_CFG_BUF          512
#define PARSER_MAX_KEY_NUM	        600

#define PARSER_MAX_KEY_NAME_LEN	    100
#define PARSER_MAX_KEY_VALUE_LEN	2000

typedef struct _ST_INI_FILE_DATA
{
	char pSectionName[PARSER_MAX_KEY_NAME_LEN];
	char pKeyName[PARSER_MAX_KEY_NAME_LEN];
	char pKeyValue[PARSER_MAX_KEY_VALUE_LEN];
	int iSectionNameLen;
	int iKeyNameLen;
	int iKeyValueLen;
}ST_INI_FILE_DATA;

extern void my_parser_exit(void);
extern int my_parser(char *path);
extern int ms_getInidata(char *section, char *ItemName, char *returnValue);
extern int ms_atoi(char *nptr);
extern int ms_ini_split_u8_array(char * key, u8 * pBuf);
extern int ms_ini_split_u16_array(char * key, u16 * pBuf);
extern int ms_ini_split_int_array(char * key, int * pBuf);
extern int ms_ini_2d_array(const char * pFile, char * pSection, u16 pArray[][2]);
extern int ms_ini_split_golden(int *pBuf, int line);

#endif