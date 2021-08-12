// Reuse some tests from modernize-pass-by-value.cpp,
// to show that header file warnings can be skipped by skip-headers,
// but fix to header files is already suppressed by -header-filter.
//
// (1) -fix --skip-headers=0, suppress warning and fix in .h file
//          showmain,noheader fixmain, nofixheader
// (2) -fix --skip-headers=1, skip warning and fix in .h file
//          showmain,skipheader fixmain, nofixheader
// (3) -fix --skip-headers=0 -header-filter=.* , show and fix warnings in .h file
//          showmain,showheader fixmain, fixheader
// (4) -fix --skip-headers=1 -header-filter=.* , show and fix warnings in .h file
//          showmain,showheader fixmain, fixheader
//
// (1)
// RUN: cp %S/Inputs/modernize-pass-by-value/header.h %T/pass-by-value-header.h
// RUN: cp %S/modernize-pass-by-value-header.cpp %T/pass-by-value-header.cpp
// RUN: clang-tidy %T/pass-by-value-header.cpp -checks='-*,modernize-pass-by-value' \
// RUN: --skip-headers=0 -fix -- -std=c++11 2>&1 \
// RUN: | FileCheck %s -check-prefixes=SHOWMAIN,NOHEADER
// RUN: FileCheck -input-file=%T/pass-by-value-header.h %s -check-prefix=NOFIXHEADER
// RUN: FileCheck -input-file=%T/pass-by-value-header.cpp %s -check-prefix=FIXMAIN
//
// (2)
// RUN: cp %S/Inputs/modernize-pass-by-value/header.h %T/pass-by-value-header.h
// RUN: cp %S/modernize-pass-by-value-header.cpp %T/pass-by-value-header.cpp
// RUN: clang-tidy %T/pass-by-value-header.cpp -checks='-*,modernize-pass-by-value' \
// RUN: --skip-headers -fix -- -std=c++11 2>&1 \
// RUN: | FileCheck %s -check-prefixes=SHOWMAIN,SKIPHEADER
// RUN: FileCheck -input-file=%T/pass-by-value-header.h %s -check-prefix=NOFIXHEADER
// RUN: FileCheck -input-file=%T/pass-by-value-header.cpp %s -check-prefix=FIXMAIN
//
// (3)
// RUN: cp %S/Inputs/modernize-pass-by-value/header.h %T/pass-by-value-header.h
// RUN: cp %S/modernize-pass-by-value-header.cpp %T/pass-by-value-header.cpp
// RUN: clang-tidy %T/pass-by-value-header.cpp -checks='-*,modernize-pass-by-value' \
// RUN: --header-filter=.* --skip-headers=0 -fix -- -std=c++11 2>&1 \
// RUN: | FileCheck %s -check-prefixes=SHOWMAIN,SHOWHEADER
// RUN: FileCheck -input-file=%T/pass-by-value-header.h %s -check-prefix=FIXHEADER
// RUN: FileCheck -input-file=%T/pass-by-value-header.cpp %s -check-prefix=FIXMAIN
//
// (4)
// RUN: cp %S/Inputs/modernize-pass-by-value/header.h %T/pass-by-value-header.h
// RUN: cp %S/modernize-pass-by-value-header.cpp %T/pass-by-value-header.cpp
// RUN: clang-tidy %T/pass-by-value-header.cpp -checks='-*,modernize-pass-by-value' \
// RUN: --header-filter=.* --skip-headers -fix -- -std=c++11 2>&1 \
// RUN: | FileCheck %s -check-prefixes=SHOWMAIN,SHOWHEADER
// RUN: FileCheck -input-file=%T/pass-by-value-header.h %s -check-prefix=FIXHEADER
// RUN: FileCheck -input-file=%T/pass-by-value-header.cpp %s -check-prefix=FIXMAIN

#include "pass-by-value-header.h"
// FIXHEADER: #include <utility>
// FIXHEADER: struct A
// FIXHEADER: A(ThreadId tid) : threadid(std::move(tid)) {}
// NOFIXHEADER-NOT: #include <utility>
// NOFIXHEADER: struct A
// NOFIXHEADER: A(const ThreadId &tid) : threadid(tid) {}
// NOFIXHEADER-NOT: A(ThreadId tid) : threadid(std::move(tid)) {}

// Test that both declaration and definition are updated.
struct D {
  D(const Movable &M);
  // FIXMAIN: D(Movable M);
  Movable M;
};
// NOTE! Use DAG tage because header file warnings may shown after
// main file warnings on Linux,  but before main file warnings on Windows.
// SHOWHEADER-DAG: :8:5: warning: pass by value and use std::move [modernize-pass-by-value]
// NOHEADER-NOT:   :8:5: warning: pass by value and use std::move [modernize-pass-by-value]
// SKIPHEADER-NOT: :8:5: warning: pass by value and use std::move [modernize-pass-by-value]

D::D(const Movable &M) : M(M) {}
// SHOWMAIN-DAG: :[[@LINE-1]]:6: warning: pass by value and use std::move
// FIXMAIN: D::D(Movable M) : M(std::move(M)) {}

// no more warning or error
// SHOWMAIN-NOT:   {{warning|error}}:
// SHOWHEADER-NOT: {{warning|error}}:
// NOHEADER-NOT:   {{warning|error}}:
// SKIPHEADER-NOT: {{warning|error}}:
//
// NOHEADER: Suppressed {{.*}} warnings
// SKIPHEADER-NOT: Suppressed {{.*}} warnings
// SHOWHEADER-NOT: Suppressed {{.*}} warnings
