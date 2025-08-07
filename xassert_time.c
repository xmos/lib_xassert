#include "xassert_time.h"

// Structure for tracking a timing block
typedef struct {
    const char *tag;
    unsigned start_time;
    unsigned deadline;
    const char *file;
    int line;
} timing_block_t;

#if XASSERT_ENABLE_ASSERTIONS0

static timing_block_t timing_blocks[MAX_TIMING_BLOCKS];
static int timing_block_count = 0;

void timing_assert_init(void) {
    timing_block_count = 0;
}

void timing_start_ex(const char *tag, unsigned max_ticks, const char *file, int line) {
    unsigned now = get_time();

    for (int i = 0; i < timing_block_count; ++i) {
        if (timing_blocks[i].tag == tag) {
            timing_blocks[i].start_time = now;
            timing_blocks[i].deadline = now + max_ticks;
            timing_blocks[i].file = file;
            timing_blocks[i].line = line;
            return;
        }
    }

    xassert(timing_block_count < MAX_TIMING_BLOCKS && msg("Too many concurrent timing blocks"));

    timing_blocks[timing_block_count++] = (timing_block_t){
        .tag = tag,
        .start_time = now,
        .deadline = now + max_ticks,
        .file = file,
        .line = line
    };
}

void timing_end_ex(const char *tag, const char *file, int line) {
    unsigned now = get_time();

    for (int i = 0; i < timing_block_count; ++i) {
        if (timing_blocks[i].tag == tag) {
            if (now > timing_blocks[i].deadline) {
                printstr("Timing deadline missed for tag: ");
                printstr(tag);
                printstr(" (Started at ");
                printint(timing_blocks[i].start_time);
                printstr(", Deadline ");
                printint(timing_blocks[i].deadline);
                printstr(", Now ");
                printint(now);
                printstr(")\nStart Location: ");
                printstr(timing_blocks[i].file);
                printstr(":");
                printint(timing_blocks[i].line);
                printstr("\nEnd Location: ");
                printstr(file);
                printstr(":");
                printint(line);
                printstr("\n");
                __builtin_trap();
            }

            // Remove this block by overwriting with the last one
            timing_blocks[i] = timing_blocks[--timing_block_count];
            return;
        }
    }

    fail("timing_end() called for unknown tag (no prior timing_start())");
}

#endif // XASSERT_ENABLE_ASSERTIONS0

