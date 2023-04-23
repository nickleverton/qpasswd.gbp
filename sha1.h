#ifndef QP_SHA1_H
#define QP_SHA1_H

#include "types.h"
#include <string.h>

#define SHA1HashSize 20

typedef struct SHA1_CTX
{
    u32 Intermediate_Hash[SHA1HashSize/4]; /* Message Digest  */
    u32 Length_Low;            /* Message length in bits      */
    u32 Length_High;           /* Message length in bits      */
    u32 Message_Block_Index;   /* Index into message block array   */
    u8 Message_Block[64];      /* 512-bit message blocks      */
} SHA1_CTX;

extern void SHA1_Init(SHA1_CTX *ctx);
extern void SHA1_Update(SHA1_CTX *ctx, const void *data, u32 size);
extern void SHA1_Final(u8 *result, SHA1_CTX *ctx);

extern void hmac_sha1(u8 *text, u8 *key, u8 *digest);

#endif
