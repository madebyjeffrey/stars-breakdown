#include "core.h"

#define MACINTI_ACTIVE_FLAG 0x0200U
#define MACINTI_MIN_DESIGNS 16U
#define MACINTI_CREATE_RETRIES 5
#define MACINTI_REPLACEMENT_AGE 20
#define MACINTI_SPECIAL_NAME (-1)

static StarsI16 macinti_active(const StarsMacintiShdef *design)
{
    return design && (design->active & MACINTI_ACTIVE_FLAG) != 0U;
}

static StarsI16 macinti_create(const StarsEnsureMacintiShdefsInput *input,
    StarsI16 slot, StarsU16 hull_id, StarsI16 name_id)
{
    return input->create_shdef ? input->create_shdef(input->context, slot,
        hull_id, name_id) : 0;
}

static void macinti_promote(const StarsEnsureMacintiShdefsInput *input,
    StarsI16 slot)
{
    StarsMacintiShdef copy;
    if (macinti_active(&input->designs[slot]) ||
            input->designs[slot].existing != 0U)
        return;
    copy = input->designs[slot];
    copy.active = (StarsU16)(copy.active | MACINTI_ACTIVE_FLAG);
    if (input->change_shdef)
        input->change_shdef(input->context, slot, &copy);
}

static StarsI16 macinti_random(const StarsEnsureMacintiShdefsInput *input,
    StarsI16 limit)
{
    StarsI16 value;
    if (!input->random_value || limit <= 0)
        return 0;
    value = input->random_value(input->context, limit);
    return value < 0 || value >= limit ? 0 : value;
}

static void macinti_retry(const StarsEnsureMacintiShdefsInput *input,
    StarsI16 slot, StarsU16 hull_id, StarsI16 name_base, StarsI16 name_count)
{
    StarsI16 attempt;
    StarsI16 name_id;
    for (attempt = 0; attempt < MACINTI_CREATE_RETRIES; ++attempt) {
        name_id = name_base + macinti_random(input, name_count);
        if (macinti_create(input, slot, hull_id, name_id) != 0)
            break;
    }
}

/* Genuine semantic reconstruction of EnsureMacintiShdefs, NE 21:2B3C-336C.
 *
 * Macinti design maintenance is organized into role families.  Slots 14 and
 * 15 select the hull-23/24 special family according to player advancement;
 * slots 12 and 13 try a preferred hull-29 design before randomized hull-6
 * fallbacks; slots 5..7 recycle late combat roles after a twenty-turn gap;
 * and slots 2..4 use the same age rule for an earlier family.  Empty starter
 * records are promoted through the change callback before their design is
 * created.  Name-pool selectors below are the recovered string-table indices,
 * kept explicit so they can be replaced by symbolic resource identifiers when
 * the surrounding resource layer is integrated. */
void stars_ensure_macinti_shdefs(StarsEnsureMacintiShdefsInput *input)
{
    StarsI16 slot;
    StarsI16 attempt;
    StarsI16 name_base;

    if (!input || !input->designs || input->design_count < MACINTI_MIN_DESIGNS)
        return;

    /* Special starbase designs: slot 14 has a one-step fallback; slot 15 is
     * the advanced variant without that fallback. */
    for (slot = 14; slot < 16; ++slot) {
        if (macinti_active(&input->designs[slot]) &&
                (input->advanced_player || slot != 15 ||
                 input->tech[3] > 14U)) {
            if (macinti_create(input, slot, (StarsU16)(24U -
                    (input->advanced_player ? 0U : 1U)),
                    MACINTI_SPECIAL_NAME) == 0 && slot == 14) {
                (void)macinti_create(input, slot,
                    (StarsU16)(input->advanced_player ? 22U : 21U),
                    input->advanced_player ? 22 : 23);
            }
        }
    }

    if (macinti_active(&input->designs[12]) && input->tech[1] > 4U &&
            input->tech[2] > 5U) {
        if (macinti_create(input, 12, 29U, 30) == 0)
            macinti_retry(input, 12, 6U, 0, 4);
    }
    if (macinti_active(&input->designs[13]) && input->tech[1] > 9U &&
            input->tech[2] > 8U) {
        if (macinti_create(input, 13, 29U, 30) == 0)
            macinti_retry(input, 13, 6U, 4, 4);
    }
    if (macinti_active(&input->designs[10])) {
        if (macinti_create(input, 10, 2U, 24) == 0)
            (void)macinti_create(input, 10, 1U, 23);
    }
    if (macinti_active(&input->designs[11]))
        (void)macinti_create(input, 11, 2U, 23);

    /* At game start, an unused slot two record is initialized from its own
     * compact record before the first hull-4 design is published. */
    if (input->turn < 20 && !macinti_active(&input->designs[2]) &&
            input->designs[2].existing == 0U)
        macinti_promote(input, 2);
    for (slot = 2; slot < 5; ++slot) {
        if (macinti_active(&input->designs[slot]) &&
                (slot == 2 || (!macinti_active(&input->designs[slot - 1]) &&
                 input->turn - input->designs[slot - 1].turn_created >
                 MACINTI_REPLACEMENT_AGE))) {
            macinti_retry(input, slot, 7U, 25, 4);
        }
    }

    if (input->turn < 40 && macinti_active(&input->designs[7]))
        (void)macinti_create(input, 7, 15U, 20);
    if (input->part_available && input->part_available(input->context,
            1U, 15U) && !macinti_active(&input->designs[1]) &&
            input->designs[1].existing == 0U) {
        macinti_promote(input, 1);
        (void)macinti_create(input, 1, 15U, 20);
    }

    /* Late roles 5..7 first attempt the shared special hull.  A random
     * three-way gate may skip that attempt, and failure falls back to five
     * randomized hull-9 attempts using one of two name pools. */
    for (slot = 5; slot < 8; ++slot) {
        if (!macinti_active(&input->designs[slot]) ||
                (slot != 5 && (macinti_active(&input->designs[slot - 1]) ||
                 input->turn - input->designs[slot - 1].turn_created <=
                 MACINTI_REPLACEMENT_AGE)))
            continue;
        name_base = slot == 5 ? 11 : 15;
        if (slot == 7 && macinti_random(input, 2) != 0)
            name_base = 11;
        attempt = macinti_random(input, 3);
        if (attempt == 0 || macinti_create(input, slot, 29U,
                MACINTI_SPECIAL_NAME) == 0)
            macinti_retry(input, slot, 9U, name_base, 4);
    }

    /* The Macinti starter is upgraded only when it is unused, the player is
     * sufficiently advanced, and the required technology levels are present. */
    if (input->designs[0].hull_id != 5U &&
            input->advanced_player && input->designs[0].existing == 0U &&
            input->tech[5] > 3U && input->tech[4] > 4U &&
            input->tech[3] > 5U && input->tech[2] > 5U &&
            input->tech[0] > 5U) {
        StarsMacintiShdef starter = input->designs[0];
        starter.active = (StarsU16)(starter.active | MACINTI_ACTIVE_FLAG);
        starter.hull_id = 5U;
        if (input->change_shdef)
            input->change_shdef(input->context, 0, &starter);
        (void)macinti_create(input, 0, 5U, 10);
    }
}
