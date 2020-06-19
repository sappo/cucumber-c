/*  =========================================================================
    cucumber_steps_args - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef CUCUMBER_STEPS_ARGS_H_INCLUDED
#define CUCUMBER_STEPS_ARGS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
//  Create a new cucumber_steps_args
CUCUMBER_EXPORT cucumber_steps_args_t *
    cucumber_steps_args_new (
            cucumber_state_constructor_fn *state_constructor,
            cucumber_state_destructor_fn *state_destructor,
            cucumber_register_step_defs_fn *register_step_defs);

//  Destroy the cucumber_steps_args
CUCUMBER_EXPORT void
    cucumber_steps_args_destroy (cucumber_steps_args_t **self_p);

CUCUMBER_EXPORT cucumber_state_constructor_fn *
    cucumber_steps_args_state_constructor_fn (cucumber_steps_args_t *self);

CUCUMBER_EXPORT cucumber_state_destructor_fn *
    cucumber_steps_args_state_destructor_fn (cucumber_steps_args_t *self);

CUCUMBER_EXPORT cucumber_register_step_defs_fn *
    cucumber_steps_args_register_step_defs_fn (cucumber_steps_args_t *self);

//  Self test of this class
CUCUMBER_EXPORT void
    cucumber_steps_args_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
