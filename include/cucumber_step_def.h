/*  =========================================================================
    cucumber_step_def - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef CUCUMBER_STEP_DEF_H_INCLUDED
#define CUCUMBER_STEP_DEF_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @interface

typedef void (cucumber_step) (zrex_t *);

//  Create a new cucumber_step_def
CUCUMBER_EXPORT cucumber_step_def_t *
    cucumber_step_def_new (const char *text, cucumber_step *step);

//  Destroy the cucumber_step_def
CUCUMBER_EXPORT void
    cucumber_step_def_destroy (cucumber_step_def_t **self_p);

//  Returns true if the text matches the previously compiled step expression, 
//  Otherwise false.
CUCUMBER_EXPORT bool
    cucumber_step_def_matches (cucumber_step_def_t *self, const char *text);

//  Runs this step definition
CUCUMBER_EXPORT void

//  Self test of this class
CUCUMBER_EXPORT void
    cucumber_step_def_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
