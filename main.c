#include "qpasswd.h"

/* misc global variables */
char *home, *shell, *default_shell=DEFAULT_SHELL;
int mode, ppid;

/* names we can have */
const char *name;
const char me1[]="multicheckpw";
const char me2[]="checkpasswd";
const char me3[]="checkqpasswd";
const char me4[]="qpasswd";

int main(int argc, char **argv)
{
	char *me;
	
	me=strrchr(argv[0],'/');
	if (me) ++me;
	else me=argv[0];

	if (!strcmp(me, me1)) {
		name=me1;
		mode=MODE_MULTICHECKPW;
	} else if (!strcmp(me, me2)) {
		name=me2;
		mode=MODE_CHECKPASSWD;
	} else if (!strcmp(me, me3)) {
		name=me3;
		mode=MODE_CHECKQPASSWD;
	} else if (!strcmp(me, me4)) {
		name=me4;
		mode=MODE_QPASSWD;
	} else
		usage();

	ppid=getppid();

	/* we remove setuid/setgid bit + execute for owner ... */
	umask(06177);

	switch (mode) {
	case MODE_QPASSWD:
		m_qpasswd(argc, argv);
		break;
	case MODE_CHECKPASSWD:
	case MODE_CHECKQPASSWD:
		m_checkpassword(argc, argv);
		break;
	case MODE_MULTICHECKPW:
		m_multicheckpw(argc, argv);
		break;
	}

	return 111;
}
