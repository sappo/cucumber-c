/*  =========================================================================
    cucumber_asserts - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    cucumber_asserts -
@discuss
@end
*/

#include "cucumber_classes.h"

//  Needed to satisfy zproject structure
struct _cucumber_asserts_t {
    int dummy;
};


char *
__to_string (const char *format, ...)
{
    va_list argptr;
    va_start (argptr, format);
    size_t needed = vsnprintf (0, 0, format, argptr);
    va_end (argptr);

    char *buffer = zmalloc (needed + 1);

    va_start (argptr, format);
    vsnprintf (buffer, needed + 1, format, argptr);
    va_end (argptr);

    return buffer;
}


#define __to_string_implementation(type, format) \
    char* type##_to_string (type value) { return __to_string (format, value); }

#define __equals_implementation(type) \
    bool type##_equals (type actual, type expected) { return actual == expected; }

#define __compare_implementation(type, format) \
    __to_string_implementation(type, format) \
    __equals_implementation(type)


__compare_implementation(bool, "%s")
__compare_implementation(char, "%c")
__compare_implementation(short, "%i")
__compare_implementation(int, "%i")
__compare_implementation(long, "%l")
__compare_implementation(float, "%f")
__compare_implementation(double, "%f")
__compare_implementation(size_t, "%zu")


void
cucumber_asserts_test (bool verbose)
{
    printf (" * cucumber_asserts: ");
    //  @selftest
    assert ( char_equals ('A', 'A'));
    assert (!char_equals ('A', 'B'));

    assert ( short_equals (1, 1));
    assert (!short_equals (1, 2));

    assert ( int_equals (1, 1));
    assert (!int_equals (1, 2));

    assert ( long_equals (1, 1));
    assert (!long_equals (1, 2));

    assert ( size_t_equals (1, 1));
    assert (!size_t_equals (1, 2));

    assert ( float_equals (1.0, 1.0));
    assert (!float_equals (1.0, 1.1));

    assert ( double_equals (1.0, 1.0));
    assert (!double_equals (1.0, 1.1));
    //  @end
    printf ("OK\n");
}
