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

    zsock_t *client = zsock_new_dealer (">tcp://127.0.0.1:8888");
    assert (client);

    zclock_sleep (250);  // Wait until connected
    // TODO wait until ready!

    cucumber_feature_runner_t *feature_runner = cucumber_feature_runner_new (filename);
    cucumber_feature_runner_run (feature_runner, client);

    zsock_destroy (&client);
    cucumber_feature_runner_destroy (&feature_runner);
    return 0;
}
