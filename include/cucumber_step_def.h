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

typedef void (cucumber_step) (cucumber_step_def_t *, void *);

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

CUCUMBER_EXPORT zrex_t *
    cucumber_step_def_rex (cucumber_step_def_t *self);

//  Runs this step definition
CUCUMBER_EXPORT void
    cucumber_step_def_run (cucumber_step_def_t *self, void *state);

//  Self test of this class
CUCUMBER_EXPORT void
    cucumber_step_def_test (bool verbose);

//  Sets the reason why the step failed
CUCUMBER_EXPORT void
    cucumber_step_def_set_failure_reason (cucumber_step_def_t *self, char *reason);

//  Returns true if the ran step def succeeded, Otherwise false.
CUCUMBER_EXPORT bool
    cucumber_step_def_failed (cucumber_step_def_t *self);

//  Returns the reason of the failure, or NULL if ran did succeed.
CUCUMBER_EXPORT const char *
    cucumber_step_def_failure_reason (cucumber_step_def_t *self);
//  @end

#ifdef __cplusplus
}
#endif

#endif
