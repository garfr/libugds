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
} Symbol;

/* Doesn't take ownership of the string, freeing it is left to the caller */
Symbol symbol_init(const char *str);

bool symbol_equal(Symbol sym1, Symbol sym2);
/* Prints string to fild */
void symbol_print(FILE *file, Symbol sym);

#endif
