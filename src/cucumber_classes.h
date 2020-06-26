/*  =========================================================================
    cucumber_classes - private header file

    Copyright (c) the Contributors as noted in the AUTHORS file. This
    file is part of cucumber-c.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
################################################################################
#  THIS FILE IS 100% GENERATED BY ZPROJECT; DO NOT EDIT EXCEPT EXPERIMENTALLY  #
#  Read the zproject/README.md for information about making permanent changes. #
################################################################################
    =========================================================================
*/

#ifndef CUCUMBER_CLASSES_H_INCLUDED
#define CUCUMBER_CLASSES_H_INCLUDED

//  Platform definitions, must come first
#include "platform.h"

//  Asserts check the invariants of methods. If they're not
//  fulfilled the program should fail fast. Therefore enforce them!
#ifdef NDEBUG
  #undef NDEBUG
  #include <assert.h>
  #define NDEBUG
#else
  #include <assert.h>
#endif

//  External API
#include "../include/cucumber_c.h"

//  Private external dependencies

//  Opaque class structures to allow forward references

//  Extra headers

//  Internal API


//  *** To avoid double-definitions, only define if building without draft ***
#ifndef CUCUMBER_BUILD_DRAFT_API

//  Self test for private classes
CUCUMBER_PRIVATE void
    cucumber_private_selftest (bool verbose, const char *subtest);

#endif // CUCUMBER_BUILD_DRAFT_API

#endif
