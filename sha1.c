#include "sha1.h"

#if WANT_CRAM_SHA1

/*  define the SHA1 circular left shift macro */
#define SHA1CircularShift(bits,word) \
	(((word) << (bits)) | ((word) >> (32-(bits))))

static void SHA1PadMessage(SHA1_CTX *);
static void SHA1ProcessMessageBlock(SHA1_CTX *);

void SHA1_Init(SHA1_CTX *ctx)
{
	ctx->Length_Low             = 0;
	ctx->Length_High            = 0;
	ctx->Message_Block_Index    = 0;
	ctx->Intermediate_Hash[0]   = 0x67452301;
	ctx->Intermediate_Hash[1]   = 0xEFCDAB89;
	ctx->Intermediate_Hash[2]   = 0x98BADCFE;
	ctx->Intermediate_Hash[3]   = 0x10325476;
	ctx->Intermediate_Hash[4]   = 0xC3D2E1F0;
}

void SHA1_Update(SHA1_CTX *ctx, const void *data, u32 size)
{
	const u8 *ptr=data;

	while (size--) {
		ctx->Message_Block[ctx->Message_Block_Index++] = (*ptr & 0xFF);
		ctx->Length_Low += 8;

		if (ctx->Length_Low == 0) {
			ctx->Length_High++;
			if (ctx->Length_High == 0) return; /* ERR */
		}

		if (ctx->Message_Block_Index == 64) {
			SHA1ProcessMessageBlock(ctx);
		}
		ptr++;
	}
}

void SHA1_Final(u8 *Message_Digest, SHA1_CTX *ctx)
{
	int i;

	SHA1PadMessage(ctx);
	for(i=0; i<64; ++i) {
		/* message may be sensitive, clear it out */
		ctx->Message_Block[i] = 0;
	}
	ctx->Length_Low = 0;    /* and clear length */
	ctx->Length_High = 0;
	
	for(i = 0; i < SHA1HashSize; ++i) {
		Message_Digest[i] = ctx->Intermediate_Hash[i>>2] >> 8*(3-(i&0x03));
	}
}

static void SHA1ProcessMessageBlock(SHA1_CTX *ctx)
{
	/* Constants defined in SHA-1 */
	const u32 K[] = { 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 };
	int t;         /* Loop counter          */
	u32 temp;      /* Temporary word value  */
	u32 W[80];     /* Word sequence         */
	u32 A,B,C,D,E; /* Word buffers          */

	for (t = 0; t < 16; t++) {
		W[t] =  ctx->Message_Block[t * 4]     << 24;
		W[t] |= ctx->Message_Block[t * 4 + 1] << 16;
		W[t] |= ctx->Message_Block[t * 4 + 2] << 8;
		W[t] |= ctx->Message_Block[t * 4 + 3];
	}

	for (t = 16; t < 80; t++) {
		W[t] = SHA1CircularShift(1,W[t-3] ^
			W[t-8] ^ W[t-14] ^ W[t-16]);
	}

	A = ctx->Intermediate_Hash[0];
	B = ctx->Intermediate_Hash[1];
	C = ctx->Intermediate_Hash[2];
	D = ctx->Intermediate_Hash[3];
	E = ctx->Intermediate_Hash[4];

	for (t = 0; t < 20; t++) {
		temp =  SHA1CircularShift(5,A) +
			((B & C) | ((~B) & D)) + E + W[t] + K[0];
		E = D;
		D = C;
		C = SHA1CircularShift(30,B);
		B = A;
		A = temp;
	}

	for (t = 20; t < 40; t++) {
		temp = SHA1CircularShift(5,A) +
			(B ^ C ^ D) + E + W[t] + K[1];
		E = D;
		D = C;
		C = SHA1CircularShift(30,B);
		B = A;
		A = temp;
	}

	for (t = 40; t < 60; t++) {
		temp = SHA1CircularShift(5,A) +
		       ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
		E = D;
		D = C;
		C = SHA1CircularShift(30,B);
		B = A;
		A = temp;
	}

	for (t = 60; t < 80; t++) {
		temp = SHA1CircularShift(5,A) +
			(B ^ C ^ D) + E + W[t] + K[3];
		E = D;
		D = C;
		C = SHA1CircularShift(30,B);
		B = A;
		A = temp;
	}

	ctx->Intermediate_Hash[0] += A;
	ctx->Intermediate_Hash[1] += B;
	ctx->Intermediate_Hash[2] += C;
	ctx->Intermediate_Hash[3] += D;
	ctx->Intermediate_Hash[4] += E;
	ctx->Message_Block_Index = 0;
}

static void SHA1PadMessage(SHA1_CTX *ctx)
{
	if (ctx->Message_Block_Index > 55) {
		ctx->Message_Block[ctx->Message_Block_Index++] = 0x80;

		while (ctx->Message_Block_Index < 64) {
			ctx->Message_Block[ctx->Message_Block_Index++] = 0;
		}

		SHA1ProcessMessageBlock(ctx);

		while (ctx->Message_Block_Index < 56) {
			ctx->Message_Block[ctx->Message_Block_Index++] = 0;
		}

	} else {

		ctx->Message_Block[ctx->Message_Block_Index++] = 0x80;
		while (ctx->Message_Block_Index < 56) {
			ctx->Message_Block[ctx->Message_Block_Index++] = 0;
		}
	}

	/*  Store the message length as the last 8 octets */
	ctx->Message_Block[56] = ctx->Length_High >> 24;
	ctx->Message_Block[57] = ctx->Length_High >> 16;
	ctx->Message_Block[58] = ctx->Length_High >> 8;
	ctx->Message_Block[59] = ctx->Length_High;
	ctx->Message_Block[60] = ctx->Length_Low >> 24;
	ctx->Message_Block[61] = ctx->Length_Low >> 16;
	ctx->Message_Block[62] = ctx->Length_Low >> 8;
	ctx->Message_Block[63] = ctx->Length_Low;

	SHA1ProcessMessageBlock(ctx);
}
#undef SHA1CircularShift
#endif
