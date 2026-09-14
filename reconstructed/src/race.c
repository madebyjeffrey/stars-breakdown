#include "core.h"
/* NE 29:30D2-30F4. Near offsets wrap at 16 bits; CBW sign-extends the byte.
 * No bounds check is present in the original. Caller validity is separate. */
StarsI16 stars_get_race_stat(const StarsU8 *dgroup, StarsU16 player, StarsI16 index)
{
    StarsU16 address;
    StarsU8 value;
    address = (StarsU16)(player + (StarsU16)index + 62U);
    value = dgroup[address];
    return value < 128U ? (StarsI16)value : (StarsI16)((int)value - 256);
}

/* Original CS:30F4-3114. Signed-byte limits for the sixteen race statistics. */
static const StarsI16 stat_min[16] = {7,5,5,5,5,2,5,0,0,0,0,0,0,0,0,0};
static const StarsI16 stat_max[16] = {25,15,25,25,25,15,25,6,2,2,2,2,2,2,9,0};

/* NE 29:3114-3175. This setter does not set the bounds-correction flag. */
StarsI16 stars_set_race_stat(StarsU8 *data, StarsU16 player, StarsI16 index, StarsI16 value)
{
    if (value < stat_min[index]) value = stat_min[index];
    if (value > stat_max[index]) value = stat_max[index];
    data[(StarsU16)(player + 62U + (StarsU16)index)] = (StarsU8)value;
    return value;
}

static StarsU32 race_flags(const StarsU8 *data, StarsU16 player)
{
    StarsU32 value = 0;
    StarsU16 i;
    for (i = 0; i < 4U; ++i)
        value |= (StarsU32)data[(StarsU16)(player + 78U + i)] << (i * 8U);
    return value;
}

static StarsU32 trait_mask(StarsI16 index)
{
    StarsU16 shift = (StarsU16)((StarsU16)index & 255U);
    /* Original CRT clears CH, then shifts DX:AX one bit CL times. */
    return shift < 32U ? (StarsU32)1 << shift : (StarsU32)0;
}

/* NE 29:3176-31B8. */
StarsI16 stars_get_race_trait(const StarsU8 *data, StarsU16 player, StarsI16 index)
{
    return (race_flags(data, player) & trait_mask(index)) ? 1 : 0;
}

/* NE 29:31B8-3209. */
void stars_set_race_trait(StarsU8 *data, StarsU16 player, StarsI16 index, StarsI16 enabled)
{
    StarsU32 value = race_flags(data, player), mask = trait_mask(index);
    StarsU16 i;
    value = enabled ? value | mask : value & ~mask;
    for (i = 0; i < 4U; ++i)
        data[(StarsU16)(player + 78U + i)] = (StarsU8)(value >> (i * 8U));
}

/* NE 29:5888-58D3. XOR of all 96 little-endian words in the player record. */
StarsU16 stars_race_checksum(const StarsU8 *data, StarsU16 player)
{
    StarsU16 result = 0, i, address;
    for (i = 0; i < 96U; ++i) {
        address = (StarsU16)(player + i * 2U);
        result ^= (StarsU16)(data[address] | ((StarsU16)data[(StarsU16)(address + 1U)] << 8));
    }
    return result;
}

static StarsI16 signed_byte(StarsU8 value)
{
    return value < 128U ? (StarsI16)value : (StarsI16)((int)value - 256);
}

static void corrected(StarsU8 *data, StarsU16 player)
{
    data[(StarsU16)(player + 84U)] |= 16U;
}

/* NE 29:40AE-440F. Correct environmental endpoints/centers and race stats.
 * This is mutating validation, not just a predicate. */
