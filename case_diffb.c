#include "case.h"

int case_diffb(const u8 *s, int len, const u8 *t)
{
	register unsigned char x;
	register unsigned char y;

	while (len > 0) {
		--len;
		x = *s++ - 'A';
		if (x <= 'Z' - 'A') x += 'a'; else x += 'A';
		y = *t++ - 'A';
		if (y <= 'Z' - 'A') y += 'a'; else y += 'A';
		if (x != y) return ((int)x-(int)y);
	}

	return 0;
}
