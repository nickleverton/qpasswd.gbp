#include "qpasswd.h"

int qpasswd_getpass(void)
{
	size_t size, i;
	char pwfile[BUFSIZE];
	char *map;
	int count=0;

	i=fmt_str(pwfile, home);
	pwfile[fmt_str(pwfile+i, "/.qpasswd")+i]=0;
	map=mmap_ro(pwfile, &size);
	if (map == 0) return 0;
	if (size > QPASSWD_MAX) goto out;

	/* use each line as one password, thats IT! */
	memcpy(qpwds, map, size);
	for (count=0,i=0; i<size; i++)
		if (qpwds[i] == '\n') {
			qpwds[i]=0;
			count++;
		}

	out:
	munmap(map,size);

	return count;
}
