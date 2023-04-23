#ifndef QP_SIG_H
#define QP_SIG_H

#include <sys/types.h>
#include <signal.h>

extern void sig_catch(int sig, void (*f)(int));
extern void sig_pipedefault(void);

#endif