void stars_bounds_check_player(StarsU8 *data, StarsU16 player)
{
    StarsU16 i, low, high, center, address;
    StarsI16 value;
    for (i = 0; i < 3U; ++i) {
        center = (StarsU16)(player + 16U + i);
        low = (StarsU16)(player + 19U + i);
        high = (StarsU16)(player + 22U + i);
        if (data[low] == 255U) {
            if (data[high] != 255U || data[center] != 255U) {
                data[center] = 255U;
                data[high] = 255U;
                corrected(data, player);
            }
        } else {
            if (signed_byte(data[low]) < 0) { data[low] = 0; corrected(data, player); }
            if (signed_byte(data[low]) > 100) { data[low] = 100; corrected(data, player); }
            if (signed_byte(data[high]) > 100) { data[high] = 100; corrected(data, player); }
            if (signed_byte(data[high]) < signed_byte(data[low])) {
                data[high] = data[low]; corrected(data, player);
            }
            value = (StarsI16)(data[low] + (data[high] - data[low]) / 2);
            if (signed_byte(data[center]) != value) {
                data[center] = (StarsU8)value; corrected(data, player);
            }
        }
    }
    address = (StarsU16)(player + 25U);
    if (signed_byte(data[address]) > 20) { data[address] = 20; corrected(data, player); }
    for (i = 0; i < 16U; ++i) {
        address = (StarsU16)(player + 62U + i);
        if (signed_byte(data[address]) < stat_min[i]) {
            data[address] = (StarsU8)stat_min[i]; corrected(data, player);
        }
        if (signed_byte(data[address]) > stat_max[i]) {
            data[address] = (StarsU8)stat_max[i]; corrected(data, player);
        }
    }
}

/* Original CS-relative tables at NE 29:4410, 4424, and 4440. */
static const StarsI16 primary_cost[10] = {40,95,45,10,-100,-150,120,180,90,-66};
static const StarsI16 secondary_points[14] = {-235,-25,-159,-201,40,-240,-155,160,240,255,325,180,70,30};
static const StarsI16 research_credit[6] = {150,330,540,780,1050,1380};

/* NE 29:444C-4C6D. Requires nondegenerate ranges after BoundsCheckPlayer.
 * Validated stat bounds keep each original 16-bit subtotal in range. */
