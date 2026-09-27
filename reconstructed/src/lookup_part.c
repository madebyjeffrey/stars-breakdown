#include "common.h"

enum {
    PART_HULL = 1,
    PART_ENGINE = 2,
    PART_SCANNER = 4,
    PART_ARMOR = 8,
    PART_SHIELD = 0x10,
    PART_TORPEDO = 0x20,
    PART_BOMB = 0x40,
    PART_MINE = 0x80,
    PART_SPECIAL = 0x100,
    PART_PLANETARY = 0x200,
    PART_MESSAGE = 0x400,
    PART_MECH = 0x800,
    PART_FUEL = 0x1000,
    PART_BATTLE = 0x2000,
    PART_COMPONENT = 0x4000
};
#define PART_ZIP ((StarsU16)0x8000U)

static StarsI16 part_race_stat(const StarsLookupPartInput *input)
{
    return input->race_stat ? input->race_stat(input->context,
        input->player, 0x0e) : 0;
}

static StarsI16 part_race_grbit(const StarsLookupPartInput *input,
    StarsI16 bit)
{
    return input->race_grbit ? input->race_grbit(input->context,
        input->player, bit) : 0;
}

static StarsI16 part_hidden(const StarsLookupPartInput *input)
{
    return input->should_hide ? input->should_hide(input->context,
        input->group, input->item) : 0;
}

static StarsI16 part_reject_hidden(const StarsLookupPartInput *input)
{
    return (StarsI16)(input->player != -1 && part_hidden(input) != 0);
}

static StarsI16 part_valid_item(const StarsLookupPartInput *input,
    StarsU16 limit)
{
    return (StarsI16)(input->item <= limit);
}

static StarsI16 part_item_limit(StarsU16 group)
{
    switch (group) {
    case PART_HULL:
        return 15;
    case PART_ENGINE:
        return 15;
    case PART_SCANNER:
        return 9;
    case PART_ARMOR:
        return 11;
    case PART_SHIELD:
        return 23;
    case PART_TORPEDO:
        return 11;
    case PART_BOMB:
        return 14;
    case PART_MINE:
        return 7;
    case PART_SPECIAL:
        return 9;
    case PART_PLANETARY:
        return 15;
    case PART_MESSAGE:
        return 4;
    case PART_MECH:
        return 16;
    case PART_FUEL:
        return 10;
    case PART_BATTLE:
        return 19;
    case PART_COMPONENT:
        return 31;
    case PART_ZIP:
        return 14;
    default:
        return -1;
    }
}

/* All sixteen valid FLookupPart branches compute a literal offset and store
 * the routine's CS in the far pointer. FLookupPart lives in NE segment 2, so
 * that CS-relative offset addresses the original tables embedded in that
 * segment. These bases/strides follow the MOV/IMUL instructions, not the
 * same-offset CodeView global labels decompilation sometimes overlays. */
static StarsI16 part_table_layout(StarsU16 group, StarsU16 *base,
    StarsU16 *stride)
{
    switch (group) {
    case PART_HULL: *base = 0x0000U; *stride = 0x4eU; return 1;
    case PART_ENGINE: *base = 0x0768U; *stride = 0x38U; return 1;
    case PART_SCANNER: *base = 0x0ae8U; *stride = 0x36U; return 1;
    case PART_ARMOR: *base = 0x04e0U; *stride = 0x36U; return 1;
    case PART_SHIELD: *base = 0x2450U; *stride = 0x3cU; return 1;
    case PART_TORPEDO: *base = 0x2180U; *stride = 0x3cU; return 1;
    case PART_BOMB: *base = 0x1e1aU; *stride = 0x3aU; return 1;
    case PART_MINE: *base = 0x1508U; *stride = 0x36U; return 1;
    case PART_SPECIAL:
        *base = 0x12ecU; *stride = 0x36U;
        return 1;
    case PART_PLANETARY:
        *base = 0x4d8aU; *stride = 0x38U;
        return 1;
    case PART_MESSAGE:
        *base = 0x4872U; *stride = 0x8fU;
        return 1;
    case PART_MECH: *base = 0x0d04U; *stride = 0x36U; return 1;
    case PART_FUEL: *base = 0x109aU; *stride = 0x36U; return 1;
    case PART_BATTLE: *base = 0x19e2U; *stride = 0x36U; return 1;
    case PART_COMPONENT: *base = 0x29f0U; *stride = 0x8fU; return 1;
    case PART_ZIP:
        /* The original machine code multiplies by 0x36 and copies CS into
         * the far pointer. Decompiler global labels at this offset are
         * overlays, not the selector/base used by the executable. */
        *base = 0x16b8U; *stride = 0x36U;
        return 1;
    default: return 0;
    }
}

