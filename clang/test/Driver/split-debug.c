/// Test -gsplit-dwarf and -gsplit-dwarf={split,single}.

/// Interaction with -g (-g2).
// RUN: %clang -### -c -target x86_64 -g -gsplit-dwarf %s 2>&1 | FileCheck %s --check-prefixes=NOINLINE,SPLIT
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf -g %s 2>&1 | FileCheck %s --check-prefixes=NOINLINE,SPLIT
// RUN: %clang -### -c -target x86_64 -g2 -gsplit-dwarf %s 2>&1 | FileCheck %s --check-prefixes=NOINLINE,SPLIT
/// -gsplit-dwarf=split is equivalent to -gsplit-dwarf.
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf=split -g %s 2>&1 | FileCheck %s --check-prefixes=NOINLINE,SPLIT

// INLINE-NOT: "-fno-split-dwarf-inlining"
// NOINLINE:   "-fno-split-dwarf-inlining"
// SPLIT:      "-debug-info-kind=limited"
// SPLIT-SAME: "-ggnu-pubnames"
// SPLIT-SAME: "-split-dwarf-file" "split-debug.dwo" "-split-dwarf-output" "split-debug.dwo"

// RUN: %clang -### -c -target wasm32 -gsplit-dwarf -g %s 2>&1 | FileCheck %s --check-prefix=SPLIT
// RUN: %clang -### -c -target amdgcn-amd-amdhsa -gsplit-dwarf -g %s 2>&1 | FileCheck %s --check-prefix=SPLIT

/// -gsplit-dwarf is a no-op on a non-ELF platform.
// RUN: %clang -### -c -target x86_64-apple-darwin  -gsplit-dwarf -g %s 2>&1 | FileCheck %s --check-prefix=DARWIN
// DARWIN:     "-debug-info-kind=standalone"
// DARWIN-NOT: "-split-dwarf

/// -gsplit-dwarf is a no-op if no -g is specified.
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf %s 2>&1 | FileCheck %s --check-prefix=G0

/// -gno-split-dwarf disables debug fission.
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf -g -gno-split-dwarf %s 2>&1 | FileCheck %s --check-prefix=NOSPLIT
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf=single -g -gno-split-dwarf %s 2>&1 | FileCheck %s --check-prefix=NOSPLIT
// RUN: %clang -### -c -target x86_64 -gno-split-dwarf -g -gsplit-dwarf %s 2>&1 | FileCheck %s --check-prefixes=NOINLINE,SPLIT

// NOSPLIT:     "-debug-info-kind=limited"
// NOSPLIT-NOT: "-ggnu-pubnames"
// NOSPLIT-NOT: "-split-dwarf

/// Test -gsplit-dwarf=single.
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf=single -g %s 2>&1 | FileCheck %s --check-prefix=SINGLE

// SINGLE: "-debug-info-kind=limited"
// SINGLE: "-split-dwarf-file" "split-debug.o"
// SINGLE-NOT: "-split-dwarf-output"

// RUN: %clang -### -c -target x86_64 -gsplit-dwarf=single -g -o %tfoo.o %s 2>&1 | FileCheck %s --check-prefix=SINGLE_WITH_FILENAME

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
// RUN: %clang -target x86_64-unknown-linux-gnu -g -S -### %s 2> %t
// RUN: FileCheck -check-prefix=CHECK-NOINLINE-WITHOUT-SPLIT < %t %s
//
// RUN: %clang -target x86_64-unknown-linux-gnu -g -fno-split-dwarf-inlining -S -### %s 2> %t
// RUN: FileCheck -check-prefix=CHECK-NOINLINE-WITHOUT-SPLIT < %t %s
//
// CHECK-NOINLINE-WITHOUT-SPLIT: "-fno-split-dwarf-inlining"
// CHECK-NOINLINE-WITHOUT-SPLIT: "-debug-info-kind=limited"
=======
// SINGLE_WITH_FILENAME: "-split-dwarf-file" "{{.*}}foo.o"
// SINGLE_WITH_FILENAME-NOT: "-split-dwarf-output"
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
// RUN: %clang -target x86_64-unknown-linux-gnu -gmlt -gsplit-dwarf -fno-split-dwarf-inlining -S -### %s 2> %t
// RUN: FileCheck -check-prefix=CHECK-SPLIT-WITH-GMLT < %t %s
//
// CHECK-SPLIT-WITH-GMLT: "-debug-info-kind=limited"
// CHECK-SPLIT-WITH-GMLT: "-split-dwarf-output"
=======
/// Without -c, clang performs linking as well. The output is unchanged.
// RUN: %clang -### -target x86_64-unknown-linux-gnu -gsplit-dwarf -g %s -o ignore.d 2>&1 | FileCheck %s --check-prefix=SPLIT
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
// RUN: %clang -target x86_64-unknown-linux-gnu -gsplit-dwarf -fno-split-dwarf-inlining -S -### %s 2> %t
// RUN: FileCheck -check-prefix=CHECK-SPLIT-WITH-NOINL < %t %s
//
// CHECK-SPLIT-WITH-NOINL: "-debug-info-kind=limited"
// CHECK-SPLIT-WITH-NOINL: "-split-dwarf-output"
=======
/// -fsplit-dwarf-inlining
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf=split -g -fsplit-dwarf-inlining %s 2>&1 | FileCheck %s --check-prefixes=INLINE,SPLIT
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