StarsI16 stars_race_advantage_points(StarsU8 *data, StarsU16 player)
{
    StarsI16 primary, growth, factor, immune = 0, i, center, a, b, c, subtotal;
    StarsI16 negative = 0, positive = 0, research = 0;
    StarsI32 points = 1650, habitability, quotient;
    StarsU16 low_word;
    stars_bounds_check_player(data, player);
    primary = stars_get_race_stat(data, player, 14);
    habitability = stars_innate_race_habitability(data, player) / 2000;
    growth = signed_byte(data[(StarsU16)(player + 25U)]);
    if (growth < 1 || growth > 20) {
        growth = 1;
        data[(StarsU16)(player + 25U)] = 1;
        corrected(data, player);
    }
    factor = growth;
    if (growth <= 5) points += (StarsI32)(6 - growth) * 4200;
    else if (growth <= 13) {
        if (growth == 6) points += 3600;
        else if (growth == 7) points += 2250;
        else if (growth == 8) points += 600;
        else if (growth == 9) points += 225;
        factor = (StarsI16)((growth - 5) * 2 + 5);
    } else if (growth < 20) factor = (StarsI16)((growth - 13) * 3 + 21);
    else factor = 45;
    points -= habitability * factor / 24;
    for (i = 0; i < 3; ++i) {
        center = signed_byte(data[(StarsU16)(player + 16U + i)]);
        if (center < 0) ++immune;
        else points += (center < 50 ? 50 - center : center - 50) * 4;
    }
    if (immune > 1) points -= 150;
    a = stars_get_race_stat(data, player, 3);
    b = stars_get_race_stat(data, player, 1);
    if (a > 10 || b > 10) {
        a = a > 10 ? (StarsI16)(a - 9) : 1;
        b = b > 10 ? (StarsI16)(b - 9) : 1;
        points -= (StarsI32)a * b * (primary == 0 ? 3 : 2) * growth / (immune >= 2 ? 2 : 9);
    }
    a = stars_get_race_stat(data, player, 0);
    if (a <= 7) points -= 2400;
    else if (a == 8) points -= 1260;
    else if (a == 9) points -= 600;
    else if (a > 10) points += (a - 10) * 120;
    if (primary == 8) points += 210;
    else {
        a = (StarsI16)(10 - stars_get_race_stat(data, player, 1));
        b = (StarsI16)(10 - stars_get_race_stat(data, player, 2));
        c = (StarsI16)(10 - stars_get_race_stat(data, player, 3));
        subtotal = (StarsI16)(a * (a > 0 ? 100 : 121));
        subtotal = (StarsI16)(subtotal - (b < 0 ? b * 55 : b * b * 60));
        subtotal = (StarsI16)(subtotal + c * (c > 0 ? 40 : 35));
        if (subtotal > 700) subtotal = (StarsI16)((subtotal - 700) / 3 + 700);
        if (c <= -7) {
            if (c >= -11) subtotal = (StarsI16)(subtotal - (-6 - c) * 30);
            else if (c >= -14) subtotal = (StarsI16)(subtotal - ((-12 - c) * 45 + 225));
            else subtotal = (StarsI16)(subtotal - 360);
        }
        if (a <= -3) subtotal = (StarsI16)(subtotal - (-2 - a) * 60);
        points += subtotal;
        if (stars_get_race_trait(data, player, 31)) points -= 175;
        a = (StarsI16)(10 - stars_get_race_stat(data, player, 4));
        b = (StarsI16)(3 - stars_get_race_stat(data, player, 5));
        c = (StarsI16)(10 - stars_get_race_stat(data, player, 6));
        subtotal = (StarsI16)(a * (a > 0 ? 100 : 169));
        subtotal = (StarsI16)(subtotal + (b <= 0 ? 80 - b * 65 : -360));
        subtotal = (StarsI16)(subtotal + c * (c > 0 ? 40 : 35));
        points += subtotal;
    }
    points -= primary_cost[primary];
    for (i = 0; i < 14; ++i) {
        if (stars_get_race_trait(data, player, i)) {
            points += secondary_points[i];
            if (secondary_points[i] < 0) ++negative;
            else ++positive;
        }
    }
    a = (StarsI16)(negative + positive);
    if (a > 4) points -= a * 10 * (a - 4);
    if (positive - negative > 3) points -= (positive - negative - 3) * 60;
    if (negative - positive > 3) points -= (negative - positive - 3) * 40;
    if (stars_get_race_trait(data, player, 10)) {
        if (primary == 6) points -= 280;
        else if (primary == 1) points -= 200;
        else if (primary == 9) points -= 40;
    }
    for (i = 8; i < 14; ++i)
        research = (StarsI16)(research + stars_get_race_stat(data, player, i) - 1);
    if (research > 0) {
        points -= research * research * 130;
        if (research == 6) points += 1430;
        else if (research == 5) points += 520;
    } else if (research < 0) {
        points += research_credit[-research - 1];
        if (research < -4 && stars_get_race_stat(data, player, 0) < 10) points -= 190;
    }
    if (stars_get_race_trait(data, player, 29)) points -= 180;
    if (primary == 8 && stars_get_race_stat(data, player, 8) == 2) points -= 100;
    /* C89 permits either direction for negative division: make truncation
     * toward zero explicit, then preserve the original AX-only return. */
    quotient = points < 0 ? -((-points) / 3) : points / 3;
    low_word = (StarsU16)quotient;
    return low_word < 32768U ? (StarsI16)low_word : (StarsI16)((StarsI32)low_word - 65536L);
}

/* NE 29:65D4-662C. Raw signed growth byte; no bounds correction. */
StarsI16 stars_true_max_growth(const StarsU8 *data, StarsI16 player_index)
{
    StarsU16 player = (StarsU16)(0x59a2U + (StarsU16)player_index * 192U);
    StarsI16 growth = signed_byte(data[(StarsU16)(player + 25U)]);
    return stars_get_race_stat(data, player, 14) == 0 ? (StarsI16)(growth * 2) : growth;
}

