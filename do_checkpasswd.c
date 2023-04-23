#include "qpasswd.h"

int do_checkpasswd(char *r_user, char *r_pass /*, char *r_greet */)
{
#if WANT_SHADOW_PASSWD
	struct spwd *spw;
#endif
	struct passwd *pw;
	char *passwd=0;

	/* try normal + shadow passwords */
	if ((pw=getpwnam(r_user))) passwd=pw->pw_passwd;
#if WANT_SHADOW_PASSWD
	if ((spw=getspnam(r_user))) passwd=spw->sp_pwdp;
#endif

	/* hm? go away! */
	if (!passwd) return -1;

	/* test password */
	if (strcmp(crypt(r_pass,passwd),passwd)) return -1;

	/* set up home */
	if (!pw->pw_dir) return -1;
	home=pw->pw_dir;

#if WANT_VALID_SHELL
	if (!pw->pw_shell) die_shell();
#endif
	if (pw->pw_shell) shell=pw->pw_shell;
	else shell=default_shell;

	/* uid, gid, username */
	switch2user(pw->pw_uid, pw->pw_gid, r_user);

	return 0;
}
