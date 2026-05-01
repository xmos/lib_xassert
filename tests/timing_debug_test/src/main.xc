// Copyright 2015-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_ENABLE_DEBUG 1
#define XASSERT_ENABLE_TIMING_ASSERTIONS 1

#include <xs1.h>
#include <xassert.h>

int main() {
  timer t;
  unsigned now;

  xassert_timing_start("timed-block", 1);
  t :> now;
  t when timerafter(now + 20) :> void;
  xassert_timing_end("timed-block");

  return 0;
}
