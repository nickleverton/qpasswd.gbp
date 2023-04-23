#include "scan.h"

size_t scan_ip4(const u8 *src, u8 ip[4])
{
	register const u8 *tmp=src;
	int i;
	u32 c;

	for (i=0; i<4; ++i) {
		register size_t j=scan_u32(tmp,&c);
		if (!j || c>255) return 0;
		tmp+=j; ip[i]=(u8)c;
		if (i<3 && *tmp!='.') return 0;
		tmp++;
	}

	return tmp-src-1;
}
