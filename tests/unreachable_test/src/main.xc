// Copyright 2015-2021 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <xassert.h>

void f(int x) {
  unreachable();
}

int main() {
  f(0);
  return 0;
}