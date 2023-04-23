#include "qpasswd.h"

int buffer_write(int fd, const char *s, ssize_t n)
{
	int w=0;

	/* write to buffer.fd */
	w=fd_write(fd, s, n);

	/* buffer.fd fails, fall back to fd=2 */
	if (w == -1) {
		if (errno != EBADF) _exit(111);
		logbuf.fd=2;
		w=fd_write(logbuf.fd, s, n);
	}

	/* can't happen! */
	if (w == -1) _exit(111);

	return w;
}
