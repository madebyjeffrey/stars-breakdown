#include "core.h"
#include <stdio.h>
#include <string.h>

/* Reuse the packed adapter, callbacks, and record layout from E1.  The
 * independent oracle below deliberately does not call the reconstruction. */
#ifdef INTEGRATION_NATIVE
#define main update_player_scores_e1_main
#include "BASE.C"
#undef main
#else
#define main update_player_scores_e1_main
#include "update_player_scores_integration_probe.c"
#undef main
#endif

static void oracle_achievement(StarsUpdatePlayerScore *score,
    StarsU16 flag, StarsI16 check)
{
    score->achievement_flags = (StarsU16)(score->achievement_flags | flag);
    if (check)
        ++score->achievement_count;
}

static void oracle_update(StarsUpdatePlayerScore *scores, StarsI16 count,
    StarsI16 planet_count, StarsI16 turn, const StarsI16 *values,
    const StarsI16 *checks, StarsI16 *dead_marks, StarsI16 *messages,
    StarsI16 *dirty_state)
{
    StarsI16 i;
    StarsI16 j;
    StarsI16 dead_count = 0;
    StarsI16 winner = -1;
    StarsI16 winner_candidate = -1;
    StarsI16 winner_count = 0;
    StarsI16 runner_up = -1;
    StarsI16 rank;
    StarsI16 threshold;
    StarsI16 needed;
    StarsI16 mask;
    StarsI16 bit;
    StarsI32 adjusted_runner_up;
    StarsI32 adjusted_winner;

    *dead_marks = 0;
    *messages = 0;
    *dirty_state = 0;
    for (i = 0; i < count; ++i) {
        scores[i].achievement_flags = 0;
        scores[i].achievement_count = 0;
        scores[i].rank = 0;
        if (scores[i].planets == 0 && scores[i].ship_value == 0 &&
                !scores[i].dead) {
            scores[i].dead = 1;
            ++*dead_marks;
            *messages = (StarsI16)(*messages + count - 1);
        }
        if (scores[i].dead)
            ++dead_count;
        threshold = values[0];
        needed = (StarsI16)((planet_count * threshold) / 100);
        if (needed <= scores[i].planets)
            oracle_achievement(&scores[i], 0x0040U, checks[0]);
        if (scores[i].ship_value >= values[6])
            oracle_achievement(&scores[i], 0x0800U, checks[6]);
        if (scores[i].total >= values[3])
            oracle_achievement(&scores[i], 0x0100U, checks[3]);
        needed = 0;
        for (j = 0; j < 6; ++j)
            if (scores[i].technology_levels[j] >= values[1]) ++needed;
        if (needed >= values[2])
            oracle_achievement(&scores[i], 0x0080U, checks[1]);
        if (scores[i].resources / 1000 >= values[5])
            oracle_achievement(&scores[i], 0x0400U, checks[5]);
    }
    for (i = 0; i < count; ++i) {
        rank = 1;
        for (j = 0; j < count; ++j)
            if (scores[j].total > scores[i].total)
                ++rank;
        scores[i].rank = rank;
        if (rank == 1) {
            winner_candidate = i;
            winner = i;
            ++winner_count;
        } else if (rank == 2) {
            runner_up = i;
        }
    }
    if (count == 1)
        scores[0].rank = 1;
    else {
        if (winner_count > 1)
            winner = -1;
        if (winner >= 0 && turn >= values[7])
            oracle_achievement(&scores[winner], 0x1000U, checks[7]);
        if (dead_count + 1 < count) {
            if (winner >= 0 && runner_up >= 0) {
                adjusted_runner_up = scores[runner_up].total *
                    (values[4] + 100);
                adjusted_winner = scores[winner].total * 100;
                if (adjusted_runner_up <= adjusted_winner)
                    oracle_achievement(&scores[winner], 0x0200U,
                        checks[4]);
            }
            if (turn >= values[9] && values[8] > 0) {
                mask = 0;
                bit = 1;
                for (i = (StarsI16)(count - 1); i >= 0; --i) {
                    if (scores[i].achievement_count >= values[8]) {
                        scores[i].achievement_flags = (StarsU16)(
                            scores[i].achievement_flags | 0x4000U);
                        mask = (StarsI16)(mask | bit);
                    }
                    bit = (StarsI16)(bit << 1);
                }
                if (mask != 0) {
                    *dirty_state = 1;
                    *messages = (StarsI16)(*messages + count);
                }
            }
        } else {
            *dirty_state = 1;
            if (winner_candidate >= 0 && !scores[winner_candidate].dead)
                ++*messages;
            *messages = (StarsI16)(*messages + count - 1);
        }
    }
}

