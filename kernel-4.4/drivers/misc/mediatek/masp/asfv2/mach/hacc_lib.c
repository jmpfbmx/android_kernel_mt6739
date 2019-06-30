
#include "sec_osal.h"
#include "sec_hal.h"
#include "hacc_mach.h"
#include "sec_error.h"


#define MOD                         "CE"

#define _CRYPTO_SEED_LEN            (16)

unsigned char masp_hal_secure_algo_init(void)
{
	bool ret = true;

	return ret;
}

unsigned char masp_hal_secure_algo_deinit(void)
{
	bool ret = true;

	return ret;
}

void masp_hal_secure_algo(unsigned char Direction, unsigned char *ContentAddr,
			  unsigned int ContentLen, unsigned char *CustomSeed,
			  unsigned char *ResText)
{
	unsigned int err;
	unsigned char *src, *dst, *seed;
	unsigned int i = 0;

	/* try to get hacc lock */
	do {
		/* If the semaphore is successfully acquired, this function returns 0. */
		err = osal_hacc_lock();
	} while (err != 0);

	/* initialize hacc crypto configuration */
	seed = (unsigned char *)CustomSeed;
	err = masp_hal_sp_hacc_init(seed, _CRYPTO_SEED_LEN);

	if (err != SEC_OK)
		goto _error;

	/* initialize source and destination address */
	src = (unsigned char *)ContentAddr;
	dst = (unsigned char *)ResText;

	/* according to input parameter to encrypt or decrypt */
	switch (Direction) {
	case true:
		/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
		/* ! CCCI driver already got HACC lock ! */
		/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
		dst =
		    masp_hal_sp_hacc_enc((unsigned char *)src, ContentLen, true, HACC_USER3, false);
		break;

	case false:
		/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
		/* ! CCCI driver already got HACC lock ! */
		/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
		dst =
		    masp_hal_sp_hacc_dec((unsigned char *)src, ContentLen, true, HACC_USER3, false);
		break;

	default:
		err = ERR_KER_CRYPTO_INVALID_MODE;
		goto _error;
	}


	/* copy result */
	for (i = 0; i < ContentLen; i++)
		*(ResText + i) = *(dst + i);

	/* try to release hacc lock */
	osal_hacc_unlock();

	return;

_error:
	/* try to release hacc lock */
	osal_hacc_unlock();

	pr_err("[%s] masp_hal_secure_algo error (0x%x)\n", MOD, err);
	BUG_ON(!(0));
}
