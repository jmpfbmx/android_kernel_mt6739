
#ifndef _MT_EEM_TURBO_
#define _MT_EEM_TURBO_
/* #define EEM_ENABLE_VTURBO */
/* #define EEM_ENABLE_TTURBO */
/* #define EEM_ENABLE_ITURBO */


#ifdef __KERNEL__
#if defined(EEM_ENABLE_VTURBO)
	extern unsigned int *get_turbo(unsigned int binLevel, unsigned int binLevelEng);
#endif
#endif
#endif
