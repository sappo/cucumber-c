/*  =========================================================================
    pickle - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    pickle -
@discuss
@end
*/

#include "cucumber_classes.h"

//  Structure of our class

typedef struct _cuc_pickle_step_t {
    char *text;
} cuc_pickle_step_t;

void
pickle_step_destroy (cuc_pickle_step_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        cuc_pickle_step_t *self = *self_p;
        //  Free class properties
        zstr_free (&self->text);
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


struct _cuc_pickle_t {
    char *name;         //  Name of this pickle
    zlistx_t *steps;     //  Steps of this pickle
};


//  --------------------------------------------------------------------------
//  Create a new pickle from a pickle JSON

cuc_pickle_t *
pickle_new (const char *json)
{
    cuc_pickle_t *self = (cuc_pickle_t *) zmalloc (sizeof (cuc_pickle_t));
    assert (self);
    //  Initialize class properties
    self->steps = zlistx_new ();
    zlistx_set_destructor (self->steps, (zlistx_destructor_fn *) pickle_step_destroy);

    cJSON *pickle_json = cJSON_Parse(json);

    cJSON *pickle = cJSON_GetObjectItemCaseSensitive (pickle_json, "pickle");
    cJSON *name = cJSON_GetObjectItemCaseSensitive (pickle, "name");
    self->name = strdup (name->valuestring);

    cJSON *steps = cJSON_GetObjectItemCaseSensitive (pickle, "steps");
    cJSON *step;
    cJSON_ArrayForEach(step, steps) {
        cJSON *text = cJSON_GetObjectItemCaseSensitive (step, "text");
        cuc_pickle_step_t * cuc_step = (cuc_pickle_step_t *) zmalloc (sizeof (cuc_pickle_step_t));
        cuc_step->text = strdup (text->valuestring);
        zlistx_add_end (self->steps, cuc_step);
    }

    cJSON_Delete (pickle_json);
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the pickle

void
pickle_destroy (cuc_pickle_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        cuc_pickle_t *self = *self_p;
        //  Free class properties
        zstr_free (&self->name);
        zlistx_destroy (&self->steps);
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Gets the name of this pickle.

const char *
pickle_name (cuc_pickle_t *self)
{
    assert (self);
    return self->name;
}


//  --------------------------------------------------------------------------
//  Returns the first step for this pickle or NULL if there are no steps.

const char *
pickle_first_step (cuc_pickle_t *self)
{
    assert (self);
    cuc_pickle_step_t *step = (cuc_pickle_step_t *) zlistx_first (self->steps);
    if (step)
        return step->text;

    return NULL;
}


//  --------------------------------------------------------------------------
//  Returns the next step for this pickle or NULL if there's no next.

const char *
pickle_next_step (cuc_pickle_t *self)
{
    assert (self);
    cuc_pickle_step_t *step = (cuc_pickle_step_t *) zlistx_next (self->steps);
    if (step)
        return step->text;

    return NULL;
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
cuc_pickle_test (bool verbose)
{
    printf (" * pickle: ");

    //  @selftest
    //  Simple create/destroy test
    cuc_pickle_t *self = pickle_new ("");
    assert (self);
    pickle_destroy (&self);
    //  @end
    printf ("OK\n");
}
