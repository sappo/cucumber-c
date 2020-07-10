/*  =========================================================================
    cucumber_feature_runner - Runs a feature file against a steps runner

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef CUCUMBER_FEATURE_RUNNER_H_INCLUDED
#define CUCUMBER_FEATURE_RUNNER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
//  Create a new cucumber_feature_runner
CUCUMBER_EXPORT cucumber_feature_runner_t *
    cucumber_feature_runner_new (const char *filename);

//  Destroy the cucumber_feature_runner
CUCUMBER_EXPORT void
    cucumber_feature_runner_destroy (cucumber_feature_runner_t **self_p);

CUCUMBER_EXPORT bool
    cucumber_feature_runner_run (cucumber_feature_runner_t *self, zsock_t *client, zlist_t *identities);

//  Self test of this class
CUCUMBER_EXPORT void
    cucumber_feature_runner_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
