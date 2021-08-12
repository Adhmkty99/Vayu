// Same output with skip-headers=0, skip-headers=1, or skip-headers
//
// With --skip-headers=0 and no --header-filter, there should be no warning
// shown for a.h or b.h. But there seems to be a bug and we are seeing the A1 warnings.
// RUN: %check_clang_tidy -check-suffixes=ALL,A1 %s bugprone-forward-declaration-namespace %t -- --skip-headers=0 \
// RUN:   -- -I%S/Inputs/bugprone-forward-declaration-namespace
// RUN: %check_clang_tidy -check-suffixes=ALL %s bugprone-forward-declaration-namespace %t -- --skip-headers=1 \
// RUN:   -- -I%S/Inputs/bugprone-forward-declaration-namespace
// RUN: %check_clang_tidy -check-suffixes=ALL %s bugprone-forward-declaration-namespace %t -- --skip-headers \
// RUN:   -- -I%S/Inputs/bugprone-forward-declaration-namespace
//
// Same output with header-filter=b.h because there is no warning on b.h
// RUN: %check_clang_tidy -check-suffixes=ALL %s bugprone-forward-declaration-namespace %t -- --skip-headers \
// RUN:   --header-filter=b.h -- -I%S/Inputs/bugprone-forward-declaration-namespace
//
// With --header-filter, we should see all warnings from a.h.
// RUN: %check_clang_tidy -check-suffixes=ALL,A,A1 %s bugprone-forward-declaration-namespace %t -- --skip-headers=0 \
// RUN:   --header-filter=.* -- -I%S/Inputs/bugprone-forward-declaration-namespace
// RUN: %check_clang_tidy -check-suffixes=ALL,A,A1 %s bugprone-forward-declaration-namespace %t -- --skip-headers \
// RUN:   --header-filter=.* -- -I%S/Inputs/bugprone-forward-declaration-namespace
// RUN: %check_clang_tidy -check-suffixes=ALL,A,A1 %s bugprone-forward-declaration-namespace %t -- --skip-headers=0 \
// RUN:   --header-filter=a.h -- -I%S/Inputs/bugprone-forward-declaration-namespace
// RUN: %check_clang_tidy -check-suffixes=ALL,A,A1 %s bugprone-forward-declaration-namespace %t -- --skip-headers \
// RUN:   --header-filter=a.h -- -I%S/Inputs/bugprone-forward-declaration-namespace

#include "a.h"

class T_A;

class T_A {
  int x;
};

class NESTED;
// CHECK-NOTES-ALL-DAG: :[[@LINE-1]]:7: warning: no definition found for 'NESTED', but a definition with the same name 'NESTED' found in another namespace '(anonymous namespace)::nq::(anonymous)'
// CHECK-NOTES-ALL-DAG: note: a definition of 'NESTED' is found here

namespace {
namespace nq {
namespace {
class NESTED {};
} // namespace
} // namespace nq
} // namespace

namespace na {
class T_B;
// CHECK-NOTES-ALL-DAG: :[[@LINE-1]]:7: warning: declaration 'T_B' is never referenced, but a declaration with the same name found in another namespace 'nb'
// CHECK-NOTES-ALL-DAG: note: a declaration of 'T_B' is found here
// CHECK-NOTES-ALL-DAG: :[[@LINE-3]]:7: warning: no definition found for 'T_B', but a definition with the same name 'T_B' found in another namespace 'nb'
// CHECK-NOTES-ALL-DAG: note: a definition of 'T_B' is found here
} // namespace na

#include "b.h"

namespace na {
class T_B;
// CHECK-NOTES-ALL-DAG: :[[@LINE-1]]:7: warning: declaration 'T_B' is never referenced, but a declaration with the same name found in another namespace 'nb'
// CHECK-NOTES-ALL-DAG: note: a declaration of 'T_B' is found here
// CHECK-NOTES-ALL-DAG: :[[@LINE-3]]:7: warning: no definition found for 'T_B', but a definition with the same name 'T_B' found in another namespace 'nb'
// CHECK-NOTES-ALL-DAG: note: a definition of 'T_B' is found here
} // namespace na

// A simple forward declaration. Although it is never used, but no declaration
// with the same name is found in other namespace.
class OUTSIDER;

namespace na {
// This class is referenced declaration, we don't generate warning.
class OUTSIDER_1;
} // namespace na

void f(na::OUTSIDER_1);

namespace nc {
// This class is referenced as friend in OOP.
class OUTSIDER_1;

class OOP {
  friend struct OUTSIDER_1;
};
} // namespace nc

namespace nd {
class OUTSIDER_1;
void f(OUTSIDER_1 *);
} // namespace nd

namespace nb {
class OUTSIDER_1;
// CHECK-NOTES-ALL-DAG: :[[@LINE-1]]:7: warning: declaration 'OUTSIDER_1' is never referenced, but a declaration with the same name found in another namespace 'na'
// CHECK-NOTES-ALL-DAG: note: a declaration of 'OUTSIDER_1' is found here
} // namespace nb

namespace na {
template <typename T>
class T_C;
}

namespace nb {
// FIXME: this is an error, but we don't consider template class declaration
// now.
template <typename T>
class T_C;
} // namespace nb

namespace na {
template <typename T>
class T_C {
  int x;
};
} // namespace na

namespace na {

template <typename T>
class T_TEMP {
  template <typename _Tp1>
  struct rebind { typedef T_TEMP<_Tp1> other; };
};

// We ignore class template specialization.
template class T_TEMP<char>;
} // namespace na

namespace nb {

template <typename T>
class T_TEMP_1 {
  template <typename _Tp1>
  struct rebind { typedef T_TEMP_1<_Tp1> other; };
};

// We ignore class template specialization.
extern template class T_TEMP_1<char>;
} // namespace nb

namespace nd {
class D;
// CHECK-NOTES-ALL-DAG: :[[@LINE-1]]:7: warning: declaration 'D' is never referenced, but a declaration with the same name found in another namespace 'nd::ne'
// CHECK-NOTES-ALL-DAG: note: a declaration of 'D' is found here
} // namespace nd

namespace nd {
namespace ne {
class D;
}
} // namespace nd

int f(nd::ne::D &d);

// This should be ignored by the check.
template <typename... Args>
class Observer {
  class Impl;
};

template <typename... Args>
class Observer<Args...>::Impl {
};

// Depending on test platforms, sometimes the warnings from .h files appear
// after the main file warnings. Use -DAG suffix to match different orders.
//
// Warnings on namespace { class T_A; }
// CHECK-NOTES-A-DAG: warning: declaration 'T_A' is never referenced, but a declaration with the same name found in another namespace 'na' [bugprone-forward-declaration-namespace]
// CHECK-NOTES-A-DAG: note: a declaration of 'T_A' is found here
// CHECK-NOTES-A1-DAG: warning: no definition found for 'T_A', but a definition with the same name 'T_A' found in another namespace '(global)' [bugprone-forward-declaration-namespace]
// CHECK-NOTES-A1-DAG: note: a definition of 'T_A' is found here
//
// Warnings on namespace na { class T_A; }
// CHECK-NOTES-A-DAG: warning: declaration 'T_A' is never referenced, but a declaration with the same name found in another namespace '(anonymous)'
// CHECK-NOTES-A-DAG: note: a declaration of 'T_A' is found here
// CHECK-NOTES-A1-DAG: warning: no definition found for 'T_A', but a definition with the same name 'T_A' found in another namespace '(global)'
// CHECK-NOTES-A1-DAG: note: a definition of 'T_A' is found here