static StarsI32 report_checksum(const PackedState *state, StarsI16 count)
{
    StarsI16 i;
    StarsU16 word;
    StarsU16 flags;
    StarsI32 sum = 0;

    for (i = 0; i < count; ++i) {
        word = get_word(state->scorex[i], 0);
        flags = (StarsU16)((word >> 6) & 0x00ffU);
        if ((word & 0x4000U) != 0U)
            flags = (StarsU16)(flags | 0x4000U);
        sum += get_long(state->scorex[i], 4);
        sum += get_long(state->scorex[i], 8);
        sum += (StarsI32)((StarsI16)get_word(state->scorex[i], 12) * 3);
        sum += (StarsI32)((StarsI16)get_word(state->scorex[i], 20) * 5);
        sum += (StarsI32)((StarsI16)get_word(state->scorex[i], 22) * 7);
        sum += (StarsI32)((StarsI16)get_word(state->scorex[i], 2) * 11);
        sum += (StarsI32)(flags * 13U);
        sum += (StarsI32)((get_word(state->player[i], 84) & 1U) * 17U);
    }
    return sum;
}

static StarsI32 expected_report_checksum(const StarsUpdatePlayerScore *scores,
    StarsI16 count)
{
    StarsI16 i;
    StarsU16 flags;
    StarsI32 sum = 0;

    for (i = 0; i < count; ++i) {
        flags = (StarsU16)(scores[i].achievement_flags & 0x00ffU);
        if ((scores[i].achievement_flags & 0x4000U) != 0U)
            flags = (StarsU16)(flags | 0x4000U);
        sum += scores[i].total;
        sum += scores[i].resources;
        sum += (StarsI32)(scores[i].planets * 3);
        sum += scores[i].ship_value * 5;
        sum += (StarsI32)(scores[i].tech_count * 7);
        sum += (StarsI32)(scores[i].rank * 11);
        sum += (StarsI32)(flags * 13U);
        sum += (StarsI32)(scores[i].dead * 17);
    }
    return sum;
}

static void seed_case(PackedState *state, StarsUpdatePlayerScore *players,
    StarsI16 count, const StarsI32 *totals, const StarsI16 *planets,
    const StarsI32 *ships, const StarsI16 *tech, const StarsI32 *resources,
    const StarsI16 *dead, const StarsI16 *values, const StarsI16 *checks)
{
    StarsI16 i;

    memset(state, 0, sizeof *state);
    memset(players, 0, sizeof(StarsUpdatePlayerScore) * PLAYER_COUNT);
    for (i = 0; i < count; ++i)
        seed_score(state, i, totals[i], planets[i], ships[i], tech[i],
            resources[i], dead[i]);
    for (i = 0; i < 10; ++i) {
        state->values[i] = values[i];
        state->checks[i] = checks[i];
    }
    memset(&state->message_ops, 0, sizeof state->message_ops);
    state->message_ops.parameter_count = parameter_count;
    state->message_ops.player_flags = player_flags;
    state->message_ops.prepend_message = prepend;
}

static void setup_ops(PackedState *state, StarsUpdatePlayerScoreOps *ops,
    StarsUpdatePlayerScoreInput *input, StarsUpdatePlayerScore *players,
    StarsI16 count, StarsI16 planets, StarsI16 turn)
{
    memset(ops, 0, sizeof *ops);
    memset(input, 0, sizeof *input);
    ops->calculate = calculate;
    ops->victory_value = victory_value;
    ops->victory_check = victory_check;
    ops->record = record;
    ops->mark_dead = mark_dead;
    ops->send_message = send_message;
    ops->mark_dirty = dirty;
    ops->context = state;
    input->player_count = count;
    input->planet_count = planets;
    input->turn = turn;
    input->players = players;
    input->ops = ops;
}

static int run_case(PackedState *state, StarsUpdatePlayerScore *players,
    StarsI16 count, StarsI16 planet_count, StarsI16 turn,
    const StarsI32 *totals, const StarsI16 *planets,
    const StarsI32 *ships, const StarsI16 *tech, const StarsI32 *resources,
    const StarsI16 *dead, const StarsI16 *values, const StarsI16 *checks)
{
    StarsUpdatePlayerScore expected[PLAYER_COUNT];
    StarsUpdatePlayerScoreOps ops;
    StarsUpdatePlayerScoreInput input;
    StarsI16 expected_dead_marks;
    StarsI16 expected_messages;
    StarsI16 expected_dirty;
    StarsI16 i;
    StarsI16 actual_dead_marks;
    int failures = 0;

    seed_case(state, players, count, totals, planets, ships, tech, resources,
        dead, values, checks);
    for (i = 0; i < count; ++i)
        load_score(state, i, &expected[i]);
    oracle_update(expected, count, planet_count, turn, values, checks,
        &expected_dead_marks, &expected_messages, &expected_dirty);
    setup_ops(state, &ops, &input, players, count, planet_count, turn);
    stars_update_player_scores(&input);

    actual_dead_marks = state->dead_marks;
    if (actual_dead_marks != expected_dead_marks ||
            state->packets != expected_messages ||
            state->dirty_marks != expected_dirty)
        ++failures;
    for (i = 0; i < count; ++i) {
        if (memcmp(&players[i], &expected[i], sizeof expected[i]) != 0)
            ++failures;
        if (get_long(state->scorex[i], 4) != expected[i].total ||
                get_long(state->scorex[i], 8) != expected[i].resources ||
                (StarsI16)get_word(state->scorex[i], 12) != expected[i].planets ||
                (StarsI16)get_word(state->scorex[i], 20) != expected[i].ship_value ||
                (StarsI16)get_word(state->scorex[i], 22) != expected[i].tech_count ||
                (StarsI16)get_word(state->scorex[i], 2) != expected[i].rank ||
                (StarsI16)(get_word(state->player[i], 84) & 1U) != expected[i].dead)
            ++failures;
    }
    if (report_checksum(state, count) !=
            expected_report_checksum(expected, count))
        ++failures;
    return failures;
}

