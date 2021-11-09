=========================
LLVM 12.0.0 Release Notes
=========================

.. contents::
    :local:

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
=======
.. warning::
   These are in-progress notes for the upcoming LLVM 12 release.
   Release notes for previous releases can be found on
   `the Download Page <https://releases.llvm.org/download.html>`_.


>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)
Introduction
============

This document contains the release notes for the LLVM Compiler Infrastructure,
release 12.0.0.  Here we describe the status of LLVM, including major improvements
from the previous release, improvements in various subprojects of LLVM, and
some of the current users of the code.  All LLVM releases may be downloaded
from the `LLVM releases web site <https://llvm.org/releases/>`_.

For more information about LLVM, including information about the latest
release, please check out the `main LLVM web site <https://llvm.org/>`_.  If you
have questions or comments, the `LLVM Developer's Mailing List
<https://lists.llvm.org/mailman/listinfo/llvm-dev>`_ is a good place to send
them.

Deprecated and Removed Features/APIs
=================================================
* BG/Q support, including QPX, will be removed in the 12.0.0 release.

Non-comprehensive list of changes in this release
=================================================

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
* The llgo frontend has been removed for now, but may be resurrected in the
  future.
=======
* The ConstantPropagation pass was removed. Users should use the InstSimplify
  pass instead.


.. NOTE
   If you would like to document a larger change, then you can add a
   subsection about it right here. You can copy the following boilerplate
   and un-indent it (the indentation causes it to be inside this comment).

   Special New Feature
   -------------------

   Makes programs 10x faster by doing Special New Thing.

>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

Changes to the LLVM IR
----------------------

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
* The callsite attribute `vector-function-abi-variant
  <https://llvm.org/docs/LangRef.html#call-site-attributes>`_ has been
  added to describe the mapping between scalar functions and vector
  functions, to enable vectorization of call sites. The information
  provided by the attribute is interfaced via the API provided by the
  ``VFDatabase`` class. When scanning through the set of vector
  functions associated with a scalar call, the loop vectorizer now
  relies on ``VFDatabase``, instead of ``TargetLibraryInfo``.
=======
* ...
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

* Added the ``byref`` attribute to better represent argument passing
  for the `amdgpu_kernel` calling convention.

* Added type parameter to the ``sret`` attribute to continue work on
  removing pointer element types.

* The ``llvm.experimental.vector.reduce`` family of intrinsics have been renamed
  to drop the "experimental" from the name, reflecting their now fully supported
  status in the IR.


* LLVM IR now supports two distinct ``llvm::FixedVectorType`` and
  ``llvm::ScalableVectorType`` vector types, both derived from the
  base class ``llvm::VectorType``. A number of algorithms dealing with
  IR vector types have been updated to make sure they work for both
  scalable and fixed vector types. Where possible, the code has been
  made generic to cover both cases using the base class. Specifically,
  places that were using the type ``unsigned`` to count the number of
  lanes of a vector are now using ``llvm::ElementCount``. In places
  where ``uint64_t`` was used to denote the size in bits of a IR type
  we have partially migrated the codebase to using ``llvm::TypeSize``.

* Branching on ``undef``/``poison`` is undefined behavior. It is needed for
  correctly analyzing value ranges based on branch conditions. This is
  consistent with MSan's behavior as well.

* ``memset``/``memcpy``/``memmove`` can take ``undef``/``poison`` pointer(s)
  if the size to fill is zero.

* Passing ``undef``/``poison`` to a standard I/O library function call
  (`printf`/`fputc`/...) is undefined behavior. The new ``noundef`` attribute
  is attached to the functions' arguments. The full list is available at
  ``llvm::inferLibFuncAttributes``.

Changes to building LLVM
------------------------

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
* The LLVM project has started the migration towards Python 3, and the build
  system now prefers Python 3 whenever available.  If the Python 3 interpreter
  (or libraries) are not found, the build system will, for the time being, fall
  back to Python 2.  It is recommended that downstream projects migrate to
  Python 3 as Python 2 has been end-of-life'd by the Python Software
  Foundation.

Changes to the JIT infrastructure
---------------------------------

* LLJIT now supports execution of static inits / deinits via the
  LLJIT::initialize and LLJIT::deinitialize methods

* Static libraries can now be added to a JITDylib using the
  StaticLibraryDefinitionGenerator class

* A C API has been added for OrcV2 (llvm-project/llvm/include/llvm-c/Orc.h)

* Several OrcV2 example projects have been added to
  llvm-project/llvm/examples/OrcV2Examples

