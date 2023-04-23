#include "qpasswd.h"

size_t fd_size(int fd)
{
	off_t size=lseek(fd, 0, SEEK_END);

	if (size == (off_t)-1)
		die_msg(111, "lseek() == -1");

	return size;
}
