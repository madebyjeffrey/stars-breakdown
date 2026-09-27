#include "core.h"

#define MOVE_GRID_LIMIT 9
#define MOVE_MAX_SCORE 30000000L
#define MOVE_SAME_SQUARE_BONUS 2L
#define MOVE_STAY_PENALTY 1L
#define MOVE_PREFERRED_UNSET 0xffU
#define MOVE_FLAG_MOVED 0x10U

static StarsI16 move_axis_distance(StarsI16 left, StarsI16 right)
{
    StarsI16 distance = left - right;
    return distance < 0 ? (StarsI16)-distance : distance;
}

static StarsU16 move_coordinate(StarsI16 x, StarsI16 y)
{
    return (StarsU16)(((StarsU16)y << 4) | (StarsU16)x);
}

/* Genuine semantic reconstruction of DxyMoveTokTo, NE 31:5F18-678F.
 * Non-mobile objects stay in place.  A mobile token evaluates every square
 * in its movement envelope using predicted battle damage, adds the original
 * same-square congestion adjustment, and chooses the lowest score with
 * distance as its secondary key.  Equal candidates are selected by the
 * original reservoir-style random tie breaker; after a distant destination
 * is chosen, only one legal step is applied this turn. */
StarsI16 stars_dxy_move_tok_to(StarsDxyMoveInput *input)
{
    StarsI16 x;
    StarsI16 y;
    StarsI16 dx;
    StarsI16 dy;
    StarsI16 step_x;
    StarsI16 step_y;
    StarsI16 range;
    StarsI16 tie_count = 0;
    StarsI16 preferred_valid;
    StarsI16 primary;
    StarsI16 best_distance = MOVE_GRID_LIMIT * 2;
    StarsI32 best_score = MOVE_MAX_SCORE;
    StarsI32 score;
    StarsU8 preferred = MOVE_PREFERRED_UNSET;
    StarsU8 best = 0;
    StarsU8 candidate;
    StarsU8 current;

    if (!input || !input->token) return 0;
    input->token->movement_flags |= MOVE_FLAG_MOVED;
    current = input->token->battle_coordinate;
    if (input->token->object_kind == 1U || input->speed == 0) return 1;
    primary = input->move_range ? input->move_range(input->context,
        input->attack_mask, &preferred) : 0;
    if (primary < 0) primary = 0;
    range = primary;
    x = (StarsI16)(current & 0x0fU);
    y = (StarsI16)((current >> 4) & 0x0fU);
    for (dx = x - range; dx <= x + range; ++dx) {
        if (dx < 0 || dx > MOVE_GRID_LIMIT) continue;
        for (dy = y - range; dy <= y + range; ++dy) {
            if (dy < 0 || dy > MOVE_GRID_LIMIT) continue;
            candidate = (StarsU8)move_coordinate(dx, dy);
            score = input->score_damage ? input->score_damage(input->context,
                candidate, primary != 0, input->attack_mask) : 0;
            if (input->same_square)
                score += (StarsI32)input->same_square(input->context,
                    candidate, input->token->object_kind) * MOVE_SAME_SQUARE_BONUS;
            if (candidate == current) score -= MOVE_STAY_PENALTY;
            if (score < best_score || (score == best_score &&
                    move_axis_distance(dx, x) + move_axis_distance(dy, y) <
                    best_distance)) {
                best_score = score;
                best_distance = move_axis_distance(dx, x) +
                    move_axis_distance(dy, y);
                best = candidate;
                tie_count = 1;
            } else if (score == best_score &&
                    move_axis_distance(dx, x) + move_axis_distance(dy, y) ==
                    best_distance) {
                ++tie_count;
                if (input->random && input->random(input->context,
                        tie_count) == 0) best = candidate;
            }
        }
    }
    preferred_valid = preferred != MOVE_PREFERRED_UNSET &&
        (preferred & 0x0fU) <= MOVE_GRID_LIMIT &&
        ((preferred >> 4) & 0x0fU) <= MOVE_GRID_LIMIT;
    if (preferred_valid) best = preferred;
    dx = (StarsI16)(best & 0x0fU) - x;
    dy = (StarsI16)((best >> 4) & 0x0fU) - y;
    if (move_axis_distance(dx, 0) > 1 || move_axis_distance(dy, 0) > 1) {
        step_x = dx < 0 ? -1 : (dx > 0 ? 1 : 0);
        step_y = dy < 0 ? -1 : (dy > 0 ? 1 : 0);
        if (step_x && step_y && input->score_damage) {
            StarsU8 x_step = (StarsU8)move_coordinate(x + step_x, y);
            StarsU8 y_step = (StarsU8)move_coordinate(x, y + step_y);
            if (input->score_damage(input->context, y_step, primary != 0,
                    input->attack_mask) < input->score_damage(input->context,
                    x_step, primary != 0, input->attack_mask)) step_x = 0;
            else step_y = 0;
        }
        x += step_x;
        y += step_y;
        best = (StarsU8)move_coordinate(x, y);
    }
    input->token->battle_coordinate = best;
    return 1;
}
