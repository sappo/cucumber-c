/*  =========================================================================
    cucumber_steps - Actor to execute a set of step definitions

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef CUCUMBER_STEPS_H_INCLUDED
#define CUCUMBER_STEPS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


//  @interface
//  Create new cucumber_steps actor instance.
//  @TODO: Describe the purpose of this actor!
//
//      zactor_t *cucumber_steps = zactor_new (cucumber_steps, NULL);
//
//  Destroy cucumber_steps instance.
//
//      zactor_destroy (&cucumber_steps);
//
//  Enable verbose logging of commands and activity:
//
//      zstr_send (cucumber_steps, "VERBOSE");
//
//  Start cucumber_steps actor.
//
//      zstr_sendx (cucumber_steps, "START", NULL);
//
//  Stop cucumber_steps actor.
//
//      zstr_sendx (cucumber_steps, "STOP", NULL);
//
//  This is the cucumber_steps constructor as a zactor_fn;
CUCUMBER_EXPORT void
    cucumber_steps_actor (zsock_t *pipe, void *args);

//  Self test of this actor
CUCUMBER_EXPORT void
    cucumber_steps_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
