/*  =========================================================================
    cucumber_feature_runner - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    cucumber_feature_runner -
@discuss
@end
*/

#include "cucumber_classes.h"

#define DEFAULT "\033[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define BLUE    "\x1B[34m"
#define YELLOW  "\033[33m"

//  Structure of our class

struct _cucumber_feature_runner_t {
    zlist_t *feature_files;
};


//  --------------------------------------------------------------------------
//  Create a new cucumber_feature_runner

cucumber_feature_runner_t *
cucumber_feature_runner_new (const char *fileOrDirname)
{
    cucumber_feature_runner_t *self = (cucumber_feature_runner_t *) zmalloc (sizeof (cucumber_feature_runner_t));
    assert (self);
    //  Initialize class properties
    self->feature_files = zlist_new ();
    zfile_t *featureFileOrDirectory = zfile_new (NULL, fileOrDirname);
    if (zfile_is_directory (featureFileOrDirectory)) {
        zdir_t *feature_directory = zdir_new (fileOrDirname, NULL);
        zlist_t *files = zdir_list (feature_directory);
        zfile_t *file = (zfile_t *) zlist_first (files);
        while (file) {
            const char *filename = zfile_filename (file, NULL);
            if (strstr (filename, ".feature")) {
                zlist_append (self->feature_files, strdup (filename));
            }
            file = (zfile_t *) zlist_next (files);
        }
        zlist_destroy (&files);
        zdir_destroy (&feature_directory);
    }
    else {
        zlist_append (self->feature_files, strdup (fileOrDirname));
    }
    zfile_destroy (&featureFileOrDirectory);
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the cucumber_feature_runner

void
cucumber_feature_runner_destroy (cucumber_feature_runner_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        cucumber_feature_runner_t *self = *self_p;
        //  Free class properties here
        zlist_destroy (&self->feature_files);
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

//  --------------------------------------------------------------------------
//  Runs the scenario in the given feature file.

bool
cucumber_feature_runner_run (cucumber_feature_runner_t *self, zsock_t *client)
{
    bool isSuccessful = true;
    char *featurefile = (char *) zlist_pop (self->feature_files);
    while (featurefile) {
        cuc_gherkin_doc_t *gherkin_document = gherkin_document_new (featurefile);
        if (gherkin_document_valid (gherkin_document)) {
            zlist_t *pickles = gherkin_document_get_pickles (gherkin_document);
            char *pickle_json = (char *) zlist_first (pickles);
            while (pickle_json != NULL) {
                cuc_pickle_t *pickle = pickle_new (pickle_json);
                zstr_free (&pickle_json);

                printf ("%sScenario: %s%s\n", YELLOW, pickle_name (pickle), DEFAULT);
                zsock_send (client, "ss", "START SCENARIO", pickle_id (pickle));
                char *command, *message;
                zsock_recv (client, "ss", &command, &message);
                assert (streq (command, "SCENARIO STARTED"));
                assert (streq (message, pickle_id (pickle)));
                zstr_free (&command);
                zstr_free (&message);

                const char *pickle_step = pickle_first_step (pickle);
                while (pickle_step != NULL) {
                    printf ("%s  Step: %s%s\r", BLUE, pickle_step, DEFAULT);
                    fflush (stdout);
                    zsock_send (client, "sss", "RUN STEP", pickle_id (pickle), pickle_step);
                    char *result;
                    zsock_recv (client, "sss", &command, &message, &result);
                    assert (streq (message, pickle_id (pickle)));

                    if (streq (command, "STEP SUCCEEDED")) {
                        char *output = __to_string("%s  Step: %s (%s)%s", GREEN, pickle_step, result, DEFAULT);
                        puts (output);
                        zstr_free (&output);
                    }
                    else
                    if (streq (command, "STEP FAILED")) {
                        char *output = __to_string("%s  Step: %s (FAILURE)%s", RED, pickle_step, DEFAULT);
                        puts (output);
                        zstr_free (&output);
                        printf ("\n\t%s%s%s\n", RED, result, DEFAULT);
                        isSuccessful = false;
                        break;
                    }
                    else
                    if (streq (command, "STEP ERRORED")) {
                        char *output = __to_string("%s  Step: %s (ERROR)%s", RED, pickle_step, DEFAULT);
                        puts (output);
                        zstr_free (&output);
                        printf ("\n\t%s%s%s\n", RED, result, DEFAULT);
                        isSuccessful = false;
                        break;
                    }
                    else {
                        printf ("\n\n%s(Unknown command: %s)%s\n", RED, command, DEFAULT);
                        isSuccessful = false;
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
                pickle_json = (char *) zlist_next (pickles);
                printf ("\n");
            }
            zlist_destroy (&pickles);
        }
        else {
            printf ("Unable to parse feature file\n");
            zlist_t *errors = gherkin_document_errors (gherkin_document);
            const char *error = (const char *) zlist_first (errors);
            while (error) {
                printf ("[ERROR] %s\n", error);
                error = (const char *) zlist_first (errors);
            }
        }
        gherkin_document_destroy (&gherkin_document);

        zstr_free (&featurefile);
        featurefile = (char *) zlist_pop (self->feature_files);
    }
    return isSuccessful;
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
cucumber_feature_runner_test (bool verbose)
{
    printf (" * cucumber_feature_runner: ");

    //  @selftest
    //  Simple create/destroy test
    cucumber_feature_runner_t *self = cucumber_feature_runner_new (
                                          "src/selftest-ro/minimal.feature"
                                      );
    assert (self);
    cucumber_feature_runner_destroy (&self);
    //  @end
    printf ("OK\n");
}
