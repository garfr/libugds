#ifndef UGDS_VECTOR_H
#define UGDS_VECTOR_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  size_t len;        /* How many items are in the buffer, in number of items */
  size_t _item_size; /* How much memory the item stored takes, in bytes */
  size_t _alloc;     /* How much memory is allocated, in number of items */
  uint8_t *_buf;
} UGDS_Vector;

/* Both of these return NULL on any allocation failure */
UGDS_Vector *UGDS_init_vector(size_t item_size);
UGDS_Vector *UGDS_init_vector_of_len(size_t item_size, size_t init_len);

/* Returns NULL on failure to push vector, otherwise a pointer to the newly
 * pushed item */
void *UGDS_push_vector(UGDS_Vector *vec, void *item);

/* Returns NULL on out of bounds */
void *UGDS_index_vector(const UGDS_Vector *vec, size_t index);

/* Will not fail */
void UGDS_destroy_vector(UGDS_Vector *vec);
bool UGDS_reserve_vector(UGDS_Vector *vec, size_t amount);

#endif
