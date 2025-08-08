// Copyright 2014-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#ifndef __xassert_h__
#define __xassert_h__


#ifdef __xassert_conf_h_exists__
#error
#include "xassert_conf.h"
#endif

#ifdef __debug_conf_h_exists__
#include "debug_conf.h"
#endif

#ifndef XASSERT_UNIT
#define XASSERT_UNIT APPLICATION
#endif

#ifndef XASSERT_ENABLE_ASSERTIONS
#define XASSERT_ENABLE_ASSERTIONS 1
#endif

#ifndef XASSERT_ENABLE_TIMING_ASSERTIONS
#define XASSERT_ENABLE_TIMING_ASSERTIONS 1
#endif

#ifndef XASSERT_ENABLE_DEBUG
#define XASSERT_ENABLE_DEBUG 0
#endif

#ifndef XASSERT_ENABLE_LINE_NUMBERS
#define XASSERT_ENABLE_LINE_NUMBERS 0
#endif

#define XASSERT_JOIN0(x,y) x ## y
#define XASSERT_JOIN(x,y) XASSERT_JOIN0(x,y)

#if XASSERT_JOIN(XASSERT_ENABLE_ASSERTIONS_,XASSERT_UNIT)
#  define XASSERT_ENABLE_ASSERTIONS0 1
#endif

#if XASSERT_JOIN(XASSERT_DISABLE_ASSERTIONS_,XASSERT_UNIT)
#  define XASSERT_ENABLE_ASSERTIONS0 0
#endif

#if !defined(XASSERT_ENABLE_ASSERTIONS0)
#  define XASSERT_ENABLE_ASSERTIONS0 XASSERT_ENABLE_ASSERTIONS
#endif

#if XASSERT_JOIN(XASSERT_ENABLE_TIMING_ASSERTIONS_,XASSERT_UNIT)
#  define XASSERT_ENABLE_TIMING_ASSERTIONS0 1
#endif

#if XASSERT_JOIN(XASSERT_DISABLE_TIMING_ASSERTIONS_,XASSERT_UNIT)
#  define XASSERT_ENABLE_TIMING_ASSERTIONS0 0
#endif

#if XASSERT_JOIN(XASSERT_ENABLE_DEBUG_,XASSERT_UNIT)
#  define XASSERT_ENABLE_DEBUG0 1
#endif

#if XASSERT_JOIN(XASSERT_DISABLE_DEBUG_,XASSERT_UNIT)
#  define XASSERT_ENABLE_DEBUG0 0
#endif

#if !defined(XASSERT_ENABLE_DEBUG0)
#  define XASSERT_ENABLE_DEBUG0 XASSERT_ENABLE_DEBUG
#endif

#if XASSERT_ENABLE_DEBUG0
#include "print.h"
#include <stdio.h>
#endif

#if XASSERT_ENABLE_LINE_NUMBERS
#  define xassert_print_line do { printstr(" (" __FILE__ ":");    \
                                  printint(__LINE__);             \
                                  printstr(")\n");                \
                                } while(0)

#define xassert_timing_print_line(file, line) do { printf(" (%s:", file); \
                                                   fflush(stdout);           \
                                                   printint(line);           \
                                                   printstr(")\n");          \
                                                } while(0)

#else
#  define xassert_print_line do { printstr("\n"); } while(0)
#  define xassert_timing_print_line(file, line) do { printstr("\n"); } while(0)
#endif

#if XASSERT_ENABLE_ASSERTIONS0 || XASSERT_ENABLE_TIMING_ASSERTIONS0
#  if XASSERT_ENABLE_DEBUG0
#    define xassert(e) do { if (!(e)) {\
       printstr(#e); xassert_print_line; \
      __builtin_trap();} \
      } while(0)
#  else
#    define xassert(e) do { if (!(e)) __builtin_trap();} while(0)
#  endif
#else
#  define xassert(e)   // disabled
#endif

#if XASSERT_ENABLE_ASSERTIONS0
#  if XASSERT_ENABLE_DEBUG0
#    define unreachable(msg) do { printstr(msg); xassert_print_line; __builtin_trap();} while(0)
#  else
#    define unreachable(msg) do { __builtin_trap();} while(0)
#  endif
#else
#  define unreachable(msg) do { __builtin_unreachable();} while(0)
#endif

