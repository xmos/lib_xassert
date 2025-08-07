// Copyright 2014-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef __xassert_time_h__
#define __xassert_time_h__

#include "xassert.h"

#ifndef __XC__
extern "C" {
#endif

#ifndef MAX_TIMING_BLOCKS
#define MAX_TIMING_BLOCKS 8
#endif

#if XASSERT_ENABLE_ASSERTIONS0

// Initialize the timing system (optional in most systems)
void timing_assert_init(void);

// Start a timing block with a deadline
void timing_start_ex(const char *tag, unsigned max_ticks, const char *file, int line);

// End a timing block and assert that it's within deadline
void timing_end_ex(const char *tag, const char *file, int line);

// Macros to insert file/line automatically
#define timing_start(tag, max_ticks) timing_start_ex(tag, max_ticks, __FILE__, __LINE__)
#define timing_end(tag)              timing_end_ex(tag, __FILE__, __LINE__)

// TIMED_BLOCK: for wrapping a block of code with a timing assertion
#define TIMED_BLOCK(tag, max_ticks, body)               \
    do {                                                \
        timing_start(tag, max_ticks);                   \
        body                                            \
        timing_end(tag);                                \
    } while (0)

#else

#define timing_start(tag, max_ticks)      ((void)0)
#define timing_end(tag)                  ((void)0)
#define TIMED_BLOCK(tag, max_ticks, body) do { body } while (0)
#define timing_assert_init()             ((void)0)

#endif

#ifndef __XC__
}
#endif

#endif // __xassert_time_h__

