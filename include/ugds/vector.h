#ifndef UGDS_VECTOR_H
#define UGDS_VECTOR_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  size_t len;        /* How many items are in the buffer, in number of items */
  size_t _item_size; /* How much memory the item stored takes, in bytes */
  size_t _alloc;     /* How much memory is allocated, in number of items */
  uint8_t *_buf;
} Vec;

/* Both of these return NULL on any allocation failure */
Vec *vec_init(size_t item_size);
Vec *vec_init_of_size(size_t item_size, size_t init_len);

/* Returns NULL on failure to push vector, otherwise a pointer to the newly
 * pushed item */
void *vec_append(Vec *vec, void *item);

/* Returns NULL on out of bounds */
void *vec_index(const Vec *vec, size_t index);

/* Will not fail */
void vec_destroy(Vec *vec);
bool vec_reserve(Vec *vec, size_t amount);

#endif