/* rgbeam is the original 24-entry `_beam` table at NE 02:2450.
 * CodeView gives sizeof(_beam)==60 and dp at byte 54.  These values are
 * decoded from that executable segment image, little-endian, at
 * 02:2450+i*0x3c+54.  FLookupPart's group-0x10 branch returns the same
 * table base/stride, so FAttack can recover damage from the selected item.
 */
StarsI16 stars_lookup_beam_dp(StarsU16 item)
{
    static const StarsU16 rgbeam_dp[24] = {
        10U, 16U, 13U, 26U, 90U, 26U, 82U, 26U,
        31U, 66U, 231U, 66U, 211U, 66U, 80U, 169U,
        592U, 169U, 140U, 541U, 169U, 204U, 433U, 433U
    };

    if (item >= 24U)
        return -1;
    return (StarsI16)rgbeam_dp[item];
}

/* Original `_beam.init` is the word at byte 56 of the same NE 02 table.
 * FAttack adds it to the attacker's initBase before comparing the current
 * initiative; the typed resolver's init_delta is that part contribution. */
StarsI16 stars_lookup_beam_init(StarsU16 item)
{
    static const StarsU16 rgbeam_init[24] = {
        9U, 9U, 12U, 9U, 10U, 7U, 14U, 5U,
        12U, 9U, 10U, 7U, 14U, 5U, 13U, 9U,
        11U, 8U, 6U, 14U, 6U, 13U, 9U, 8U
    };

    if (item >= 24U)
        return -1;
    return (StarsI16)rgbeam_init[item];
}

/* Original `_beam.dRangeMax` is the word at byte 52. */
StarsI16 stars_lookup_beam_range(StarsU16 item)
{
    static const StarsU16 rgbeam_range[24] = {
        1U, 1U, 2U, 1U, 0U, 2U, 3U, 3U,
        2U, 1U, 0U, 2U, 3U, 3U, 2U, 1U,
        0U, 2U, 3U, 3U, 3U, 2U, 1U, 2U
    };

    if (item >= 24U)
        return -1;
    return (StarsI16)rgbeam_range[item];
}

/* rgtorp is the original twelve-entry `_torp` table at NE 02:2180.
 * CodeView gives sizeof(_torp)==60 and dp at byte 54.  The data below is
 * decoded from that executable image, little-endian, at 02:2180+i*0x3c+54.
 * FLookupPart's group-0x20 branch returns CS:(0x2180+i*0x3c), so this is the
 * same record family consumed by FAttack, not the overlapping DGROUP text
 * name assigned by the decompiler. */
StarsI16 stars_lookup_torpedo_dp(StarsU16 item)
{
    static const StarsU16 rgtorp_dp[12] = {
        5U, 12U, 26U, 48U, 90U, 169U,
        316U, 60U, 85U, 150U, 280U, 525U
    };

    if (item >= 12U)
        return -1;
    return (StarsI16)rgtorp_dp[item];
}

/* Original `_torp.init` is the word at byte 56 of the NE 02:2180 table. */
StarsI16 stars_lookup_torpedo_init(StarsU16 item)
{
    static const StarsU16 rgtorp_init[12] = {
        0U, 1U, 1U, 2U, 2U, 3U,
        4U, 0U, 0U, 1U, 2U, 3U
    };

    if (item >= 12U)
        return -1;
    return (StarsI16)rgtorp_init[item];
}

/* Original `_torp.dRangeMax` is the word at byte 52 of the NE 02:2180 table. */
StarsI16 stars_lookup_torpedo_range(StarsU16 item)
{
    static const StarsU16 rgtorp_range[12] = {
        4U, 4U, 4U, 5U, 5U, 5U,
        5U, 6U, 5U, 5U, 6U, 6U
    };

    if (item >= 12U)
        return -1;
    return (StarsI16)rgtorp_range[item];
}

