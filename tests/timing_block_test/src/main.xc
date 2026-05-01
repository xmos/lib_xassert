// Copyright 2015-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <xassert.h>

int main()
{
    timer t;
    unsigned now;

    xassert_timing_start("timed-block", 20);
    t :> now;
    t when timerafter(now + 200) :> void;
    xassert_timing_end("timed-block");

    return 0;
}
