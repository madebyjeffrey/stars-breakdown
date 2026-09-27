#include "core.h"

#define CYBER_ACTIVE_FLAG 0x0200U
#define CYBER_MIN_DESIGNS 16U
#define CYBER_SPECIAL_NAME (-1)
#define CYBER_PREFERRED_TURN 76
#define CYBER_REPLACE_TURNS 20
#define CYBER_LATE_REPLACE_TURNS 30

/* These are indices in the Cyber AI's recovered design-template table.  They
 * are deliberately named by their role family, rather than being scattered
 * as unexplained pointer arithmetic throughout the routine. */
#define CYBER_TEMPLATE_STARTER 12U
#define CYBER_TEMPLATE_TRANSPORT 0U
#define CYBER_TEMPLATE_TRANSPORT_FALLBACK_BASE 0U
#define CYBER_TEMPLATE_TRANSPORT_FALLBACK_COUNT 5
#define CYBER_TEMPLATE_ESCORT 5U
#define CYBER_TEMPLATE_ESCORT_FALLBACK_BASE 5U
#define CYBER_TEMPLATE_ESCORT_FALLBACK_COUNT 5
#define CYBER_TEMPLATE_MINER 10U
#define CYBER_TEMPLATE_SCOUT 11U
#define CYBER_TEMPLATE_HEAVY 16U
#define CYBER_TEMPLATE_ASSAULT 15U
#define CYBER_TEMPLATE_RAIDER 14U
#define CYBER_TEMPLATE_RAIDER_ALT 13U
#define CYBER_TEMPLATE_HULL7_EARLY_BASE 17U
#define CYBER_TEMPLATE_HULL7_LATE_BASE 20U
#define CYBER_TEMPLATE_HULL7_LATE_COUNT 9
#define CYBER_TEMPLATE_HULL9_BASE 26U
#define CYBER_TEMPLATE_HULL9_COUNT 3
#define CYBER_TEMPLATE_HULL9_WIDE_BASE 29U
#define CYBER_TEMPLATE_HULL9_WIDE_COUNT 4
#define CYBER_TEMPLATE_HULL29_BASE 33U
#define CYBER_TEMPLATE_HULL29_COUNT 3

static StarsI16 cyber_active(const StarsCyberAiShdef *design)
{
    return design && (design->active & CYBER_ACTIVE_FLAG) != 0U;
}

static StarsI16 cyber_random(const StarsEnsureCyberAiShdefsInput *input,
    StarsI16 limit)
{
    StarsI16 value;
    if (!input->random_value || limit <= 0)
        return 0;
    value = input->random_value(input->context, limit);
    return value < 0 || value >= limit ? 0 : value;
}

static StarsI16 cyber_create(const StarsEnsureCyberAiShdefsInput *input,
    StarsI16 slot, StarsU16 hull_id, StarsU16 template_id)
{
    return input->create_shdef ? input->create_shdef(input->context, slot,
        hull_id, template_id) : 0;
}

static StarsI16 cyber_random_family(
    const StarsEnsureCyberAiShdefsInput *input, StarsI16 slot,
    StarsU16 hull_id, StarsU16 template_base, StarsI16 template_count,
    StarsI16 attempts)
{
    StarsI16 attempt;
    StarsI16 template_id;
    for (attempt = 0; attempt < attempts; ++attempt) {
        template_id = (StarsI16)(template_base +
            cyber_random(input, template_count));
        if (cyber_create(input, slot, hull_id, (StarsU16)template_id) != 0)
            return 1;
    }
    return 0;
}

static void cyber_promote_starter(
    const StarsEnsureCyberAiShdefsInput *input)
{
    StarsCyberAiShdef copy;
    if (cyber_active(&input->designs[0]) || input->designs[0].hull_id == 5U ||
            input->designs[0].existing != 0U || input->turn <= 5 ||
            input->ai_level <= 1)
        return;
    copy = input->designs[0];
    copy.active = (StarsU16)(copy.active | CYBER_ACTIVE_FLAG);
    if (input->change_shdef)
        input->change_shdef(input->context, 0, &copy);
}

