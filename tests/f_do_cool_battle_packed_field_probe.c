#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 typed_checks;
    StarsI16 attack_calls;
    StarsI16 typed_failure;
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
    *initiative_min = 10;
    *initiative_max = 12;
    return 1;
}

static StarsI16 move_token(void *context, StarsI16 round,
    StarsCoolBattleToken *token)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)round;
    if (token->id == 0x1234 &&
            token->player == 5 && token->object_kind == 6 &&
            token->design == 7 && token->battle_coordinate == 8 &&
            token->init_base == 9 && token->init_min == 10 &&
            token->init_max == 11 && token->target_token == 12 &&
            token->pct_cloak == 13 && token->pct_jam == 14 &&
            token->pct_battle_crit == 15 && token->pct_capture == 16 &&
            token->pct_beam_def == 17 && token->weight == 0x2345U &&
            token->dp_shield == 0x3456U && token->count == 0x4567U &&
            token->defense_value == 0x5678U &&
            token->target_flags == 0x6789U &&
            token->movement_packed == 0xdabcU && token->warp == 0xaU &&
            token->movement_flags == 0xa3ffU && token->active != 0U)
        ++trace->typed_checks;
    else if (token->id == 0x1234)
        trace->typed_failure = 1;
    /* Change only the speed nibble on the first token.  The packed bridge
     * must preserve the other movement nibbles and flags on write-back. */
    if (token->id == 0x1234)
        token->warp = 3U;
    return 1;
}

static StarsI16 initiative_available(void *context, StarsI16 initiative)
{
    (void)context;
    return initiative == 11;
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
    (void)surviving;
    ++trace->attack_calls;
    token->active = 0U;
    token->count = 0U;
    return 1;
}

static StarsI16 send_report(void *context,
    const StarsCoolBattleReportSummary *summary)
{
    (void)context;
    (void)summary;
    return 1;
}

int main(void)
{
    unsigned char raw[2][32];
    unsigned char expected0[32];
    unsigned char expected1[32];
    StarsCoolBattleFleet fleets[2];
    StarsCoolBattleToken tokens[2];
    StarsCoolBattleInput input;
    Trace trace;

    memset(raw, 0, sizeof raw);
    memset(expected0, 0, sizeof expected0);
    memset(expected1, 0, sizeof expected1);
    memset(fleets, 0, sizeof fleets);
    memset(tokens, 0, sizeof tokens);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);

    put_word(raw[0], 0, 0x1234U);
    raw[0][2] = 5; raw[0][3] = 6; raw[0][4] = 7; raw[0][5] = 8;
    raw[0][6] = 9; raw[0][7] = 10; raw[0][8] = 11; raw[0][9] = 12;
    raw[0][10] = 13; raw[0][11] = 14; raw[0][12] = 15;
    raw[0][13] = 16; raw[0][14] = 17;
    put_word(raw[0], 15, 0x2345U);
    put_word(raw[0], 17, 0x3456U);
    put_word(raw[0], 19, 0x4567U);
    put_word(raw[0], 21, 0x5678U);
    put_word(raw[0], 23, 0x6789U);
    put_word(raw[0], 25, 0xdabcU);
    put_word(raw[0], 27, 0xa3ffU);
    raw[0][29] = 0xa1; raw[0][30] = 0xb2; raw[0][31] = 0xc3;

    put_word(raw[1], 0, 0x2234U);
    raw[1][2] = 6; raw[1][3] = 7; raw[1][4] = 8; raw[1][5] = 9;
    raw[1][6] = 10; raw[1][7] = 11; raw[1][8] = 12; raw[1][9] = 13;
    raw[1][10] = 14; raw[1][11] = 15; raw[1][12] = 16;
    raw[1][13] = 17; raw[1][14] = 18;
    put_word(raw[1], 15, 0x2346U);
    put_word(raw[1], 17, 0x3457U);
    put_word(raw[1], 19, 0x4568U);
    put_word(raw[1], 21, 0x5679U);
    put_word(raw[1], 23, 0x678aU);
    put_word(raw[1], 25, 0xedcbU);
    put_word(raw[1], 27, 0xb3ffU);
    raw[1][29] = 0xd4; raw[1][30] = 0xe5; raw[1][31] = 0xf6;

    memcpy(expected0, raw[0], sizeof expected0);
    memcpy(expected1, raw[1], sizeof expected1);
    put_word(expected0, 25, 0xd3bcU);
    put_word(expected0, 27, 0xa3ffU);
    put_word(expected1, 19, 0U);
    put_word(expected1, 25, 0xedcbU);
    put_word(expected1, 27, 0xb3feU);

    fleets[0].owner = 5; fleets[0].planet_id = 10; fleets[0].ship_count = 1;
    fleets[1].owner = 6; fleets[1].planet_id = 10; fleets[1].ship_count = 1;
    input.fleets = fleets;
    input.fleet_count = 2;
    input.player_count = 7;
    input.player_mask = (StarsU16)(0x20U | 0x40U);
    input.tokens = tokens;
    input.token_capacity = 2;
    input.battle_id = 1;
    input.packed_tokens = &raw[0][0];
    input.packed_token_stride = 32;
    input.packed_token_capacity = 2;
    input.ops.initialize = initialize;
    input.ops.move_token = move_token;
    input.ops.initiative_available = initiative_available;
    input.ops.attack_token = attack_token;
    input.ops.send_messages_ex = send_report;
    input.ops.context = &trace;

    if (!stars_f_do_cool_battle(&input) || trace.typed_checks != 1 ||
            trace.typed_failure || trace.attack_calls != 1 ||
            memcmp(raw[0], expected0, sizeof raw[0]) != 0 ||
            memcmp(raw[1], expected1, sizeof raw[1]) != 0 ||
            get_word(raw[0], 29) != 0xb2a1U ||
            get_word(raw[1], 29) != 0xe5d4U) {
        return 1;
    }

    puts("F_DO_COOL_BATTLE_PACKED_FIELD_CHECKS=24 FAILURES=0 RESULT=PASS");
    return 0;
}
