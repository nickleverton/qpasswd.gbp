#include "qpasswd.h"

str_t buf;

void read3()
{
	int fd=READFD;
	char *e=getenv("READFD");

	if (e)
		if (scan_i32(e, &fd) == 0)
			fd=READFD;

	dozero(buf.s);
	for (buf.l=0; buf.l < BUFSIZE;) {
		int tmp;

		do {
			tmp=read(fd, buf.s + buf.l, BUFSIZE - buf.l);
		} while ((tmp==-1) && (errno==EINTR));

		if (tmp == -1)
			return die_msg(111,"read3() = -1");

		if (tmp == 0)
			break;

		buf.l+=tmp;
	}

	if (buf.l == 0)
		die_msg(111, "read3() = 0 bytes");

	/* we check this, because READFD=0 would close(0) */
	if (fd>2) close(fd);
}