#if XASSERT_ENABLE_DEBUG0
#  define fail(msg) do { printstr(msg); xassert_print_line; __builtin_trap();} while(0)
#  define fail_timing(tag, actual, limit, file, line) do { printstr("Timing failed for: "); \
                                printf("%s", (const char *) tag); \
                                fflush(stdout); \
                                printstr("\nΔt = "); printint(actual); printstr(" ticks ("); printint((actual) * 10); printstr(" ns), "); \
                                printstr("limit = "); printint(limit); printstr(" ticks ("); printint((limit) * 10); printstr(" ns) "); \
                                xassert_timing_print_line(file, line); __builtin_trap();\
                              } while(0)
#else
#  define fail(msg) do { __builtin_trap();} while(0)
#  define fail_timing(tag, actual, limit, file, line) do { __builtin_trap();} while(0)
#endif

#ifndef UNUSED
#ifdef __XC__
#define UNUSED(x)     do { x; } while(0);
#else
#define UNUSED(x)     (void)(x)
#endif
#endif // UNUSED

inline int xassert_msg(const char msg[]) { UNUSED(msg); return 1; }

#ifdef __XC__
#define _msg(x) xassert_msg(x)
#define  msg(x) xassert_msg(x)
#else
#define _msg(x) x
#define  msg(x) x
#endif

#if !defined(assert) && !XASSERT_DISABLE_ASSERT_DEF
#define assert(...) xassert(__VA_ARGS__)
#endif

