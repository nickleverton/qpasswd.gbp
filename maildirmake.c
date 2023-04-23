#include "qpasswd.h"

#if WANT_QPASSWD
static int do_mkdir(const char *path)
{
	int fd=mkdir(path, 0700);

	if (fd == -1 && errno != EEXIST)
		die_msg(111, "mkdir() == -1");

	return fd;
}


void maildirmake(const char *home)
{
	int fd=open_ro(".");

	/* make sure the home and a maildir exist */
	if (chdir(home) == -1) {
		char *c=alloca(strlen(home)+1);
		char *d;
		fmt_str(c,home);
		while ((d=strrchr(c,'/'))) {
			if (!d[1]) { *d=0; continue; }
			break;
		}
		*d=0;   do_mkdir(c);
		*d='/'; do_mkdir(c);
		chdir(home);
	}

	do_mkdir("Maildir"); chdir("Maildir");
	do_mkdir("cur");
	do_mkdir("new");
	do_mkdir("tmp");

	if (fchdir(fd) == -1)
		die_msg(111, "fchdir() = -1");

	close(fd);
}
#endif
