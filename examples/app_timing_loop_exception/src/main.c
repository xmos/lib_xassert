// Copyright 2024-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

extern void fn_no_assert(void);
extern void fn_assert(void);

int main() {
    fn_no_assert(); // This shouldn't assert
    return 0;
}