#ifndef __STDC__
extern "C" {
#endif

#ifndef MAX_TIMING_BLOCKS
#define MAX_TIMING_BLOCKS 8
#endif

#if XASSERT_ENABLE_TIMING_ASSERTIONS0

#ifdef __XC__
# define UNSAFE unsafe
#endif

// DJB2 hash for string to int ID
static inline unsigned xassert_hash(const char *str)
{
    unsigned hash = 5381;
    char c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

#define XASSERT_TAG_ID(tag) (xassert_hash(tag))

typedef struct {
    const char * UNSAFE tag;
    unsigned start_time;
    unsigned deadline;
    const char *file;
    int line;
    int is_loop;
    unsigned loop_interval_ticks;
    unsigned id;
} timing_block_t;

static inline unsigned get_time(void)
{
    unsigned time;
    asm volatile("gettime %0" : "=r"(time));
    return time;
}

static timing_block_t timing_blocks[MAX_TIMING_BLOCKS];
static int head = 0;
static int tail = 0;

#define CIRCULAR_INC(x) (((x) + 1) % MAX_TIMING_BLOCKS)
#define CIRCULAR_FULL (CIRCULAR_INC(tail) == head)
#define CIRCULAR_EMPTY (head == tail)

static inline void drop_oldest_entry() { head = CIRCULAR_INC(head); }

static inline void timing_start_impl(const char *tag, unsigned id, unsigned max_ticks, const char *file, int line)
{
    unsigned now = get_time();

    for (int i = head; i != tail; i = CIRCULAR_INC(i))
    {
        if (timing_blocks[i].id == id && !timing_blocks[i].is_loop)
        {
            timing_blocks[i].start_time = now;
            timing_blocks[i].deadline = now + max_ticks;
            timing_blocks[i].file = file;
            timing_blocks[i].line = line;
            return;
        }
    }

    if (CIRCULAR_FULL)
    {
#if XASSERT_ENABLE_DEBUG0
        printstr("WARNING: Timing buffer full. Dropping oldest entry.\n");
#endif
        drop_oldest_entry();
    }

    timing_blocks[tail].tag = tag;
    timing_blocks[tail].start_time = now;
    timing_blocks[tail].deadline = now + max_ticks;
    timing_blocks[tail].file = file;
    timing_blocks[tail].line = line;
    timing_blocks[tail].is_loop = 0;
    timing_blocks[tail].loop_interval_ticks = 0;
    timing_blocks[tail].id = id;

    tail = CIRCULAR_INC(tail);
}

static inline void timing_end_impl(const char *tag, unsigned id, const char *file, int line)
{
    unsigned now = get_time();
    for (int i = head; i != tail; i = CIRCULAR_INC(i))
    {
        if ((timing_blocks[i].id == id) && !timing_blocks[i].is_loop)
        {
            if (now > timing_blocks[i].deadline)
            {
                fail_timing(tag, now - timing_blocks[i].start_time, timing_blocks[i].deadline - timing_blocks[i].start_time, file, line);
            }
            for (int j = i; j != tail; j = CIRCULAR_INC(j))
            {
                timing_blocks[j] = timing_blocks[CIRCULAR_INC(j)];
            }
            tail = (tail == 0) ? MAX_TIMING_BLOCKS - 1 : tail - 1;
            return;
        }
    }
    fail("timing_end() called without matching timing_start()");
}

static inline void timing_loop_impl(const char *tag, unsigned id, unsigned min_freq_hz, const char *file, int line)
{
    unsigned now = get_time();
    unsigned interval = XS1_TIMER_HZ / min_freq_hz;

    for (int i = head; i != tail; i = CIRCULAR_INC(i))
    {
        // Now use id (hash) for matching to support multiple literals!
        if ((timing_blocks[i].id == id) && timing_blocks[i].is_loop)
        {
            unsigned delta = now - timing_blocks[i].start_time;
            if (delta > interval)
            {
                fail_timing(tag, delta, interval, file, line);
            }
            timing_blocks[i].start_time = now;
            return;
        }
    }

    if (CIRCULAR_FULL)
    {
        /* TODO maybe we should fail here instead? */
#if XASSERT_ENABLE_DEBUG0
        printstr("WARNING: Timing buffer full. Dropping oldest entry.\n");
#endif
        drop_oldest_entry();
    }

    timing_blocks[tail].tag = tag;
    timing_blocks[tail].start_time = now;
    timing_blocks[tail].deadline = 0;
    timing_blocks[tail].loop_interval_ticks = interval;
    timing_blocks[tail].file = file;
    timing_blocks[tail].line = line;
    timing_blocks[tail].is_loop = 1;
    timing_blocks[tail].id = id;

    tail = CIRCULAR_INC(tail);
}

// Remove a loop timing entry for tag/id (used in exceptional cases)
static inline void xassert_loop_exception(const char *tag)
{
    unsigned id = XASSERT_TAG_ID(tag);
    for (int i = head; i != tail; i = CIRCULAR_INC(i))
    {
        if ((timing_blocks[i].id == id) && timing_blocks[i].is_loop)
        {
            for (int j = i; j != tail; j = CIRCULAR_INC(j))
                timing_blocks[j] = timing_blocks[CIRCULAR_INC(j)];
            tail = (tail == 0) ? MAX_TIMING_BLOCKS - 1 : tail - 1;
            break;
        }
    }
}

#define xassert_timing_start(tag, max_ticks) timing_start_impl(tag, XASSERT_TAG_ID(tag), max_ticks, __FILE__, __LINE__)
#define xassert_timing_end(tag)              timing_end_impl(tag, XASSERT_TAG_ID(tag), __FILE__, __LINE__)
#define xassert_loop_freq(tag, hz)           timing_loop_impl(tag, XASSERT_TAG_ID(tag), hz, __FILE__, __LINE__)
// Optionally, a macro for exception for API symmetry:
#define xassert_loop_exception(tag)          xassert_loop_exception(tag)

#define XASSERT_TIMED_BLOCK(tag, max_ticks, body) \
    do { \
        xassert_timing_start(tag, max_ticks); \
        body \
        xassert_timing_end(tag); \
    } while (0)

#else

#define xassert_timing_start(tag, max_ticks)
#define xassert_timing_end(tag)
#define xassert_loop_freq(tag, hz)
#define xassert_loop_exception(tag)
#define XASSERT_TIMED_BLOCK(tag, max_ticks, body) body

#endif

#ifndef __STDC__
}
#endif

#endif // __xassert_h__