/* Genuine semantic reconstruction of FLookupPart, NE 02:524E-60BD.
 * The original routine is a typed dispatch over the sixteen component
 * families.  Each family first validates its item number and records the
 * selected table element, then applies the player race/visibility rules;
 * only a surviving element reaches TechStatus.  The callbacks represent the
 * original far tables and player services while the limits and gates remain
 * ordinary, reviewable C. */
StarsI16 stars_lookup_part(StarsLookupPartInput *input)
{
    StarsI16 race;
    StarsU16 item;
    StarsI16 hidden;
    StarsI16 limit;

    if (!input)
        return 0;
    item = input->item;
    race = part_race_stat(input);
    limit = part_item_limit(input->group);
    if (limit < 0 || item > (StarsU16)limit)
        return 0;
    input->table_segment = 0U;
    input->table_segment_is_current_cs = 0U;
    input->table_base_offset = 0U;
    input->table_offset = 0U;
    input->table_stride = 0U;
    {
        StarsU16 base;
        StarsU16 stride;
        if (part_table_layout(input->group, &base, &stride)) {
            input->table_segment_is_current_cs = 1U;
            input->table_base_offset = base;
            input->table_offset = (StarsU16)(base + item * stride);
            input->table_stride = stride;
        }
    }
    if (input->resolve)
        input->component = input->resolve(input->context, input->group, item);

    switch (input->group) {
    case PART_HULL:
        if (!part_valid_item(input, 15U)) return 0;
        if (item == 0U && input->player != -1 && race != 0) return -1;
        if (item >= 10U && input->player != -1 &&
                part_race_grbit(input, 7) != 0) return -1;
        if ((item == 15U || item == 2U) && input->player != -1 &&
                part_race_grbit(input, 0) == 0) return -1;
        if (item == 7U && input->player != -1 &&
                part_race_grbit(input, 7) == 0) return -1;
        if (part_reject_hidden(input)) return -1;
        break;

    case PART_ENGINE:
        if (!part_valid_item(input, 15U)) return 0;
        if (input->player != -1 &&
                (item == 7U || item == 8U || item == 12U) &&
                part_race_grbit(input, 10) != 0) return -1;
        if ((item == 6U || item == 5U || item == 14U) && race != 1)
            return -1;
        break;

    case PART_SCANNER:
        if (!part_valid_item(input, 9U)) return 0;
        if (item == 4U && race != 1) return -1;
        if (item == 3U && race != 4) return -1;
        if (part_hidden(input)) return -1;
        break;

    case PART_ARMOR:
        if (!part_valid_item(input, 11U)) return 0;
        if (item == 7U && race != 1) return -1;
        if (item == 6U && race != 4) return -1;
        if (part_hidden(input)) return -1;
        break;

    case PART_SHIELD:
        if (!part_valid_item(input, 23U)) return 0;
        if (item == 2U && race != 4) return -1;
        if ((item == 16U || item == 14U) && race != 2) return -1;
        if (part_hidden(input)) return -1;
        break;

    case PART_TORPEDO:
        if (!part_valid_item(input, 11U)) return 0;
        if (input->player != -1 && part_hidden(input)) return -1;
        break;

    case PART_BOMB:
        if (!part_valid_item(input, 14U)) return 0;
        if (input->player != -1) {
            if (item > 9U && item < 15U && race == 4) return -1;
            if (item == 9U && race != 3) return -1;
            if (part_hidden(input)) return -1;
        }
        break;

    case PART_MINE:
        if (!part_valid_item(input, 7U)) return 0;
        if (input->player != -1) {
            if ((item == 2U || item == 3U || item == 4U ||
                    item == 0U || item == 5U) &&
                    part_race_grbit(input, 9) != 0) return -1;
            if ((item == 0U || item == 5U) &&
                    part_race_grbit(input, 2) == 0) return -1;
            if (item == 7U && race != 3) return -1;
            if (part_hidden(input)) return -1;
        }
        break;

    case PART_SPECIAL:
        if (!part_valid_item(input, 9U)) return 0;
        if (input->player != -1) {
            if ((item == 0U || item == 2U || item == 3U || item == 4U ||
                    item == 5U || item == 6U || item == 8U || item == 9U) &&
                    race != 5) return -1;
            if (item == 7U && race != 5 && race != 4) return -1;
            if (item == 1U && race == 2) return -1;
        }
        break;

    case PART_PLANETARY:
        if (!part_valid_item(input, 15U)) return 0;
        if (input->player != -1) {
            if (item < 7U) {
                if (race != 7 && (item == 1U || item > 3U)) return -1;
                if (race == 0) return -1;
            } else if (item != 9U && item != 12U && race != 6) return -1;
            if (part_hidden(input)) return -1;
        }
        break;

    case PART_MESSAGE:
        if (!part_valid_item(input, 4U)) return 0;
        if (input->player != -1) {
            if ((item == 1U || item == 3U) &&
                    part_race_grbit(input, 3) == 0) return -1;
            if (item == 4U && race != 8) return -1;
        }
        break;

    case PART_MECH:
        if (!part_valid_item(input, 16U)) return 0;
        if (input->player != -1) {
            if (part_hidden(input)) return -1;
            if ((item == 0U || item == 3U) && race != 1) return -1;
            if ((item == 8U || item == 11U || item == 15U) && race != 4)
                return -1;
            if (item == 13U && race != 0) return -1;
            if (item == 14U && race != 5) return -1;
            if (item == 16U && race != 7) return -1;
        }
        break;

    case PART_FUEL:
        if (!part_valid_item(input, 10U)) return 0;
        if (input->player != -1) {
            if (part_hidden(input)) return -1;
            if (item == 0U && race == 8) return -1;
            if (item == 1U && race != 8) return -1;
        }
        break;

    case PART_BATTLE:
        if (!part_valid_item(input, 19U)) return 0;
        if (input->player != -1 && item < 8U &&
                part_race_grbit(input, 1) == 0) return -1;
        break;

    case PART_COMPONENT:
        if (!part_valid_item(input, 31U)) return 0;
        if (input->player != -1) {
            if ((item == 14U || item == 31U) && race != 0) return -1;
            if ((item == 25U || item == 3U) && race != 4) return -1;
            if ((item == 22U || item == 23U || item == 20U || item == 24U) &&
                    part_race_grbit(input, 9) != 0) return -1;
            if ((item == 20U || item == 22U || item == 24U) &&
                    part_race_grbit(input, 2) == 0) return -1;
            if ((item == 10U || item == 8U) && race != 2) return -1;
            if (item == 12U && race != 1) return -1;
            if (item == 18U && race != 1) return -1;
            if ((item == 27U || item == 28U) && race != 5) return -1;
            if (part_hidden(input)) return -1;
        }
        break;

    case PART_ZIP:
        if (!part_valid_item(input, 14U)) return 0;
        if (input->player != -1) {
            if (item < 9U && input->armor_dp &&
                    input->armor_dp(input->context, input->component) < 0 &&
                    part_race_grbit(input, 10) != 0) return -1;
            if (item < 9U && race == 8) return -1;
            if (item > 8U && item < 14U && race == 8) return -1;
            if (item > 10U && item < 14U && race == 2) return -1;
            if (part_hidden(input)) return -1;
        }
        break;

    default:
        return 0;
    }

    hidden = input->tech_status ? input->tech_status(input->context,
        input->component) : 0;
    return hidden;
}

