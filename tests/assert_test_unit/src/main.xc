// Copyright (c) 2015-2016, XMOS Ltd, All rights reserved
#define XASSERT_UNIT TESTER
#define XASSERT_ENABLE_DEBUG_TESTER 1
#include <xassert.h>

void f(int x) {
  assert(x > 5 && msg("x is not greater than 5"));
}

int main() {
  f(0);
  return 0;
}
