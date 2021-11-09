<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
======================================
Extra Clang Tools 11.0.0 Release Notes
======================================
=======
====================================================
Extra Clang Tools 12.0.0 (In-Progress) Release Notes
====================================================
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

.. contents::
   :local:
   :depth: 3

Written by the `LLVM Team <https://llvm.org/>`_

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
=======
.. warning::

   These are in-progress notes for the upcoming Extra Clang Tools 12 release.
   Release notes for previous releases can be found on
   `the Download Page <https://releases.llvm.org/download.html>`_.

>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)
Introduction
============

This document contains the release notes for the Extra Clang Tools, part of the
Clang release 12.0.0. Here we describe the status of the Extra Clang Tools in
some detail, including major improvements from the previous release and new
feature work. All LLVM releases may be downloaded from the `LLVM releases web
site <https://llvm.org/releases/>`_.

For more information about Clang or LLVM, including information about
the latest release, please see the `Clang Web Site <https://clang.llvm.org>`_ or
the `LLVM Web Site <https://llvm.org>`_.

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
What's New in Extra Clang Tools 11.0.0?
=======
Note that if you are reading this file from a Git checkout or the
main Clang web page, this document applies to the *next* release, not
the current one. To see the release notes for a specific release, please
see the `releases page <https://llvm.org/releases/>`_.

What's New in Extra Clang Tools 12.0.0?
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)
=======================================

Some of the major new features and improvements to Extra Clang Tools are listed
here. Generic improvements to Extra Clang Tools as a whole or to its underlying
infrastructure are described first, followed by tool-specific sections.

Improvements to clangd
----------------------

Performance
^^^^^^^^^^^

- Eliminated long delays after adding/removing includes ("async preambles")

- Faster indexing

- Less memory used to index headers used by open files ("dynamic index")

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
- Many requests are implicitly cancelled rather than queued when the file is
  edited, preventing a backlog
=======
- The IgnoreImplicitCastsAndParentheses traversal mode has been removed.
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

- Background indexing can be selectively disabled per-path through config

Selecting and targeting
^^^^^^^^^^^^^^^^^^^^^^^

- Improved understanding and selection around broken code ("recovery AST")

- Operations like "go-to-definition" will target things on the left of the
  cursor, if there is nothing eligible on the right.

- Arguments to ``assert()``-like macros can be properly selected.

Diagnostics
^^^^^^^^^^^

- When a header is saved, diagnostics for files that use it are updated.

- Calls ``std::make_unique`` produce diagnostics for the constructor call.
  (Template functions *in general* are not expanded for performance reasons).

- Diagnostics update more quickly for files that build quickly (no 500ms delay)

- Automatic fixes are offered even when they affect macro arguments.

- Warnings from included headers are not shown (but errors still are).

- A handful of high-quality clang-tidy checks are enabled by default:

  - readability-misleading-indentation,

  - readability-deleted-default,

  - bugprone-integer-division,

  - bugprone-sizeof-expression,

  - bugprone-suspicious-missing-comma,

  - bugprone-unused-raii,

  - bugprone-unused-return-value,

  - misc-unused-using-decls,

  - misc-unused-alias-decls,

  - misc-definitions-in-headers

Refactorings
^^^^^^^^^^^^

- Rename applies across the project, using the index.

- Accuracy of rename improved in many places.

- New refactoring: add using declaration for qualified name.

- New refactoring: move function definition out-of-line.

Code completion
^^^^^^^^^^^^^^^

- Function call parentheses are not inserted if they already exist.

- Completion of ``#include`` filenames triggers earlier (after ``<``, ``"``, and
  ``/``) and is less aggressive about replacing existing text.

- Documentation is reflowed in the same way as on hover.

Go-to-definition
^^^^^^^^^^^^^^^^

- Dependent names in templates may be heuristically resolved

- Identifiers in comments may be resolved using other occurrences in the file
  or in the index.

- Go-to-definition on an ``override`` or ``final`` specifier jumps to the
  overridden method.

Hover
^^^^^

- Expressions passed as function arguments show parameter name, conversions etc.

- Members now include the access specifier in the displayed declaration.

- Classes and fields show memory layout information (size and offset).

- Somewhat improved understanding of formatting in documentation comments.

- Trivial inline getters/setters are implicitly documented as such.

