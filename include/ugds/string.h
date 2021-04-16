#ifndef UGDS_STRING_H
#define UGDS_STRING_H

#include <stdio.h>

#include "utf8proc.h"

#include "ugds/vector.h"

/* The String is a mutable string class designed as an alternative to
 * regular C strings.  It can be shrunk and grown and holds ownership of its own
 * data.  It has non-insignificant overhead in creation and destruction, so it
 * shouldn't be used when its features aren't needed */

typedef Vec String;

typedef struct StringIter {
  uint8_t *buf_ptr;
  utf8proc_ssize_t len;
} StringIter;

/* Creates an empty string */
String *string_init();

/* Copies the string, doesn't take ownership or borrow */
String *string_init_of_c_string(const char *c);

/* Returns true if they are equal */
bool string_equal(const String *str1, const String *str2);

/* This appends str2's value onto str1, returning false on failure */
bool string_concat(String *str1, const String *str2);
/* Returns the Unicode codepoint at index, or -1 if the string or index is
 * invalid */
int32_t string_index(String *str, size_t index);

/* Returns an iterator over the codepoints in the string */
StringIter string_chars(String *str);

/* Returns -1 if there are no more codepoints to iterate */
int32_t string_iter_next(StringIter *iter);
int32_t string_iter_peek(StringIter *iter);

void string_destroy(String *string);
void string_print(FILE *file, const String *string);

#endif
