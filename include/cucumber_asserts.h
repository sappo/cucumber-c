/*  =========================================================================
    cucumber_asserts - Assert that work within cucumber step definitions

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef CUCUMBER_ASSERTS
#define CUCUMBER_ASSERTS

//  Assert Macros
//  -------------------------------------------------------------------------

#define __to_string_declaration(type, format) \
    char* type##_to_string (type value);

#define __equals_declaration(type) \
    bool type##_equals (type actual, type expected);

#define __compare_declaration(type, format) \
    __to_string_declaration(type, format) \
    __equals_declaration(type)

__compare_declaration(bool, "%s")
__compare_declaration(char, "%c")
__compare_declaration(short, "%i")
__compare_declaration(int, "%i")
__compare_declaration(long, "%l")
__compare_declaration(float, "%f")
__compare_declaration(double, "%f")
__compare_declaration(size_t, "%zu")
__compare_declaration(int8_t, "%zu")
__compare_declaration(int16_t, "%zu")
__compare_declaration(int32_t, "%zu")
__compare_declaration(int64_t, "%zu")
__compare_declaration(uint8_t, "%zu")
__compare_declaration(uint16_t, "%zu")
__compare_declaration(uint32_t, "%zu")
__compare_declaration(uint64_t, "%zu")

char *
__to_string (const char *format, ...);

#define assert_that(actual_value, compare_fn, expected_value, to_string_fn) \
if (!compare_fn (actual_value, expected_value)) { \
    char *actual_str = to_string_fn (actual_value); \
    char *expected_str = to_string_fn (expected_value); \
    cucumber_step_def_set_failure_reason (self, \
            __to_string ("Expected '%s' but got '%s'", expected_str, actual_str)); \
    zstr_free (&actual_str); \
    zstr_free (&expected_str); \
    return; \
}

#define assert_that_bool(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, bool_to_string)

#define assert_that_char(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, char_to_string)

#define assert_that_short(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, short_to_string)

#define assert_that_int(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, int_to_string)

#define assert_that_long(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, long_to_string)

#define assert_that_float(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, float_to_string)

#define assert_that_double(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, double_to_string)

#define assert_that_size_t(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, size_t_to_string)

#define assert_that_int8_t(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, int8_t_to_string)

#define assert_that_int16_t(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, int16_t_to_string)

#define assert_that_int32_t(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, int32_t_to_string)

#define assert_that_int64_t(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, int64_t_to_string)

#define assert_that_uint8_t(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, uint8_t_to_string)

#define assert_that_uint16_t(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, uint16_t_to_string)

#define assert_that_uint32_t(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, uint32_t_to_string)

#define assert_that_uint64_t(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, uint64_t_to_string)

#define assert_that_str(actual_value, compare_fn, expected_value)\
    assert_that(actual_value, compare_fn, expected_value, strdup)

CUCUMBER_EXPORT void
    cucumber_asserts_test (bool verbose);

#endif