Highlighting
^^^^^^^^^^^^

- The ``semanticTokens`` protocol from LSP 3.16 is supported.
  (Only token types are exposed, no modifiers yet).

- The non-standard ``textDocument/semanticHighlighting`` notification is
  deprecated and will be removed in clangd 12.

- Placing the cursor on a control flow keyword highlights related flow
  (e.g. ``break`` -> ``for``).

Language support
^^^^^^^^^^^^^^^^

- clangd features now work inside templates on windows.
  (MSVC-compatible delayed-template-parsing is no longer used).

- Objective-C properties can be targeted and cross-references are indexed.

- Field names in designated initializers (C++20) can be targeted, and code
  completion works in many cases.

- ``goto`` labels: go-to-defintion, cross-references, and rename all work.

- Concepts (C++20): go-to-definition on concept names, and some limited code
  completion support for concept members.

System integration
^^^^^^^^^^^^^^^^^^

- The project index is now written to ``$PROJECT/.cache/clangd/index``.
  ``$PROJECT/.clangd`` is now expected to be a configuration file.

  Old ``$PROJECT/.clangd`` directories can safely be deleted.

  We recommend including both ``.cache/`` and ``.clangd/`` (with trailing slash)
  in ``.gitignore``, for backward-compatibility with earlier releases of clangd.

- For non-project files (those without a compilation database), the index
  location better reflects OS conventions:

  - ``%LocalAppData%\clangd\index`` on Windows

  - ``$(getconf DARWIN_USER_CACHE_DIR)/clangd/index`` on Mac

  - ``$XDG_CACHE_HOME/clangd/index`` or ``~/.cache/clangd/index`` on others

  Old ``~/.clangd/index`` directories can safely be deleted.

- clangd now reads configuration from ``.clangd`` files inside your project,
  and from a user configuration file in an OS-specific location:

  - ``%LocalAppData%\clangd\config.yaml`` on Windows

  - ``~/Library/Preferences/clangd/config.yaml`` on Mac

  - ``$XDG_CONFIG_HOME/clangd/config.yaml`` or ``~/.config/clangd/config.yaml``
    on others

  See `clangd configuration format <https://clangd.llvm.org/config.html>`_.

- clangd will search for compilation databases (``compile_commands.json``) in
  a ``build/`` subdirectory, as well as in the project root.
  This follows CMake conventions, avoiding the need for a symlink in many cases.

- Compile flags can be selectively modified per-path, using configuration.

- Improved filtering of unhelpful compile flags (such as those relating to
  pre-compiled headers).

- Improved detection of standard library headers location.

Miscellaneous
^^^^^^^^^^^^^

- Background indexing status is reported using LSP 3.15 progress events
  (``window/workDoneProgress/create``).

- Infrastructure for gathering internal metrics.
  (Off by default, set ``$CLANGD_METRICS`` to generate a named CSV file).

- Document versions are now tracked, version is reported along with diagnostics.

- Too many stability and correctness fixes to mention.

Improvements to clang-tidy
--------------------------

- Checks that allow configuring names of headers to include now support wrapping
  the include in angle brackets to create a system include. For example,
  :doc:`cppcoreguidelines-init-variables
  <clang-tidy/checks/cppcoreguidelines-init-variables>` and
  :doc:`modernize-make-unique <clang-tidy/checks/modernize-make-unique>`.

New modules
^^^^^^^^^^^

- New ``altera`` module.

  Includes checks related to OpenCL for FPGA coding guidelines, based on the
  `Altera SDK for OpenCL: Best Practices Guide
  <https://www.altera.com/en_US/pdfs/literature/hb/opencl-sdk/aocl_optimization_guide.pdf>`_.

- New ``concurrency`` module.

  Includes checks related to concurrent programming (e.g. threads, fibers,
  coroutines, etc.).

New checks
^^^^^^^^^^

- New :doc:`altera-kernel-name-restriction
  <clang-tidy/checks/altera-kernel-name-restriction>` check.

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
  Finds ``s.find(...) == string::npos`` comparisons (for various string-like
  types) and suggests replacing with ``absl::StrContains()``.
=======
  Finds kernel files and include directives whose filename is `kernel.cl`,
  `Verilog.cl`, or `VHDL.cl`.

