
###############################
lib_xassert: Assertions library
###############################

************
Introduction
************

This library provides a lightweight and flexible replacement for the standard C header ``assert.h``.

The assertions in this library can be enabled/disabled and configured as to how much information
they show. This configuration can be per `xassert unit` (i.e. for sets of files).

``lib_xassert`` is intended to be used with the `XCommon CMake <https://www.xmos.com/file/xcommon-cmake-documentation/?version=latest>`_
, the `XMOS` application build and dependency management system.

***
API
***

To use this module, include ``lib_xassert`` in the application's ``APP_DEPENDENT_MODULES`` list and
include the ``xassert.h`` header file.

Assertions
==========

An assertion can be inserted into code with the ``assert`` macro e.g.::

   assert(i < n);

Optionally a debug message can be added with the ``msg`` macro::

   assert(i < n && msg("i must be less than the array bound"));

If assertions are enabled and the expression in the assertion is false then a trap will occur.
The same check is available through the ``xassert()`` macro. By default, the library also defines
``assert()`` if it has not already been defined by another header.

Ordinary and timing assertions are controlled independently. Disabling ordinary assertions
removes ``assert()`` (when defined by this library) and ``xassert()`` checks without disabling
timing assertions. Disabling timing assertions does not disable ordinary assertions. See
`Controlling assertions`_.

Unreachable
===========

If the logic of a program dictates that certain code cannot be reached, the ``unreachable`` macro
can be used e.g.::

  switch (message) {
  case 0:
     ...
  case 1:
     ...
  default:
    unreachable("message must be 0 or 1");
    break;
  }

If assertions are enabled then this macro will cause a trap if executed.

Fail
====

A failure can be indicated with the ``fail`` macro e.g.::

  if (reg_value != 0xA5)
    fail("device not connected properly");

A fail will always cause a trap if executed. A failure differs from unreachable in that an
unreachable macro should never execute in a correct program whereas a fail could happen in
catastrophic circumstances even if the program is correct.

******
Timing
******

`lib_xassert` provides a mechanism to check that certain code executes within a certain time.

Two mechanisms are provided: checking that a block of code completes within a given number of
reference timer ticks, and checking that a loop executes at a minimum frequency (in Hz).
The reference timer frequency is supplied by ``lib_basix`` as ``BASIX_TIMER_HZ``. One tick is
``1e9 / BASIX_TIMER_HZ`` nanoseconds; at 100 MHz, one tick is 10 ns. Any application override of
``BASIX_TIMER_HZ`` must match the hardware reference timer frequency.

.. note:: The nanosecond values printed in timing-failure messages currently assume a 100 MHz
   reference timer. With a different reference timer frequency, use the reported tick counts
   and ``BASIX_TIMER_HZ`` to calculate the elapsed time.

Timing assertions can be enabled/disabled via the ``XASSERT_ENABLE_TIMING_ASSERTIONS`` define.
They respect other settings such as ``XASSERT_ENABLE_DEBUG`` and ``XASSERT_ENABLE_LINE_NUMBERS``
as documented in `Controlling assertions`_.

.. note:: Adding timing asserts adds some overhead to the code.

.. warning:: Timing assertions maintain mutable state and are not thread-safe. Within each source
   file, they must only be used from a single thread/core. Concurrent calls from multiple
   threads/cores sharing that state can produce incorrect results.

.. warning:: Timing assertion state is defined in the header as translation-unit-local static data.
   Each source file gets its own timing buffer. A ``xassert_timing_start()`` in one source file
   will not match a ``xassert_timing_end()`` in another source file, and timing state memory is
   duplicated across source files.

``XASSERT_MAX_TIMING_BLOCKS`` sets the size of each source file's timing buffer. The current ring
buffer reserves one slot, so it can track at most ``XASSERT_MAX_TIMING_BLOCKS - 1`` active items
(blocks and loops combined). The default buffer size of 8 therefore holds 7 active items. When
the buffer is full, adding a new item drops the entry at the head of the buffer. This drop is
silent by default; a warning is printed only when debug output is enabled for the unit.

Timing blocks
=============

Code blocks can be timed with the ``xassert_timing_start()`` and ``xassert_timing_end()`` functions. For example::

    xassert_timing_start("test", 1000);
    // code to be timed
    xassert_timing_end("test");

The first argument to these functions is a string `tag` used to match start/end calls.
To avoid collisions, tags should be unique for each concurrently active timed block that
you want to track independently.

For convenience, code blocks can also be timed with the ``XASSERT_TIMED_BLOCK`` macro. This wraps
the passed code in calls to ``xassert_timing_start()`` and ``xassert_timing_end()``, for example::

    XASSERT_TIMED_BLOCK("test", 1000,
        // code to be timed
    );

