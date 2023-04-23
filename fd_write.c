#include "qpasswd.h"

int fd_write(int fd, const char *s, ssize_t n)
{
	return (int)write(fd, s, n);
}
