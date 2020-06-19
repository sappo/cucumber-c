/*  =========================================================================
    gherkin_document - Reads feature files and exposes scenarios as pickles

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef GHERKIN_DOCUMENT_H_INCLUDED
#define GHERKIN_DOCUMENT_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
//  Create a new gherkin_document
CUCUMBER_EXPORT cuc_gherkin_doc_t *
    gherkin_document_new (const char *filename);

//  Destroy the gherkin_document
CUCUMBER_EXPORT void
    gherkin_document_destroy (cuc_gherkin_doc_t **self_p);

//  Returns true if the read feature file was valid, otherwise false.
CUCUMBER_EXPORT bool
    gherkin_document_valid (cuc_gherkin_doc_t *self);

//  Returns a list of errors if document is not valid, otherwise NULL.
CUCUMBER_EXPORT zlist_t *
    gherkin_document_errors (cuc_gherkin_doc_t *self);

//  Get pickles
CUCUMBER_EXPORT zlist_t *
gherkin_document_get_pickles (cuc_gherkin_doc_t *self);

//  Self test of this class
CUCUMBER_EXPORT void
    cuc_gherkin_doc_test (bool verbose);

//  @end

#ifdef __cplusplus
}
#endif

#endif
