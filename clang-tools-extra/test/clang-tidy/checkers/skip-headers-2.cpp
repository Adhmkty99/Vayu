// Test --skip-headers, --show-all-warnings, and --header-filter
// with nested Decls. Users should be able to select exactly
// which .h file to check for warnings.
// Here a.h includes/uses b.h. Maintainers of a.h wants to check/see
// warnings in a.h but not b.h. So they use --header-filer=a.h --skip-headers.
//
// RUN: clang-tidy %s --skip-headers=0 --show-all-warnings \
// RUN: -checks='-*,readability-convert-member-functions-to-static' -- \
// RUN: | FileCheck %s -check-prefixes WARNA,WARNB,WARNMAIN
//
// RUN: clang-tidy %s --skip-headers=1 --show-all-warnings \
// RUN: -checks='-*,readability-convert-member-functions-to-static' -- \
// RUN: | FileCheck %s -check-prefixes WARNMAIN,NOWARNA,NOWARNB
//
// RUN: clang-tidy %s --skip-headers=1 --show-all-warnings --header-filter=a.h \
// RUN: -checks='-*,readability-convert-member-functions-to-static' -- \
// RUN: | FileCheck %s -check-prefixes WARNMAIN,WARNA,NOWARNB
//
// RUN: clang-tidy %s --skip-headers=1 --show-all-warnings --header-filter=.* \
// RUN: -checks='-*,readability-convert-member-functions-to-static' -- \
// RUN: | FileCheck %s -check-prefixes WARNMAIN,WARNA,WARNB
//
// Current limitation of --skip-haders: b.h is skipped if a.h is skipped.
// RUN: clang-tidy %s --skip-headers=1 --show-all-warnings --header-filter=b.h \
// RUN: -checks='-*,readability-convert-member-functions-to-static' -- \
// RUN: | FileCheck %s -check-prefixes WARNMAIN,NOWARNA,NOWARNB

#include "Inputs/skip-headers/a.h"

// WARNA: warning: method 'fooA' can be made static
// NOWARNA-NOT: warning: method 'fooA' can be made static

// WARNB: warning: method 'fooB' can be made static
// NOWARNB-NOT: warning: method 'fooB' can be made static

class C {
  void foo(int x) { x = 3; };
  // WARNMAIN: warning: method 'foo' can be made static
};
