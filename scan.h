#ifndef QP_SCAN_H
#define QP_SCAN_H

#include "qpasswd.h"

extern size_t scan_u32(const u8 *src, u32 *dst);
extern size_t scan_i32(const u8 *src, i32 *dst);
extern size_t scan_ip4(const u8 *src, u8 ip[4]);
extern size_t scan_plusminus(const u8 *src, int *dst);
extern size_t scan_charset(const u8 *src, const u8 *charset, size_t limit);

#endif
