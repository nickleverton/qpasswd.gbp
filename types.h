#ifndef QP_TYPES_H
#define QP_TYPES_H

/* most functions need those two header files */
#include "params.h"
#include <sys/types.h>
#include <unistd.h>

/* types i often use */
typedef signed int   i32;
typedef signed char   i8;

typedef unsigned int u32;
typedef unsigned char u8;

/* for broken glibc headers! */
#ifndef __dietlibc__
extern char **environ;
extern int setresuid(uid_t ruid, uid_t euid, uid_t suid);
extern int setresgid(gid_t rgid, gid_t egid, gid_t sgid);
#endif

#endif
