#include "core.h"

static StarsI32 miner_distance_squared(StarsPoint first, StarsPoint second)
{
    StarsI32 dx = (StarsI32)first.x - (StarsI32)second.x;
    StarsI32 dy = (StarsI32)first.y - (StarsI32)second.y;
    return dx * dx + dy * dy;
}

static StarsU16 miner_score(const StarsRetargetMinerCandidate *candidate)
{
    return (StarsU16)(candidate->weight_a * 8U +
        candidate->weight_b * 10U + candidate->weight_c * 7U);
}

/* Genuine C reconstruction of FRetargetMiner, NE 15:336C-3523. */
StarsI16 stars_f_retarget_miner(const StarsRetargetMinerInput *input,
    StarsRetargetMinerMove move_fleet, void *context)
{
    const StarsRetargetMinerCandidate *best;
    StarsU16 best_score;
    StarsU16 current_score;
    StarsU16 score;
    StarsU16 index;
    StarsMoveOrder order;

    if (!input || (!input->candidates && input->candidate_count != 0U))
        return 0;
    best = (const StarsRetargetMinerCandidate *)0;
    best_score = 0;
    current_score = 0;
    for (index = 0; index < input->candidate_count; ++index) {
        const StarsRetargetMinerCandidate *candidate =
            &input->candidates[index];
        score = miner_score(candidate);
        if (miner_distance_squared(input->fleet_point, candidate->point) <
            0x1440L) {
            if (candidate->planet_id == input->current_planet)
                current_score = score;
            if (score > best_score) {
                best = candidate;
                best_score = score;
            }
        }
    }
    if (!best || (StarsI32)best_score <=
        ((StarsI32)current_score * 6L) / 5L)
        return 0;
    order.point = best->point;
    order.id = best->planet_id;
    order.flags = 0x1160U;
    if (!move_fleet)
        return 0;
    return move_fleet(context, &input->fleet_point, &order, 0);
}
