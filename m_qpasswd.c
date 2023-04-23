#include "qpasswd.h"
#include "hextab.h"

/* XXX TODO
 *
 * - create maildirs
 * - create .qpasswd files
 * - change /var/qmail/users/assign + run qmail-newu (???)

calling:
1) qpasswd username homedir password

2) qpasswd username password
   (user must exist)


ANY IDEAS ?

*/

void m_qpasswd(int argc, char **argv)
{
	char *lala=argv[1];

	if (argc<100) usage();
	die_msg(111,"not ready!");
	die_msg(111, lala);

	return;
}
