// Copyright 2015-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_ENABLE_TIMING_ASSERTIONS 1

#include <xs1.h>
#include <xassert.h>

int main() {
  timer t;
  unsigned now;

  xassert_loop_freq("timed-loop", 100000000);

  t :> now;
  t when timerafter(now + 100) :> void;

  xassert_loop_freq("timed-loop", 100000000);

  return 0;
}
