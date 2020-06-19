/*  =========================================================================
    cucumber_steps_args - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    cucumber_steps_args -
@discuss
@end
*/

#include "cucumber_classes.h"

//  Structure of our class

struct _cucumber_steps_args_t {
    cucumber_state_constructor_fn *state_constructor;
    cucumber_state_destructor_fn *state_destructor;
    cucumber_register_step_defs_fn *register_step_defs;
};


//  --------------------------------------------------------------------------
//  Create a new cucumber_steps_args

cucumber_steps_args_t *
cucumber_steps_args_new (
        cucumber_state_constructor_fn *state_constructor,
        cucumber_state_destructor_fn *state_destructor,
        cucumber_register_step_defs_fn *register_step_defs)
{
    cucumber_steps_args_t *self = (cucumber_steps_args_t *) zmalloc (sizeof (cucumber_steps_args_t));
    assert (self);
    //  Initialize class properties
    self->state_constructor = state_constructor;
    self->state_destructor = state_destructor;
    self->register_step_defs = register_step_defs;
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the cucumber_steps_args

void
cucumber_steps_args_destroy (cucumber_steps_args_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        cucumber_steps_args_t *self = *self_p;
        //  Free class properties
        self->state_constructor = NULL;
        self->state_destructor = NULL;
        self->register_step_defs = NULL;
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

cucumber_state_constructor_fn *
cucumber_steps_args_state_constructor_fn (cucumber_steps_args_t *self)
{
    assert (self);
    return self->state_constructor;
}

cucumber_state_destructor_fn *
cucumber_steps_args_state_destructor_fn (cucumber_steps_args_t *self)
{
    assert (self);
    return self->state_destructor;
}

cucumber_register_step_defs_fn *
cucumber_steps_args_register_step_defs_fn (cucumber_steps_args_t *self)
{
    assert (self);
    return self->register_step_defs;
}
//  --------------------------------------------------------------------------
//  Self test of this class

// If your selftest reads SCMed fixture data, please keep it in
// src/selftest-ro; if your test creates filesystem objects, please
// do so under src/selftest-rw.
// The following pattern is suggested for C selftest code:
//    char *filename = NULL;
//    filename = zsys_sprintf ("%s/%s", SELFTEST_DIR_RO, "mytemplate.file");
//    assert (filename);
//    ... use the "filename" for I/O ...
//    zstr_free (&filename);
// This way the same "filename" variable can be reused for many subtests.
#define SELFTEST_DIR_RO "src/selftest-ro"
#define SELFTEST_DIR_RW "src/selftest-rw"

void
cucumber_steps_args_test (bool verbose)
{
    printf (" * cucumber_steps_args: ");

    //  @selftest
    //  Simple create/destroy test
    cucumber_steps_args_t *self = cucumber_steps_args_new (NULL, NULL, NULL);
    assert (self);
    cucumber_steps_args_destroy (&self);
    //  @end
    printf ("OK\n");
}
