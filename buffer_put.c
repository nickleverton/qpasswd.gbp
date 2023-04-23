#include "buffer.h"

size_t buffer_put(buffer_t *b, const u8 *msg)
{
	size_t len=strlen(msg);
	size_t rlen=0;

	/* nothing to add/put */
	if (!len) return 0;

	/* *msg to big, write it directly */
	if (len >= BUFSIZE) {
		rlen=buffer_flush(b); /* flush old and new */
		rlen+=buffer_write(b->fd, msg, len);
		return rlen;
	}

	/* old buf + new is to big -> flush it before */
	if (len+b->l >= BUFSIZE)
		rlen=buffer_flush(b);

	/* add to buf */
	rlen+=fmt_str(b->b+b->l, msg);
	b->l+=len;

	return rlen;
}
