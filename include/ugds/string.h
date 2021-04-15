#ifndef UGDS_STRING_H
#define UGDS_STRING_H

#include <stdio.h>

#include "ugds/vector.h"

/* The UGDS_String is a mutable string class designed as an alternative to
 * regular C strings.  It can be shrunk and grown and holds ownership of its own
 * data.  It has non-insignificant overhead in creation and destruction, so it
 * shouldn't be used when its features aren't needed */

typedef UGDS_Vector UGDS_String;

/* Creates an empty string */
UGDS_String *UGDS_init_string();

/* Copies the string, doesn't take ownership or borrow */
UGDS_String *UGDS_init_string_from_c_string(const char *c);

/* Returns true if they are equal */
bool UGDS_equal_string(const UGDS_String *str1, const UGDS_String *str2);

/* This appends str2's value onto str1, returning false on failure */
bool UGDS_concat_string(UGDS_String *str1, const UGDS_String *str2);

void UGDS_destroy_string(UGDS_String *string);
void UGDS_print_string(FILE *file, const UGDS_String *string);

#endif
