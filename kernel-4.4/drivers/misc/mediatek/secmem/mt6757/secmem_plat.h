
#ifndef SECMEM_PLATFORM_H
#define SECMEM_PLATFORM_H

/* Enable the support of secure memory dump detail */
#define SECMEM_DEBUG_DUMP

/* Enable secure memory test interface */
/* #define SECMEM_DEBUG_INTERFACE */

/* Enable secure memory API */
#if defined(CONFIG_TRUSTONIC_TEE_SUPPORT) \
	|| defined(CONFIG_BLOWFISH_TEE_SUPPORT)
#define SECMEM_KERNEL_API
#endif

#endif				/* end of SECMEM_PLATFORM_H */
