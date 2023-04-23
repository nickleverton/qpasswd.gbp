#include "buffer.h"

size_t buffer_flush(buffer_t *b)
{
	size_t rlen=0;

	if (b->l>0) {
		rlen=buffer_write(b->fd, b->b, b->l);
		memset(b->b, 0, b->l);
	}
	b->l=0;

	return rlen;
}
