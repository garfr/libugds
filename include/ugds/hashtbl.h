#ifndef UGDS_HASHTBL_H
#define UGDS_HASHTBL_H

#include "ugds/symbol.h"

typedef struct HashEntry {
  Symbol sym;
  void *data;
  struct HashEntry *next;
} HashEntry;

typedef void (*HashtblFree)(void *data);

typedef struct {
  size_t _n_entries;
  size_t _n_buckets;
  HashEntry **_buckets;
  HashtblFree _free_fn;
} Hashtbl;

typedef struct {
  Hashtbl *tbl;
  size_t bucket;
  HashEntry *entry;
} HashtblIter;

/* If a function pointer is NULL, the hashtable will do nothing when it is
 * called */
Hashtbl *hashtbl_init(HashtblFree free_fn);
void hashtbl_destroy(Hashtbl *tbl);

/* Returns NULL on memory allocation failure, replaces the value if the entry is
 * already there */
HashEntry *hashtbl_insert(Hashtbl *tbl, Symbol sym, void *data);
HashEntry *hashtbl_find(const Hashtbl *tbl, Symbol sym);

HashtblIter hashtbl_iter_init(Hashtbl *tbl);
HashEntry *hashtbl_iter_next(HashtblIter *iter);
HashEntry *hashtbl_iter_peek(HashtblIter *iter);
#endif
