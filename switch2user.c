#include "qpasswd.h"

/* switch2user()
 *
 * 1) if we're root, set up uid and gid
 *
 * 2) environ has to be set:
 *    - USER=user(via arg)
 *    - HOME=home(global)
 *    - SHELL=shell(global)
 *    - UID=uid(via arg)
 *    - GID=gid(via arg)
 *
 * 3) ...
 *
 */

void switch2user(const int uid, const int gid, const char *user)
{
	char *s, b[BUFSIZE];

	doname();
	if (geteuid() == 0) {
		douser("switch from:");

		if (initgroups(user, gid) == -1)
			die_msg(111, "initgroups() = -1");

		if (SETgid(gid) == -1)
			die_msg(111, "SETgid() = -1");

		if (SETuid(uid) == -1)
			die_msg(111, "SETuid() = -1");

		douser(" to:");
	} else {
		douser("can't switch uid/gid with these id's:");
	}
	log1("\n");
	flush();

	if (chdir(home) == -1)
		die_msg(111, "chdir(home) = -1");

	/* set up the environment */
	s=b; s+=fmt_str(b, "UID=");
	s[fmt_u32(s,uid)]=0;
	if (putenv(b) == -1) die_env();

	s=b; s+=fmt_str(b, "GID=");
	s[fmt_u32(s,gid)]=0;
	if (putenv(b) == -1) die_env();

	s=b; s+=fmt_str(b, "HOME=");
	s[fmt_str(s,home)]=0;
	if (putenv(b) == -1) die_env();

	s=b; s+=fmt_str(b, "SHELL=");
	s[fmt_str(s,shell)]=0;
	if (putenv(b) == -1) die_env();

	s=b; s+=fmt_str(b, "USER=");
	s[fmt_str(s,user)]=0;
	if (putenv(b) == -1) die_env();

	doname();
	lognl("switch2user() ende!");
	flush();

#if WANT_LASTPOP
	qpasswd_lastpop_update();
#endif
	return;
}