- New :doc:`altera-struct-pack-align
  <clang-tidy/checks/altera-struct-pack-align>` check.

  Finds structs that are inefficiently packed or aligned, and recommends
  packing and/or aligning of said structs as needed.

- New :doc:`cppcoreguidelines-prefer-member-initializer
  <clang-tidy/checks/cppcoreguidelines-prefer-member-initializer>` check.

  Finds member initializations in the constructor body which can be placed into
  the initialization list instead.
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

- New :doc:`bugprone-misplaced-pointer-arithmetic-in-alloc
  <clang-tidy/checks/bugprone-misplaced-pointer-arithmetic-in-alloc>` check.

- New :doc:`bugprone-redundant-branch-condition
  <clang-tidy/checks/bugprone-redundant-branch-condition>` check.

  Finds condition variables in nested ``if`` statements that were also checked
  in the outer ``if`` statement and were not changed.

- New :doc:`concurrency-mt-unsafe <clang-tidy/checks/concurrency-mt-unsafe>`
  check.

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
- New :doc:`bugprone-reserved-identifier
  <clang-tidy/checks/bugprone-reserved-identifier>` check.

  Checks for usages of identifiers reserved for use by the implementation.

- New :doc:`bugprone-spuriously-wake-up-functions
  <clang-tidy/checks/bugprone-spuriously-wake-up-functions>` check.
=======
  Finds thread-unsafe functions usage. Currently knows about POSIX and
  Glibc function sets.
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

- New :doc:`bugprone-signal-handler
  <clang-tidy/checks/bugprone-signal-handler>` check.

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
- New :doc:`bugprone-suspicious-include
  <clang-tidy/checks/bugprone-suspicious-include>` check.
=======
  Finds functions registered as signal handlers that call non asynchronous-safe
  functions.

- New :doc:`cert-sig30-c
  <clang-tidy/checks/cert-sig30-c>` check.

  Alias to the :doc:`bugprone-signal-handler
  <clang-tidy/checks/bugprone-signal-handler>` check.
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

- New :doc:`performance-no-int-to-ptr
  <clang-tidy/checks/performance-no-int-to-ptr>` check.

  Diagnoses every integer to pointer cast.

- New :doc:`readability-function-cognitive-complexity
  <clang-tidy/checks/readability-function-cognitive-complexity>` check.

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
- New :doc:`cppcoreguidelines-avoid-non-const-global-variables
  <clang-tidy/checks/cppcoreguidelines-avoid-non-const-global-variables>` check.

  Finds non-const global variables as described in check I.2 of C++ Core
  Guidelines.

- New :doc:`llvmlibc-callee-namespace
  <clang-tidy/checks/llvmlibc-callee-namespace>` check.

  Checks all calls resolve to functions within ``__llvm_libc`` namespace.

- New :doc:`llvmlibc-implementation-in-namespace
  <clang-tidy/checks/llvmlibc-implementation-in-namespace>` check.

  Checks all llvm-libc implementation is within the correct namespace.

- New :doc:`llvmlibc-restrict-system-libc-headers
  <clang-tidy/checks/llvmlibc-restrict-system-libc-headers>` check.

  Finds includes of system libc headers not provided by the compiler within
  llvm-libc implementations.

- New :doc:`misc-no-recursion
  <clang-tidy/checks/misc-no-recursion>` check.

  Finds recursive functions and diagnoses them.

- New :doc:`modernize-replace-disallow-copy-and-assign-macro
  <clang-tidy/checks/modernize-replace-disallow-copy-and-assign-macro>` check.

  Finds macro expansions of ``DISALLOW_COPY_AND_ASSIGN`` and replaces them with
  a deleted copy constructor and a deleted assignment operator.

- New :doc:`objc-dealloc-in-category
  <clang-tidy/checks/objc-dealloc-in-category>` check.

  Finds implementations of -dealloc in Objective-C categories.

- New :doc:`objc-nsinvocation-argument-lifetime
  <clang-tidy/checks/objc-nsinvocation-argument-lifetime>` check.

  Finds calls to ``NSInvocation`` methods under ARC that don't have proper
  argument object lifetimes.

- New :doc:`readability-use-anyofallof
  <clang-tidy/checks/readability-use-anyofallof>` check.

  Finds range-based for loops that can be replaced by a call to ``std::any_of``
  or ``std::all_of``.

