#include "fmt.h"

size_t fmt_str(u8 *dst, const u8 *src)
{
	register const u8 *t=src;
	register u8 *s=dst;

	for (;;) {
		if (!*t) break;
		if (s) { *s=*t; ++s; }
		++t;
	}

	return t-src;
}
