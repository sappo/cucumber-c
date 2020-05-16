/*  =========================================================================
    cucumber_step_def - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    cucumber_step_def -
@discuss
@end
*/

#include "cucumber_classes.h"

//  Structure of our class

struct _cucumber_step_def_t {
    zrex_t *rex;
    cucumber_step *step;
    char *failed_str;
};


//  --------------------------------------------------------------------------
//  Create a new cucumber_step_def

cucumber_step_def_t *
cucumber_step_def_new (const char *text, cucumber_step *step)
{
    cucumber_step_def_t *self = (cucumber_step_def_t *) zmalloc (sizeof (cucumber_step_def_t));
    assert (self);

    self->rex = zrex_new (text);
    self->step = step;
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the cucumber_step_def

void
cucumber_step_def_destroy (cucumber_step_def_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        cucumber_step_def_t *self = *self_p;
        //  Free class properties
        zrex_destroy (&self->rex);
        zstr_free (&self->failed_str);
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Returns true if the text matches the previously compiled step expression,
//  Otherwise false.

bool
cucumber_step_def_matches (cucumber_step_def_t *self, const char *text)
{
    assert (self);
    assert (text);
    return zrex_matches (self->rex, text);
}


//  --------------------------------------------------------------------------

zrex_t *
cucumber_step_def_rex (cucumber_step_def_t *self)
{
    assert (self);
    return self->rex;
}

//  --------------------------------------------------------------------------
//  Runs this step definition

void
cucumber_step_def_run (cucumber_step_def_t *self, void *state)
{
    assert (self);
    self->step (self, state);
}


//  --------------------------------------------------------------------------
//  Sets the reason why the step failed

void
cucumber_step_def_set_failure_reason (cucumber_step_def_t *self, char *reason)
{
    assert (self);
    self->failed_str = reason;
}


//  --------------------------------------------------------------------------
//  Returns true if the ran step def succeeded, Otherwise false.

bool
cucumber_step_def_failed (cucumber_step_def_t *self)
{
    assert (self);
    return self->failed_str != NULL;
}


//  --------------------------------------------------------------------------
//  Returns the reason of the failure, or NULL if ran did succeed.

const char *
cucumber_step_def_failure_reason (cucumber_step_def_t *self)
{
    assert (self);
    return self->failed_str;
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
cucumber_step_def_test (bool verbose)
{
    printf (" * cucumber_step_def: ");

    //  @selftest
    //  Simple create/destroy test
    cucumber_step_def_t *self = cucumber_step_def_new ("", NULL);
    assert (self);
    cucumber_step_def_destroy (&self);
    //  @end
    printf ("OK\n");
}
