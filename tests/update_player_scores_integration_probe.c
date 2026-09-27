#include "core.h"
#include <stdio.h>
#include <string.h>

#define PLAYER_COUNT 3

typedef struct PackedState {
    StarsU8 scorex[PLAYER_COUNT][24];
    StarsU8 player[PLAYER_COUNT][192];
    StarsI16 values[10];
    StarsI16 checks[10];
    StarsMessageState messages;
    StarsMessageOps message_ops;
    StarsI16 records;
    StarsI16 dead_marks;
    StarsI16 packets;
    StarsI16 dirty_marks;
    StarsU16 packet_lengths[16];
    StarsU8 packet_data[16][42];
} PackedState;

static StarsU16 get_word(const StarsU8 *p, StarsU16 offset)
{
    return (StarsU16)(p[offset] | ((StarsU16)p[offset + 1U] << 8));
}

static StarsI32 get_long(const StarsU8 *p, StarsU16 offset)
{
    StarsU32 value = (StarsU32)get_word(p, offset) |
        ((StarsU32)get_word(p, (StarsU16)(offset + 2U)) << 16);
    return (StarsI32)value;
}

static void put_word(StarsU8 *p, StarsU16 offset, StarsU16 value)
{
    p[offset] = (StarsU8)value;
    p[offset + 1U] = (StarsU8)(value >> 8);
}

static void put_long(StarsU8 *p, StarsU16 offset, StarsI32 value)
{
    StarsU32 u = (StarsU32)value;
    put_word(p, offset, (StarsU16)u);
    put_word(p, (StarsU16)(offset + 2U), (StarsU16)(u >> 16));
}

/* _scorex.score begins at 4; these offsets are the recovered _score views:
 * lScore=0, cResources=4, cPlanet=8, cStarbase=10, rgcsh=12,
 * cTechLevels=18.  _player.fDead is the low bit at offset 84. */
static void seed_score(PackedState *state, StarsI16 player,
    StarsI32 total, StarsI16 planets, StarsI32 ship_value,
    StarsI16 tech_count, StarsI32 resources, StarsI16 dead)
{
    StarsU8 *score = state->scorex[player];
    memset(score, 0, 24U);
    memset(state->player[player], 0, 192U);
    put_word(score, 0, (StarsU16)player);
    put_long(score, 4, total);
    put_long(score, 8, resources);
    put_word(score, 12, (StarsU16)planets);
    put_word(score, 20, (StarsU16)ship_value);
    put_word(score, 22, (StarsU16)tech_count);
    put_word(state->player[player], 84, dead ? 1U : 0U);
}

static void load_score(PackedState *state, StarsI16 player,
    StarsUpdatePlayerScore *score)
{
    const StarsU8 *packed = state->scorex[player];
    StarsU16 axis;
    memset(score, 0, sizeof *score);
    score->total = get_long(packed, 4);
    score->resources = get_long(packed, 8);
    score->planets = (StarsI16)get_word(packed, 12);
    score->ship_value = (StarsI32)(StarsI16)get_word(packed, 20);
    score->tech_count = (StarsI16)get_word(packed, 22);
    for (axis = 0U; axis < 6U; ++axis)
        score->technology_levels[axis] =
            (StarsI16)(signed char)state->player[player][26U + axis];
    score->dead = (StarsI16)(get_word(state->player[player], 84) & 1U);
}

static StarsI16 calculate(void *context, StarsI16 player,
    StarsUpdatePlayerScore *score)
{
    load_score((PackedState *)context, player, score);
    return 1;
}

static StarsI16 victory_value(void *context, StarsI16 index)
{
    return ((PackedState *)context)->values[index];
}

static StarsI16 victory_check(void *context, StarsI16 index)
{
    return ((PackedState *)context)->checks[index];
}

static void record(void *context, StarsI16 player,
    const StarsUpdatePlayerScore *score)
{
    PackedState *state = (PackedState *)context;
    StarsU16 flags;
    StarsU16 word;

    ++state->records;
    put_long(state->scorex[player], 4, score->total);
    put_long(state->scorex[player], 8, score->resources);
    put_word(state->scorex[player], 12, (StarsU16)score->planets);
    put_word(state->scorex[player], 20, (StarsU16)score->ship_value);
    put_word(state->scorex[player], 22, (StarsU16)score->tech_count);
    put_word(state->scorex[player], 2, (StarsU16)score->rank);
    word = get_word(state->scorex[player], 0);
    word = (StarsU16)((word & 0x003fU) | 0x0020U);
    flags = score->achievement_flags;
    word = (StarsU16)(word | (StarsU16)((flags & 0x00ffU) << 6));
    if ((flags & 0x4000U) != 0U)
        word = (StarsU16)(word | 0x4000U);
    put_word(state->scorex[player], 0, word);
}

