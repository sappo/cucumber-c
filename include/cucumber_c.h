/*  =========================================================================
    cucumber-c - Cucumber implementation for C

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef CUCUMBER_C_H_INCLUDED
#define CUCUMBER_C_H_INCLUDED

//  Include the project library file
#include "cucumber_library.h"
#include "cucumber_asserts.h"

//  Step Definition Macros
//  -------------------------------------------------------------------------

#define STEP_DEFS(name, state_constructor, state_destructor)\
    void register_##name##_step_defs (cucumber_t *cucumber);\
    int main(int argc, char** argv) {\
        zargs_t *args = zargs_new (argc, argv);\
        if (zargs_hasx (args, "--help", "-h", NULL)) {\
            printf ("Usage: %s_step_defs [--verbose]\n", "" #name "");\
            return 0;\
        }\
        void *steps_args[3];\
        steps_args[0] = state_constructor;\
        steps_args[1] = state_destructor;\
        steps_args[2] = register_##name##_step_defs;\
        zactor_t *steps_runner = zactor_new (cucumber_steps_actor, steps_args);\
        if (zargs_has (args, "--verbose")) {\
            zstr_send (steps_runner, "VERBOSE");\
        }\
        assert (steps_runner);\
        printf ("Terminate by pressing ctrl-d\n");\
        while (true) {\
            if (feof(stdin)) break;\
            int c = getc(stdin);\
            if (c == EOF) break;\
        }\
        zstr_send (steps_runner, "$TERM");\
        zactor_destroy (&steps_runner);\
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
    zrex_fetch (cucumber_step_def_rex (self), __VA_ARGS__, NULL);

#endif
