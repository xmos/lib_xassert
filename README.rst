:orphan:

###############################
lib_xassert: Assertions library
###############################

:vendor: XMOS
:version: 4.3.2
:scope: General Use
:description: Lightweight assertions library
:category: General Purpose
:keywords: debugging, logging
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

************
Known issues
************

  * None

****************
Development repo
****************

* `lib_xassert <https://www.github.com/xmos/lib_xassert>`_

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

This package is supported by XMOS Ltd. Issues can be raised against the software at www.xmos.com/support
