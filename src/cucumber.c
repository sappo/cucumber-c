/*  =========================================================================
    cucumber - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    cucumber -
@discuss
@end
*/

#include "cucumber_classes.h"

//  Structure of our class

struct _cucumber_t {
    zlistx_t *step_defs;
    void *state;
};


//  --------------------------------------------------------------------------
//  Create a new cucumber

cucumber_t *
cucumber_new (void *state)
{
    cucumber_t *self = (cucumber_t *) zmalloc (sizeof (cucumber_t));
    assert (self);

    self->state = state;
    self->step_defs = zlistx_new ();
    zlistx_set_destructor (self->step_defs, (zlistx_destructor_fn *) cucumber_step_def_destroy);
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the cucumber

void
cucumber_destroy (cucumber_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        cucumber_t *self = *self_p;
        //  Free class properties
        self->state = NULL;
        zlistx_destroy (&self->step_defs);
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Returns the first matching step definition or NULL if no step definition 

void *
cucumber_state (cucumber_t *self) {
    assert (self);
    return self->state;
}


//  --------------------------------------------------------------------------
//  Returns the first matching step definition or NULL if no step definition 
//  matches.

cucumber_step_def_t *
cucumber_find_step_def (cucumber_t *self, const char *text) {
    assert (self);
    assert (text);
    cucumber_step_def_t *step_def = (cucumber_step_def_t *) zlistx_first (self->step_defs);
    while (step_def != NULL) {
        if (cucumber_step_def_matches (step_def, text)) {
            return step_def;
        }
        step_def = (cucumber_step_def_t *) zlistx_next (self->step_defs);
    }
    return NULL;
}


//  --------------------------------------------------------------------------
//  Get the step definitions

zlistx_t *
cucumber_get_step_defs (cucumber_t *self) {
    assert (self);
    return self->step_defs;
}


//  --------------------------------------------------------------------------
//  Add a step definition

void
cucumber_add_step_def (cucumber_t *self, cucumber_step_def_t *step_def) {
    assert (self);
    assert (step_def);
    zlistx_add_end (self->step_defs, step_def);
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
cucumber_test (bool verbose)
{
    printf (" * cucumber: ");

    //  @selftest
    //  Simple create/destroy test
    cucumber_t *self = cucumber_new (NULL);
    assert (self);
    cucumber_destroy (&self);
    //  @end
    printf ("OK\n");
}
