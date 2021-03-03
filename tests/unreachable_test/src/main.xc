// Copyright (c) 2015-2021, XMOS Ltd, All rights reserved
// This software is available under the terms provided in LICENSE.txt.
#include <xassert.h>

void f(int x) {
  unreachable();
}

int main() {
  f(0);
  return 0;
}