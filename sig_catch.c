#include "sig.h"

void sig_catch(int sig, void (*f)(int))
{
	struct sigaction sa;

	sa.sa_handler = f;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(sig, &sa, (struct sigaction *)0);
}
