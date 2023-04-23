#include "rmd.h"

/* Keyed RMD160 (HMAC-RIPEMD) - rfc 2104 */
#if WANT_CRAM_RIPEMD

#define PAD 64
#define TK  20

void hmac_rmd160(text, key, digest)
	u8 *text;   /* pointer to data stream */
	u8 *key;    /* pointer to authentication key */
	u8 *digest; /* caller digest to be filled in */
{
	RMD160_CTX ctx;
	u8 k_ipad[PAD+1]; /* inner padding - key XORd with ipad */
	u8 k_opad[PAD+1]; /* outer padding - key XORd with opad */
	u8 tk[TK];
	size_t text_len=strlen(text);
	size_t key_len=strlen(key);
	int i;

	if (key_len > PAD) {
		RMD160_CTX tctx;
		RMD160_Init(&tctx);
		RMD160_Update(&tctx, key, key_len);
		RMD160_Final(tk, &tctx);
		key = tk;
		key_len = TK;
	}

	/* start out by storing key in pads */
	memset(k_ipad, 0, PAD);
	memcpy(k_ipad, key, key_len);
	memset(k_opad, 0, PAD);
	memcpy(k_opad, key, key_len);

	/* XOR key with ipad and opad values */
	for (i=0; i<PAD; i++) {
		k_ipad[i] ^= 0x36;
		k_opad[i] ^= 0x5c;
	}

	/* perform inner RMD160 */
	RMD160_Init(&ctx);                   /* init ctx for 1st pass */
	RMD160_Update(&ctx, k_ipad, PAD);    /* start with inner pad */
	RMD160_Update(&ctx, text, text_len); /* then text of datagram */
	RMD160_Final(digest, &ctx);          /* finish up 1st pass */

	/* perform outer RMD160 */
	RMD160_Init(&ctx);                   /* init ctx for 2nd pass */
	RMD160_Update(&ctx, k_opad, PAD);    /* start with outer pad */
	RMD160_Update(&ctx, digest, TK);     /* then results of 1st hash */
	RMD160_Final(digest, &ctx);          /* finish up 2nd pass */
}
#undef PAD
#undef TK
#endif
