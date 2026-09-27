#include "core.h"
#include <math.h>

enum {
    PKT_STARBASE_FLAG = 9,
    PKT_AI_STATUS_SHIFT = 5,
    PKT_AI_STATUS_MASK = 3,
    PKT_STARBASE_HULL_CLASS = 7,
    PKT_MAX_AGE_NIBBLE = 99,
    PKT_MIN_STRENGTH = 1,
    PKT_PACKET_AGE_BIAS = 5,
    PKT_STRENGTH_BIAS = 25,
    PKT_SCORE_FLOOR = 1000
};

static StarsI16 packet_is_starbase(const StarsPacketAttackPlanet *planet)
{
    return (planet->status_flags & (1U << PKT_STARBASE_FLAG)) != 0U;
}

static StarsI16 packet_ai_status(const StarsFEnumPktAttackInput *input,
    const StarsPacketAttackPlanet *planet)
{
    if (!input->ai_planet_flags || planet->id >= input->planet_count)
        return 0;
    return (StarsI16)((input->ai_planet_flags[planet->id] >>
        PKT_AI_STATUS_SHIFT) & PKT_AI_STATUS_MASK);
}

/* NE 16:4204-45A6.  FEnumPktAttack is an enumerator predicate, not an
 * attacker selector: it answers whether one packet target is eligible for a
 * source planet.  The original's far pointers into global planet/AI tables
 * become typed records and callbacks here; the four floating-point values
 * are the recovered global tuning constants at 1120:1E2E, 1E36, 1E3E, and
 * 1E5E. */
StarsI16 stars_f_enum_pkt_attack(const StarsFEnumPktAttackInput *input)
{
    StarsI16 source_gate;
    StarsI16 target_gate;
    StarsI16 source_warp;
    StarsI16 target_warp;
    StarsI16 age_nibble;
    StarsI16 strength;
    StarsI32 weighted_distance;
    StarsI32 range_score;
    StarsI32 score_limit;
    double route_distance;
    double distance;
    double ratio;
    double exponent;
    double final_score;

    if (!input || !input->source || !input->test || !input->warp ||
            !input->distance || !input->ai_planet_flags ||
            input->source->id >= input->planet_count ||
            input->test->id >= input->planet_count)
        return 0;

    /* Friendly, unowned, already-known, and race-stat-8 planets are not
     * packet targets.  The two AI status bits are the original knowledge
     * filter stored in vlpbAiData. */
    if (input->test->owner == input->player || input->test->owner == -1 ||
            packet_ai_status(input, input->test) != 0 ||
            (input->race_stat && input->race_stat(input->context,
             input->test->owner) == 8))
        return 0;

    /* A starbase is eligible only when its decoded hull class is the packet
     * target class.  Its warp gate, when present, adds one to its effective
     * range exactly as in the original IWarpMAFromLppl call. */
    target_warp = 0;
    if (packet_is_starbase(input->test)) {
        if (input->test->starbase_hull_class != PKT_STARBASE_HULL_CLASS)
            return 0;
        target_warp = input->warp(input->context, input->test,
            &target_gate);
        if (target_gate != 0)
            ++target_warp;
    }

    source_warp = input->warp(input->context, input->source, &source_gate);
    if ((StarsI16)(source_warp + 3) == target_warp)
        return 0;

    /* The first floating-point gate limits candidates to the source's
     * effective warp radius.  DGetDistance returns Euclidean distance, while
     * the original tuning constant scales the squared source range. */
    route_distance = (double)(source_warp + 3) *
        (double)(source_warp + 3) * input->warp_distance_scale;
    distance = input->distance(input->context, input->source->id,
        input->test->id);
    if (distance > route_distance)
        return 0;

    age_nibble = (StarsI16)((input->test->packet_word >> 12) & 0x0fU);
    strength = (StarsI16)(input->test->packet_word & 0x0fffU);
    if (age_nibble > PKT_MAX_AGE_NIBBLE || strength < PKT_MIN_STRENGTH)
        return 0;

    /* A warp gate changes which exponent controls the distance weighting.
     * The weighted result is converted to the same signed long used by the
     * original x87 __aFftol helper before the final integer comparison. */
    exponent = source_gate != 0 ? input->gated_distance_exponent :
        input->ordinary_distance_exponent;
    if ((source_warp + 3) == 0)
        return 0;
    ratio = distance / ((double)(source_warp + 3) *
        (double)(source_warp + 3));
    final_score = (double)input->ai_packet_weight * pow(ratio, exponent);
    weighted_distance = (StarsI32)final_score;

    /* Packet strength is compared with a range-adjusted capacity.  A weak
     * packet gets a floor of 1000; otherwise the packet's low twelve bits are
     * biased by 25 and scaled by four. */
    range_score = (StarsI32)(source_warp + 3) * (source_warp + 3) -
        (StarsI32)target_warp * target_warp;
    range_score *= (StarsI32)(100 - (age_nibble + PKT_PACKET_AGE_BIAS));
    if (input->score_divisor == 0.0)
        return 0;
    range_score = (StarsI32)((double)range_score / input->score_divisor);
    score_limit = (StarsI32)(strength + PKT_STRENGTH_BIAS) * 4;
    if (score_limit < PKT_SCORE_FLOOR)
        score_limit = PKT_SCORE_FLOOR;
    if (range_score <= 0)
        return 0;
    return weighted_distance <= (StarsI32)((double)score_limit /
        (double)range_score);
}