* Many bug fixes and API improvements

Changes to the AArch64 Backend
------------------------------

* Back up and restore x18 in functions with windows calling convention on
  non-windows OSes.

* Clearly error out on unsupported relocations when targeting COFF, instead
  of silently accepting some (without being able to do what was requested).

* Implemented codegen support for the SVE C-language intrinsics
  documented in `Arm C Language Extensions (ACLE) for SVE
  <https://developer.arm.com/documentation/100987/>`_ (version
  ``00bet5``). For more information, see the ``clang`` 11 release
  notes.

* Added support for Armv8.6-A:

  Assembly support for the following extensions:

  - Enhanced Counter Virtualization (ARMv8.6-ECV).
  - Fine Grained Traps (ARMv8.6-FGT).
  - Activity Monitors virtualization (ARMv8.6-AMU).
  - Data gathering hint (ARMv8.0-DGH).

  Assembly and intrinsics support for the Armv8.6-A Matrix Multiply extension
  for Neon and SVE vectors.

  Support for the ARMv8.2-BF16 BFloat16 extension. This includes a new C-level
  storage-only `__bf16` type, a `BFloat` IR type, a `bf16` MVT, and assembly
  and intrinsics support.

* Added support for Cortex-A34, Cortex-A77, Cortex-A78 and Cortex-X1 cores.
=======
* The internal ``llvm-build`` Python script and the associated ``LLVMBuild.txt``
  files used to describe the LLVM component structure have been removed and
  replaced by a pure ``CMake`` approach, where each component stores extra
  properties in the created targets. These properties are processed once all
  components are defined to resolve library dependencies and produce the header
  expected by llvm-config.

Changes to TableGen
-------------------

* The new "TableGen Programmer's Reference" replaces the "TableGen Language
  Introduction" and "TableGen Language Reference" documents.

* The syntax for specifying an integer range in a range list has changed.
  The old syntax used a hyphen in the range (e.g., ``{0-9}``). The new syntax
  uses the "`...`" range punctuation (e.g., ``{0...9}``). The hyphen syntax
  is deprecated.
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

Changes to the ARM Backend
--------------------------

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
* Implemented C-language intrinsics for the full Arm v8.1-M MVE instruction
  set. ``<arm_mve.h>`` now supports the complete API defined in the Arm C
  Language Extensions.

* Added support for assembly for the optional Custom Datapath Extension (CDE)
  for Arm M-profile targets.

* Implemented C-language intrinsics ``<arm_cde.h>`` for the CDE instruction set.

* Clang now defaults to ``-fomit-frame-pointer`` when targeting non-Android
  Linux for arm and thumb when optimizations are enabled. Users that were
  previously not specifying a value and relying on the implicit compiler
  default may wish to specify ``-fno-omit-frame-pointer`` to get the old
  behavior. This improves compatibility with GCC.

* Added support for Armv8.6-A:
=======
During this release ...

Changes to the MIPS Target
--------------------------
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

  Assembly and intrinsics support for the Armv8.6-A Matrix Multiply extension
  for Neon vectors.

  Support for the ARMv8.2-AA32BF16 BFloat16 extension. This includes a new
  C-level storage-only `__bf16` type, a `BFloat` IR type, a `bf16` MVT, and
  assembly and intrinsics support.

* Added support for CMSE.

* Added support for Cortex-M55, Cortex-A77, Cortex-A78 and Cortex-X1 cores.

* The Machine Outliner is now supported for ARM and Thumb2, it is not
  turned on by default and can be enabled with the ``-moutline`` clang flag.


Changes to the PowerPC Target
-----------------------------

Optimization:

* Improved Loop Unroll-and-Jam legality checks, allowing it to handle more than two level loop nests
* Improved Loop Unroll to be able to unroll more loops
* Implemented an option to allow loop fusion to work on loops with different constant trip counts

Codegen:

* POWER10 support

  * Added PC Relative addressing
  * Added __int128 vector bool support

* Security enhancement via probe-stack attribute support to protect against stack clash
* Floating point support enhancements

  * Improved half precision and quad precision support, including GLIBC
  * constrained FP operation support for arithmetic/rounding/max/min
  * cleaning up fast math flags checks in DAGCombine, Legalizer, and Lowering

* Performance improvements from instruction exploitation, especially for vector permute on LE
* Scheduling enhancements

  * Added MacroFusion for POWER8
  * Added post-ra heuristics for POWER9

