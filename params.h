#ifndef QP_PARAMS_H
#define QP_PARAMS_H

/* default FD's for multilog */
#define FD_QPASSWD              20
#define FD_CHECKPASSWD          21
#define FD_CHECKQPASSWD         22
#define FD_MULTICHECKPW         23

#define DEFAULT_SHELL           "/bin/sh"
#define QPASSWD_FILE            "/etc/qpasswd"
#define QPASSWD_MAX             1024*11 /* max size in bytes of a .qpasswd file */
#define LASTPOP_MAX             1024*11 /* max size in bytes of a .lastpop file */
#define DIGEST_MAX              1024*11 /* max size in bytes of a /var/qmail/authdir/stamp file */
#define BUFSIZE                 4096    /* size for buffers */
#define READFD                  3       /* reading fd for checkpasswords */

#define WANT_USAGE              1 /* see usage.c */
#define WANT_PIDINFO            1 /* see doname.c */
#define WANT_VALID_SHELL        1 /* check for a valid shell */
#define WANT_SHADOW_PASSWD      1 /* support for getspnam() */
#define WANT_LASTPOP            0 /* support for the .lastpop file */
#define WANT_DIE_MSG            1 /* die_msg(code, "description") -> exit(exit) */
#define WANT_DEBUG              1 /* enable extra debugging messages */
#define WANT_ERRFDS             1 /* descriptors for logging ... see log.c !!! */
#define WANT_ENVIRON_CHECK      1 /* check for authtype via $AUTH or $SASL */

/* select which methods should be checked with checkqpasswd */
#define WANT_CRAM_MD5           1
#define WANT_CRAM_SHA1          1
#define WANT_CRAM_RIPEMD        1 /* size += 9k */
#define WANT_DIGEST_MD5         1 /* size += 4k */

/* logging */
#define LOG_LOGINFAILURES       1 /* see die1.c */
#define LOG_MULTICHECKPW_TRY    1 /* see m_multicheckpw.c */
#define LOG_MULTICHECKPW_STATUS 1 /* see m_multicheckpw.c + main.h */
#define LOG_CHECKQPASSWD_METHOD 1 /* see do_checkqpasswd.c */
#define LOG_USER_SWITCHES       1 /* see switch2user.c / douser.c */
#define LOG_VHOME_FOUND		1 /* see doc/ENVIRON */

/* define here the setuid/setgid functions of your system */
#define SETuid(x) setresuid(x,x,x)
#define SETgid(x) setresgid(x,x,x)

/* seconds - for pop3-before-smtp (todo!) */
#define LASTPOP_DEFAULT         23

#endif
