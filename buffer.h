#ifndef QP_BUFFER_H
#define QP_BUFFER_H

#include "qpasswd.h"

typedef struct {
	int fd;        /* fd, where we flush the buffer */
	size_t l;      /* len of current content */
	u8 b[BUFSIZE]; /* the buffer */
} buffer_t;

extern buffer_t logbuf;
extern size_t buffer_put(buffer_t *b, const u8 *msg);
extern size_t buffer_flush(buffer_t *b);
extern int buffer_write(int fd, const char *s, ssize_t n);

#endif
