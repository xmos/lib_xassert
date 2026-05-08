// Copyright 2015-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xassert.h>
#include <xcore/hwtimer.h>

int main(void)
{
    hwtimer_t t = hwtimer_alloc();

    xassert_timing_start("timed-block", 20);
    hwtimer_delay(t, 200);
    xassert_timing_end("timed-block");

    hwtimer_free(t);
    return 0;
}
