#ifndef QP_OPEN_H
#define QP_OPEN_H

/* misc own stuff */
#include "types.h"

/* open_file.c
 *
 * open fn for reading/writing and return the fd
 */
extern int open_file(const unsigned char *fn, int flags);
#define open_ro(fn) open_file(fn,O_RDONLY)
#define open_wo(fn) open_file(fn,O_WRONLY)
#define open_rw(fn) open_file(fn,O_RDWR)
#define open_trunc(fn) open_file(fn,O_RDWR|O_TRUNC|O_CREAT)
#define open_append(fn) open_file(fn,O_RDWR|O_APPEND|O_CREAT)

#endif
