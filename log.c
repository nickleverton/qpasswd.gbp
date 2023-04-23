#include "qpasswd.h"

/*
 * if WANT_ERRFDS is enabled, but writing to it returns an error,
 * we write to fd=2!
 */

int log1(const char *s1) { return log4(s1,0,0,0); }
int log2(const char *s1, const char *s2) { return log4(s1,s2,0,0); }
int log3(const char *s1, const char *s2, const char *s3) { return log4(s1,s2,s3,0); }
int log4(const char *s1, const char *s2, const char *s3, const char *s4)
{
	int len=0;
#if WANT_ERRFDS
	static int logfd=0;
	char *e=0;
	int fd;

	if (!logfd) {
		switch(mode) {
		case MODE_QPASSWD:
			e=getenv("FD_QPASSWD");
			logbuf.fd=FD_QPASSWD;
			break;
		case MODE_CHECKPASSWD:
			e=getenv("FD_CHECKPASSWD");
			logbuf.fd=FD_CHECKPASSWD;
			break;
		case MODE_CHECKQPASSWD:
			e=getenv("FD_CHECKQPASSWD");
			logbuf.fd=FD_CHECKQPASSWD;
			break;
		case MODE_MULTICHECKPW:
			e=getenv("FD_MULTICHECKPW");
			logbuf.fd=FD_MULTICHECKPW;
			break;
// /* we do it exra, because the openssl bloat is to big! */
//		case MODE_QMAIL_TLS:
//			e=getenv("FD_QMAIL_TLS");
//			logbuf.fd=FD_QMAIL_TLS;
//			break;
		}
	}

	if (e)
		if (scan_i32(e, &fd) > 0)
			logbuf.fd=fd;
#else
	logbuf.fd=2;
#endif
	/* writing to fd was okay, so we expect no errors */
	if (s1) len+=buffer_put(&logbuf, s1);
	if (s2) len+=buffer_put(&logbuf, s2);
	if (s3) len+=buffer_put(&logbuf, s3);
	if (s4) len+=buffer_put(&logbuf, s4);

	return len;
}