static void mark_dead(void *context, StarsI16 player)
{
    PackedState *state = (PackedState *)context;
    put_word(state->player[player], 84,
        (StarsU16)(get_word(state->player[player], 84) | 1U));
    ++state->dead_marks;
}

static StarsU16 parameter_count(void *context, StarsI16 message)
{
    (void)context;
    (void)message;
    return 1;
}

static StarsU16 player_flags(void *context, StarsI16 player)
{
    (void)context;
    (void)player;
    return 0;
}

static void prepend(void *context, const StarsU8 *packet, StarsU16 length)
{
    PackedState *state = (PackedState *)context;
    if (state->packets < 16) {
        memcpy(state->packet_data[state->packets], packet, length);
        state->packet_lengths[state->packets] = length;
    }
    ++state->packets;
}

static void send_message(void *context, StarsI16 player, StarsI16 message,
    StarsU16 mask)
{
#ifdef INTEGRATION_NO_MESSAGE
    (void)context;
    (void)player;
    (void)message;
    (void)mask;
#else
    PackedState *state = (PackedState *)context;
    StarsI16 params[7];
    StarsI16 i;

    for (i = 0; i < 7; ++i)
        params[i] = 0;
    params[0] = (StarsI16)mask;
    stars_send_player_message_prepend(&state->messages, player, message, -4,
        params, &state->message_ops, state);
#endif
}

static void dirty(void *context)
{
    ++((PackedState *)context)->dirty_marks;
}

int main(void)
{
    PackedState state;
    StarsUpdatePlayerScoreOps ops;
    StarsUpdatePlayerScoreInput input;
    StarsUpdatePlayerScore players[PLAYER_COUNT];
    StarsI16 i;
    int failures = 0;

    puts("UPDATE_PLAYER_SCORES_INTEGRATION_BEGIN");

    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(players, 0, sizeof players);
    seed_score(&state, 0, 1000L, 10, 100L, 4, 5000L, 0);
    seed_score(&state, 1, 800L, 5, 50L, 3, 1000L, 0);
    seed_score(&state, 2, 0L, 0, 0L, 0, 0L, 0);
    state.values[0] = 50; state.values[1] = 5; state.values[2] = 3;
    state.values[3] = 900; state.values[4] = 10; state.values[5] = 4;
    state.values[6] = 75; state.values[7] = 5; state.values[8] = 2;
    state.values[9] = 5;
    for (i = 0; i < 10; ++i)
        state.checks[i] = 1;
    memset(&state.message_ops, 0, sizeof state.message_ops);
    state.message_ops.parameter_count = parameter_count;
    state.message_ops.player_flags = player_flags;
    state.message_ops.prepend_message = prepend;
    ops.calculate = calculate;
    ops.victory_value = victory_value;
    ops.victory_check = victory_check;
    ops.record = record;
    ops.mark_dead = mark_dead;
    ops.send_message = send_message;
    ops.mark_dirty = dirty;
    ops.context = &state;
    input.player_count = PLAYER_COUNT;
    input.planet_count = 10;
    input.turn = 10;
    input.players = players;
    input.ops = &ops;
#ifndef INTEGRATION_NO_UPDATE
    stars_update_player_scores(&input);
#endif

    if (state.records != 3 || state.dead_marks != 1
#ifndef INTEGRATION_NO_MESSAGE
        || state.packets != 5
#endif
        )
        ++failures;
    if (get_long(state.scorex[0], 4) != 1000L ||
        (StarsI16)get_word(state.scorex[0], 12) != 10 ||
        (StarsI16)get_word(state.scorex[0], 20) != 100 ||
        (StarsI16)get_word(state.scorex[0], 22) != 4)
        ++failures;
    if ((get_word(state.scorex[0], 0) & 0x003fU) != 0x0020U ||
        (get_word(state.scorex[0], 0) & 0x4000U) == 0U ||
        get_word(state.scorex[0], 2) != 1U)
        ++failures;
    if ((get_word(state.player[2], 84) & 1U) == 0U ||
        get_word(state.scorex[1], 2) != 2U ||
        get_word(state.scorex[2], 2) != 3U)
        ++failures;
#ifndef INTEGRATION_NO_MESSAGE
    if (state.packet_lengths[0] < 6U || state.packet_lengths[4] < 6U)
        ++failures;
    if ((state.packet_data[0][1] & 0xffU) != 0xb8U &&
        (state.packet_data[0][1] & 0xffU) != 0xb5U &&
        (state.packet_data[0][1] & 0xffU) != 0xbbU)
        ++failures;
#endif
    if (failures == 0)
        puts("UPDATE_PLAYER_SCORES_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS");
    else
        puts("UPDATE_PLAYER_SCORES_INTEGRATION_CHECKS=6 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}
