#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct BeamFixture {
    StarsU16 capacity_percent;
    StarsU16 part_dp;
    StarsU16 distance;
    StarsU16 beam_defense_percent;
    StarsU16 broadcast;
    StarsU16 selected_item;
    StarsI32 callback_damage;
    StarsU16 callback_class;
    StarsI16 calls;
} BeamFixture;

static StarsI16 beam_target_distance;

static StarsI16 resolve_beam(void *context, StarsU16 group, StarsU16 item,
    StarsU16 count, StarsAttackSection *section)
{
    BeamFixture *fixture = (BeamFixture *)context;

    if (group != 0x0010U || item != fixture->selected_item || count != 1U)
        return 0;
    section->active = 1U;
    section->base_damage = 100L;
    section->raw_scaling = 1U;
    section->raw_capacity_percent = fixture->capacity_percent;
    section->raw_part_dp = fixture->part_dp;
    section->raw_distance = fixture->distance;
    section->beam_passes = 1;
    section->beam_broadcast = fixture->broadcast;
    return 1;
}

static StarsI16 capture_beam(void *context, StarsI16 attacker,
    StarsI16 target, StarsI16 section, StarsI32 *damage,
    StarsI32 torpedo_damage, StarsU16 weapon_class, StarsI16 shields_only,
    StarsU32 *torpedo_packets)
{
    BeamFixture *fixture = (BeamFixture *)context;

    (void)attacker;
    (void)target;
    (void)section;
    (void)torpedo_damage;
    (void)shields_only;
    (void)torpedo_packets;
    ++fixture->calls;
    fixture->callback_damage = *damage;
    fixture->callback_class = weapon_class;
    return 1;
}

static StarsI16 ignore_typed(void *context, StarsI16 attacker,
    StarsI16 target, StarsI16 section, StarsI32 damage, StarsI16 mode)
{
    (void)context;
    (void)attacker;
    (void)target;
    (void)section;
    (void)damage;
    (void)mode;
    return 0;
}

/* Link-only stubs: this probe exercises beam sections, not range or torpedoes. */
StarsI16 stars_dz_from_brc_brc(StarsU8 first, StarsU8 second)
{
    (void)first;
    (void)second;
    return beam_target_distance;
}

StarsI32 stars_c_torp_hit(StarsI32 count, const StarsTorpHitToken *target,
    StarsI16 base_percent, StarsI16 computer_percent,
    StarsTorpHitRandom random_fn, void *context)
{
    (void)count;
    (void)target;
    (void)base_percent;
    (void)computer_percent;
    (void)random_fn;
    (void)context;
    return 0L;
}

static StarsI16 run_case(StarsAttackInput *input, BeamFixture *fixture,
    StarsAttackToken *target, StarsAttackPackedPartSlot *packed_part,
    StarsU16 item, StarsU16 capacity, StarsU16 dp,
    StarsU16 distance, StarsU16 defense, StarsI32 expected_damage,
    StarsU16 expected_class, StarsU16 broadcast)
{
    fixture->capacity_percent = capacity;
    fixture->part_dp = dp;
    fixture->selected_item = item;
    fixture->distance = distance;
    fixture->broadcast = broadcast;
    packed_part->item = (StarsU8)item;
    target->beam_defense_percent = defense;
    fixture->callback_damage = -1L;
    fixture->callback_class = 0U;
    fixture->calls = 0;
    input->hit_count = 0;
    input->first_hit_target = -1;
    input->first_hit_section = -1;

    if (!stars_f_attack(input))
        return expected_damage < 0L && fixture->calls == 0;
    return fixture->calls == 1 &&
        fixture->callback_damage == expected_damage &&
        fixture->callback_class == expected_class;
}

