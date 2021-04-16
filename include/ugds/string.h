#ifndef UGDS_STRING_H
#define UGDS_STRING_H

#include <stdio.h>

#include "ugds/vector.h"

/* The String is a mutable string class designed as an alternative to
 * regular C strings.  It can be shrunk and grown and holds ownership of its own
 * data.  It has non-insignificant overhead in creation and destruction, so it
 * shouldn't be used when its features aren't needed */

typedef Vec String;

/* Creates an empty string */
String *string_init();

/* Copies the string, doesn't take ownership or borrow */
String *string_init_of_c_string(const char *c);

/* Returns true if they are equal */
bool string_equal(const String *str1, const String *str2);

/* This appends str2's value onto str1, returning false on failure */
bool string_concat(String *str1, const String *str2);

void string_destroy(String *string);
void string_print(FILE *file, const String *string);

#endif
