#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "ugds/hashtbl.h"
#include "ugds/symbol.h"
#include "ugds/vector.h"
#include "ugds/string.h"

#define HASHTBL_INIT_BUCKETS 8

/* ---- UGDS_Vector ---- */

/* Rounds number to nearest power of 2 */
static inline size_t
roundup(size_t num) {
  size_t ret = 1;
  while (ret < num) {
    ret *= 2;
  }
  return ret;
}

UGDS_Vector *
UGDS_init_vector(size_t item_size) {
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

UGDS_Vector *
UGDS_init_vector_of_len(size_t item_size, size_t init_len) {
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
  return ret;
}

void *
UGDS_push_vector(UGDS_Vector *vec, void *item) {
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

void *
UGDS_index_vector(const UGDS_Vector *vec, size_t index) {
  if (index >= vec->len) {
    return NULL;
  }
  return &vec->_buf[index * vec->_item_size];
}

bool
UGDS_reserve_vector(UGDS_Vector *vec, size_t amount) {
  if (vec->len + amount >= vec->_alloc) {
    size_t new_items = roundup(vec->len + amount);
    size_t new_size = new_items * vec->_item_size;
    uint8_t *buffer = realloc(vec->_buf, new_size);
    if (!buffer) {
      return false;
    }
    vec->_buf = buffer;
    vec->_alloc = new_items;
  }
  return true;
}

void
UGDS_destroy_vector(UGDS_Vector *vec) {
  if (vec->_buf) {
    free(vec->_buf);
  }
  free(vec);
}

/* ---- UGDS_Symbol ---- */

UGDS_Symbol
UGDS_init_symbol_from_c_string(const char *str) {
  UGDS_Symbol ret = {.text = (const unsigned char *)str, .len = strlen(str)};
  return ret;
}

void
UGDS_print_symbol(FILE *file, UGDS_Symbol sym) {
  fprintf(file, "%.*s", (int)sym.len, sym.text);
}

bool
UGDS_equal_symbol(UGDS_Symbol sym1, UGDS_Symbol sym2) {
  if (sym1.len != sym2.len) {
    return false;
  }

  return strncmp((const char *)sym1.text, (const char *)sym2.text, sym1.len) ==
         0;
}

/* ---- UGDS_Hashtbl ---- */

UGDS_Hashtbl *
UGDS_init_hashtbl(UGDS_Hashtbl_free free_fn) {
  UGDS_Hashtbl *table = calloc(1, sizeof(UGDS_Hashtbl));
  if (!table) {
    return NULL;
  }

  table->_buckets = calloc(HASHTBL_INIT_BUCKETS, sizeof(UGDS_HashEntry *));
  if (!table->_buckets) {
    free(table);
    return NULL;
  }

  table->_n_buckets = HASHTBL_INIT_BUCKETS;
  table->_n_entries = 0;
  table->_free_fn = free_fn;
  return table;
}

void
UGDS_destroy_hashtbl(UGDS_Hashtbl *tbl) {
  for (size_t i = 0; i < tbl->_n_buckets; i++) {
    for (UGDS_HashEntry *entry = tbl->_buckets[i]; entry != NULL;) {
      if (tbl->_free_fn) {
        tbl->_free_fn(entry->data);
      }
      UGDS_HashEntry *old = entry;
      entry = entry->next;
      free(old);
    }
  }
  free(tbl->_buckets);
  free(tbl);
}

static size_t
hash_symbol(UGDS_Symbol sym) {
  size_t total = 0;
  for (size_t i = 0; i < sym.len; i++) {
    total += sym.text[i];
  }
  return total;
}

UGDS_HashEntry *
UGDS_insert_hashtbl(UGDS_Hashtbl *tbl, UGDS_Symbol sym, void *data) {
  size_t index = hash_symbol(sym) % tbl->_n_buckets;

  UGDS_HashEntry *forward, *follow;
  forward = tbl->_buckets[index];
  follow = forward;

  while (forward != NULL) {
    if (UGDS_equal_symbol(forward->sym, sym)) {
      forward->data = data;
      return forward;
    }
    follow = forward;
    forward = forward->next;
  }

  forward = calloc(1, sizeof(UGDS_HashEntry));
  forward->sym = sym;
  forward->data = data;
  forward->next = NULL;
  if (follow) {
    follow->next = forward;
  } else {
    tbl->_buckets[index] = forward;
  }
  return forward;
}

UGDS_HashEntry *
UGDS_find_hashtbl(const UGDS_Hashtbl *tbl, UGDS_Symbol sym) {
  size_t index = hash_symbol(sym) % tbl->_n_buckets;

  UGDS_HashEntry *entry;
  for (entry = tbl->_buckets[index]; entry != NULL; entry = entry->next) {
    if (UGDS_equal_symbol(entry->sym, sym)) {
      return entry;
    }
  }
  return NULL;
}

/* ---- UGDS_String ---- */

UGDS_String *
UGDS_init_string() {
  return UGDS_init_vector(sizeof(unsigned char));
}

UGDS_String *
UGDS_init_string_from_c_string(const char *c) {
  size_t len = strlen(c);
  UGDS_Vector *ret = UGDS_init_vector_of_len(sizeof(unsigned char), len);
  if (!ret) {
    return NULL;
  }
  ret->len = len;
  memcpy(ret->_buf, c, len);
  return ret;
}

void
UGDS_destroy_string(UGDS_String *string) {
  UGDS_destroy_vector(string);
}

bool
UGDS_equal_string(const UGDS_String *str1, const UGDS_String *str2) {
  if (str1->len != str2->len) {
    return false;
  }
  for (size_t i = 0; i < str1->len; i++) {
    if (str1->_buf[i] != str2->_buf[i]) {
      return false;
    }
  }
  return true;
}

bool
UGDS_concat_string(UGDS_String *str1, const UGDS_String *str2) {
  if (!UGDS_reserve_vector(str1, str2->len)) {
    return false;
  }

  memcpy(&str1->_buf[str1->len], str2->_buf, str2->len);
  str1->len += str2->len;
  return true;
}

void
UGDS_print_string(FILE *file, const UGDS_String *string) {
  fprintf(file, "%.*s", (int)string->len, (const unsigned char *)string->_buf);
}
