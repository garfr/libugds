#ifndef UGDS_STRING_H
#define UGDS_STRING_H

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

void UGDS_destroy_string(UGDS_String *string);

#endif
