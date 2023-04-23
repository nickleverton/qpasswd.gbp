#ifndef QP_RMD_H
#define QP_RMD_H

#include "types.h"
#include <string.h>

typedef struct {
	u32 MD[5]; /* used for the digest */
	u32 X[16]; /* current 16 bit chunk */
	u32 len;   /* full length */
	u8  D[64]; /* 512 bit data block */
	u32 l;     /* current length of data block -> D[l] */
} RMD160_CTX;

void RMD160_Init(RMD160_CTX *ctx);
void RMD160_Update(RMD160_CTX *ctx, const void *data, u32 size);
void RMD160_Final(u8 *result, RMD160_CTX *ctx);

extern void hmac_rmd160(u8 *text, u8 *key, u8 *digest);

#endif