// RUN: %clang -### -c -target x86_64 -gsplit-dwarf=split -g -gno-pubnames %s 2>&1 | FileCheck %s --check-prefixes=NOPUBNAMES
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf=split -g -gno-gnu-pubnames %s 2>&1 | FileCheck %s --check-prefixes=NOPUBNAMES
// NOPUBNAMES:      "-debug-info-kind=limited"
// NOPUBNAMES-NOT:  "-ggnu-pubnames"
// NOPUBNAMES-SAME: "-split-dwarf-file" "split-debug.dwo" "-split-dwarf-output" "split-debug.dwo"

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
// RUN: %clang -target x86_64-unknown-linux-gnu -gmlt -gsplit-dwarf -S -### %s 2> %t
// RUN: FileCheck -check-prefix=CHECK-SPLIT-OVER-GMLT < %t %s
//
// CHECK-SPLIT-OVER-GMLT: "-debug-info-kind=limited"
// CHECK-SPLIT-OVER-GMLT: "-split-dwarf-file"
// CHECK-SPLIT-OVER-GMLT: "-split-dwarf-output"
=======
/// Invoke objcopy if not using the integrated assembler.
// RUN: %clang -### -c -target x86_64-unknown-linux-gnu -fno-integrated-as -gsplit-dwarf -g %s 2>&1 | FileCheck %s --check-prefix=OBJCOPY
// OBJCOPY:      objcopy" "--extract-dwo"
// OBJCOPY-NEXT: objcopy" "--strip-dwo"
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

/// ... but not for assembly output.
// RUN: %clang -### -S -target x86_64-unknown-linux-gnu -fno-integrated-as -gsplit-dwarf -g %s 2>&1 | FileCheck %s --check-prefix=NOOBJCOPY
// NOOBJCOPY-NOT: objcopy"

/// Interaction with -g0.
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf -g0 -### %s 2>&1 | FileCheck %s --check-prefix=G0
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf=single -g0 %s 2>&1 | FileCheck %s --check-prefix=G0
// RUN: %clang -### -c -target x86_64 -g0 -gsplit-dwarf %s 2>&1 | FileCheck %s --check-prefixes=G0
// RUN: %clang -### -c -target x86_64 -g0 -gsplit-dwarf=single %s 2>&1 | FileCheck %s --check-prefix=G0
// RUN: %clang -### -c -target x86_64 -gsplit-dwarf=single -g0 -fsplit-dwarf-inlining %s 2>&1 | FileCheck %s --check-prefix=G0

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
// RUN: %clang -target x86_64-unknown-linux-gnu -g0 -gsplit-dwarf -S -### %s 2> %t
// RUN: FileCheck -check-prefix=CHECK-SPLIT-OVER-G0 < %t %s
// RUN: %clang -target x86_64-unknown-linux-gnu -g0 -gsplit-dwarf=split -S -### %s 2> %t
// RUN: FileCheck -check-prefix=CHECK-SPLIT-OVER-G0 < %t %s
//
// CHECK-SPLIT-OVER-G0: "-debug-info-kind=limited"
// CHECK-SPLIT-OVER-G0: "-split-dwarf-file"
// CHECK-SPLIT-OVER-G0: "-split-dwarf-output"
=======
// G0-NOT: "-debug-info-kind=
// G0-NOT: "-split-dwarf-

/// Interaction with -g1 (-gmlt).
// RUN: %clang -### -S -target x86_64 -gsplit-dwarf -g1 %s 2>&1 | FileCheck %s --check-prefix=G1_WITH_SPLIT
// RUN: %clang -### -S -target x86_64 -gsplit-dwarf -g1 -fno-split-dwarf-inlining %s 2>&1 | FileCheck %s --check-prefix=G1_WITH_SPLIT
// RUN: %clang -### -S -target x86_64 -gmlt -gsplit-dwarf -fno-split-dwarf-inlining %s 2>&1 | FileCheck %s --check-prefix=G1_WITH_SPLIT

// G1_WITH_SPLIT: "-debug-info-kind=line-tables-only"
// G1_WITH_SPLIT: "-split-dwarf-file"
// G1_WITH_SPLIT: "-split-dwarf-output"

// RUN: %clang -### -S -target x86_64 -gsplit-dwarf -g1 -fsplit-dwarf-inlining %s 2>&1 | FileCheck %s --check-prefix=G1_NOSPLIT

// G1_NOSPLIT: "-debug-info-kind=line-tables-only"
// G1_NOSPLIT-NOT: "-split-dwarf-file"
// G1_NOSPLIT-NOT: "-split-dwarf-output"
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)
