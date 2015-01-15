#include <xassert.h>

void f(int x) {
  unreachable();
}

int main() {
  f(0);
  return 0;
}