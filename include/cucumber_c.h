/*  =========================================================================
    cucumber-c - Cucumber implementation for C

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef CUCUMBER_C_H_H_INCLUDED
#define CUCUMBER_C_H_H_INCLUDED

//  Include the project library file
#include "cuc_library.h"

//  Add your own public definitions here, if you need them
#define STEP_DEFS(name)\
    void register_##name##_step_defs (cucumber_t *cucumber);\
    \
    int main() {\
        zsock_t *server = zsock_new_pull ("@tcp://127.0.0.1:8888");\
        assert (server);\
        cucumber_t *cucumber = cucumber_new ();\
        register_##name##_step_defs (cucumber);\
        while (true) {\
            char *pickle_json = zstr_recv (server);\
            if (!pickle_json)\
                break;\
            \
            cuc_pickle_t *pickle = pickle_new (pickle_json);\
            zstr_free (&pickle_json);\
            printf ("Scenario: %s\n", pickle_name (pickle));\
            const char *pickle_step = pickle_first_step (pickle);\
            while (pickle_step != NULL) {\
                cucumber_step_def_t *step_def = cucumber_find_step_def (cucumber, pickle_step);\
                printf ("  Step: %s ", pickle_step);\
                if (step_def) {\
                    cucumber_step_def_run (step_def);\
                    printf ("(OK)\n");\
                }\
                else {\
                    printf ("(Error: Step definition not found!)\n");\
                    break;\
                }\
                pickle_step = pickle_next_step (pickle);\
            }\
            pickle_destroy (&pickle);\
        }\
        zsock_destroy (&server);\
        return 0;\
    }\
    \
    void register_##name##_step_defs (cucumber_t *cucumber)

#define STEP(step_text, step_fun) {\
    cucumber_step_def_t *step_def = cucumber_step_def_new (step_text, step_fun);\
    cucumber_add_step_def (cucumber, step_def);\
}

#define GIVEN(step_text, step_fun) STEP(step_text, step_fun)
#define WHEN(step_text, step_fun) STEP(step_text, step_fun)
#define THEN(step_text, step_fun) STEP(step_text, step_fun)

#define FETCH_PARAMS(...)\
    zrex_fetch (rex, __VA_ARGS__, NULL);


#endif
