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

    zsock_t *client = zsock_new_push (">tcp://127.0.0.1:8888");
    assert (client);

    zclock_sleep (250);  // Wait until connected
    // TODO wait until ready!

    cuc_gherkin_doc_t *gherkin_document = gherkin_document_new (filename);
    if (gherkin_document_valid (gherkin_document)) {
        zlist_t *pickles = gherkin_document_get_pickles (gherkin_document);
        char *pickle_json = zlist_first (pickles);
        while (pickle_json != NULL) {
            zstr_send (client, pickle_json);
            pickle_json = zlist_next (pickles);
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
