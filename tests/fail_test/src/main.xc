#include <xassert.h>

void f(int x) {
  fail("epic fail");
}

int main() {
  f(0);
  return 0;
}