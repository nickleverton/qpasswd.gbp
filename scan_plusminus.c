#include "scan.h"

size_t scan_plusminus(const u8 *src, int *dst)
{
	*dst=1;

	if (*src == '-') { *dst=-1; return 1; }
	if (*src == '+') { return 1; }

	return 0;
}
