/*  =========================================================================
    gherkin_document - class description

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of DAFKA, a decentralized distributed streaming
    platform: http://zeromq.org.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

/*
@header
    gherkin_document -
@discuss
@end
*/

#include "cucumber_classes.h"

#include <file_reader.h>
#include <string_token_scanner.h>
#include <token_matcher.h>
#include <parser.h>
#include <ast_builder.h>
#include <compiler.h>
#include <gherkin_document_event.h>
#include <attachment_event.h>
#include <pickle_event.h>
#include <source_event.h>

//  Structure of our class

struct _cuc_gherkin_doc_t {
    const GherkinDocumentEvent *gherkin_document_event;
    SourceEvent* source_event;
    zlist_t *errors;
    bool valid;
    bool verbose;
};

//  Static helper methods

//  Prints an event into a newly allocated string
char *
s_event_to_string (const Event *event)
{
    assert (event);

    FILE *stream;
    char *string;
    size_t len;
    stream = open_memstream (&string, &len);
    Event_print (event, stream);
    fflush (stream);

    return string;
}

//  --------------------------------------------------------------------------
//  Create a new gherkin_document

cuc_gherkin_doc_t *
gherkin_document_new (const char *filename)
{
    cuc_gherkin_doc_t *self = (cuc_gherkin_doc_t *) zmalloc (sizeof (cuc_gherkin_doc_t));
    assert (self);

    self->verbose = false;

    TokenMatcher* token_matcher = TokenMatcher_new (L"en");
    Builder* builder = AstBuilder_new ();
    Parser* parser = Parser_new (builder);
    int rc = 0;

    FileReader* file_reader = FileReader_new(filename);
    self->source_event = SourceEvent_new(filename, FileReader_read(file_reader));

    TokenScanner* token_scanner = StringTokenScanner_new (self->source_event->source);
    rc = Parser_parse (parser, token_matcher, token_scanner);
    self->valid = rc == 0;

    if (self->valid) {
        self->gherkin_document_event = GherkinDocumentEvent_new (AstBuilder_get_result (builder, filename));
    }
    else {
        self->errors = zlist_new ();
        while (Parser_has_more_errors (parser)) {
            Error* error = Parser_next_error (parser);
            AttachmentEvent* attachment_event = AttachmentEvent_new (filename, error->location);
            AttacnmentEvent_transfer_error_text (attachment_event, error);
            zlist_append (self->errors, s_event_to_string ((const Event *) attachment_event));
            Event_delete ((Event *) attachment_event);
            Error_delete (error);
        }
    }

    TokenScanner_delete (token_scanner);
    FileReader_delete(file_reader);
    Parser_delete (parser);
    AstBuilder_delete (builder);
    TokenMatcher_delete (token_matcher);
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the gherkin_document

void
gherkin_document_destroy (cuc_gherkin_doc_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        cuc_gherkin_doc_t *self = *self_p;
        //  Free class properties
        Event_delete ((const Event *) self->source_event);
        if (self->valid)
            Event_delete ((const Event *) self->gherkin_document_event);
        else
            zlist_destroy (&self->errors);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Returns true if the read feature file was valid, otherwise false.

bool
gherkin_document_valid (cuc_gherkin_doc_t *self)
{
    assert (self);
    return self->valid;
}


//  --------------------------------------------------------------------------
//  Returns a list of errors if document is not valid, otherwise NULL.

zlist_t *
gherkin_document_errors (cuc_gherkin_doc_t *self)
{
    assert (self);
    return self->errors;
}


//  --------------------------------------------------------------------------
//  Get pickles

zlist_t *
gherkin_document_get_pickles (cuc_gherkin_doc_t *self)
{
    assert (self);
    Compiler* compiler = Compiler_new();
    zlist_t *pickles = zlist_new ();

    int rc = Compiler_compile (compiler,
                               self->gherkin_document_event->gherkin_document,
                               self->source_event->source);
    assert (rc == 0);
    while (Compiler_has_more_pickles (compiler)) {
        const Event* pickle_event = (const Event *) PickleEvent_new (Compiler_next_pickle (compiler));

        char *pickle_json = s_event_to_string (pickle_event);

        zlist_append (pickles, pickle_json);
        Event_delete (pickle_event);
    }
    return pickles;
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
cuc_gherkin_doc_test (bool verbose)
{
    printf (" * gherkin_document: ");

    //  @selftest
    //  GOOD feature file
    char *filename = NULL;
    filename = zsys_sprintf ("%s/%s", SELFTEST_DIR_RO, "minimal.feature");
    assert (filename);

    cuc_gherkin_doc_t *self = gherkin_document_new (filename);
    assert (self);
    assert (self->valid);

    zstr_free (&filename);
    gherkin_document_destroy (&self);

    //  BAD feature file
    filename = NULL;
    filename = zsys_sprintf ("%s/%s", SELFTEST_DIR_RO, "single_parser_error.feature");
    assert (filename);

    self = gherkin_document_new (filename);
    assert (self);
    assert (!self->valid);

    zstr_free (&filename);
    gherkin_document_destroy (&self);
    //  @end
    printf ("OK\n");
}