/* Map FLookupPart's CS-relative far address into a supplied segment image.
 * The span may be original executable bytes or a live segment snapshot; this
 * function does not assume Ghidra's synthetic selector is a runtime selector.
 */
const StarsU8 *stars_lookup_part_record(const StarsLookupPartInput *input,
    const StarsU8 *current_cs, StarsU16 current_cs_size)
{
    StarsU16 base;
    StarsU16 stride;
    StarsI16 limit;
    StarsU32 expected_offset;

    if (!input || !current_cs || input->table_segment != 0U ||
            input->table_segment_is_current_cs != 1U)
        return (const StarsU8 *)0;
    limit = part_item_limit(input->group);
    if (limit < 0 || input->item > (StarsU16)limit ||
            !part_table_layout(input->group, &base, &stride) ||
            input->table_base_offset != base || input->table_stride != stride)
        return (const StarsU8 *)0;
    expected_offset = (StarsU32)base +
        (StarsU32)input->item * (StarsU32)stride;
    if (expected_offset > 0xffffUL ||
            input->table_offset != (StarsU16)expected_offset ||
            input->table_offset > current_cs_size ||
            stride > (StarsU16)(current_cs_size - input->table_offset))
        return (const StarsU8 *)0;
    return current_cs + input->table_offset;
}
