#include "md5.h"

/* Keyed MD5 (HMAC-MD5) - rfc 2104 */
#if WANT_CRAM_MD5

#define PAD 64
#define TK  16

void hmac_md5(text, key, digest)
	u8 *text;   /* pointer to data stream */
	u8 *key;    /* pointer to authentication key */
	u8 *digest; /* caller digest to be filled in */
{
	MD5_CTX ctx;
	u8 k_ipad[PAD+1]; /* inner padding - key XORd with ipad */
	u8 k_opad[PAD+1]; /* outer padding - key XORd with opad */
	u8 tk[TK];
	size_t text_len=strlen(text);
	size_t key_len=strlen(key);
	int i;

	/* if key is longer than 64 bytes reset it to key=MD5(key) */
	if (key_len > PAD) {
		MD5_CTX tctx;
		MD5_Init(&tctx);
		MD5_Update(&tctx, key, key_len);
		MD5_Final(tk, &tctx);
		key = tk;
		key_len = TK;
	}

	/*
	 * the HMAC_MD5 transform looks like:
	 *
	 * MD5(K XOR opad, MD5(K XOR ipad, text))
	 *
	 * where K is an n byte key
	 * ipad is the byte 0x36 repeated 64 times
	 * opad is the byte 0x5c repeated 64 times
	 * and text is the data being protected
	 */

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

	/* perform inner MD5 */
	MD5_Init(&ctx);                   /* init ctx for 1st pass */
	MD5_Update(&ctx, k_ipad, PAD);    /* start with inner pad */
	MD5_Update(&ctx, text, text_len); /* then text of datagram */
	MD5_Final(digest, &ctx);          /* finish up 1st pass */

	/* perform outer MD5 */
	MD5_Init(&ctx);                   /* init ctx for 2nd pass */
	MD5_Update(&ctx, k_opad, PAD);    /* start with outer pad */
	MD5_Update(&ctx, digest, TK);     /* then results of 1st hash */
	MD5_Final(digest, &ctx);          /* finish up 2nd pass */
}
#undef PAD
#undef TK
#endif
