#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ComplexSelectionTrace {
    StarsI16 calls;
    StarsI16 target;
    StarsI32 amount;
} ComplexSelectionTrace;

static StarsI16 record_damage(void *context, StarsI16 attacker,
    StarsI16 target, StarsI16 section, StarsI32 amount, StarsI16 mode)
{
    ComplexSelectionTrace *trace;
    (void)attacker;
    (void)section;
    (void)mode;
    trace = (ComplexSelectionTrace *)context;
    ++trace->calls;
    trace->target = target;
    trace->amount = amount;
    return 1;
}

int main(void)
{
    StarsAttackSection section;
    StarsAttackToken targets[2];
    StarsAttackInput input;
    StarsAttackOps ops;
    StarsU8 battle_record[6];
    ComplexSelectionTrace trace;
    StarsI16 failures;

    memset(&section, 0, sizeof section);
    memset(targets, 0, sizeof targets);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(battle_record, 0, sizeof battle_record);
    memset(&trace, 0, sizeof trace);
    section.init = 4;
    section.target_slots = 3U;
    section.battle_rating = 5;
    section.base_damage = 100L;
    section.raw_capacity_percent = 50U;
    section.raw_part_dp = 50U;
    section.raw_distance = 10U;
    section.active = 1U;
    section.beam_passes = 1;
    section.raw_scaling = 1U;
    targets[0].owner = 1;
    targets[0].active_flags = 1U;
    targets[0].target_mask = 3U;
    targets[0].battle_rating = 5;
    targets[0].beam_defense_percent = 100U;
    targets[0].resource_cost = 10L;
    targets[0].ship_count = 1U;
    targets[0].shield_points = 10L;
    targets[0].hull_dp = 100L;
    targets[1] = targets[0];
    targets[1].owner = 2;
    targets[1].resource_cost = 100L;
    targets[1].beam_defense_percent = 80U;
    ops.damage_token = record_damage;
    ops.context = &trace;
    input.attacker_index = 7;
    input.attacker_owner = 0;
    input.init = 4;
    input.attack_players = 0x0006U;
    input.section_count = 1;
    input.sections = &section;
    input.target_count = 2;
    input.targets = targets;
    input.ops = ops;
    input.battle_record = battle_record;
    failures = 0;
    if (!stars_f_attack(&input) || trace.calls != 1 || trace.target != 1)
        failures |= 1;
    if (trace.amount != 39L)
        failures |= 2;
    if (input.hit_count != 1 || input.first_hit_target != 1 ||
            battle_record[5] != 1U)
        failures |= 4;
    puts("F_ATTACK_COMPLEX_SELECTION_CHECKS=3 FAILURES=0 RESULT=PASS");
    return failures;
}
