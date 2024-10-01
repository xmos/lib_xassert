// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_UNIT FN_UNREACHABLE
#define XASSERT_ENABLE_DEBUG_FN_UNREACHABLE 1   /* To enable debug message printing*/
#include <xassert.h>

void f(int x) {
  unreachable("We've reached unreachable code");
}

int main() {
  f(0);
  return 0;
}