#ifdef INTEGRATION_NATIVE
#define E2_MAIN update_player_scores_e2_probe
#else
#define E2_MAIN main
#endif

int E2_MAIN(void)
{
    PackedState state;
    StarsUpdatePlayerScore players[PLAYER_COUNT];
    StarsI32 totals[PLAYER_COUNT];
    StarsI16 planets[PLAYER_COUNT];
    StarsI32 ships[PLAYER_COUNT];
    StarsI16 tech[PLAYER_COUNT];
    StarsI32 resources[PLAYER_COUNT];
    StarsI16 dead[PLAYER_COUNT];
    StarsI16 values[10];
    StarsI16 checks[10];
    StarsI16 i;
    int failures = 0;

    puts("UPDATE_PLAYER_SCORES_E2_BEGIN");
    for (i = 0; i < 10; ++i) {
        values[i] = 0;
        checks[i] = 1;
    }
    values[0] = 50; values[1] = 5; values[3] = 900;
    values[4] = 10; values[5] = 4; values[6] = 75;
    values[7] = 5; values[8] = 2; values[9] = 5;

    totals[0] = 1000L; totals[1] = 800L; totals[2] = 0L;
    planets[0] = 10; planets[1] = 5; planets[2] = 0;
    ships[0] = 100L; ships[1] = 50L; ships[2] = 0L;
    tech[0] = 4; tech[1] = 3; tech[2] = 0;
    resources[0] = 5000L; resources[1] = 1000L; resources[2] = 0L;
    dead[0] = 0; dead[1] = 0; dead[2] = 0;
    failures += run_case(&state, players, 3, 10, 10, totals, planets, ships,
        tech, resources, dead, values, checks);

    totals[0] = 900L; totals[1] = 800L; totals[2] = 700L;
    planets[0] = 5; planets[1] = 4; planets[2] = 3;
    ships[0] = 80L; ships[1] = 60L; ships[2] = 40L;
    tech[0] = 4; tech[1] = 3; tech[2] = 2;
    resources[0] = 4000L; resources[1] = 3000L; resources[2] = 2000L;
    dead[0] = 0; dead[1] = 0; dead[2] = 0;
    values[8] = 99;
    failures += run_case(&state, players, 3, 10, 2, totals, planets, ships,
        tech, resources, dead, values, checks);

    totals[0] = 1000L; totals[1] = 1000L; totals[2] = 500L;
    failures += run_case(&state, players, 3, 10, 2, totals, planets, ships,
        tech, resources, dead, values, checks);

    totals[0] = 1000L; totals[1] = 100L; totals[2] = 0L;
    planets[0] = 5; planets[1] = 0; planets[2] = 0;
    ships[0] = 100L; ships[1] = 0L; ships[2] = 0L;
    tech[0] = 4; tech[1] = 0; tech[2] = 0;
    resources[0] = 4000L; resources[1] = 0L; resources[2] = 0L;
    dead[0] = 0; dead[1] = 1; dead[2] = 0;
    failures += run_case(&state, players, 3, 10, 2, totals, planets, ships,
        tech, resources, dead, values, checks);

    totals[0] = 500L; planets[0] = 2; ships[0] = 30L; tech[0] = 2;
    resources[0] = 2000L; dead[0] = 0;
    failures += run_case(&state, players, 1, 10, 10, totals, planets, ships,
        tech, resources, dead, values, checks);

    totals[0] = 900L; totals[1] = 800L; totals[2] = 700L;
    planets[0] = 10; planets[1] = 10; planets[2] = 1;
    ships[0] = 100L; ships[1] = 100L; ships[2] = 10L;
    tech[0] = 5; tech[1] = 5; tech[2] = 0;
    resources[0] = 5000L; resources[1] = 5000L; resources[2] = 1000L;
    dead[0] = 0; dead[1] = 0; dead[2] = 0;
    values[8] = 2;
    failures += run_case(&state, players, 3, 10, 10, totals, planets, ships,
        tech, resources, dead, values, checks);

    if (failures == 0)
        puts("UPDATE_PLAYER_SCORES_E2_CHECKS=36 FAILURES=0 RESULT=PASS");
    else
        puts("UPDATE_PLAYER_SCORES_E2_CHECKS=36 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}
