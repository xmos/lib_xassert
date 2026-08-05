// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_ENABLE_DEBUG 1
#include <xassert.h>

int main(void)
{
    unreachable("unreachable");
    return 0;
}
