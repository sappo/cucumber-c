/*  =========================================================================
    step defs -

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/
#include "cucumber_classes.h"

struct _my_state_t {
    int filler;
};
typedef struct _my_state_t my_state_t;

my_state_t *
my_state_new ()
{
    return (my_state_t *) zmalloc (sizeof (my_state_t));
}

void
my_state_destroy (my_state_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        my_state_t *self = *self_p;
        //  Free class properties
        self->filler = 0;
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

void
given_a_topic (cucumber_step_def_t *self, void *state_p) {
    const char *text, *topic;
    FETCH_PARAMS(&text, &topic)
    my_state_t *state = (my_state_t *) state_p;
}

void
when_message_is_sent (cucumber_step_def_t *self, void *state_p) {
    const char *text, *topic;
    FETCH_PARAMS(&text, &topic)
    my_state_t *state = (my_state_t *) state_p;
}

STEP_DEFS(protocol, my_state_new, my_state_destroy) {
    GIVEN("a dafka (\\w+) subscribed to topic '(\\w+)'",
          given_a_topic)

    WHEN("a (\\w+) message with sequence larger 0 is sent on topic '(\\w+)'",
         when_message_is_sent)
}
