#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    unsigned char raw[2][29];
    StarsCoolBattleToken *tokens;
    StarsI16 attack_calls;
    StarsCoolBattleReportSummary summary;
} Trace;

static StarsU16 get_word(const unsigned char *raw, StarsI16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1] << 8));
}

static void put_word(unsigned char *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (unsigned char)(value & 0xffU);
    raw[offset + 1] = (unsigned char)(value >> 8);
}

static StarsI16 initialize(void *context, const StarsCoolBattleFleet *fleets,
    StarsI16 fleet_count, StarsI16 player_count, StarsU16 player_mask,
    StarsCoolBattleToken *tokens, StarsI16 token_capacity,
    StarsI16 *token_count, StarsI16 *round_count, StarsI16 *initiative_min,
    StarsI16 *initiative_max)
{
    (void)context;
    (void)fleets;
    (void)fleet_count;
    (void)player_count;
    (void)player_mask;
    (void)tokens;
    (void)token_capacity;
    *token_count = 2;
    *round_count = 0;
    *initiative_min = 2;
    *initiative_max = 2;
    return 1;
}

static StarsI16 move_token(void *context, StarsI16 round,
    StarsCoolBattleToken *token)
{
    (void)context;
    (void)round;
    (void)token;
    return 1;
}

static StarsI16 initiative_available(void *context, StarsI16 initiative)
{
    (void)context;
    return initiative == 2;
}

static StarsI16 attack_token(void *context, StarsI16 round,
    StarsI16 initiative, StarsI16 index, StarsCoolBattleToken *token,
    StarsU16 surviving)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)round;
    (void)initiative;
    (void)index;
    (void)token;
    (void)surviving;
    ++trace->attack_calls;
    trace->tokens[1].active = 0;
    trace->tokens[1].count = 0;
    return 1;
}

static StarsI16 send_report(void *context,
    const StarsCoolBattleReportSummary *summary)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->summary = *summary;
    return 1;
}

int main(void)
{
    StarsCoolBattleFleet fleets[2];
    StarsCoolBattleToken tokens[2];
    StarsCoolBattleInput input;
    StarsCoolBattleOps ops;
    Trace trace;
    StarsU16 token0_flags;
    StarsU16 token1_flags;
    unsigned char raw0_before[29];
    unsigned char raw1_expected[29];

    memset(fleets, 0, sizeof fleets);
    memset(tokens, 0, sizeof tokens);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    put_word(trace.raw[0], 0, 100U);
    trace.raw[0][2] = 0;
    trace.raw[0][3] = 1;
    trace.raw[0][4] = 3;
    trace.raw[0][5] = 0x11U;
    trace.raw[0][9] = 7;
    trace.raw[0][10] = 11;
    trace.raw[0][11] = 13;
    trace.raw[0][12] = 17;
    trace.raw[0][13] = 19;
    trace.raw[0][14] = 23;
    put_word(trace.raw[0], 15, 0x2345U);
    trace.raw[0][6] = 4;
    trace.raw[0][7] = 2;
    trace.raw[0][8] = 2;
    put_word(trace.raw[0], 17, 9U);
    put_word(trace.raw[0], 19, 2U);
    put_word(trace.raw[0], 21, 0x3456U);
    put_word(trace.raw[0], 23, 0x1234U);
    put_word(trace.raw[0], 25, 0x4321U);
    token0_flags = 1U | 0x0060U;
    put_word(trace.raw[0], 27, token0_flags);
    put_word(trace.raw[1], 0, 101U);
    trace.raw[1][2] = 1;
    trace.raw[1][3] = 2;
    trace.raw[1][4] = 4;
    trace.raw[1][5] = 0x22U;
    trace.raw[1][9] = 8;
    trace.raw[1][10] = 12;
    trace.raw[1][11] = 14;
    trace.raw[1][12] = 18;
    trace.raw[1][13] = 20;
    trace.raw[1][14] = 24;
    put_word(trace.raw[1], 15, 0x2346U);
    trace.raw[1][6] = 4;
    trace.raw[1][7] = 2;
    trace.raw[1][8] = 2;
    put_word(trace.raw[1], 17, 10U);
    put_word(trace.raw[1], 19, 3U);
    put_word(trace.raw[1], 21, 0x3457U);
    put_word(trace.raw[1], 23, 0x5678U);
    put_word(trace.raw[1], 25, 0x4322U);
    token1_flags = 1U | 0x00a0U;
    put_word(trace.raw[1], 27, token1_flags);
    fleets[0].owner = 0;
    fleets[0].planet_id = 10;
    fleets[0].point.x = 20;
    fleets[0].point.y = 30;
    fleets[0].ship_count = 1;
    fleets[1].owner = 1;
    fleets[1].planet_id = 10;
    fleets[1].ship_count = 1;
    trace.tokens = tokens;
    ops.initialize = initialize;
    ops.move_token = move_token;
    ops.initiative_available = initiative_available;
    ops.attack_token = attack_token;
    ops.send_messages_ex = send_report;
    ops.context = &trace;
    input.fleets = fleets;
    input.fleet_count = 2;
    input.player_count = 2;
    input.player_mask = 3;
    input.tokens = tokens;
    input.token_capacity = 2;
    input.battle_id = 0x220;
    input.packed_tokens = &trace.raw[0][0];
    input.packed_token_stride = 29;
    input.packed_token_capacity = 2;
    input.ops = ops;
    memcpy(raw0_before, trace.raw[0], sizeof raw0_before);
    memcpy(raw1_expected, trace.raw[1], sizeof raw1_expected);
    put_word(raw1_expected, 19, 0U);
    put_word(raw1_expected, 27, (StarsU16)(token1_flags & (StarsU16)~1U));

    if (!stars_f_do_cool_battle(&input) || trace.attack_calls != 1 ||
            tokens[0].id != 100 || tokens[1].id != 101 ||
            trace.raw[0][2] != 0 || trace.raw[0][4] != 3 ||
            get_word(trace.raw[0], 19) != 2U ||
            trace.raw[1][2] != 1 || trace.raw[1][4] != 4 ||
            get_word(trace.raw[1], 19) != 0U ||
            (get_word(trace.raw[1], 27) & 1U) != 0U ||
            memcmp(trace.raw[0], raw0_before, sizeof raw0_before) != 0 ||
            memcmp(trace.raw[1], raw1_expected, sizeof raw1_expected) != 0 ||
            input.surviving_players != 1U ||
            trace.summary.active_player_mask != 3U)
        return 1;
    {
        StarsI16 second_result;
        input.packed_token_capacity = 1;
        second_result = stars_f_do_cool_battle(&input);
        if (second_result > 0) {
            return 1;
        }
    }
    puts("F_DO_COOL_BATTLE_PACKED_TOKEN_CHECKS=11 FAILURES=0 RESULT=PASS");
    return 0;
}
