
#ifndef __STKCHKCODE_H__
#define __STKCHKCODE_H__
#define STKLSB			256
#define CHECK_CODE_SIZE  (STKLSB + 1)
extern int get_hw_ver(void);
static int stkcheckcode[CHECK_CODE_SIZE][CHECK_CODE_SIZE];

static int STK8BAXX_CheckCode(s16 acc[])
{
	int a, b;

	if (acc[0] > 0)
		a = acc[0];
	else
		a = -acc[0];
	if (acc[1] > 0)
		b = acc[1];
	else
		b = -acc[1];
	if (a >= CHECK_CODE_SIZE || b >= CHECK_CODE_SIZE)
		acc[2] = 0;
	else
		acc[2] = (s16) stkcheckcode[a][b];
	return 0;
}

#endif				/* #ifndef __STKCHKCODE_H__ */
