#ifndef QP_QPASSWD_H
#define QP_QPASSWD_H

#define _XOPEN_SOURCE  /* needed for crypt(3) */
#define _BSD_SOURCE    /* needed by initgroups(3) + fchdir(2) */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <shadow.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <wait.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>

/* misc own stuff */
#include "types.h"
#include "params.h"
#include "buffer.h"
#include "scan.h"
#include "sig.h"
#include "fmt.h"

/* is used for our buffers */
typedef struct {
	char s[BUFSIZE];
	int l;
} str_t;

#define flush() buffer_flush(&logbuf)
extern void lognl(const char *s);

extern int log1(const char *s1);
extern int log2(const char *s1, const char *s2);
extern int log3(const char *s1, const char *s2, const char *s3);
extern int log4(const char *s1, const char *s2, const char *s3, const char *s4);

/* mmap_file.c
 *
 * mmap_read - map content of a file
 *
 * - maps content of a file to returned area
 * - saves length in *len
 */
extern char *mmap_file(const char *fn, size_t *len, int prot);
#define mmap_ro(fn,len) mmap_file(fn,len,PROT_READ)
#define mmap_wo(fn,len) mmap_file(fn,len,PROT_WRITE)
#define mmap_rw(fn,len) mmap_file(fn,len,PROT_WRITE|PROT_READ)
extern void *mmap_remap(void *area, size_t old, size_t new);

/* open_file.c
 *
 * open fn for reading/writing and return the fd
 */
extern int open_file(const unsigned char *fn, int flags);
#define open_ro(fn) open_file(fn,O_RDONLY)
#define open_wo(fn) open_file(fn,O_WRONLY)
#define open_rw(fn) open_file(fn,O_RDWR)
#define open_trunc(fn) open_file(fn,O_RDWR|O_TRUNC|O_CREAT)
#define open_append(fn) open_file(fn,O_RDWR|O_APPEND|O_CREAT)

extern size_t fd_size(int fd); /* returns filesize of fd */
extern int fd_write(int fd, const char *str, ssize_t n);

extern void doexit(int x);
extern void dozero(char *buf);     /* should zero a special buffer */
extern void zeroall(void);         /* should zero all buffers */
extern void usage(void);           /* print usefull information */

/* die = log + exit */
extern void die1(const char *user);
extern void die2(void);
extern void die111(void);
extern void die_env(void);
extern void die_shell(void);

/* read3.c - should normally read input from fd3 */
extern str_t buf;
extern void read3(void);

#if WANT_PIDINFO
extern void doname(void);
#else
#define doname()
#endif

#if WANT_DEBUG
extern void debug(const char *format, ...);
#else
#define debug(fmt, arg...)
#endif

#if WANT_DIE_MSG
extern void die_msg(int r, const char *s);
#else
#define die_msg(code,s) _exit(code)
#endif

#if LOG_USER_SWITCHES
extern void douser(const char *m); /* logs current used privileges */
#else
#define douser(m)
#endif

#if LOG_MULTICHECKPW_STATUS
#define die_multi(x,y) die_msg(x,y)
#else
#define die_multi(code,s) _exit(code)
#endif

/* modes of our multicall binary */
#define MODE_QPASSWD        1
#define MODE_CHECKPASSWD    2
#define MODE_CHECKQPASSWD   3
#define MODE_MULTICHECKPW   4
//#define MODE_QMAIL_TLS      5

/* called from main */
extern void m_qpasswd(int argc, char **argv);
extern void m_multicheckpw(int argc, char **argv);
extern void m_checkpassword(int argc, char **argv);
//extern void m_qmail_tls(int argc, char **argv);

/* called from m_checkpassword */
extern int do_checkpasswd(char *r_user, char *r_pass/*, char *challenge*/);
extern int do_checkqpasswd(char *r_user, char *r_pass, char *challenge);

/* main.c */
extern char *default_shell, *shell, *home;
extern const char *name;
extern int mode, ppid;

/* function, for switch to another user (should setup all needed stuff!) */
extern void switch2user(const int uid, const int gid, const char *user);

/* creating a maildirs ... for qpasswd later! */
extern void maildirmake(const char *home);

/* /etc/qpasswd functions */
extern char qpwds[QPASSWD_MAX];
extern int qpasswd_gethome(const char *r_user, int *uid, int *gid, char *home);
extern int qpasswd_getpass(void); /* read password from .qpasswd */
#if WANT_LASTPOP
extern void qpasswd_lastpop_update(void); /* add timestamp to .lastpop */
extern int qpasswd_lastpop_check(void);
#endif

/* delete timestamps older than t */
extern u32 lastpop;
extern int qpasswd_lastpop_clean(void);
/* if timestamps older that t are found, return -1 */

#endif
