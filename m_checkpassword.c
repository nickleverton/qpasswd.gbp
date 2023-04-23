#include "qpasswd.h"

/* we do a lot of sanity checking here, not really everything is needed :) */
void m_checkpassword(int argc, char **argv)
{
	char *r_pass, *r_user, *r_greet;
	int i=0;

	if (argc < 2) { usage(); } /* program takes at least one argument */

	read3();

	r_user = buf.s;
	while (buf.s[i++]) if (i >= buf.l) die2();
	if (!r_user)  die2();

	r_pass = buf.s + i;
	while (buf.s[i++]) if (i >= buf.l) die2();
	if (!r_pass)  die2();

	r_greet = buf.s + i;
	while (buf.s[i++]) if (i >= buf.l) die2();
	if (!r_greet) die2();

	switch (mode) {
	case MODE_CHECKPASSWD:
		/* checking against /etc/passwd failed! */
		if (do_checkpasswd(r_user, r_pass/*, r_greet*/) == -1)
			die1(r_user);
		break;
	case MODE_CHECKQPASSWD:
		/* checking against /etc/qpasswd failed! */
		if (do_checkqpasswd(r_user, r_pass, r_greet) == -1)
			die1(r_user);
		break;
	}

	/* be sure, that no passwords remain in memory! */
	zeroall();
	execve(argv[1], argv + 1, environ);

	/* shouldn't happen at all! */
	die_msg(111, "execve() = -1");
}
