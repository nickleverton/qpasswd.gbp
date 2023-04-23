#include "qpasswd.h"

void usage(void)
{
#if WANT_USAGE
	log1("usage: ");
	switch(mode) {
	case MODE_QPASSWD:
		lognl("qpasswd(v0.1) username homedir password");
		break;
	case MODE_CHECKPASSWD:
		lognl("checkpasswd(v0.1) (password checking against /etc/passwd)");
		break;
	case MODE_CHECKQPASSWD:
		lognl("checkqpasswd(v0.1) (password checking against /etc/qpasswd)");
		break;
	case MODE_MULTICHECKPW:
		lognl("multichkpw(v0.1) n checkpw1 checkpw2 ... checkpwN prog");
		break;
	default:
		lognl("call me as: qpasswd, multicheckpw, checkpasswd or checkqpasswd!");
		break;
	}
#endif
	doexit(2);
}
