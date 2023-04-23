#include "qpasswd.h"

/* flush buffers + exit */
void doexit(int x)
{
	flush();_exit(x);
}
