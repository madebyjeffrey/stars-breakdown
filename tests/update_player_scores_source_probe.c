#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 records;
    StarsI16 dead;
    StarsI16 messages;
    StarsI16 dirty;
    StarsI16 message_b5;
    StarsI16 message_b7;
    StarsI16 message_b8;
    StarsI16 message_bc;
    StarsI16 empty_player_one;
    StarsU16 last_mask;
    StarsI16 values[10];
    StarsI16 checks[10];
    StarsI16 nonmilitary_empty;
    StarsI16 technology_case;
    StarsUpdatePlayerScore captured[3];
} Trace;
static StarsI16 calculate(void *c, StarsI16 player,
    StarsUpdatePlayerScore *score)
{
    Trace *t = (Trace *)c;
    memset(score, 0, sizeof *score);
    if (player == 0) {
        score->total = 1000;
        score->planets = 10;
        score->ship_value = 100;
        score->tech_count = 4;
        score->resources = 5000;
    } else if (player == 1) {
        if (!t->empty_player_one) {
            score->total = 800;
            score->planets = 5;
            score->ship_value = 50;
            score->tech_count = 3;
            score->resources = 1000;
        } else if (t->nonmilitary_empty) {
            score->resources = 1000;
            score->tech_count = 6;
        }
    }
    if (t->technology_case) {
        /* Equal total levels must yield different category eligibility. */
        score->tech_count = 30;
        if (t->technology_case == 1)
            score->technology_levels[0] = 30;
        else {
            score->technology_levels[0] = 10;
            score->technology_levels[1] = 10;
            score->technology_levels[2] = 10;
        }
    }
    return 1;
}
static StarsI16 victory_value(void *c, StarsI16 index)

{
    return ((Trace *)c)->values[index];
}
static StarsI16 victory_check(void *c, StarsI16 index)
{
    return ((Trace *)c)->checks[index];
}
static void record(void *c, StarsI16 player,
    const StarsUpdatePlayerScore *score)
{
    Trace *t = (Trace *)c;
    ++t->records;
    t->captured[player] = *score;
}
static void mark_dead(void *c, StarsI16 player)
{
    (void)player; ++((Trace *)c)->dead;
}
static void message(void *c, StarsI16 player, StarsI16 id, StarsU16 mask)
{
    Trace *t = (Trace *)c;
    (void)player; ++t->messages; t->last_mask = mask;
    if (id == 0xb5) ++t->message_b5;
    if (id == 0xb7) ++t->message_b7;
    if (id == 0xb8) ++t->message_b8;
    if (id == 0xbc) ++t->message_bc;
}
static void dirty(void *c)
{
    ++((Trace *)c)->dirty;
}
static void install(StarsUpdatePlayerScoreOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->calculate = calculate;
    ops->victory_value = victory_value;
    ops->victory_check = victory_check;
    ops->record = record;
    ops->mark_dead = mark_dead;
    ops->send_message = message;
    ops->mark_dirty = dirty;
    ops->context = trace;
}
int main(void)
{
    StarsUpdatePlayerScoreInput input;
    StarsUpdatePlayerScoreOps ops;
    StarsUpdatePlayerScore players[3];
    Trace trace;
    StarsI16 i;

    memset(&input, 0, sizeof input);
    memset(players, 0, sizeof players);
    memset(&trace, 0, sizeof trace);
    trace.values[0] = 50;
    trace.values[1] = 5;
    trace.values[2] = 3;
    trace.values[3] = 900;
    trace.values[4] = 10;
    trace.values[5] = 4;
    trace.values[6] = 75;
    trace.values[7] = 5;
    trace.values[8] = 2;
    trace.values[9] = 5;
    for (i = 0; i < 10; ++i)
        trace.checks[i] = 1;
    install(&ops, &trace);
    input.player_count = 3;
    input.planet_count = 10;
    input.turn = 10;
    input.players = players;
    input.ops = &ops;
    stars_update_player_scores(&input);
    if (trace.records != 3 || trace.dead != 1 || trace.messages != 5 ||
            trace.dirty != 1 || trace.message_b7 != 0 || trace.message_b8 != 1 ||
            trace.last_mask != 4 || players[0].rank != 1 ||
            players[1].rank != 2 || players[2].dead != 1 ||
            (players[0].achievement_flags & 0x1000U) == 0 ||
            (players[0].achievement_flags & 0x0080U) != 0 ||
            (players[0].achievement_flags & 0x4000U) == 0 ||
            players[0].achievement_count < 5)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(players, 0, sizeof players);
    trace.empty_player_one = 1;
    trace.nonmilitary_empty = 1;
    for (i = 0; i < 10; ++i)
        trace.values[i] = 0;
    install(&ops, &trace);
    input.player_count = 3;
    input.turn = 0;
    stars_update_player_scores(&input);
    if (trace.dead != 2 || trace.messages != 7 || trace.dirty != 1 ||
            trace.message_bc != 1 || trace.message_b8 != 2)
        return 1;
    memset(&trace, 0, sizeof trace);
    trace.values[0] = 0;
    trace.values[1] = 1;
    trace.values[2] = 1;
    trace.values[3] = 0;
    trace.values[4] = 0;
    trace.values[5] = 0;
    trace.values[6] = 0;
    trace.values[7] = 0;
    trace.values[8] = 1;
    trace.values[9] = 0;
    install(&ops, &trace);
    input.player_count = 1;
    input.turn = 0;
    memset(players, 0, sizeof players);
    stars_update_player_scores(&input);
    if (trace.records != 1 || players[0].rank != 1 || trace.messages != 0)
        return 1;
    memset(&trace, 0, sizeof trace);
    trace.values[1] = 10; trace.values[2] = 3;
    trace.checks[1] = 1; trace.technology_case = 1;
    input.player_count = 1;
    stars_update_player_scores(&input);
    if ((players[0].achievement_flags & 0x0080U) != 0 ||
            players[0].tech_count != 30) return 2;
    trace.technology_case = 2;
    stars_update_player_scores(&input);
    if ((players[0].achievement_flags & 0x0080U) == 0 ||
            players[0].tech_count != 30) return 3;
    trace.values[2] = 4;
    stars_update_player_scores(&input);
    if ((players[0].achievement_flags & 0x0080U) != 0) return 4;
    puts("UPDATE_PLAYER_SCORES_CHECKS=37 FAILURES=0 RESULT=PASS");
    return 0;
}
