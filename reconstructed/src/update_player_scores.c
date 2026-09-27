#include "core.h"

/* Typed semantic reconstruction of UpdatePlayerScores, NE 18:6258-6dca.
 * Score records, player records, victory settings, and prepended-message
 * storage are represented through callbacks; the metric tests, rank ordering,
 * dead-player detection, achievement masks, and message selection remain here. */

static StarsI16 score_value(const StarsUpdatePlayerScoreInput *input,
    StarsI16 index)
{
    if (!input->ops->victory_value)
        return 0;
    return input->ops->victory_value(input->ops->context, index);
}

static void score_achievement(StarsUpdatePlayerScore *score,
    StarsU16 flag, StarsI16 check)
{
    score->achievement_flags = (StarsU16)(score->achievement_flags | flag);
    if (check)
        ++score->achievement_count;
}

static StarsI16 score_check(const StarsUpdatePlayerScoreInput *input,
    StarsI16 index)
{
    if (!input->ops->victory_check)
        return 0;
    return input->ops->victory_check(input->ops->context, index) != 0;
}

void stars_update_player_scores(StarsUpdatePlayerScoreInput *input)
{
    StarsUpdatePlayerScore scores[16];
    StarsI16 count;
    StarsI16 i;
    StarsI16 j;
    StarsI16 dead_count;
    StarsI16 winner;
    StarsI16 winner_candidate;
    StarsI16 winner_count;
    StarsI16 runner_up;
    StarsI16 mask;
    StarsI16 bit;
    StarsI16 threshold;
    StarsI16 needed;
    StarsI16 technology_categories;
    StarsI32 adjusted_runner_up;
    StarsI32 adjusted_winner;

    if (!input || !input->ops || !input->players || !input->ops->calculate)
        return;
    count = input->player_count;
    if (count < 0)
        count = 0;
    if (count > 16)
        count = 16;
    dead_count = 0;
    winner = -1;
    winner_candidate = -1;
    winner_count = 0;
    runner_up = -1;
    for (i = 0; i < count; ++i) {
        scores[i] = input->players[i];
        scores[i].achievement_flags = 0;
        scores[i].achievement_count = 0;
        scores[i].rank = 0;
        if (!input->ops->calculate(input->ops->context, i, &scores[i]))
            continue;
        if (scores[i].planets == 0 && scores[i].ship_value == 0 &&
                !scores[i].dead) {
            scores[i].dead = 1;
            if (input->ops->mark_dead)
                input->ops->mark_dead(input->ops->context, i);
            if (input->ops->send_message) {
                for (j = 0; j < count; ++j)
                    if (j != i)
                        input->ops->send_message(input->ops->context, j,
                            0xbb, (StarsU16)(i | 0x30));
            }
        }
        if (scores[i].dead)
            ++dead_count;
        threshold = score_value(input, 0);
        needed = (StarsI16)((input->planet_count * threshold) / 100);
        if (needed <= scores[i].planets) {
            score_achievement(&scores[i], 0x0040U, score_check(input, 0));
        }
        if (scores[i].ship_value >= score_value(input, 6))
            score_achievement(&scores[i], 0x0800U, score_check(input, 6));
        if (scores[i].total >= score_value(input, 3))
            score_achievement(&scores[i], 0x0100U, score_check(input, 3));
        /* Victory checks count qualifying categories, not summed tech levels. */
        technology_categories = 0;
        for (j = 0; j < 6; ++j)
            if (scores[i].technology_levels[j] >= score_value(input, 1))
                ++technology_categories;
        if (technology_categories >= score_value(input, 2))
            score_achievement(&scores[i], 0x0080U, score_check(input, 1));
        if (scores[i].resources / 1000 >= score_value(input, 5))
            score_achievement(&scores[i], 0x0400U, score_check(input, 5));
    }
    for (i = 0; i < count; ++i) {
        scores[i].rank = 1;
        for (j = 0; j < count; ++j)
            if (scores[j].total > scores[i].total)
                ++scores[i].rank;
        if (scores[i].rank == 1) {
            winner_candidate = i;
            winner = i;
            ++winner_count;
        } else if (scores[i].rank == 2) {
            runner_up = i;
        }
    }
    if (count == 1) {
        scores[0].rank = 1;
    } else {
        if (winner_count > 1)
            winner = -1;
        if (winner >= 0 && input->turn >= score_value(input, 7))
            score_achievement(&scores[winner], 0x1000U,
                score_check(input, 7));
        if (dead_count + 1 < count) {
            if (winner >= 0 && runner_up >= 0) {
                adjusted_runner_up = scores[runner_up].total *
                    (score_value(input, 4) + 100);
                adjusted_winner = scores[winner].total * 100;
                if (adjusted_runner_up <= adjusted_winner)
                    score_achievement(&scores[winner], 0x0200U,
                        score_check(input, 4));
            }
            if (input->turn >= score_value(input, 9) &&
                    score_value(input, 8) > 0) {
                mask = 0;
                bit = 1;
                for (i = (StarsI16)(count - 1); i >= 0; --i) {
                    if (scores[i].achievement_count >= score_value(input, 8)) {
                        scores[i].achievement_flags = (StarsU16)(
                            scores[i].achievement_flags | 0x4000U);
                        mask = (StarsI16)(mask | bit);
                    }
                    bit = (StarsI16)(bit << 1);
                }
                if (mask != 0) {
                    if (input->ops->mark_dirty)
                        input->ops->mark_dirty(input->ops->context);
                    if (input->ops->send_message) {
                        bit = (StarsI16)(1 << (count - 1));
                        for (i = 0; i < count; ++i) {
                            if (scores[i].dead)
                                threshold = 0xb8;
                            else if ((bit & mask) == 0)
                                threshold = 0xb5;
                            else if (bit == mask)
                                threshold = 0xb6;
                            else
                                threshold = 0xb7;
                            input->ops->send_message(input->ops->context, i,
                                threshold, (StarsU16)((bit == mask ||
                                (bit & mask) == 0) ? mask : mask & ~bit));
                            bit = (StarsI16)(bit >> 1);
                        }
                    }
                }
            }
        } else {
            if (input->ops->mark_dirty)
                input->ops->mark_dirty(input->ops->context);
            if (input->ops->send_message) {
                if (winner_candidate >= 0 &&
                        !scores[winner_candidate].dead)
                    input->ops->send_message(input->ops->context,
                        winner_candidate, 0xbc, 0);
                for (i = 0; i < count; ++i)
                    if (i != winner_candidate)
                        input->ops->send_message(input->ops->context,
                            i, 0xb8, 0);
            }
        }
    }
    for (i = 0; i < count; ++i) {
        input->players[i] = scores[i];
        if (input->ops->record)
            input->ops->record(input->ops->context, i, &scores[i]);
    }
}
