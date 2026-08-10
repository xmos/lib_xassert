// Copyright 2015-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xassert.h>

int main(void)
{
    xassert_timing_start("timed-block", 20);
    basix_delay_ticks(200);
    xassert_timing_end("timed-block");

    return 0;
}