This will check that the code in the block executes within 1000 timer ticks, typically
1000 * 10 ns = 10 us at a 100 MHz reference timer frequency. If the code takes longer than this
and timing assertions are enabled, a trap will occur, independently of
``XASSERT_ENABLE_ASSERTIONS``.

Timing loops
============

In some instances it is desirable to be able to check that a loop executes at a given frequency.
That can be done with the ``xassert_loop_freq()`` function. This function can be inserted at
any point in a loop, for example::

    while (1) {
        // do some work
        xassert_loop_freq("test loop", 1000);
    }

This checks for a minimum loop frequency of 1000 Hz. The first call records a timestamp;
subsequent calls with the same tag check the interval since the preceding call. If that interval
is too long and timing assertions are enabled, a trap will occur. A small tolerance is allowed
via ``XASSERT_TIMING_DELTA_ERROR`` (defaults to 2 timer ticks).
The first argument is a string `tag` used to identify the loop being timed.

It is not unusual for a loop, on occasion, to execute some additional code outside of its normal
operation, for example, a loop processing audio may occasionally change sample rate, which may
require configuring external hardware etc that may take some time but does not affect the normal
operation of the program.
It is not desirable for such code to cause a timing assertion failure. To allow for this, the
``xassert_loop_exception()`` function can be used to indicate that a loop iteration is executing
code that should be excluded from timing assertions. For example::

    while (1) {
        // do some work
        if (need_to_change_sample_rate) {
            xassert_loop_exception("test loop");
            // code to change sample rate
        }
        xassert_loop_freq("test loop", 1000);
    }


**********************
Controlling assertions
**********************

Assertions can be enabled/disabled via command line options to your application build.
The following defines can be set by using the ``-D`` option to the compiler.
For example, the following in your application ``CMakeLists.txt`` enables debug messages and
includes the file name and line number::

    set(APP_COMPILER_FLAGS
        -DXASSERT_ENABLE_DEBUG=1
        -DXASSERT_ENABLE_LINE_NUMBERS=1)

With no per-unit overrides, the following disables both ordinary and timing assertion checks::

    set(APP_COMPILER_FLAGS
        -DXASSERT_ENABLE_ASSERTIONS=0
        -DXASSERT_ENABLE_TIMING_ASSERTIONS=0)

These settings do not disable ``fail()``, which always traps. ``unreachable()`` is controlled by
the ordinary assertion setting alone; when disabled it becomes a compiler unreachable-code hint,
so executing that path is not valid. ``NDEBUG`` does not control this library's assertion settings.

.. list-table:: Global assertion controls without per-unit overrides
   :header-rows: 1

   * - ``XASSERT_ENABLE_ASSERTIONS``
     - ``XASSERT_ENABLE_TIMING_ASSERTIONS``
     - Enabled checks
   * - 1
     - 1
     - Ordinary and timing assertions
   * - 1
     - 0
     - Ordinary assertions only
   * - 0
     - 1
     - Timing assertions only
   * - 0
     - 0
     - Neither ordinary nor timing assertions

The following defines can be set:

XASSERT_ENABLE_ASSERTIONS
  Enables ordinary assertion checks (defaults to 1). Setting it to 0 removes ``assert()`` and
  ``xassert()`` checks, including evaluation of their expressions, and disables the trap in
  ``unreachable()``. It does not disable timing assertions.

XASSERT_ENABLE_TIMING_ASSERTIONS
  Enables timing assertions (defaults to 1), independently of ordinary assertions. When disabled,
  timing API arguments are not evaluated, but the body of ``XASSERT_TIMED_BLOCK`` still executes.

XASSERT_ENABLE_DEBUG
  This define will cause assertions to print out the failing expression before
  trapping (defaults to 0). Note that this option could significantly increase
  the code size of your application.

XASSERT_ENABLE_LINE_NUMBERS
  Includes the file and line number in assertion messages (defaults to 0). Debug output must
  also be enabled for the unit; this setting alone does not enable printing. Note that this
  option could significantly increase the code size of your application.

XASSERT_MAX_TIMING_BLOCKS
   Sets the timing buffer size per source file (defaults to 8). Set this to at least 2. The usable
   capacity is one less than the configured size, so the default holds 7 active items. Adding an
   item to a full buffer drops the entry at its head.

If ``XASSERT_UNIT`` is defined when ``xassert.h`` is included then all the
assertions in that file belong to that unit. Assertions can then be
controlled per debug unit. The mechanism is similar to that used in ``lib_logging``.

Per-unit enable and disable overrides take effect when set to 1. Setting a per-unit enable
macro to 0 does not force the feature off; it falls back to the global setting. For example,
use ``XASSERT_DISABLE_ASSERTIONS_MY_UNIT=1`` to disable ordinary assertions for ``MY_UNIT``.
Use ``XASSERT_DISABLE_TIMING_ASSERTIONS_MY_UNIT=1`` to disable timing assertions for that unit.
These overrides are independent, just like the global settings.

