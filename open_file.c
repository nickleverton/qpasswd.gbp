#include "qpasswd.h"

int open_file(const unsigned char *fn, int flags)
{
	int fd;

#ifdef O_LARGEFILE
	fd=open(fn, O_NONBLOCK|O_LARGEFILE|flags, 0600);
#else
	fd=open(fn, O_NONBLOCK|flags, 0600);
#endif

	return fd;
}
