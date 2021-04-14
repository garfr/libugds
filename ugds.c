#include <stdlib.h>
#include <string.h>

#include "ugds/vector.h"

/* Rounds number to nearest power of 2 */
static inline size_t roundup(size_t num) {
  size_t ret = 1;
  while (ret < num) {
    ret *= 2;
  }
  return ret;
}

UGDS_Vector *UGDS_init_vector(size_t item_size) {
  UGDS_Vector *ret = calloc(1, sizeof(UGDS_Vector));
  if (!ret) {
    return NULL;
  }

  ret->_item_size = item_size;
  ret->_buf = NULL;
  ret->_alloc = 0;
  ret->len = 0;
  return ret;
}

UGDS_Vector *UGDS_init_vector_of_len(size_t item_size, size_t init_len) {
  UGDS_Vector *ret = calloc(1, sizeof(UGDS_Vector));
  if (!ret) {
    return NULL;
  }

  ret->_item_size = item_size;
  ret->_alloc = roundup(init_len);
  if (init_len == 0) {
    ret->_buf = NULL;
  } else {
    ret->_buf = calloc(ret->_alloc, ret->_item_size);
    if (!ret->_buf) {
      return NULL;
    }
  }

  ret->len = 0;
  return NULL;
}

void *UGDS_push_vector(UGDS_Vector *vec, void *item) {
  /* Realloc needed */
  if (vec->len + 1 >= vec->_alloc) {
    size_t new_alloc = vec->len == 0 ? 1 : vec->len * 2;
    size_t new_size = new_alloc * vec->_item_size;
    uint8_t *new_buffer = realloc(vec->_buf, new_size);
    if (!new_buffer) {
      return NULL;
    }

    vec->_alloc = new_alloc;
    vec->_buf = new_buffer;
  }

  uint8_t *index = &vec->_buf[(vec->len++) * vec->_item_size];
  memcpy(index, item, vec->_item_size);
  return index;
}

void *UGDS_index_vector(UGDS_Vector *vec, size_t index) {
  if (index >= vec->len) {
    return NULL;
  }
  return &vec->_buf[index * vec->_item_size];
}

void UGDS_destroy_vector(UGDS_Vector *vec) {
  if (vec->_buf) {
    free(vec->_buf);
  }
  free(vec);
}
