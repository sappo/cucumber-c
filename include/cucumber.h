/*  =========================================================================
    cucumber - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef CUCUMBER_H_INCLUDED
#define CUCUMBER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
//  Destructor for the state object
typedef void (cucumber_state_destructor_fn) (void **);

//  Create a new cucumber
CUCUMBER_EXPORT cucumber_t *
    cucumber_new (void *state, cucumber_state_destructor_fn state_destructor);

//  Destroy the cucumber
CUCUMBER_EXPORT void
    cucumber_destroy (cucumber_t **self_p);

//  Returns the first matching step definition or NULL if no step definition
//  matches.
CUCUMBER_EXPORT cucumber_step_def_t *
    cucumber_find_step_def (cucumber_t *self, const char *text);

//  Get the step definitions
CUCUMBER_EXPORT zlistx_t *
    cucumber_get_step_defs (cucumber_t *self);

CUCUMBER_EXPORT void *
    cucumber_state (cucumber_t *self);

//  Add a step definition
CUCUMBER_EXPORT void
    cucumber_add_step_def (cucumber_t *self, cucumber_step_def_t *step_def);

//  Self test of this class
CUCUMBER_EXPORT void
    cucumber_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
