#include "scan.h"

size_t scan_i32(const u8 *src, i32 *dst)
{
	register const u8 *tmp=src;
	u32 l=0;
	int pm;

	tmp+=scan_plusminus(tmp,&pm);
	tmp+=scan_u32(tmp,&l);
	*dst=(pm==-1)?-l:l;

	return tmp-src;
}