int main(void)
{
    static const StarsU16 expected_beam_dp[24] = {
        10U, 16U, 13U, 26U, 90U, 26U, 82U, 26U,
        31U, 66U, 231U, 66U, 211U, 66U, 80U, 169U,
        592U, 169U, 140U, 541U, 169U, 204U, 433U, 433U
    };
    static const StarsU16 expected_beam_init[24] = {
        9U, 9U, 12U, 9U, 10U, 7U, 14U, 5U,
        12U, 9U, 10U, 7U, 14U, 5U, 13U, 9U,
        11U, 8U, 6U, 14U, 6U, 13U, 9U, 8U
    };
    static const StarsU16 expected_beam_range[24] = {
        1U, 1U, 2U, 1U, 0U, 2U, 3U, 3U,
        2U, 1U, 0U, 2U, 3U, 3U, 2U, 1U,
        0U, 2U, 3U, 3U, 3U, 2U, 1U, 2U
    };
    StarsAttackSection section;
    StarsAttackPackedPartSlot packed_part;
    StarsAttackToken target;
    StarsAttackInput input;
    BeamFixture fixture;
    StarsI16 checks;
    StarsI16 failures;
    StarsU16 item;

    memset(&section, 0, sizeof section);
    memset(&packed_part, 0, sizeof packed_part);
    memset(&target, 0, sizeof target);
    memset(&input, 0, sizeof input);
    memset(&fixture, 0, sizeof fixture);
    section.init = 4;
    section.part_group = 0x0010U;
    section.part_item = 0U;
    section.part_count = 1U;
    packed_part.group = 0x0010U;
    packed_part.item = 0U;
    packed_part.count = 1U;
    target.owner = 1;
    target.active_flags = 1U;
    target.target_mask = 1U;
    target.ship_count = 1U;
    target.resource_cost = 100L;
    target.hull_dp = 10L;
    target.beam_defense_percent = 100U;
    input.attacker_owner = 0;
    input.init = 4;
    input.attack_players = 2U;
    input.section_count = 1;
    input.sections = &section;
    input.packed_parts = &packed_part;
    input.packed_part_count = 1U;
    input.target_count = 1;
    input.targets = &target;
    input.ops.context = &fixture;
    input.ops.lookup_part = resolve_beam;
    input.ops.damage_token = ignore_typed;
    input.ops.raw_damage_token = capture_beam;

    checks = 0;
    failures = 0;
    for (item = 0U; item < 24U; ++item) {
        ++checks;
        if (stars_lookup_beam_dp(item) !=
                (StarsI16)expected_beam_dp[item]) ++failures;
        ++checks;
        if (stars_lookup_beam_init(item) !=
                (StarsI16)expected_beam_init[item]) ++failures;
        ++checks;
        if (stars_lookup_beam_range(item) !=
                (StarsI16)expected_beam_range[item]) ++failures;
    }
    ++checks;
    if (stars_lookup_beam_dp(24U) != -1) ++failures;
    ++checks;
    if (stars_lookup_beam_init(24U) != -1) ++failures;
    ++checks;
    if (stars_lookup_beam_range(24U) != -1) ++failures;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 0U,
            50U, 50U, 1U, 80U,
            39L, 1U, 0U)) ++failures;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 15U,
            0U, 50U, 0U, 100U,
            100L, 1U, 0U)) ++failures;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 0U,
            100U, 50U, 1U, 50U,
            49L, 1U, 0U)) ++failures;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 0U,
            100U, 50U, 4U, 100U,
            96L, 1U, 0U)) ++failures;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 21U,
            100U, 200U, 0U, 100U,
            100L, 2U, 0U)) ++failures;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 0U,
            50U, 50U, 1U, 0U,
            -1L, 0U, 0U)) ++failures;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 0U,
            100U, 50U, 501U, 100U,
            -1L, 0U, 0U)) ++failures;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 0U,
            50U, 50U, 1U, 80U,
            40L, 1U, 1U)) ++failures;

    /* Item 0 has dRangeMax=1. Raw FAttack adds one for grobj==1, so
     * distance 2 is included and distance 3 is excluded. */
    input.attacker_object_kind = 1U;
    beam_target_distance = 2;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 0U,
            100U, 50U, 0U, 100U,
            100L, 1U, 0U)) ++failures;
    beam_target_distance = 3;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 0U,
            100U, 50U, 0U, 100U,
            -1L, 0U, 0U)) ++failures;
    /* A fleet attacker (grobj==2) gets no +1 range allowance. */
    input.attacker_object_kind = 2U;
    beam_target_distance = 2;
    ++checks;
    if (!run_case(&input, &fixture, &target, &packed_part, 0U,
            100U, 50U, 0U, 100U,
            -1L, 0U, 0U)) ++failures;

#ifndef BEAM_SCALE_NATIVE
    printf("F_ATTACK_BEAM_SCALE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
