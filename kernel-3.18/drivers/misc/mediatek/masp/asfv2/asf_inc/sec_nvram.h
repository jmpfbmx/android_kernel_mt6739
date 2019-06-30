
#ifndef SEC_META_H
#define SEC_META_H

/* used for META library */
#define NVRAM_CIPHER_LEN (16)

typedef struct {
	unsigned char data[NVRAM_CIPHER_LEN];
	unsigned int ret;
} META_CONTEXT;

extern int sec_nvram_enc(META_CONTEXT *meta_ctx);
extern int sec_nvram_dec(META_CONTEXT *meta_ctx);

#endif				/* SEC_META_H */