static StarsI16 race_random(StarsU8 *data, StarsI16 count)
{
    StarsSeeds seeds;
    StarsU16 i;
    StarsI16 result;
    seeds.first = seeds.second = 0;
    for (i = 0; i < 4U; ++i) {
        seeds.first |= (StarsU32)data[0x1370U + i] << (i * 8U);
        seeds.second |= (StarsU32)data[0x1374U + i] << (i * 8U);
    }
    result = stars_random(&seeds, count);
    for (i = 0; i < 4U; ++i) {
        data[0x1370U + i] = (StarsU8)(seeds.first >> (i * 8U));
        data[0x1374U + i] = (StarsU8)(seeds.second >> (i * 8U));
    }
    return result;
}

static void race_environment(StarsU8 *data, StarsU16 player, StarsI16 axis,
    StarsI16 low, StarsI16 high)
{
    data[(StarsU16)(player + 22U + axis)] = (StarsU8)high;
    data[(StarsU16)(player + 19U + axis)] = (StarsU8)low;
    data[(StarsU16)(player + 16U + axis)] = low < 0 ? 255U : (StarsU8)(low + (high - low) / 2);
}

static StarsI16 race_score_distance(StarsI16 score)
{
    return -score > score - 50 ? (StarsI16)-score : (StarsI16)(score - 50);
}

static int race_improved(StarsU8 *data, StarsU16 player, StarsI16 distance)
{
    return race_score_distance(stars_race_advantage_points(data, player)) < distance;
}

/* NE 29:5B08-65D4. Player is an existing initialized record, not blank storage.
 * Preserves all RNG calls, trial order, selective rollback and fallback name.
 * Records, default template and string scratch must be disjoint; see report. */
