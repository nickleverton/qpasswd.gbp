#ifndef QP_FMT_H
#define QP_FMT_H

#include "types.h"

extern size_t fmt_strn(u8 *dst, const u8 *src, size_t limit);
extern size_t fmt_str(u8 *dst, const u8 *src);
extern size_t fmt_u32(u8 *dst, const u32 i);

#endif