New check aliases
^^^^^^^^^^^^^^^^^

- New alias :doc:`cert-con36-c
  <clang-tidy/checks/cert-con36-c>` to
  :doc:`bugprone-spuriously-wake-up-functions
  <clang-tidy/checks/bugprone-spuriously-wake-up-functions>` was added.

- New alias :doc:`cert-con54-cpp
  <clang-tidy/checks/cert-con54-cpp>` to
  :doc:`bugprone-spuriously-wake-up-functions
  <clang-tidy/checks/bugprone-spuriously-wake-up-functions>` was added.

- New alias :doc:`cert-dcl37-c
  <clang-tidy/checks/cert-dcl37-c>` to
  :doc:`bugprone-reserved-identifier
  <clang-tidy/checks/bugprone-reserved-identifier>` was added.

- New alias :doc:`cert-dcl51-cpp
  <clang-tidy/checks/cert-dcl51-cpp>` to
  :doc:`bugprone-reserved-identifier
  <clang-tidy/checks/bugprone-reserved-identifier>` was added.

- New alias :doc:`cert-str34-c
  <clang-tidy/checks/cert-str34-c>` to
  :doc:`bugprone-signed-char-misuse
  <clang-tidy/checks/bugprone-signed-char-misuse>` was added.

- New alias :doc:`llvm-else-after-return
  <clang-tidy/checks/llvm-else-after-return>` to
  :doc:`readability-else-after-return
  <clang-tidy/checks/readability-else-after-return>` was added.
=======
  Flags functions with Cognitive Complexity metric exceeding the configured limit.
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)

Changes in existing checks
^^^^^^^^^^^^^^^^^^^^^^^^^^

- Improved :doc:`modernize-loop-convert
  <clang-tidy/checks/modernize-loop-convert>` check.

  Now able to transform iterator loops using ``rbegin`` and ``rend`` methods.

- Improved :doc:`readability-identifier-naming
  <clang-tidy/checks/readability-identifier-naming>` check.

  Added an option `GetConfigPerFile` to support including files which use
  different naming styles.

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
=======
  Now renames overridden virtual methods if the method they override has a
  style violation.
  
  Added support for specifying the style of scoped ``enum`` constants. If 
  unspecified, will fall back to the style for regular ``enum`` constants.

  Added an option `IgnoredRegexp` per identifier type to suppress identifier
  naming checks for names matching a regular expression.

- Removed `google-runtime-references` check because the rule it checks does
  not exist in the Google Style Guide anymore.

>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)
- Improved :doc:`readability-redundant-string-init
  <clang-tidy/checks/readability-redundant-string-init>` check.

<<<<<<< HEAD   (1fdec5 [lldb] Fix fallout caused by D89156 on 11.0.1 for MacOS)
- Checks supporting the ``HeaderFileExtensions`` flag now support ``;`` as a
  delimiter in addition to ``,``, with the latter being deprecated as of this
  release. This simplifies how one specifies the options on the command line:
  ``--config="{CheckOptions: [{ key: HeaderFileExtensions, value: h;;hpp;hxx }]}"``

- Improved :doc:`readability-qualified-auto
  <clang-tidy/checks/readability-qualified-auto>` check now supports a
  `AddConstToQualified` to enable adding ``const`` qualifiers to variables
  typed with ``auto *`` and ``auto &``.

Renamed checks
^^^^^^^^^^^^^^

- The 'fuchsia-restrict-system-headers' check was renamed to :doc:`portability-restrict-system-includes
  <clang-tidy/checks/portability-restrict-system-includes>`

Other improvements
^^^^^^^^^^^^^^^^^^

- For `run-clang-tidy.py` add option to use alpha checkers from
  `clang-analyzer`.
=======
  Added `std::basic_string_view` to default list of ``string``-like types.

Improvements to include-fixer
-----------------------------

The improvements are...

Improvements to clang-include-fixer
-----------------------------------

The improvements are...

Improvements to modularize
--------------------------

The improvements are...

Improvements to pp-trace
------------------------

The improvements are...

Clang-tidy visual studio plugin
-------------------------------
>>>>>>> BRANCH (664b18 Reland Pin -loop-reduce to legacy PM)
