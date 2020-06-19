/*  =========================================================================
    pickle - Reads a pickle JSON and exposes pickle steps

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef PICKLE_H_INCLUDED
#define PICKLE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
//  Create a new pickle
CUCUMBER_EXPORT cuc_pickle_t *
    pickle_new (const char *json);

//  Destroy the pickle
CUCUMBER_EXPORT void
    pickle_destroy (cuc_pickle_t **self_p);

//  Gets the ID of this pickle.
CUCUMBER_EXPORT const char *
    pickle_id (cuc_pickle_t *self);

//  Gets the name of this pickle.
CUCUMBER_EXPORT const char *
    pickle_name (cuc_pickle_t *self);

//  Returns the first step for this pickle.
CUCUMBER_EXPORT const char *
    pickle_first_step (cuc_pickle_t *self);

//  Returns the next step for this pickle.
CUCUMBER_EXPORT const char *
    pickle_next_step (cuc_pickle_t *self);

//  Self test of this class
CUCUMBER_EXPORT void
    cuc_pickle_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
