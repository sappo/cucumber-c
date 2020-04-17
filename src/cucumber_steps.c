/*  =========================================================================
    cucumber_steps -

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    cucumber_steps -
@discuss
@end
*/

#include "cucumber_classes.h"

//  Structure of our actor

struct _cucumber_steps_t {
    //  Actor properties
    zsock_t *pipe;              //  Actor command pipe
    zpoller_t *poller;          //  Socket poller
    bool terminated;            //  Did caller ask us to quit?
    bool verbose;               //  Verbose logging enabled?
    //  Declare properties
    zsock_t *pickle_socket;
    cucumber_t *cucumber;
    void *state;
};


//  --------------------------------------------------------------------------
//  Create a new cucumber_steps instance

static cucumber_steps_t *
cucumber_steps_new (zsock_t *pipe, cucumber_t *cucumber)
{
    cucumber_steps_t *self = (cucumber_steps_t *) zmalloc (sizeof (cucumber_steps_t));
    assert (self);

    self->pipe = pipe;
    self->terminated = false;
    self->poller = zpoller_new (self->pipe, NULL);

    //  Initialize properties
    self->pickle_socket = zsock_new_pull ("@tcp://127.0.0.1:8888");
    assert (self->pickle_socket);

    zpoller_add (self->poller, self->pickle_socket);

    self->cucumber = cucumber;
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the cucumber_steps instance

static void
cucumber_steps_destroy (cucumber_steps_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        cucumber_steps_t *self = *self_p;

        //  Free actor properties
        zsock_destroy (&self->pickle_socket);
        cucumber_destroy (&self->cucumber);

        //  Free object itself
        zpoller_destroy (&self->poller);
        free (self);
        *self_p = NULL;
    }
}


//  Start this actor. Return a value greater or equal to zero if initialization
//  was successful. Otherwise -1.

static int
cucumber_steps_start (cucumber_steps_t *self)
{
    assert (self);

    //  TODO: Add startup actions

    return 0;
}


//  Stop this actor. Return a value greater or equal to zero if stopping
//  was successful. Otherwise -1.

static int
cucumber_steps_stop (cucumber_steps_t *self)
{
    assert (self);

    //  TODO: Add shutdown actions

    return 0;
}


//  Here we handle incoming message from the cucumber runner

static void
cucumber_steps_recv_pickle (cucumber_steps_t *self)
{
    char *pickle_json = zstr_recv (self->pickle_socket);
    if (!pickle_json)
        return;

    cuc_pickle_t *pickle = pickle_new (pickle_json);
    zstr_free (&pickle_json);
    printf ("Scenario: %s\n", pickle_name (pickle));
    const char *pickle_step = pickle_first_step (pickle);
    while (pickle_step != NULL) {
        cucumber_step_def_t *step_def = cucumber_find_step_def (self->cucumber, pickle_step);
        printf ("  Step: %s ", pickle_step);
        if (step_def) {
            cucumber_step_def_run (step_def, cucumber_state (self->cucumber));
            printf ("(OK)\n");
        }
        else {
            printf ("(Error: Step definition not found!)\n");
            break;
        }
        pickle_step = pickle_next_step (pickle);
    }
    pickle_destroy (&pickle);
}


//  Here we handle incoming message from the node

static void
cucumber_steps_recv_api (cucumber_steps_t *self)
{
    //  Get the whole message of the pipe in one go
    zmsg_t *request = zmsg_recv (self->pipe);
    if (!request)
       return;        //  Interrupted

    char *command = zmsg_popstr (request);
    if (streq (command, "VERBOSE"))
        self->verbose = true;
    else
    if (streq (command, "$TERM"))
        //  The $TERM command is send by zactor_destroy() method
        self->terminated = true;
    else {
        zsys_error ("invalid command '%s'", command);
        assert (false);
    }
    zstr_free (&command);
    zmsg_destroy (&request);
}


//  --------------------------------------------------------------------------
//  This is the actor which runs in its own thread.

void
cucumber_steps_actor (zsock_t *pipe, void *args)
{
    cucumber_steps_t * self = cucumber_steps_new (pipe, (cucumber_t *) args);
    if (!self)
        return;          //  Interrupted

    //  Signal actor successfully initiated
    zsock_signal (self->pipe, 0);

    while (!self->terminated) {
        zsock_t *which = (zsock_t *) zpoller_wait (self->poller, 0);
        if (which == self->pipe)
            cucumber_steps_recv_api (self);
        else
        if (which == self->pickle_socket)
            cucumber_steps_recv_pickle (self);
    }
    cucumber_steps_destroy (&self);
}

//  --------------------------------------------------------------------------
//  Self test of this actor.

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
cucumber_steps_test (bool verbose)
{
    printf (" * cucumber_steps: ");
    //  @selftest
    //  Simple create/destroy test
    zactor_t *cucumber_steps = zactor_new (cucumber_steps_actor, NULL);
    assert (cucumber_steps);

    zactor_destroy (&cucumber_steps);
    //  @end

    printf ("OK\n");
}