XASSERT_ENABLE_ASSERTIONS_[xassert unit]
   Enable asserts for a particular debug unit. If set to 1,
   this overrides the default set by ``XASSERT_ENABLE_ASSERTIONS`` for
   that debug unit.

XASSERT_DISABLE_ASSERTIONS_[xassert unit]
   Disable asserts for a particular debug unit. If set to 1,
   this overrides the default set by ``XASSERT_ENABLE_ASSERTIONS`` for
   that debug unit.

XASSERT_ENABLE_TIMING_ASSERTIONS_[xassert unit]
   Enable timing asserts for a particular debug unit. If set to 1,
   this overrides the default set by ``XASSERT_ENABLE_TIMING_ASSERTIONS`` for
   that debug unit.

XASSERT_DISABLE_TIMING_ASSERTIONS_[xassert unit]
   Disable timing asserts for a particular debug unit. If set to 1,
   this overrides the default set by ``XASSERT_ENABLE_TIMING_ASSERTIONS`` for
   that debug unit.

XASSERT_ENABLE_DEBUG_[xassert unit]
   Enable debug messages for a particular debug unit.  If set to 1,
   this overrides the default set by ``XASSERT_ENABLE_DEBUG`` for that
   debug unit.

XASSERT_DISABLE_DEBUG_[xassert unit]
   Disable debug messages for a particular debug unit.  If set to 1,
   this overrides the default set by ``XASSERT_ENABLE_DEBUG`` for that
   debug unit.

********
Examples
********

The ``examples`` directory contains the following applications:

.. list-table:: Example applications
   :header-rows: 1

   * - Directory
     - Demonstrates
     - Expected result
   * - ``app_assert``
     - Assertion expressions, messages and per-unit configuration
     - Assertion trap
   * - ``app_unreachable``
     - Detecting a path that should not execute
     - Assertion trap
   * - ``app_fail``
     - Reporting an unconditional failure
     - Assertion trap
   * - ``app_timed_block``
     - Detecting a block that exceeds its time limit
     - Timing assertion trap
   * - ``app_timed_loop``
     - Detecting a loop that runs too slowly
     - Timing assertion trap
   * - ``app_timing_loop_exception``
     - Excluding an exceptional iteration from loop timing; uses XC on XS only
     - Normal exit without a trap

An assertion trap is intentional in the examples listed above; it is not a build or simulator
failure. Under ``xsim``, the expected trap is reported as
``Unhandled exception: ECALL, data: 0x00000000`` and the simulator exits with a non-zero status.
The ``app_assert`` example disables ordinary assertions in ``fn_no_assert()`` while leaving
timing assertions enabled, then deliberately traps in ``fn_assert()``. The timed-block and
timed-loop examples disable timing assertions in their ``fn_no_assert()`` functions, then
deliberately exceed the block or loop timing limit in ``fn_assert()``.

Building the examples
=====================

Run the commands from a shell with the appropriate XTC Tools environment active. XS and VX
builds require toolchains that support their respective architectures; an XS-only toolchain
cannot build the VX target.

From the repository root, build all six examples for XS::

    cd examples
    cmake -G "Unix Makefiles" -B build-xs -DAPP_HW_TARGET=XK-EVK-XU316 -DXASSERT_BUILD_XC_EXAMPLES=ON
    xmake -C build-xs

Alternatively, with a VX-capable toolchain active, run the following from ``examples`` to build
the five C examples for VX::

    cmake -G "Unix Makefiles" -B build-vx -DAPP_HW_TARGET=XK-EVK-XU416 -DXASSERT_BUILD_XC_EXAMPLES=OFF
    xmake -C build-vx

``XASSERT_BUILD_XC_EXAMPLES`` defaults to ``ON``. Set it to ``OFF`` for VX because XC sources
cannot be compiled for that target. Each application writes its executable into its own ``bin``
directory, so building for another target replaces the corresponding binaries. Rebuild for the
desired target before running an example. The ``app_unreachable`` directory currently produces
an executable named ``app_upreachable.xe``.

Running an example
==================

For example, from ``examples`` run the timed-block application in the simulator::

    xsim --max-cycles 15000000 app_timed_block/bin/app_timed_block.xe

This prints a timing-failure message and then the expected assertion trap. The cycle limit
bounds the run; reaching that limit instead of observing the expected trap is not a successful
demonstration. Simulator wall-clock runtime is not the application's target execution time.

On a connected board matching the build target, the following enables debug-message output
over JTAG::

    xrun --io app_timed_block/bin/app_timed_block.xe

JTAG I/O can pause the target and perturb its timing. Use this command to observe the example's
messages, not to benchmark real-time performance.
