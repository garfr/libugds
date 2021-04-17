#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "ugds/hashtbl.h"
#include "ugds/symbol.h"
#include "ugds/vector.h"
#include "ugds/string.h"
#include "utf8proc.h"

#define HASHTBL_INIT_BUCKETS 8

/* ---- Vec ---- */

/* Rounds number to nearest power of 2 */
static inline size_t
roundup(size_t num) {
  size_t ret = 1;
  while (ret < num) {
    ret *= 2;
  }
  return ret;
}

Vec *
vec_init(size_t item_size) {
  Vec *ret = calloc(1, sizeof(Vec));
  if (!ret) {
    return NULL;
  }

  ret->_item_size = item_size;
  ret->_buf = NULL;
  ret->_alloc = 0;
  ret->len = 0;
  return ret;
}

Vec *
vec_init_of_size(size_t item_size, size_t init_len) {
  Vec *ret = calloc(1, sizeof(Vec));
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
vec_push(Vec *vec, void *item) {
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
vec_index(const Vec *vec, size_t index) {
  if (index >= vec->len) {
    return NULL;
  }
  return &vec->_buf[index * vec->_item_size];
}

bool
vec_reserve(Vec *vec, size_t amount) {
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
vec_destroy(Vec *vec) {
  if (vec->_buf) {
    free(vec->_buf);
  }
  free(vec);
}

/* ---- Symbol ---- */

Symbol
symbol_init(const char *str) {
  Symbol ret = {.text = (const unsigned char *)str, .len = strlen(str)};
  return ret;
}

void
symbol_print(FILE *file, Symbol sym) {
  fprintf(file, "%.*s", (int)sym.len, sym.text);
}

bool
symbol_equal(Symbol sym1, Symbol sym2) {
  if (sym1.len != sym2.len) {
    return false;
  }

  return strncmp((const char *)sym1.text, (const char *)sym2.text, sym1.len) ==
         0;
}

/* ---- Hashtbl ---- */

Hashtbl *
hashtbl_init(HashtblFree free_fn) {
  Hashtbl *table = calloc(1, sizeof(Hashtbl));
  if (!table) {
    return NULL;
  }

  table->_buckets = calloc(HASHTBL_INIT_BUCKETS, sizeof(HashEntry *));
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
hashtbl_destroy(Hashtbl *tbl) {
  for (size_t i = 0; i < tbl->_n_buckets; i++) {
    for (HashEntry *entry = tbl->_buckets[i]; entry != NULL;) {
      if (tbl->_free_fn) {
        tbl->_free_fn(entry->data);
      }
      HashEntry *old = entry;
      entry = entry->next;
      free(old);
    }
  }
  free(tbl->_buckets);
  free(tbl);
}

static size_t
hash_symbol(Symbol sym) {
  size_t total = 0;
  for (size_t i = 0; i < sym.len; i++) {
    total += sym.text[i];
  }
  return total;
}

HashEntry *
hashtbl_insert(Hashtbl *tbl, Symbol sym, void *data) {
  size_t index = hash_symbol(sym) % tbl->_n_buckets;

  HashEntry *forward, *follow;
  forward = tbl->_buckets[index];
  follow = forward;

  while (forward != NULL) {
    if (symbol_equal(forward->sym, sym)) {
      forward->data = data;
      return forward;
    }
    follow = forward;
    forward = forward->next;
  }

  forward = calloc(1, sizeof(HashEntry));
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

HashEntry *
hashtbl_find(const Hashtbl *tbl, Symbol sym) {
  size_t index = hash_symbol(sym) % tbl->_n_buckets;

  HashEntry *entry;
  for (entry = tbl->_buckets[index]; entry != NULL; entry = entry->next) {
    if (symbol_equal(entry->sym, sym)) {
      return entry;
    }
  }
  return NULL;
}

HashtblIter
hashtbl_iter_init(Hashtbl *tbl) {
  HashtblIter iter;
  iter.tbl = tbl;
  iter.bucket = 0;
  iter.entry = NULL;
  return iter;
}

HashEntry *
hashtbl_iter_next(HashtblIter *iter) {
  if (iter->entry != NULL) {
    HashEntry *ret = iter->entry;
    iter->entry = iter->entry->next;
    return ret;
  }
  if (iter->bucket < iter->tbl->_n_buckets) {
    iter->entry = iter->tbl->_buckets[iter->bucket];
    iter->bucket++;
    return hashtbl_iter_next(iter);
  }
  return NULL;
}

HashEntry *
hashtbl_iter_peek(HashtblIter *iter) {
  if (iter->entry != NULL) {
    return iter->entry;
  }
  if (iter->bucket < iter->tbl->_n_buckets) {
    return iter->tbl->_buckets[iter->bucket];
  }
  return NULL;
}

/* ---- String ---- */

String *
string_init() {
  return vec_init(sizeof(uint8_t));
}

String *
string_init_of_c_string(const char *c) {
  size_t len = strlen(c);
  Vec *ret = vec_init_of_size(sizeof(uint8_t), len);
  if (!ret) {
    return NULL;
  }
  ret->len = len;
  memcpy(ret->_buf, c, len);
  return ret;
}

void
string_destroy(String *string) {
  vec_destroy(string);
}

bool
string_equal(const String *str1, const String *str2) {
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
string_concat(String *str1, const String *str2) {
  if (!vec_reserve(str1, str2->len)) {
    return false;
  }

  memcpy(&str1->_buf[str1->len], str2->_buf, str2->len);
  str1->len += str2->len;
  return true;
}

void
string_print(FILE *file, const String *string) {
  fprintf(file, "%.*s", (int)string->len, (const unsigned char *)string->_buf);
}

int32_t
string_index(String *str, size_t index) {
  uint8_t *temp_text = str->_buf;
  utf8proc_ssize_t temp_len = str->len;
  int32_t codepoint;
  for (size_t i = 0; i < index; i++) {
    utf8proc_ssize_t result = utf8proc_iterate(temp_text, temp_len, &codepoint);
    if (codepoint == -1) {
      return -1;
    }
    temp_text += result;
    temp_len -= result;
    if (temp_len <= 0) {
      return -1;
    }
  }
  utf8proc_iterate(temp_text, temp_len, &codepoint);
  return codepoint;
}

StringIter
string_chars(String *str) {
  StringIter ret;
  ret.buf_ptr = str->_buf;
  ret.len = str->len;
  return ret;
}

int32_t
string_iter_next(StringIter *iter) {
  int32_t ret;
  utf8proc_ssize_t bytes_used =
      utf8proc_iterate(iter->buf_ptr, iter->len, &ret);
  if (ret != -1) {
    iter->buf_ptr += bytes_used;
    iter->len -= bytes_used;
  }
  return ret;
}

int32_t
string_iter_peek(StringIter *iter) {
  int32_t ret;

  utf8proc_iterate(iter->buf_ptr, iter->len, &ret);
  return ret;
}
