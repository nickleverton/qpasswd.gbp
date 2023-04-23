#include "qpasswd.h"

/*
 * - every home/vhome gets a .lastpop file
 * - each line @ this file contains: 'timestamp,ip:port'
 */
#if WANT_LASTPOP
void qpasswd_lastpop_update(void)
{
	size_t i, w;
	char tmp[BUFSIZE], *e;
	int fd;

	dozero(tmp);
	i=fmt_str(tmp, home);
	tmp[fmt_str(tmp+i, "/.lastpop")+i]=0;
	fd=open_append(tmp);
	if (fd == -1) die_msg(111, "open(.lastpop) = -1");

	dozero(tmp);
	i=fmt_u32(tmp,time(0));
	if ((e=getenv("TCPREMOTEIP"))) {
		i+=fmt_str(tmp+i,",");
		i+=fmt_str(tmp+i,e);
	}
	if ((e=getenv("TCPREMOTEPORT"))) {
		i+=fmt_str(tmp+i,":");
		i+=fmt_str(tmp+i,e);
	}
	i+=fmt_str(tmp+i,"\n");
	tmp[i]=0;

	w=write(fd, tmp, i);
	if (w == (size_t)-1) die_msg(111, "write() = -1");
	if (w!=i) die_msg(111, "short write()");

	close(fd);
}
#endif
