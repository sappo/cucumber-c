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
    zsock_t *server_socket;
    cucumber_t *cucumber;
    cucumber_state_constructor_fn *state_constructor;
    cucumber_state_destructor_fn *state_destructor;
    cucumber_register_step_defs_fn *register_step_defs;
    void *state;
};


//  --------------------------------------------------------------------------
//  Create a new cucumber_steps instance

static cucumber_steps_t *
cucumber_steps_new (zsock_t *pipe, cucumber_steps_args_t *steps_args)
{
    cucumber_steps_t *self = (cucumber_steps_t *) zmalloc (sizeof (cucumber_steps_t));
    assert (self);

    self->pipe = pipe;
    self->terminated = false;
    self->poller = zpoller_new (self->pipe, NULL);

    //  Initialize properties
    self->server_socket = zsock_new_dealer (">tcp://127.0.0.1:8888");
    assert (self->server_socket);

    self->state_constructor = cucumber_steps_args_state_constructor_fn (steps_args);
    self->state_destructor = cucumber_steps_args_state_destructor_fn (steps_args);
    self->register_step_defs = cucumber_steps_args_register_step_defs_fn (steps_args);
    zpoller_add (self->poller, self->server_socket);
    zsock_send (self->server_socket, "s", "HELLO");
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
        zsock_destroy (&self->server_socket);
        cucumber_destroy (&self->cucumber);

        //  Free object itself
        zpoller_destroy (&self->poller);
        free (self);
        *self_p = NULL;
    }
}

//  Here we handle incoming message from the cucumber runner

static void
cucumber_steps_recv_pickle (cucumber_steps_t *self)
{
    char *command = zstr_recv (self->server_socket);
    if (!command)
        return;

    if (streq (command, "START SCENARIO")) {
        char *id = zstr_recv (self->server_socket);
        void *state = NULL;
        if (self->state_constructor) {
            state = self->state_constructor (self->verbose);
        }
        self->cucumber = cucumber_new (state, (cucumber_state_destructor_fn *) self->state_destructor);
        self->register_step_defs (self->cucumber);
        zsock_send (self->server_socket, "ss", "SCENARIO STARTED", id);
        zstr_free (&id);
        if (self->verbose)
            printf ("Scenario started\n");
    }
    else
    if (streq (command, "END SCENARIO")) {
        char *id = zstr_recv (self->server_socket);
        cucumber_destroy (&self->cucumber);
        zsock_send (self->server_socket, "ss", "SCENARIO ENDED", id);
        zstr_free (&id);
        if (self->verbose)
            printf ("Scenario ended\n");
    }
    else
    if (streq (command, "RUN STEP")) {
        char *id = zstr_recv (self->server_socket);
        char *pickle_step = zstr_recv (self->server_socket);
        cucumber_step_def_t *step_def = cucumber_find_step_def (self->cucumber, pickle_step);
        if (step_def) {
            cucumber_step_def_run (step_def, cucumber_state (self->cucumber));
            if (cucumber_step_def_failed (step_def)) {
                zsock_send (self->server_socket,
                            "sss", "STEP FAILED",
                            id, cucumber_step_def_failure_reason (step_def));
            } else {
                zsock_send (self->server_socket, "sss", "STEP SUCCEEDED", id, "OK");
            }
        }
        else {
            zsock_send (self->server_socket, "ss", "STEP NOT FOUND", id);
        }

        if (self->verbose)
            printf ("Step %s ran\n", id);

        zstr_free (&id);
        zstr_free (&pickle_step);
    }

    zstr_free (&command);
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
    cucumber_steps_t * self = cucumber_steps_new (pipe, (cucumber_steps_args_t *) args);
    if (!self)
        return;          //  Interrupted

    //  Signal actor successfully initiated
    zsock_signal (self->pipe, 0);

    while (!self->terminated) {
        zsock_t *which = (zsock_t *) zpoller_wait (self->poller, 0);
        if (which == self->pipe)
            cucumber_steps_recv_api (self);
        else
        if (which == self->server_socket)
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
    /* zactor_t *cucumber_steps = zactor_new (cucumber_steps_actor, NULL); */
    /* assert (cucumber_steps); */
    /* zstr_send (cucumber_steps, "$TERM"); */
    /* zactor_destroy (&cucumber_steps); */
    //  @end

    printf ("OK\n");
}
