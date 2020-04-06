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

typedef struct _my_state_t {
    int filler;
} my_state_t;

void
given_a_topic (zrex_t *rex, void *state_p) {
    const char *text, *topic;
    FETCH_PARAMS(&text, &topic)
    my_state_t *state = (my_state_t *) state_p;
}

void
when_message_is_sent (zrex_t *rex, void *state_p) {
    const char *text, *topic;
    FETCH_PARAMS(&text, &topic)
    my_state_t *state = (my_state_t *) state_p;
}

STEP_DEFS(protocol, my_state_t) {
    GIVEN("a dafka (\\w+) subscribed to topic '(\\w+)'",
          given_a_topic)

    WHEN("a (\\w+) message with sequence larger 0 is sent on topic '(\\w+)'",
         when_message_is_sent)
}
