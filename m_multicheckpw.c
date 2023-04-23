/*
 * Authors:
 *
 * Andreas Aardal Hanssen <andreaha-qmail@lexx.infeline.org>
 * Tino Reichardt <milky-qpasswd@mcmilk.de>
 */

#include "qpasswd.h"

void m_multicheckpw(int argc, char **argv)
{
	int nprogs, i, result, pid;
	char **newargs;

	if (argc < 2) { usage(); } /* program takes at least one argument */

	if (scan_u32(argv[1],&nprogs) == 0) {
		die_msg(2, "1st arg should be a number!");
	}

	if (argc < nprogs + 2) {
		die_msg(2, "not enough args!");
	}

	/* will be the argv[] for the real checkpasswords */
	newargs = argv + nprogs + 1;

	/* read user/pass/greet from our parent process, through fd3. */
	read3();

	/* create pipes to fd 3 and execute the checkpasswords */
	for (i = 0; i < nprogs; ++i) {
		char *prog=argv[i+2];
		int pi[2];

		if (pipe(pi) == -1) {
			die_msg(111, "pipe() = -1");
		} /* pipe([3, 4]); */

		if (pi[0] != 3) {
			die_msg(111, "pipe[0] != 3");
		} /* be sure, we have fd3 for our childs! */

		switch ((pid=fork())) {
		case 0: /* child */
		{
			/* we're not writing anything back, so we close the
			 * writing end */
			close(pi[1]);
			sig_pipedefault();
#if LOG_MULTICHECKPW_TRY
			doname(); /* show, which types ot auth. are tried */
			log3("trying auth with ", prog, "\n");
			flush();
#endif
			newargs[0]=prog; /* has to be adjusted! */
			execve(prog, newargs, environ);
			die_msg(111, "execve() = -1");
		}
		case -1:
			die_msg(111, "fork() = -1");
		default:
			break;
		}

		/* parent process */
		close(pi[0]);               /* -> we're not reading from the child  */
		write(pi[1], buf.s, buf.l); /* -> writing to the childs reading fd, */
		close(pi[1]);               /*    and close it then */

		/* wait for child to die and examine exit code */
		do i=waitpid(pid, &result, 0);
		while ((i==-1) && (errno==EINTR));
		if (i!=pid) die_msg(111, "error @ waitpid()");

		switch (WEXITSTATUS(result)) {
		case 0:
			/* correct password, yeah :) */
			die_multi(0, "end / status: was ok");
			break;
		case 2:
			/* misuse */
			die_multi(2, "end / status: misuse!");
			break;
		case 111:
			/* temp. failure */
			die_multi(111, "end / status: all checkpasswords failed!");
			break;
		default:
			/* 1 is skipped! */
			break;
		}
	}

	die_multi(1, "end / status: all checkpasswords failed!");
}
