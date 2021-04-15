#ifndef UGDS_HASHTBL_H
#define UGDS_HASHTBL_H

#include "ugds/symbol.h"

typedef struct UGDS_HashEntry {
  UGDS_Symbol sym;
  void *data;
  struct UGDS_HashEntry *next;
} UGDS_HashEntry;

typedef void (*UGDS_Hashtbl_free)(void *data);

typedef struct {
  size_t _n_entries;
  size_t _n_buckets;
  UGDS_HashEntry **_buckets;
  UGDS_Hashtbl_free _free_fn;
} UGDS_Hashtbl;

/* If a function pointer is NULL, the hashtable will do nothing when it is
 * called */
UGDS_Hashtbl *UGDS_init_hashtbl(UGDS_Hashtbl_free free_fn);
void UGDS_destroy_hashtbl(UGDS_Hashtbl *tbl);

/* Returns NULL on memory allocation failure, replaces the value if the entry is
 * already there */
UGDS_HashEntry *UGDS_insert_hashtbl(UGDS_Hashtbl *tbl, UGDS_Symbol sym,
                                    void *data);
UGDS_HashEntry *UGDS_find_hashtbl(const UGDS_Hashtbl *tbl, UGDS_Symbol sym);

#endif