void stars_create_random_race(StarsU8 *data, const StarsU8 *strings, StarsU16 player)
{
    StarsI16 mode, i, j, width, low, old, choice, distance, points;
    StarsU16 passes = 0, at, source;
    StarsU8 saved[192];
    data[(StarsU16)(player + 160U)] = 0;
    mode = race_random(data, 25);
    if (mode < 4) {
        for (i = 0; i < 3; ++i) race_environment(data, player, i, -1, -1);
        data[(StarsU16)(player + 25U)] = (StarsU8)(race_random(data, 4) + 2);
    } else if (mode < 7) {
        for (i = 0; i < 3; ++i) race_environment(data, player, i, 0, 100);
        data[(StarsU16)(player + 25U)] = (StarsU8)(race_random(data, 4) + 3);
    } else if (mode < 9) {
        for (i = 0; i < 3; ++i) {
            choice = race_random(data, 2);
            if (i == 2 && data[(StarsU16)(player + 16U)] == data[(StarsU16)(player + 17U)])
                choice = data[(StarsU16)(player + 16U)] != 0;
            if (!choice) race_environment(data, player, i, 0, 100);
            else {
                /* Original 5C91 changes growth, leaves this axis untouched,
                 * and consumes a random draw whose growth value is overwritten. */
                data[(StarsU16)(player + 25U)] = (StarsU8)(race_random(data, 4) + 2);
            }
        }
        data[(StarsU16)(player + 25U)] = (StarsU8)(race_random(data, 5) + 2);
    } else {
        for (i = 0; i < 3; ++i) {
            width = (StarsI16)(race_random(data, 40) * 2 + 20);
            low = race_random(data, (StarsI16)(101 - width));
            race_environment(data, player, i, low, (StarsI16)(low + width));
        }
        if (mode < 12) {
            i = race_random(data, 3);
            race_environment(data, player, i, -1, -1);
        } else if (mode < 14) {
            i = race_random(data, 3);
            race_environment(data, player, i, 0, 100);
        } else if (mode < 17) {
            i = race_random(data, 3); low = race_random(data, 81);
            race_environment(data, player, i, low, (StarsI16)(low + 20));
        }
        data[(StarsU16)(player + 25U)] = (StarsU8)(race_random(data, 9) + 7);
    }
    mode = race_random(data, 3);
    for (i = 8; i < 14; ++i) {
        choice = mode ? race_random(data, 3) : 1;
        stars_set_race_stat(data, player, i, choice);
    }
    choice = race_random(data, 10);
    stars_set_race_stat(data, player, 14, choice);
    mode = race_random(data, 4);
    for (i = 0; i < 14; ++i) {
        choice = mode ? race_random(data, 2) : 0;
        stars_set_race_trait(data, player, i, choice);
    }
    choice = race_random(data, 2); stars_set_race_trait(data, player, 29, choice);
    choice = race_random(data, 2); stars_set_race_trait(data, player, 31, choice);
    mode = race_random(data, 3);
    if (!mode) {
        for (i = 0; i < 7; ++i) data[(StarsU16)(player + 62U + i)] = data[0xde0U + i];
        data[(StarsU16)(player + 69U)] = (StarsU8)race_random(data, 5);
    } else {
        for (i = 0; i < 8; ++i) {
            choice = race_random(data, (StarsI16)(stat_max[i] + 1 - stat_min[i]));
            data[(StarsU16)(player + 62U + i)] = (StarsU8)(stat_min[i] + choice);
        }
    }
    source = stars_get_compressed_string(data, strings, 0x532);
    at = (StarsU16)(player + 128U);
    while (data[at] && data[at] == data[source]) { ++at; ++source; }
    if (data[at] == data[source]) {
        choice = (StarsI16)(race_random(data, 24) + 0x56e);
        stars_copy_game_string(data, strings, choice, (StarsU16)(player + 128U));
    }
    points = stars_race_advantage_points(data, player);
    if (points >= 0 && points <= 50) return;
    for (;;) {
        points = stars_race_advantage_points(data, player);
        if (points >= 0 && points <= 50) return;
        if (passes++ > 250U) {
            for (i = 0; i < 192; ++i) saved[i] = data[(StarsU16)(player + i)];
            for (i = 0; i < 192; ++i) data[(StarsU16)(player + i)] = data[0xda2U + i];
            i = 128;
            do { data[(StarsU16)(player + i)] = saved[i]; } while (saved[i++]);
            return;
        }
        choice = race_random(data, 10);
        distance = race_score_distance(points);
        if (choice < 3) {
            i = race_random(data, 6);
            at = (StarsU16)(player + 70U + i);
            old = signed_byte(data[at]);
            if (old > 0) {
                --data[at];
                if (race_improved(data, player, distance)) continue;
                data[at] = (StarsU8)old;
            }
            if (old < 2) {
                ++data[at];
                if (race_improved(data, player, distance)) continue;
                data[at] = (StarsU8)old;
            }
        } else if (choice < 6) {
            i = race_random(data, 14);
            old = stars_get_race_trait(data, player, i);
            for (j = 0; j < 2; ++j) {
                stars_set_race_trait(data, player, i, j);
                if (race_improved(data, player, distance)) break;
            }
            if (j == 2) stars_set_race_trait(data, player, i, old);
        } else if (choice < 9) {
            i = race_random(data, 7);
            old = stars_get_race_stat(data, player, i);
            for (j = -1; j <= 1; j += 2) {
                stars_set_race_stat(data, player, i, (StarsI16)(old + j));
                if (race_improved(data, player, distance)) break;
            }
            if (j > 1) stars_set_race_stat(data, player, i, old);
        } else if (race_random(data, 2)) {
            at = (StarsU16)(player + 25U);
            old = signed_byte(data[at]);
            if (old > 1) {
                data[at] = (StarsU8)(old - 1);
                if (race_improved(data, player, distance)) continue;
            }
            if (old < 15) {
                data[at] = (StarsU8)(old + 1);
                if (race_improved(data, player, distance)) continue;
            }
            data[at] = (StarsU8)old;
        } else {
            i = race_random(data, 3);
            if (signed_byte(data[(StarsU16)(player + 16U + i)]) < 0) {
                low = race_random(data, 31);
                race_environment(data, player, i, low, (StarsI16)(low + 70));
                if (race_improved(data, player, distance)) continue;
                race_environment(data, player, i, -1, -1);
            } else {
                for (j = 0; j < 192; ++j) saved[j] = data[(StarsU16)(player + j)];
                race_environment(data, player, i, -1, -1);
                if (race_improved(data, player, distance)) continue;
                for (j = 0; j < 192; ++j) data[(StarsU16)(player + j)] = saved[j];
            }
        }
    }
}
