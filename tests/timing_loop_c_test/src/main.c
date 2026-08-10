// Copyright 2015-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_ENABLE_TIMING_ASSERTIONS 1

#include <xassert.h>

int main(void)
{
    xassert_loop_freq("timed-loop", 100000000);
    basix_delay_ticks(100);
    xassert_loop_freq("timed-loop", 100000000);

    return 0;
}
