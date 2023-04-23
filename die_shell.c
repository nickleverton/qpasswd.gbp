#include "qpasswd.h"

#if WANT_VALID_SHELL
void die_shell(void)
{
	die_msg(1, "user has no shell!");
}
#endif
