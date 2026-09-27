#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned allocations;
static unsigned frees;
static unsigned checks;
static unsigned loads;
static unsigned runs;
static unsigned ticks;
static StarsU32 sizes[4];
static StarsI16 tick_value = 100;
static int force_error;

static void *allocate(void *context, StarsU32 size, StarsU16 type)
{
    static unsigned char blocks[4][8];
    (void)context; (void)type;
    sizes[allocations] = size;
    return blocks[allocations++ % 4U];
}
static void free_memory(void *context, void *memory, StarsU16 type)
{ (void)context; (void)memory; (void)type; ++frees; }
static StarsI16 check_log(void *context, StarsI16 player, StarsI16 *code)
{
    (void)context; ++checks;
    if (force_error) { *code = 0; return 1; }
    if (player == 2) { *code = 0; return 0; }
    *code = player == 0 ? 0 : 0x1c;
    return 0;
}
static StarsI16 load_log(void *context, StarsI16 player)
{ (void)context; (void)player; ++loads; return 1; }
static StarsI16 run_log(void *context)
{ (void)context; ++runs; return 0; }
static StarsU32 get_tick(void *context)
{ (void)context; ++ticks; return (StarsU32)++tick_value + 1000UL; }
static StarsVerifyTurnsOps ops = { allocate, free_memory, check_log,
    load_log, run_log, get_tick };

static int normal_case(void)
{
    StarsVerifyTurnsState state;
    memset(&state, 0, sizeof(state));
    state.id_player = 7; state.player_count = 3; state.plan_count = 4;
    state.global_flags = 0x80U;
    state.player_flags[1] = 0x0200U;
    state.player_dead[2] = 1U;
    state.output[0] = 9; state.output[1] = 7; state.output[2] = 3;
    allocations = frees = checks = loads = runs = ticks = 0U;
    tick_value = 100;
    stars_verify_turns(&state, &ops, 0);
    return allocations == 4U && sizes[0] == 12000UL &&
        sizes[1] == 25000UL && sizes[2] == 8UL && sizes[3] == 48UL &&
        frees == 4U && checks == 2U && loads == 0U && runs == 0U &&
        ticks == 3U && state.output[0] == 2 && state.output[1] == 0 &&
        state.output[2] == -1 && state.id_player == 7 &&
        state.col_drop == 0U && state.xfer_full == 0U &&
        state.message_index == 0U ? 0 : 1;
}
static int log_error_case(void)
{
    StarsVerifyTurnsState state;
    memset(&state, 0, sizeof(state));
    state.player_count = 1; state.output[0] = 0;
    force_error = 1;
    allocations = frees = checks = loads = runs = ticks = 0U;
    stars_verify_turns(&state, &ops, 0);
    force_error = 0;
    return checks == 1U && loads == 1U && runs == 1U &&
        state.output[0] == 3 ? 0 : 1;
}
int main(void)
{
    int failures = normal_case() + log_error_case();
    printf("VERIFY_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
