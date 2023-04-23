#ifndef QP_TEST_CASES_H
#define QP_TEST_CASES_H

#include "types.h"

typedef struct {
  char *name;    /* name, to be listed when testing */
  char *key;     /* the key for testing */
  int   keylen;  /* if 0 - take the key as it is */
  char *data;    /* the data for testing */
  int   datalen; /* if 0 - take the data as it is */
  char *test1, *test2;
} rfc_test_t;

extern rfc_test_t test_hmac_16[]; /* md5 + rmd128 test-cases */
extern rfc_test_t test_hmac_20[]; /* sha1 + rmd160 test-cases */

#endif
