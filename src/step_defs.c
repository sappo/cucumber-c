/*  =========================================================================
    step defs -

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/
#include "cuc_classes.h"

void
given_a_topic (zrex_t *rex) {
    const char *text, *topic;
    FETCH_PARAMS(&text, &topic)
    //printf ("OUTPUT: Creating topic %s for %s\n", topic, text);
}

void
when_message_is_sent (zrex_t *rex) {
    const char *text, *topic;
    FETCH_PARAMS(&text, &topic)
    //printf ("OUTPUT: Sending %s on topic %s\n", text, topic);
}

STEP_DEFS(protocol) {
    GIVEN("a dafka (\\w+) subscribed to topic '(\\w+)'",
          given_a_topic)

    WHEN("a (\\w+) message with sequence larger 0 is sent on topic '(\\w+)'",
         when_message_is_sent)
}
