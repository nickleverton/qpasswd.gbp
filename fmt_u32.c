#include "fmt.h"

size_t fmt_u32(u8 *dst, const u32 i)
{
	register size_t len;
	register u32 tmp,len2;

	/* first count the number of bytes needed */
	for (len=1, tmp=i; tmp>9; ++len) tmp/=10;
	if (dst)
		for (tmp=i, dst+=len, len2=len+1; --len2; tmp/=10)
			*--dst = (tmp%10)+'0';

	return len;
}
