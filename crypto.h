#ifndef QP_CRYPTO_H
#define QP_CRYPTO_H

#include "types.h"
#include "md5.h"
#include "sha1.h"
#include "rmd.h"

#ifdef NEED_ALL_CRYPTO_STUFF
#undef  WANT_CRAM_MD5
#undef  WANT_CRAM_SHA1
#undef  WANT_CRAM_RIPEMD
#define WANT_CRAM_MD5     1
#define WANT_CRAM_SHA1    1
#define WANT_CRAM_RIPEMD  1
#include "md5.c"
#include "sha1.c"
#include "rmd160.c"
#include "hmac_md5.c"
#include "hmac_sha1.c"
#include "hmac_rmd160.c"
#endif

#endif
