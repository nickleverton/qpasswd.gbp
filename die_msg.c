#include "qpasswd.h"

#if WANT_DIE_MSG
/* when you want to log sth. special - place it here */
void die_msg(int r, const char *s)
{
	doname();
	lognl(s);
	zeroall();
	doexit(r);
}
#endif
