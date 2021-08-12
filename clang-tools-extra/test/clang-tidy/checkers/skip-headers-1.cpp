// Test --skip-headers, --show-all-warnings, and --header-filter.
//
// Default shows no warning in .h files, with hint to use --header-filter
// RUN: clang-tidy %s -checks='*' --skip-headers=0 -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes COUNTER,NOHEADER,MAIN,HINT
//
// --skip-headers skips all included files warnings; without hint
// RUN: clang-tidy %s -checks='*' --skip-headers -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes COUNTER,NOHEADER,MAIN,NOHINT
//
// --show-all-warnings reports all warnings, even without --header-filters
// RUN: clang-tidy %s -checks='*' --show-all-warnings --skip-headers=0 -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes COUNTER,HEADER,MAIN,NOHINT
//
// --header-filter='.*' is like --show-all-warnings
// RUN: clang-tidy %s -checks='*' --header-filter='.*' --skip-headers=0 -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes COUNTER,HEADER,MAIN,NOHINT
//
// With --show-all-warnings and --skip-headers,
// no warnings should be reported from header files.
// RUN: clang-tidy %s -checks='*' --skip-headers --show-all-warnings -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes COUNTER,NOHEADER,MAIN,NOHINT
//
// --header-filter='header1.h' shows only warnings in header1.h
// RUN: clang-tidy %s -checks='*' --header-filter='header1.h' --skip-headers=0 -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes COUNTER,H1,MAIN,HINT
//
// RUN: clang-tidy %s -checks='*' --header-filter='header1.h' --skip-headers -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes COUNTER,H1,MAIN,NOHINT
//
// --header-filter='header2.h' shows only warnings in header2.h
// RUN: clang-tidy %s -checks='*' --header-filter='header2.h' --skip-headers=0 -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes COUNTER,H2,MAIN,HINT
//
// RUN: clang-tidy %s -checks='*' --header-filter='header2.h' --skip-headers -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes COUNTER,H2,MAIN,NOHINT

// All options have warning count reported:
// COUNTER: {{[0-9]+}} warnings generated.

#include "Inputs/skip-headers/my_header1.h"
// H1: my_header1.h:1:1: warning: header is missing header guard
// HEADER: my_header1.h:1:1: warning: header is missing header guard
// H2-NOT: my_header1.h
// NOHEADER-NOT: my_header1.h

// H2: my_header2.h:1:1: warning: header is missing header guard
// HEADER: my_header2.h:1:1: warning: header is missing header guard
// H1-NOT: my_header2.h
// NOHEADER-NOT: my_header2.h

int xyz = 135;
// MAIN: skip-headers-1.cpp:{{[0-9]+}}:{{[0-9]+}}: warning:

// HINT: Use -header-filter={{.*}} to display errors{{.*}}
// NOHINT-NOT: Use -header-filter=
