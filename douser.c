#include "qpasswd.h"

#if LOG_USER_SWITCHES
void douser(const char *m)
{
	char b[100], *s=b;

	if (m) log1(m);

	s+=fmt_str(s, " uid=");
	s+=fmt_u32(s,getuid());
	s+=fmt_str(s, " gid=");
	s+=fmt_u32(s,getgid());
	s+=fmt_str(s, " euid=");
	s+=fmt_u32(s,geteuid());
	s+=fmt_str(s, " egid=");
	s+=fmt_u32(s,getegid());
	*s=0; log1(b);

	return;
}
#endif
