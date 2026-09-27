#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 move_calls;
    StarsI16 attack_calls;
    StarsU16 attack_surviving;
} Trace;

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
    *initiative_min = 4;
    *initiative_max = 4;
    return 1;
}

static StarsI16 move_token(void *context, StarsI16 round,
    StarsCoolBattleToken *token)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)round;
    (void)token;
    ++trace->move_calls;
    return 1;
}

static StarsI16 initiative_available(void *context, StarsI16 initiative)
{
    (void)context;
    return initiative == 4;
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
    ++trace->attack_calls;
    trace->attack_surviving = surviving;
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
    StarsCoolBattleFleet fleets[2];
    StarsCoolBattleToken tokens[2];
    StarsCoolBattleInput input;
    StarsCoolBattleOps ops;
    StarsU16 attack_masks[2];
    Trace trace;

    memset(fleets, 0, sizeof fleets);
    memset(tokens, 0, sizeof tokens);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    tokens[0].active = 1; tokens[0].player = 0;
    tokens[0].init_min = 4; tokens[0].init_max = 4;
    tokens[1].active = 1; tokens[1].player = 1;
    tokens[1].init_min = 4; tokens[1].init_max = 4;
    fleets[0].owner = 0; fleets[0].planet_id = 10; fleets[0].ship_count = 1;
    fleets[1].owner = 1; fleets[1].planet_id = 10; fleets[1].ship_count = 1;
    /* Player 0 may attack player 1; player 1 has no permitted target. */
    attack_masks[0] = 2U;
    attack_masks[1] = 0U;
    ops.initialize = initialize;
    ops.move_token = move_token;
    ops.initiative_available = initiative_available;
    ops.attack_token = attack_token;
    ops.send_messages_ex = send_report;
    ops.context = &trace;
    input.fleets = fleets;
    input.fleet_count = 2;
    input.player_count = 2;
    input.player_mask = 3U;
    input.attack_masks = attack_masks;
    input.tokens = tokens;
    input.token_capacity = 2;
    input.ops = ops;

    if (!stars_f_do_cool_battle(&input) || trace.move_calls != 2 ||
            trace.attack_calls != 0 || input.surviving_players != 1U)
        return 1;
    puts("F_DO_COOL_BATTLE_ATTACK_MASK_ORDER_CHECKS=3 FAILURES=0 RESULT=PASS");
    return 0;
}
