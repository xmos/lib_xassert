:orphan:

###############################
lib_xassert: Assertions library
###############################

:vendor: XMOS
:version: 4.3.3
:scope: General Use
:description: Lightweight assertions library
:category: General purpose
:keywords: Debug
:devices: xcore.ai, xcore-200

*******
Summary
*******

This library provides a lightweight and flexible replacement for the standard C header ``assert.h``.

The assertions in this library can be be enabled/disabled and configured as to how much information
they show. This configuration can be per `xassert unit` (i.e. for sets of files).

********
Features
********

* Low memory usage
* Ability to enable or disable various features via compile time defines
* Timing assertion system to check that code executes within a specified time limit

************
Known issues
************

  * Timing assertion APIs currently use translation-unit-local static state and are not thread safe;
    xassert_timing_start()/xassert_timing_end() must be used within the same source file and from a
    single thread (#64)

****************
Development repo
****************

* `lib_xassert <https://www.github.com/xmos/lib_xassert>`_ (https://www.github.com/xmos/lib_xassert)

**************
Required tools
**************

* XMOS XTC Tools: 15.3.1

*********************************
Required libraries (dependencies)
*********************************

* None

*************************
Related application notes
*************************

* None

*******
Support
*******

This package is supported by XMOS Ltd. Issues can be raised against the software at
`www.xmos.com/support <https://www.xmos.com/support>`_ or using GitHub `issues <https://github.com/xmos/lib_xassert/issues>`_.

