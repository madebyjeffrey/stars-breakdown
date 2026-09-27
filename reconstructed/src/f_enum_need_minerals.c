#include "core.h"

enum {
    NEED_MINERAL_COUNT = 3,
    NEED_MINERAL_THRESHOLD = 0x02bd,
    NEED_PLAYER_STATUS_BIT = 9,
    NEED_AI_KNOWN_SHIFT = 5,
    NEED_AI_KNOWN_MASK = 3
};
static const StarsU16 need_mineral_flags[NEED_MINERAL_COUNT] = { 8, 9, 10 };

/* NE 16:3F5E-4204. Select a same-player planet that is both eligible and
 * known to contain at least one useful mineral, then accept it when it is
 * within the lesser of the two warp ranges.  The original stores minerals as
 * three 32-bit values and tests the corresponding knowledge bits in the AI
 * planet tables; the typed adapter exposes those records directly. */
StarsI16 stars_f_enum_need_minerals(const StarsNeedMineralsInput *input)
{
    StarsI16 mineral;
    StarsI16 source_warp;
    StarsI16 test_warp;
    StarsI16 source_gate;
    StarsI16 test_gate;
    StarsI16 limiting_warp;
    StarsPoint source_point;
    StarsPoint test_point;
    double warp_distance;
    StarsI32 distance_squared;

    if (!input || !input->source || !input->test ||
            !input->source_ai_flags || !input->test_ai_flags ||
            !input->planet_points || !input->warp ||
            !input->distance_squared)
        return 0;
    if (input->test->owner != input->player ||
            (input->test->status_flags & (1U << NEED_PLAYER_STATUS_BIT)) == 0U ||
            (((input->source_ai_flags[input->test->id] >>
            NEED_AI_KNOWN_SHIFT) & NEED_AI_KNOWN_MASK) != 0U))
        return 0;
    if (input->source->id >= input->planet_count ||
            input->test->id >= input->planet_count)
        return 0;
    for (mineral = 0; mineral < NEED_MINERAL_COUNT; ++mineral) {
            StarsU16 known = (StarsU16)(input->test_ai_flags[input->test->id] >>
            need_mineral_flags[mineral]);
        if (input->source->minerals[mineral] >= NEED_MINERAL_THRESHOLD &&
                (known & 1U) != 0U)
            break;
    }
    if (mineral == NEED_MINERAL_COUNT)
        return 0;
    source_warp = input->warp(input->context, input->source, &source_gate);
    if (source_gate != 0) ++source_warp;
    test_warp = input->warp(input->context, input->test, &test_gate);
    if (test_gate != 0) ++test_warp;
    limiting_warp = source_warp < test_warp ? source_warp : test_warp;
    warp_distance = input->warp_scale * (double)limiting_warp *
        (double)limiting_warp;
    warp_distance *= warp_distance;
    source_point = input->planet_points[input->source->id];
    test_point = input->planet_points[input->test->id];
    distance_squared = input->distance_squared(input->context,
        source_point, test_point);
    return (double)distance_squared <= warp_distance;
}
