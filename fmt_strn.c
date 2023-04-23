#include "fmt.h"

size_t fmt_strn(u8 *dst, const u8 *src, size_t limit)
{
	register const u8 *t=src;
	register const u8 *u=src+limit;
	register u8 *s=dst;

	for (;;) {
		if (!*t) break;
		if (s) { *s=*t; ++s; }
		if (t==u) break;
		++t;
	}

	return t-src;
}
