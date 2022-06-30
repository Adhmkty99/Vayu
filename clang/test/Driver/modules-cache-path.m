// RUN: env HOME=%t.home %clang -fmodules -### %s 2>&1 | FileCheck %s -check-prefix=CHECK-DEFAULT
// CHECK-DEFAULT: -fmodules-cache-path={{.*}}clang{{[/\\]+}}ModuleCache
