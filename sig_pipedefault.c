#include "sig.h"

void sig_pipedefault(void)
{
	sig_catch(SIGPIPE, SIG_DFL);
}
