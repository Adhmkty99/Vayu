// When the statement of a template function call does not have a
// "valid" source location and its default argument value cast expression
// is in an included header file, the warning on the expression should
// be suppressed by --skip-headers.
//
// RUN: clang-tidy %s --skip-headers=0 \
// RUN: -checks='-*,modernize-use-nullptr' -- \
// RUN: | FileCheck %s -check-prefixes MAIN,NOWARNC
//
// RUN: clang-tidy %s --skip-headers --show-all-warnings \
// RUN: -checks='-*,modernize-use-nullptr' -- \
// RUN: | FileCheck %s -check-prefixes MAIN,NOWARNC
//
// RUN: clang-tidy %s --skip-headers=0 \
// RUN: -checks='-*,modernize-use-nullptr' -header-filter=c.h -- \
// RUN: | FileCheck %s -check-prefixes MAIN,WARNC
//
// RUN: clang-tidy %s --skip-headers --show-all-warnings \
// RUN: -checks='-*,modernize-use-nullptr' -header-filter=c.h -- \
// RUN: | FileCheck %s -check-prefixes MAIN,WARNC

#include "Inputs/skip-headers/c.h"
// WARNC: c.h:5:34: warning: use nullptr [modernize-use-nullptr]
// NOWARNC-NOT: c.h:5:34: warning: use nullptr [modernize-use-nullptr]
// NOWARNC-NOT: c.h:

template <class T>
class D {
public:
  template <class R>
  explicit D(const R r, int *x = 0) : p(x) {}
  // MAIN: :[[@LINE-1]]:34: warning: use nullptr [modernize-use-nullptr]
private:
  int *p;
};

C<int> x = C<int>(2);
D<int> y = D<int>(4);

// MAIN-NOT: warning:
