#include "ugds/endian.h"

bool
endian_is_little() {
  int i = 1;
  return ((const uint8_t *)&i)[0];
}

bool
endian_is_big() {
  int i = 1;
  return !((const uint8_t *)&i)[0];
}

static inline void
swap_bytes(uint8_t *bt1, uint8_t *bt2) {
  uint8_t temp = *bt1;
  *bt1 = *bt2;
  *bt2 = temp;
}

uint16_t
endian_hbe16(uint16_t bit) {
  if (ENDIAN_IS_LITTLE) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 1);
    return *((uint16_t *)buf);
  }
  return bit;
}

uint16_t
endian_hle16(uint16_t bit) {
  if (ENDIAN_IS_BIG) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 1);
    return *((uint16_t *)buf);
  }
  return bit;
}

uint16_t
endian_beh16(uint16_t bit) {
  if (ENDIAN_IS_LITTLE) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 1);
    return *((uint16_t *)buf);
  }
  return bit;
}

uint16_t
endian_leh16(uint16_t bit) {
  if (ENDIAN_IS_BIG) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 1);
    return *((uint16_t *)buf);
  }
  return bit;
}

uint32_t
endian_hbe32(uint32_t bit) {
  if (ENDIAN_IS_LITTLE) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 3);
    swap_bytes(buf + 1, buf + 2);
    return *((uint32_t *)buf);
  }
  return bit;
}

uint32_t
endian_hle32(uint32_t bit) {
  if (ENDIAN_IS_BIG) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 3);
    swap_bytes(buf + 1, buf + 2);
    return *((uint32_t *)buf);
  }
  return bit;
}

uint32_t
endian_beh32(uint32_t bit) {
  if (ENDIAN_IS_LITTLE) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 3);
    swap_bytes(buf + 1, buf + 2);
    return *((uint32_t *)buf);
  }
  return bit;
}

uint32_t
endian_leh32(uint32_t bit) {
  if (ENDIAN_IS_BIG) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 3);
    swap_bytes(buf + 1, buf + 2);
    return *((uint32_t *)buf);
  }
  return bit;
}

uint64_t
endian_hbe64(uint64_t bit) {
  if (ENDIAN_IS_LITTLE) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 7);
    swap_bytes(buf + 1, buf + 6);
    swap_bytes(buf + 2, buf + 5);
    swap_bytes(buf + 3, buf + 4);
    return *((uint64_t *)buf);
  }
  return bit;
}

uint64_t
endian_hle64(uint64_t bit) {
  if (ENDIAN_IS_BIG) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 7);
    swap_bytes(buf + 1, buf + 6);
    swap_bytes(buf + 2, buf + 5);
    swap_bytes(buf + 3, buf + 4);
    return *((uint64_t *)buf);
  }
  return bit;
}

uint64_t
endian_beh64(uint64_t bit) {
  if (ENDIAN_IS_LITTLE) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 7);
    swap_bytes(buf + 1, buf + 6);
    swap_bytes(buf + 2, buf + 5);
    swap_bytes(buf + 3, buf + 4);
    return *((uint64_t *)buf);
  }
  return bit;
}

uint64_t
endian_leh64(uint64_t bit) {
  if (ENDIAN_IS_BIG) {
    uint8_t *buf = (uint8_t *)&bit;
    swap_bytes(buf, buf + 7);
    swap_bytes(buf + 1, buf + 6);
    swap_bytes(buf + 2, buf + 5);
    swap_bytes(buf + 3, buf + 4);
    return *((uint64_t *)buf);
  }
  return bit;
}
