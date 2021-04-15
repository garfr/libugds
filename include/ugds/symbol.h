#ifndef UGDS_SYMBOL_H
#define UGDS_SYMBOL_H

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

/* A Symbol is an immutable string that is not designed for string operations.
 * There will be a seperate type for a mutable string designed for string
 * processing. */

typedef struct {
  unsigned const char *text;
  size_t len;
} UGDS_Symbol;

/* Doesn't take ownership of the string, freeing it is left to the caller */
UGDS_Symbol UGDS_symbol_from_c_string(const char *str);

bool UGDS_symbol_equal(UGDS_Symbol sym1, UGDS_Symbol sym2);
/* Prints string to fild */
void UGDS_print_symbol(FILE *file, UGDS_Symbol);

#endif