* Target dependent passes tuning

  * Updated LoopStrengthReduce to use instruction number as first priority
  * Enhanced MachineCombiner to expose more ILP

* Code quality and maintenance enhancements

  * Enabled more machine verification passes
  * Added ability to parse and emit additional extended mnemonics
  * Numerous bug fixes

AIX Support Improvements:

* Enabled compile and link such that a simple <stdio.h> "Hello World" program works with standard headers
* Added support for the C calling convention for non-vector code
* Implemented correct stack frame layout for functions
* In llvm-objdump, added support for relocations, improved selection of symbol labels, and added the --symbol-description option


Changes to the RISC-V Target
----------------------------

New features:

* After consultation through an RFC, the RISC-V backend now accepts patches for
  proposed instruction set extensions that have not yet been ratified.  For these
  experimental extensions, there is no expectation of ongoing support - the
  compiler support will continue to change until the specification is finalised.
  In line with this policy, MC layer and code generation support was added for
  version 0.92 of the proposed Bit Manipulation Extension and MC layer support
  was added for version 0.8 of the proposed RISC-V Vector instruction set
  extension. As these extensions are not yet ratified, compiler support will
  continue to change to match the specifications until they are finalised.
* ELF attribute sections are now created, encoding information such as the ISA
  string.
* Support for saving/restoring callee-saved registers via libcalls (a code
  size optimisation).
* llvm-objdump will now print branch targets as part of disassembly.

Improvements:

* If an immediate can be generated using a pair of `addi` instructions, that
  pair will be selected rather than materialising the immediate into a
  separate register with an `lui` and `addi` pair.
* Multiplication by a constant was optimised.
* `addi` instructions are now folded into the offset of a load/store instruction
  even if the load/store itself has a non-zero offset, when it is safe to do
  so.
* Additional target hooks were implemented to minimise generation of
  unnecessary control flow instruction.
* The RISC-V backend's load/store peephole optimisation pass now supports
  constant pools, improving code generation for floating point constants.
* Debug scratch register names `dscratch0` and `dscratch1` are now recognised in
  addition to the legacy `dscratch` register name.
* Codegen for checking isnan was improved, removing a redundant `and`.
* The `dret` instruction is now supported by the MC layer.
* `.option pic` and `.option nopic` are now supported in assembly and `.reloc`
  was extended to support arbitrary relocation types.
* Scheduling info metadata was improved.
* The `jump` pseudo instruction is now supported.

Bug fixes:

* A failure to insert indirect branches in position independent code
  was fixed.
* The calculated expanded size of atomic pseudo operations was fixed, avoiding
  "fixup value out of range" errors during branch relaxation for some inputs.
* The `mcountinhibit` CSR is now recognised.
* The correct libcall is now emitted for converting a float/double to a 32-bit
  signed or unsigned integer on RV64 targets lacking the F or D extensions.

Changes to the SystemZ Target
-----------------------------

* Added support for the MemorySanitizer and the LeakSanitizer.
* Added support for the ``-fstack-clash-protection`` command line option.
* Enhanced the assembler parser to allow using `%r0` even in an address
  register context, and to allow specifying registers using plain integer
  numbers instead of register names everywhere.
* Fixed wrong code generation violating the platform ABI when passing
  a C++ class (not struct) type having only a single member of
  floating-point type.
* Fixed wrong code generation when using the `vec_store_len_r` or
  `vec_load_len_r` intrinsics with an immediate length argument of
  16 or larger.
* Miscellaneous codegen enhancements, in particular to improve vector code.

Changes to the X86 Target
-------------------------

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
* Functions with the probe-stack attribute set to "inline-asm" are now protected
  against stack clash without the need of a third-party probing function and
  with limited impact on performance.
* -x86-enable-old-knl-abi command line switch has been removed. v32i16/v64i8
  vectors are always passed in ZMM register when avx512f is enabled and avx512bw
  is disabled.
* Vectors larger than 512 bits with i16 or i8 elements will be passed in
  multiple ZMM registers when avx512f is enabled. Previously this required
  avx512bw otherwise they would split into multiple YMM registers. This means
  vXi16/vXi8 vectors are consistently treated the same as
  vXi32/vXi64/vXf64/vXf32 vectors of the same total width.
* Support was added for Intel AMX instructions.
* Support was added for TSXLDTRK instructions.
* A pass was added for mitigating the Load Value Injection vulnerability.
* The Speculative Execution Side Effect Suppression pass was added which can
  be used to as a last resort mitigation for speculative execution related
  CPU vulnerabilities.
