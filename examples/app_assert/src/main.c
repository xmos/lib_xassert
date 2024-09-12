// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

extern void fn_no_assert(int x);
extern void fn_assert(int x);

int main() {
    fn_no_assert(0); // This shouldn't assert
    fn_assert(0);   // This asserts
    return 0;
}
