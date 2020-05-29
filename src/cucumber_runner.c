/*  =========================================================================
    cucumber_runner - description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    cucumber_runner -
@discuss
@end
*/

#include "cucumber_classes.h"

#define RED    "\x1B[31m"
#define GREEN  "\x1B[32m"
#define BLUE   "\x1B[34m"
#define YELLOW "\033[33m"

int
main (int argc, char *argv [])
{
    zargs_t *args = zargs_new (argc, argv);

    bool verbose = false;
    if (zargs_has (args, "--verbose")) {
        verbose = true;
    }

    const char *filename = zargs_first (args);

    if (verbose)
        zsys_info ("cucumber_runner - ");

    zsock_t *client = zsock_new_dealer (">tcp://127.0.0.1:8888");
    assert (client);

    zclock_sleep (250);  // Wait until connected
    // TODO wait until ready!

    cuc_gherkin_doc_t *gherkin_document = gherkin_document_new (filename);
    if (gherkin_document_valid (gherkin_document)) {
        zlist_t *pickles = gherkin_document_get_pickles (gherkin_document);
        char *pickle_json = zlist_first (pickles);
        while (pickle_json != NULL) {
            cuc_pickle_t *pickle = pickle_new (pickle_json);
            zstr_free (&pickle_json);

            printf ("%sScenario: %s\n", YELLOW, pickle_name (pickle));
            zsock_send (client, "ss", "START SCENARIO", pickle_id (pickle));
            char *command, *message;
            zsock_recv (client, "ss", &command, &message);
            assert (streq (command, "SCENARIO STARTED"));
            assert (streq (message, pickle_id (pickle)));
            zstr_free (&command);
            zstr_free (&message);

            const char *pickle_step = pickle_first_step (pickle);
            while (pickle_step != NULL) {
                printf ("%s  Step: %s\r", BLUE, pickle_step);
				fflush (stdout);
                zsock_send (client, "sss", "RUN STEP", pickle_id (pickle), pickle_step);
                char *result;
                zsock_recv (client, "sss", &command, &message, &result);
                assert (streq (message, pickle_id (pickle)));

                if (streq (command, "STEP SUCCEEDED")) {
                    puts (__to_string("%s  Step: %s (%s)", GREEN, pickle_step, result));
                }
                else
                if (streq (command, "STEP FAILED")) {
                    puts (__to_string("%s  Step: %s (FAILURE)", RED, pickle_step, result));
                    printf ("\n\t%s\n", result);
                    break;
                }
                else
                if (streq (command, "STEP ERRORED")) {
                    puts (__to_string("%s  Step: %s (ERROR)", RED, pickle_step, result));
                    printf ("(ERROR)\n\t%s\n", result);
                    break;
                }
                else {
                    printf ("(Unknown command: %s)\n", command);
                    break;
                }

                zstr_free (&command);
                zstr_free (&message);
                zstr_free (&result);

                pickle_step = pickle_next_step (pickle);
            }
            zsock_send (client, "ss", "END SCENARIO", pickle_id (pickle));
            zsock_recv (client, "ss", &command, &message);
            assert (streq (command, "SCENARIO ENDED"));
            assert (streq (message, pickle_id (pickle)));
            zstr_free (&command);
            zstr_free (&message);

            pickle_destroy (&pickle);
            zstr_send (client, pickle_json);
            pickle_json = zlist_next (pickles);
            printf ("\n");
        }
        zlist_destroy (&pickles);
    }
    else {
        printf ("Unable to parse %s:\n", filename);
        zlist_t *errors = gherkin_document_errors (gherkin_document);
        const char *error = (const char *) zlist_first (errors);
        while (error) {
            printf ("[ERROR] %s\n", error);
            error = (const char *) zlist_first (errors);
        }
    }

    zsock_destroy (&client);
    gherkin_document_destroy (&gherkin_document);
    return 0;
}
