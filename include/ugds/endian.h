#ifndef UGDS_ENDIAN_H
#define UGDS_ENDIAN_H

#include <stdint.h>
#include <stdbool.h>

/* This is very similar to glibc's <endian.h>, but with (slightly?) better
 * naming and can be used across all platforms */

bool endian_is_little();
bool endian_is_big();

#define ENDIAN_IS_LITTLE endian_is_little()
#define ENDIAN_IS_BIG endian_is_big()

uint16_t endian_hbe16(uint16_t bits);
uint16_t endian_hle16(uint16_t bits);
uint16_t endian_beh16(uint16_t bits);
uint16_t endian_leh16(uint16_t bits);

uint32_t endian_hbe32(uint32_t bits);
uint32_t endian_hle32(uint32_t bits);
uint32_t endian_beh32(uint32_t bits);
uint32_t endian_leh32(uint32_t bits);

uint64_t endian_hbe64(uint64_t bits);
uint64_t endian_hle64(uint64_t bits);
uint64_t endian_beh64(uint64_t bits);
uint64_t endian_leh64(uint64_t bits);

#endif
