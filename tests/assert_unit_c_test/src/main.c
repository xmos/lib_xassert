// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_UNIT TESTER
#define XASSERT_ENABLE_DEBUG_TESTER 1
#include <xassert.h>

int main(void)
{
    xassert(0);
    return 0;
}