* Improved recognition of boolean vector reductions with better MOVMSKB/PTEST
  handling
* Exteded recognition of rotation patterns to handle funnel shift as well,
  allowing us to remove the existing x86-specific SHLD/SHRD combine.
=======
During this release ...

* The 'mpx' feature was removed from the backend. It had been removed from clang
  frontend in 10.0. Mention of the 'mpx' feature in an IR file will print a
  message to stderr, but IR should still compile.
* Support for ``-march=alderlake``, ``-march=sapphirerapids``,
  ``-march=znver3`` and ``-march=x86-64-v[234]`` has been added.
* The assembler now has support for {disp32} and {disp8} pseudo prefixes for
  controlling displacement size for memory operands and jump displacements. The
  assembler also supports the .d32 and .d8 mnemonic suffixes to do the same.
* A new function attribute "tune-cpu" has been added to support -mtune like gcc.
  This allows microarchitectural optimizations to be applied independent from
  the "target-cpu" attribute or TargetMachine CPU which will be used to select
  Instruction Set. If the attribute is not present, the tune CPU will follow
  the target CPU.
* Support for ``HRESET`` instructions has been added.
* Support for ``UINTR`` instructions has been added.
* Support for ``AVXVNNI`` instructions has been added.
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

Changes to the AMDGPU Target
-----------------------------

During this release ...

* The new ``byref`` attribute is now the preferred method for
  representing aggregate kernel arguments.

Changes to the AVR Target
-----------------------------

During this release ...

Changes to the WebAssembly Target
---------------------------------

During this release ...

Changes to the Windows Target
-----------------------------

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
* Produce COFF weak external symbols for IR level weak symbols without a comdat
  (e.g. for `__attribute__((weak))` in C)
=======

Changes to the C API
--------------------


Changes to the Go bindings
--------------------------
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)


Changes to the DAG infrastructure
---------------------------------

* A SelDag-level freeze instruction has landed. It is simply lowered as a copy
  operation to MachineIR, but to make it fully correct either IMPLICIT_DEF
  should be fixed or the equivalent FREEZE operation should be added to
  MachineIR.

Changes to the Debug Info
-------------------------

During this release ...

Changes to the Gold Plugin
--------------------------

* ``--plugin-opt=whole-program-visibility`` is added to specify that classes have hidden LTO visibility in LTO and ThinLTO links of source files compiled with ``-fwhole-program-vtables``. See `LTOVisibility <https://clang.llvm.org/docs/LTOVisibility.html>`_ for details.
  (`D71913 <https://reviews.llvm.org/D71913>`_)

Changes to the LLVM tools
---------------------------------

* llvm-readobj and llvm-readelf behavior has changed to report an error when
  executed with no input files instead of reading an input from stdin.
  Reading from stdin can still be achieved by specifying `-` as an input file.

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
* llvm-rc now tolerates -1 as menu item ID, supports the language id option
  and allows string table values to be split into multiple string literals

* llvm-lib supports adding import library objects in addition to regular
  object files
=======
Changes to LLDB
---------------------------------
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

Changes to Sanitizers
---------------------

The integer sanitizer `-fsanitize=integer` now has a new sanitizer:
`-fsanitize=unsigned-shift-base`. It's not undefined behavior for an unsigned
left shift to overflow (i.e. to shift bits out), but it has been the source of
bugs and exploits in certain codebases in the past.

External Open Source Projects Using LLVM 12
===========================================

Zig Programming Language
------------------------

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
`Zig <https://ziglang.org>`_  is a general-purpose programming language and
toolchain for maintaining robust, optimal, and reusable software. In addition
to supporting LLVM as an optional backend, Zig links Clang and LLD to provide
an out-of-the-box cross compilation experience, not only for Zig code but for
C and C++ code as well. Using a sophisticated caching system, Zig lazily builds
from source compiler-rt, mingw-w64, musl, glibc, libcxx, libcxxabi, and
libunwind for the selected target - a "batteries included" drop-in for GCC/Clang
that works the same on every platform.

=======
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)
Additional Information
======================

A wide variety of additional information is available on the `LLVM web page
<https://llvm.org/>`_, in particular in the `documentation
<https://llvm.org/docs/>`_ section.  The web page also contains versions of the
API documentation which is up-to-date with the Git version of the source
code.  You can access versions of these documents specific to this release by
going into the ``llvm/docs/`` directory in the LLVM tree.

If you have any questions or comments about LLVM, please feel free to contact
us via the `mailing lists <https://llvm.org/docs/#mailing-lists>`_.
