#include "qpasswd.h"

#if WANT_PIDINFO
void doname(void)
{
	u8 b[10];
	u8 *s=b;

	s[fmt_u32(s,getpid())]=0;
	if (name && s) { log3(name, ": pid ", s); }

	s[fmt_u32(s,ppid)]=0;
	if (s) { log3("[", s, "] "); }

	return;
}
#endif
