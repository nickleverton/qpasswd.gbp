#ifndef QP_MD5_H
#define QP_MD5_H

#include "types.h"
#include <string.h>

#ifdef __dietlibc__

/* dietlibc has md5.h :) */
#include <md5.h>
#define MD5_Init   MD5Init
#define MD5_Update MD5Update
#define MD5_Final  MD5Final

#else
/* glibc ... */
typedef struct {
	u32 lo, hi;
	u32 a,b,c,d;
	u32 block[16];
	u8 buffer[64];
} MD5_CTX;
extern void MD5_Init(MD5_CTX *ctx);
extern void MD5_Update(MD5_CTX *ctx, void *data, u32 size);
extern void MD5_Final(u8 *result, MD5_CTX *ctx);
#endif

extern void hmac_md5(u8 *text, u8 *key, u8 *digest);

#endif
