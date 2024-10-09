// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_UNIT FN_NO_ASSERT
#define XASSERT_ENABLE_ASSERTIONS_FN_NO_ASSERT 0
#define XASSERT_ENABLE_DEBUG_FN_NO_ASSERT 1
#include <xassert.h>


void fn_no_assert(int x)
{
    assert(x > 5 && msg("assert from fn_no_assert()"));
}
