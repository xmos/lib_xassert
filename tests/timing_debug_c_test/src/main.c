// Copyright 2015-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_ENABLE_DEBUG 1
#define XASSERT_ENABLE_TIMING_ASSERTIONS 1

#include <xassert.h>
#include <xcore/hwtimer.h>

int main(void)
{
    hwtimer_t t = hwtimer_alloc();

    xassert_timing_start("timed-block", 1);
    hwtimer_delay(t, 20);
    xassert_timing_end("timed-block");

    hwtimer_free(t);
    return 0;
}
