#include "core.h"

#define SCORE_GUESS_ACTIVE 1
#define SCORE_GUESS_INITIAL_BEST 10
#define SCORE_GUESS_SCORE_INF 30000000L

static StarsI16 score_guess_distance(StarsU8 first, StarsU8 second)
{
    StarsI16 dx;
    StarsI16 dy;
    dx = (StarsI16)(first & 0x0fU) - (StarsI16)(second & 0x0fU);
    dy = (StarsI16)(first >> 4) - (StarsI16)(second >> 4);
    if (dx < 0) dx = (StarsI16)-dx;
    if (dy < 0) dy = (StarsI16)-dy;
    return dx > dy ? dx : dy;
}

static StarsU8 score_guess_clamp_coordinate(StarsI16 coordinate)
{
    if (coordinate < 0)
        return 0;
    if (coordinate > 9)
        return 9;
    return (StarsU8)coordinate;
}

/* NE 31:598C-5E33. Estimate the battle score for a source token at a
 * candidate coordinate.  The engine checks every active hostile token, uses
 * the worst tactic score for that target, keeps the largest incoming damage
 * and total outgoing damage, and finally scores those aggregates with the
 * source's tactic. */
StarsI32 stars_score_guess_battle_damage(const StarsScoreGuessInput *input,
    StarsU8 candidate_coordinate)
{
    StarsU16 i;
    StarsI16 target_class;
    StarsI16 distance;
    StarsI16 maximum_distance;
    StarsI16 offset_x;
    StarsI16 offset_y;
    StarsI16 score;
    StarsI16 target_tactic;
    StarsU8 attack_coordinate;
    StarsU8 nearby_coordinate;
    StarsI32 incoming;
    StarsI32 outgoing;
    StarsI32 maximum_incoming;
    StarsI32 total_outgoing;
    StarsI32 best_score;
    StarsI32 candidate_score;
    StarsI32 best_incoming;
    StarsI32 best_outgoing;
    StarsMdTargetToken md_target;
    const StarsScoreGuessToken *source;
    const StarsScoreGuessToken *target;

    if (!input || !input->source || !input->targets || !input->damage)
        return 0L;
    source = input->source;
    target_class = input->primary_attack ?
        (StarsI16)(source->md_descriptor & 0x000fU) :
        (StarsI16)((source->md_descriptor >> 4) & 0x000fU);
    maximum_incoming = 0L;
    total_outgoing = 0L;
    for (i = 0; i < input->target_count; ++i) {
        target = &input->targets[i];
        if ((target->active_flags & SCORE_GUESS_ACTIVE) == 0U ||
                target->owner == source->owner || target->owner < 0 ||
                target->owner >= 16 ||
                (input->attack_players & (StarsU16)(1U << target->owner)) == 0U)
            continue;
        distance = score_guess_distance(target->battle_coordinate,
            candidate_coordinate);
        maximum_distance = distance;
        if (source->initiative_phase <= target->initiative_phase) {
            distance = (StarsI16)(distance - 1);
            if (distance < 0)
                distance = 0;
            for (offset_x = 0; offset_x < 2; ++offset_x) {
                for (offset_y = 0; offset_y < 2; ++offset_y) {
                    nearby_coordinate = (StarsU8)(
                        (score_guess_clamp_coordinate(
                            (StarsI16)(target->battle_coordinate & 0x0fU) -
                            1 + offset_x)) |
                        (StarsU8)(score_guess_clamp_coordinate(
                            (StarsI16)(target->battle_coordinate >> 4) -
                            1 + offset_y) << 4));
                    score = score_guess_distance(candidate_coordinate,
                        nearby_coordinate);
                    if (score > maximum_distance)
                        maximum_distance = score;
                }
            }
        }
        md_target.grobj = target->md_grobj;
        md_target.overlap_23 = target->md_descriptor;
        if (!stars_is_target_of_md_target(&md_target, target_class))
            continue;
        best_score = SCORE_GUESS_SCORE_INF;
        best_incoming = 0L;
        best_outgoing = 0L;
        for (; distance <= maximum_distance; ++distance) {
            attack_coordinate = (StarsU8)((distance & 0x0f) << 4);
            incoming = stars_is_target_of_md_target(&md_target, target_class) ?
                input->damage(input->context, source, target,
                    attack_coordinate, 0) : 0L;
            outgoing = input->damage(input->context, target, source,
                attack_coordinate, 1);
            target_tactic = (StarsI16)((target->md_descriptor >> 8) & 0x0fU);
            candidate_score = stars_score_from_give_take_tactic(
                outgoing, incoming, target_tactic);
            if (candidate_score <= best_score) {
                best_score = candidate_score;
                best_incoming = incoming;
                best_outgoing = outgoing;
            }
        }
        if (best_incoming > maximum_incoming)
            maximum_incoming = best_incoming;
        total_outgoing += best_outgoing;
    }
    target_tactic = (StarsI16)((source->md_descriptor >> 8) & 0x0fU);
    return stars_score_from_give_take_tactic(maximum_incoming, total_outgoing,
        target_tactic);
}
