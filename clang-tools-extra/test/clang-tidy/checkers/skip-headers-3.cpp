// Test --skip-headers, which should skip statements and expressions
// in a called function of header files even if the calling function
// is in the main file.
//
// RUN: clang-tidy %s --skip-headers=0 --show-all-warnings \
// RUN: -checks='-*,cert-dcl16-c' \
// RUN: | FileCheck %s -check-prefixes WARNC,MAIN
//
// RUN: clang-tidy %s --skip-headers --show-all-warnings \
// RUN: -checks='-*,cert-dcl16-c' --header-filter=c1.h \
// RUN: | FileCheck %s -check-prefixes MAIN,WARNC
//
// RUN: clang-tidy %s --skip-headers --show-all-warnings \
// RUN: -checks='-*,cert-dcl16-c' \
// RUN: | FileCheck %s -check-prefixes MAIN,NOWARNC

#include "Inputs/skip-headers/c1.h"
// WARNC: c1.h:5:27: warning: integer literal has suffix 'll', which is not uppercase [cert-dcl16-c]
// WARNC: c1.h:6:20: warning: integer literal has suffix 'll', which is not uppercase [cert-dcl16-c]
// NOWARNC-NOT: c1.h:5:27: warning: integer literal has suffix 'll', which is not uppercase [cert-dcl16-c]
// NOWARNC-NOT: c1.h:6:20: warning: integer literal has suffix 'll', which is not uppercase [cert-dcl16-c]

void foo(int x = 3ll, C1 *p = nullptr) {
  // MAIN: skip-headers-3.cpp:[[@LINE-1]]:18: warning: integer literal has suffix 'll', which is not uppercase [cert-dcl16-c]
  C1::foo1();
  p->foo2();
  p->foo3(x);
}

// MAIN-NOT: warning: integer literal has suffix 'll', which is not uppercase [cert-dcl16-c]
