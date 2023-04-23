#include "qpasswd.h"

/* only for our bufsize bufs! */
void dozero(char *b)
{
	memset(b, 0, BUFSIZE);
}
