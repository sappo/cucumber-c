/*  =========================================================================
    step_defs - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef STEP_DEFS_H_INCLUDED
#define STEP_DEFS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
typedef struct _my_state_t my_state_t;

//  Create a new step_defs state
CUCUMBER_EXPORT my_state_t *
    my_state_new (bool verbose);

//  Destroy the step_defs state
CUCUMBER_EXPORT void
    my_state_destroy (my_state_t **self_p);

CUCUMBER_EXPORT void
    register_protocol_step_defs (cucumber_t *cucumber);

CUCUMBER_EXPORT void
    step_defs_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
