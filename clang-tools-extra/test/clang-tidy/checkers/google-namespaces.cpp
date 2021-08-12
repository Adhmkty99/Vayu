// RUN: clang-tidy %s --skip-headers=0 -checks='-*,google-build-namespaces,google-build-using-namespace' -header-filter='.*' -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes CHECK,CHECK1 -implicit-check-not="{{warning|error}}:"

// RUN: clang-tidy %s --skip-headers=0 -checks='-*,google-build-namespaces,google-build-using-namespace' -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes CHECK,CHECK0 -implicit-check-not="{{warning|error}}:"
//
// RUN: clang-tidy %s --skip-headers -checks='-*,google-build-namespaces,google-build-using-namespace' -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes CHECK,CHECK2 -implicit-check-not="{{warning|error}}:"
//
// -header-filter overrides --skip-header
// RUN: clang-tidy %s --skip-headers -header-filter='.*' -checks='-*,google-build-namespaces,google-build-using-namespace' -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes CHECK,CHECK1 -implicit-check-not="{{warning|error}}:"
//
// --show-all-warnings is like -header-filter=.* + -system-headers
// RUN: clang-tidy %s --skip-headers=0 --show-all-warnings -checks='-*,google-build-namespaces,google-build-using-namespace' -- \
// RUN: 2>&1 | FileCheck %s -check-prefixes CHECK,CHECK1 -implicit-check-not="{{warning|error}}:"

// --skip-header skips 1 warning in the header file.
// CHECK0: 7 warnings generated
// CHECK1: 7 warnings generated
// CHECK2: 6 warnings generated

#include "Inputs/google-namespaces.h"
// with -header-filter, warning in .h file is shown
// CHECK1: warning: do not use unnamed namespaces in header files [google-build-namespaces]
// without -header-filter, warning in .h files are not shown
// CHECK0-NOT: warning: do not use unnamed namespaces in header files [google-build-namespaces]
// with --skip-header, no warning in .h file is detected at all
// CHECK2-NOT: warning: do not use unnamed namespaces in header files [google-build-namespaces]

using namespace spaaaace;
// CHECK: :[[@LINE-1]]:1: warning: do not use namespace using-directives; use using-declarations instead [google-build-using-namespace]

using spaaaace::core; // no-warning

namespace std {
inline namespace literals {
inline namespace chrono_literals {
}
inline namespace complex_literals {
}
inline namespace string_literals {
}
}
}

using namespace std::chrono_literals;            // no-warning
using namespace std::complex_literals;           // no-warning
using namespace std::literals;                   // no-warning
using namespace std::literals::chrono_literals;  // no-warning
using namespace std::literals::complex_literals; // no-warning
using namespace std::literals::string_literals;  // no-warning
using namespace std::string_literals;            // no-warning

namespace literals {}

using namespace literals;
// CHECK: :[[@LINE-1]]:1: warning: do not use namespace using-directives; use using-declarations instead [google-build-using-namespace]

namespace foo {
inline namespace literals {
inline namespace bar_literals {}
}
}

using namespace foo::literals;
// CHECK: :[[@LINE-1]]:1: warning: do not use namespace using-directives; use using-declarations instead [google-build-using-namespace]

using namespace foo::bar_literals;
// CHECK: :[[@LINE-1]]:1: warning: do not use namespace using-directives; use using-declarations instead [google-build-using-namespace]

using namespace foo::literals::bar_literals;
// CHECK: :[[@LINE-1]]:1: warning: do not use namespace using-directives; use using-declarations instead [google-build-using-namespace]

namespace foo_literals {}

using namespace foo_literals;
// CHECK: :[[@LINE-1]]:1: warning: do not use namespace using-directives; use using-declarations instead [google-build-using-namespace]

// If -header-filter= is not used and there is some warning in .h file,
// give a reminder to use -header-filter.
// CHECK0: Use -header-filter={{.*}} to display errors{{.*}}
//
// If -header-filter= is used, no summary of this message.
// CHECK1-NOT: Use -header-filter={{.*}} to display errors{{.*}}
//
// With --skip-header, no warning in .h file is detected or hidden,
// no need to give a reminder to use -header-filter.
// CHECK2-NOT: Use -header-filter={{.*}} to display errors{{.*}}
