#include "scan.h"

size_t scan_u32(const u8 *src, u32 *dst)
{
	register const u8 *tmp=src;
	register u8 c;
	register u32 l=0;

	while ((c=*tmp - '0')<10) {
		u32 m=l;
		l=l*10+c;
		if ((l>>3) < m) break;
		tmp++;
	}
	*dst=l;

	return tmp-src;
}
