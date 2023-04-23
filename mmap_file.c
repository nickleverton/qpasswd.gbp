#include "qpasswd.h"

char *mmap_file(const char *fn, size_t *len, int prot) 
{
	int fd=open_ro(fn);
	char *buf=0;

	if (fd == -1) die_msg(111, "open(mmap) = -1");
	*len=fd_size(fd);
	buf=(char *)mmap(0, *len, prot, MAP_SHARED, fd, 0);
	if (buf==(char *)-1) buf=0;
	close(fd);

	return buf;
}
