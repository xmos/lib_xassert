// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_UNIT CONTROL
#include <xassert.h>
#include <basix/print.h>

static void check_state(int condition)
{
    if (!condition)
        fail("Incorrect assertion control state");
}

int main(void)
{
    int regular_evaluations = 0;
    assert(++regular_evaluations);
    xassert(++regular_evaluations);
    check_state(regular_evaluations == 2 * CONTROL_EXPECT_ASSERTIONS);

    int timing_evaluations = 0;
    xassert_timing_start("arguments", ++timing_evaluations * 1000000);
    xassert_timing_end("arguments");
    xassert_loop_freq("arguments", ++timing_evaluations * 1000);
    xassert_loop_exception("arguments");
    check_state(timing_evaluations == 2 * CONTROL_EXPECT_TIMING);

    int body_evaluations = 0;
    XASSERT_TIMED_BLOCK("preserved-body", 1000000,
        ++body_evaluations;
    );
    check_state(body_evaluations == 1);
    printstr("STATE OK\n");

#if CONTROL_PROBE_REGULAR
    assert(0);
#elif CONTROL_PROBE_TIMING
    xassert_timing_start("overrun", 1);
    basix_delay_ticks(1000);
    xassert_timing_end("overrun");
#elif CONTROL_PROBE_LOOP
    xassert_loop_freq("slow-loop", 1000000);
    basix_delay_ticks(1000);
    xassert_loop_freq("slow-loop", 1000000);
#elif !CONTROL_PROBE_STATE
#error "An assertion control probe must be selected"
#endif

    printstr("CONTINUED\n");
    // The STATE probe reaches this in every profile, including both disabled.
    fail("fail() must remain unconditional");
    return 0;
}