/* Genuine semantic reconstruction of EnsureCyberAiShdefs, NE 16:4826-4EBA.
 *
 * This is the Cyber AI's periodic design planner.  It first promotes an
 * unused starter record once the AI is sufficiently advanced, then services
 * fixed early roles, replacement roles, and two late-game role families.  A
 * failed FCreateAiShdef call means that the design table could not accept the
 * requested template, so the original makes a bounded number of randomized
 * attempts before moving to its next family.  The callback's template index
 * stands for the original far pointer into the Cyber role/template table. */
void stars_ensure_cyber_ai_shdefs(StarsI16 iro_cur,
    StarsEnsureCyberAiShdefsInput *input)
{
    StarsI16 slot;
    StarsI16 attempt;
    StarsI16 remaining;
    StarsI16 template_count;
    StarsI16 template_base;

    (void)iro_cur;
    if (!input || !input->designs ||
            input->design_count < CYBER_MIN_DESIGNS)
        return;

    /* An empty starter record is copied and marked active before its hull-5
     * design is published.  This preserves the value-style FChangeAiShdef
     * update instead of silently mutating the caller's record. */
    cyber_promote_starter(input);

    /* Slot 0 is the starter hull. */
    if (cyber_active(&input->designs[0]))
        (void)cyber_create(input, 0, 5U, CYBER_TEMPLATE_STARTER);

    /* Slot 4 is the first transport/escort role: use its preferred hull while
     * the game is young, then five randomized templates from the same family. */
    if (cyber_active(&input->designs[4]) && input->turn > 30) {
        if (input->turn < CYBER_PREFERRED_TURN &&
                cyber_create(input, 4, 6U, CYBER_TEMPLATE_TRANSPORT) == 0)
            (void)cyber_random_family(input, 4, 6U,
                CYBER_TEMPLATE_TRANSPORT_FALLBACK_BASE,
                CYBER_TEMPLATE_TRANSPORT_FALLBACK_COUNT, 5);
        else if (input->turn >= CYBER_PREFERRED_TURN)
            (void)cyber_random_family(input, 4, 6U,
                CYBER_TEMPLATE_TRANSPORT_FALLBACK_BASE,
                CYBER_TEMPLATE_TRANSPORT_FALLBACK_COUNT, 5);
    }

    /* Slot 5 is staged behind slot 4.  It becomes eligible twenty turns after
     * the preceding role has become inactive. */
    if (cyber_active(&input->designs[5]) && !cyber_active(&input->designs[4]) &&
            input->designs[4].turn_created + CYBER_REPLACE_TURNS <
            input->turn) {
        if (input->turn < CYBER_PREFERRED_TURN &&
                cyber_create(input, 5, 6U, CYBER_TEMPLATE_ESCORT) == 0)
            (void)cyber_random_family(input, 5, 6U,
                CYBER_TEMPLATE_ESCORT_FALLBACK_BASE,
                CYBER_TEMPLATE_ESCORT_FALLBACK_COUNT, 5);
        else if (input->turn >= CYBER_PREFERRED_TURN)
            (void)cyber_random_family(input, 5, 6U,
                CYBER_TEMPLATE_ESCORT_FALLBACK_BASE,
                CYBER_TEMPLATE_ESCORT_FALLBACK_COUNT, 5);
    }

    /* The mineral and scout support roles are fixed hull-11 designs. */
    if (cyber_active(&input->designs[2]) && input->turn > 20)
        (void)cyber_create(input, 2, 11U, CYBER_TEMPLATE_MINER);
    if (cyber_active(&input->designs[3]) && !cyber_active(&input->designs[2]) &&
            input->designs[2].turn_created + CYBER_REPLACE_TURNS <
            input->turn)
        (void)cyber_create(input, 3, 11U, CYBER_TEMPLATE_SCOUT);

    /* Slots 6 and 10 are the late role-family triggers.  Each fills a set of
     * later design slots, trying hull-29, then hull-9, then hull-7 templates.
     * The decrementing destination slot is intentional: a successful create
     * consumes one of the available role positions. */
    for (slot = 6; slot < 11; slot = (StarsI16)(slot + 4)) {
        if (!cyber_active(&input->designs[slot]) ||
                !((slot == 6 && input->turn > 40) ||
                (!cyber_active(&input->designs[6]) &&
                 input->designs[6].turn_created + CYBER_LATE_REPLACE_TURNS <
                 input->turn)))
            continue;

        remaining = (StarsI16)(slot + 2);
        for (attempt = 0; attempt < 3; ++attempt)
            if (cyber_create(input, remaining, 29U,
                    (StarsU16)(CYBER_TEMPLATE_HULL29_BASE +
                    cyber_random(input, CYBER_TEMPLATE_HULL29_COUNT))) != 0)
                --remaining;
        for (attempt = 0; attempt < 4; ++attempt)
            if (cyber_create(input, remaining, 9U,
                    (StarsU16)(CYBER_TEMPLATE_HULL9_WIDE_BASE +
                    cyber_random(input, CYBER_TEMPLATE_HULL9_WIDE_COUNT))) != 0)
                --remaining;
        for (attempt = 0; attempt < 3; ++attempt)
            if (cyber_create(input, remaining, 9U,
                    (StarsU16)(CYBER_TEMPLATE_HULL9_BASE +
                    cyber_random(input, CYBER_TEMPLATE_HULL9_COUNT))) != 0)
                --remaining;

        if (remaining >= slot) {
            template_count = (StarsI16)(9 - 3 * (remaining - slot));
            template_base = (StarsI16)(CYBER_TEMPLATE_HULL9_BASE -
                template_count);
            (void)cyber_random_family(input, remaining, 7U,
                (StarsU16)template_base, template_count, template_count);
            --remaining;
            if (remaining >= slot) {
                template_base = remaining == slot ?
                    CYBER_TEMPLATE_HULL7_EARLY_BASE :
                    CYBER_TEMPLATE_HULL7_LATE_BASE;
                (void)cyber_random_family(input, remaining, 7U,
                    (StarsU16)template_base, 3, 3);
                --remaining;
            }
        }

        /* The final slot gets a fixed heavy design and progressively lighter
         * fallbacks when the preferred template cannot be allocated. */
        (void)(cyber_create(input, (StarsI16)(slot + 3), 29U,
            CYBER_TEMPLATE_HEAVY) != 0 ||
            cyber_create(input, (StarsI16)(slot + 3), 9U,
                CYBER_TEMPLATE_ASSAULT) != 0 ||
            cyber_create(input, (StarsI16)(slot + 3), 19U,
                CYBER_TEMPLATE_RAIDER) != 0 ||
            cyber_create(input, (StarsI16)(slot + 3), 19U,
                CYBER_TEMPLATE_RAIDER_ALT) != 0);
    }

    /* Slots 14 and 15 are the late Cyber combat roles.  Each prefers the
     * three hull-9 templates, then widens to nine hull-7 templates, and ends
     * with ten hull-6 templates. */
    for (slot = 14; slot < 16; ++slot) {
        if (!cyber_active(&input->designs[slot]) ||
                !((slot == 14 && input->turn > 30) ||
                (!cyber_active(&input->designs[14]) &&
                 input->designs[14].turn_created + CYBER_REPLACE_TURNS <
                 input->turn)))
            continue;
        if (!cyber_random_family(input, slot, 9U,
                CYBER_TEMPLATE_HULL9_BASE, CYBER_TEMPLATE_HULL9_COUNT, 7) &&
            !cyber_random_family(input, slot, 7U,
                CYBER_TEMPLATE_HULL7_EARLY_BASE,
                CYBER_TEMPLATE_HULL7_LATE_COUNT, 9))
            (void)cyber_random_family(input, slot, 6U, 0U, 10, 10);
    }
}
