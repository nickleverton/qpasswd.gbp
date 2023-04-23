#include "qpasswd.h"

void die1(const char *user)
{
#if LOG_LOGINFAILURES
	doname();
	log1("password failure for: ");
	if (user) {
		lognl(user);
	} else {
		lognl("unknown");
	}
#endif
	zeroall();
	doexit(1);
}
