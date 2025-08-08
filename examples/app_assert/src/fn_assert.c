// Copyright 2024-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_UNIT FN_ASSERT
#define XASSERT_ENABLE_ASSERTIONS_FN_ASSERT 1 /* Enable assertions*/
#define XASSERT_ENABLE_DEBUG_FN_ASSERT 1    /* Enable printing debug message when asserting */
#define XASSERT_ENABLE_LINE_NUMBERS 1 /* Enable line numbers in assert messages */


#include <xassert.h>

void fn_assert(int x)
{
    assert(x > 5 && msg("assert from fn_assert()"));
}
