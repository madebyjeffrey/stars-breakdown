#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct TorpedoTrace {
    StarsI16 random_calls;
    StarsI16 damage_calls;
    StarsI16 target;
    StarsI32 damage;
} TorpedoTrace;

static StarsI16 hit_every_torpedo(void *context, StarsI16 limit)
{
    TorpedoTrace *trace;

    trace = (TorpedoTrace *)context;
    ++trace->random_calls;
    (void)limit;
    return 0;
}

static StarsI16 record_torpedo_damage(void *context, StarsI16 attacker,
    StarsI16 target, StarsI16 section, StarsI32 damage, StarsI16 flags)
{
    TorpedoTrace *trace;

    trace = (TorpedoTrace *)context;
    ++trace->damage_calls;
    trace->target = target;
    trace->damage = damage;
    (void)attacker;
    (void)section;
    (void)flags;
    return 1;
}

int main(void)
{
    StarsAttackSection section;
    StarsAttackToken targets[2];
    StarsAttackInput input;
    StarsU8 battle_record[6];
    TorpedoTrace trace;
    StarsI16 failures;

    memset(&section, 0, sizeof section);
    memset(targets, 0, sizeof targets);
    memset(&input, 0, sizeof input);
    memset(battle_record, 0, sizeof battle_record);
    memset(&trace, 0, sizeof trace);

    section.init = 4;
    section.part_item = 8U;
    section.weapon_mode = 2;
    section.target_slot = 0;
    section.active = 1U;
    section.torpedo_count = 3L;
    section.torpedo_base_percent = 50;
    section.battle_computer_percent = 60;
    section.torpedo_passes = 1;
    section.torpedo_weapon_flags = 4U;
    section.torpedo_shield_damage = 2L;
    targets[0].owner = 1;
    targets[0].active_flags = 1U;
    targets[0].target_mask = 1U;
    targets[0].hull_dp = 100L;
    targets[0].ship_count = 1U;
    targets[0].resource_cost = 10L;
    targets[1] = targets[0];
    targets[1].resource_cost = 100L;
    targets[1].torpedo_jam_percent = 50U;

    input.attacker_owner = 0;
    input.init = 4;
    input.attack_players = 2U;
    input.section_count = 1;
    input.sections = &section;
    input.target_count = 2;
    input.targets = targets;
    input.ops.damage_token = record_torpedo_damage;
    input.ops.torpedo_random = hit_every_torpedo;
    input.ops.context = &trace;
    input.battle_record = battle_record;

    failures = 0;
    if (!stars_f_attack(&input)) failures |= 1;
    if (trace.target != 1 || input.first_hit_target != 1 ||
            battle_record[5] != 1U) failures |= 2;
    if (trace.random_calls != 3) failures |= 4;
    if (trace.damage_calls != 1 || trace.damage != 6L ||
            input.hit_count != 1) failures |= 8;
    memset(&trace, 0, sizeof trace);
    section.battle_computer_percent = 0;
    targets[0].torpedo_jam_percent = 100U;
    targets[1].torpedo_jam_percent = 100U;
    if (stars_f_attack(&input) || trace.random_calls != 0 ||
            trace.damage_calls != 0 || input.hit_count != 0)
        failures |= 16;
    if (failures != 0) {
        printf("F_ATTACK_TORPEDO_TARGET_CHECKS=5 FAILURES=%d RESULT=FAIL "
            "DAMAGE_CALLS=%d DAMAGE=%ld HIT_COUNT=%d\n", failures,
            trace.damage_calls, (long)trace.damage, input.hit_count);
        return 1;
    }
#ifndef TORPEDO_TARGET_NATIVE
    puts("F_ATTACK_TORPEDO_TARGET_CHECKS=5 FAILURES=0 RESULT=PASS");
#endif
    return 0;
}
