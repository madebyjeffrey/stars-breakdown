#include "core.h"

static StarsI32 torp_hit_percentage(StarsI16 base, StarsI16 bc,
    StarsU16 jam)
{
    StarsI32 result;
    if (bc == 0) {
        if (jam == 0U)
            return (StarsI32)base;
        return ((StarsI32)base * (100L - (StarsI32)jam)) / 100L;
    }
    result = ((StarsI32)(100 - base) * (100L - (StarsI32)bc)) / 100L;
    return 100L - result;
}

/* Genuine C reconstruction of CTorpHit, NE 1F:6790-6A6C. */
StarsI32 stars_c_torp_hit(StarsI32 torpedo_count,
    const StarsTorpHitToken *token, StarsI16 base_percent,
    StarsI16 battle_computer_percent, StarsTorpHitRandom random,
    void *context)
{
    StarsU16 jam;
    StarsI16 bc;
    StarsI32 hit_percent;
    StarsI32 hits;
    StarsI32 index;

    if (torpedo_count == 0L || base_percent == 0)
        return 0L;
    jam = token ? token->jam_percent : 0U;
    bc = battle_computer_percent;
    if (jam != 0U && bc != 0) {
        if ((StarsI32)jam < (StarsI32)bc) {
            bc = (StarsI16)(bc - (StarsI16)jam);
            jam = 0U;
        } else {
            jam = (StarsU16)(jam - (StarsU16)bc);
            bc = 0;
        }
    }
    hit_percent = torp_hit_percentage(base_percent, bc, jam);
    if (hit_percent < 1L)
        hit_percent = 1L;
    if (hit_percent < 100L) {
        if (torpedo_count < 0xc9L) {
            hits = 0L;
            for (index = 0; index < torpedo_count; ++index) {
                if (random && random(context, 100) < (StarsI16)hit_percent)
                    ++hits;
            }
            return hits;
        }
        return (torpedo_count * hit_percent) / 100L;
    }
    return torpedo_count;
}
