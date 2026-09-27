#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 calls;
    StarsI16 dispatches;
    StarsI16 targets[8];
    StarsI16 attacker;
    StarsI16 target;
    StarsI16 section;
    StarsI16 mode;
    StarsI16 reject_hull;
    StarsI32 damage;
} Trace;
static StarsI16 damage(void *c, StarsI16 attacker, StarsI16 target,
    StarsI16 section, StarsI32 amount, StarsI16 mode)
{
    Trace *t = (Trace *)c;
    t->attacker = attacker;
    t->target = target;
    t->section = section;
    t->damage = amount;
    t->mode = mode;
    ++t->dispatches;
    if (t->reject_hull && mode == 4)
        return 0;
    if (t->calls < 8)
        t->targets[t->calls] = target;
    ++t->calls;
    return 1;
}
static StarsI16 random_fifty(void *c, StarsI16 limit)
{
    (void)c;
    (void)limit;
    return 50;
}
int main(void)
{
    StarsAttackSection sections[4];
    StarsAttackToken targets[3];
    StarsAttackInput input;
    StarsU8 battle_record[6];
    Trace trace;
    memset(&sections, 0, sizeof sections);
    memset(&targets, 0, sizeof targets);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    memset(battle_record, 0, sizeof battle_record);
    battle_record[4] = 0x50U;
    sections[0].init = 4;
    sections[0].weapon_mode = 0;
    sections[0].target_slot = 1;
    sections[0].battle_rating = 5;
    sections[0].base_damage = 20L;
    sections[0].fuel_factor = 2L;
    sections[0].active = 1;
    sections[0].beam_passes = 1;
    sections[1].init = 4;
    sections[1].weapon_mode = 1;
    sections[1].target_slot = 2;
    sections[1].battle_rating = 5;
    sections[1].base_damage = 30L;
    sections[1].fuel_factor = 1L;
    sections[1].cargo_flags = 1;
    sections[1].active = 1;
    sections[1].beam_passes = 1;
    sections[2].init = 7;
    sections[2].active = 1;
    sections[3].init = 4;
    sections[3].weapon_mode = 2;
    sections[3].target_slot = 3;
    sections[3].battle_rating = 5;
    sections[3].torpedo_count = 10L;
    sections[3].torpedo_base_percent = 100;
    sections[3].torpedo_weapon_flags = 4;
    sections[3].torpedo_passes = 2;
    sections[3].torpedo_shield_damage = 3L;
    sections[3].torpedo_hull_damage = 5L;
    sections[3].active = 1;
    targets[0].owner = 1;
    targets[0].active_flags = 1;
    targets[0].target_mask = 2;
    targets[0].battle_rating = 3;
    targets[0].beam_defense_percent = 50;
    targets[1].owner = 1;
    targets[1].active_flags = 1;
    targets[1].target_mask = 12;
    targets[1].battle_rating = 4;
    targets[1].beam_defense_percent = 100;
    targets[1].torpedo_jam_percent = 50;
    targets[2].owner = 1;
    targets[2].active_flags = 1;
    targets[2].target_mask = 8;
    targets[2].battle_rating = 2;
    targets[2].torpedo_jam_percent = 0;
    targets[2].resource_cost = 100L;
    targets[2].ship_count = 1U;
    targets[2].hull_dp = 100L;
    input.attacker_index = 7;
    input.attacker_owner = 0;
    input.init = 4;
    input.attack_players = (StarsU16)((1U << 1) | (1U << 2));
    input.section_count = 4;
    input.sections = sections;
    input.target_count = 3;
    input.targets = targets;
    input.ops.damage_token = damage;
    input.ops.torpedo_random = random_fifty;
    input.ops.context = &trace;
    input.battle_record = battle_record;
    if (!stars_f_attack(&input) || trace.calls != 4 ||
            trace.attacker != 7 || trace.target != 2 || trace.section != 3 ||
            trace.mode != 4 || trace.damage != 50L ||
            input.first_hit_target != 0 || input.first_hit_section != 0 ||
            input.hit_count != 4 || battle_record[2] != 4U ||
            battle_record[3] != 0U || battle_record[4] != 0x50U ||
            battle_record[5] != 0U)
        return 1;
    memset(&trace, 0, sizeof trace);
    sections[0].target_slots = (StarsU16)((1U << 1) | (1U << 2));
    sections[0].beam_passes = 2;
    targets[1].beam_defense_percent = 50;
    input.section_count = 1;
    input.target_count = 2;
    if (!stars_f_attack(&input) || trace.calls != 2 ||
            input.hit_count != 2 || input.first_hit_target != 0 ||
            input.first_hit_section != 0 || trace.targets[0] != 0 ||
            trace.targets[1] != 0)
        return 1;
    memset(&trace, 0, sizeof trace);
    sections[0].beam_broadcast = 1;
    sections[0].beam_passes = 2;
    if (!stars_f_attack(&input) || trace.calls != 2 ||
            input.hit_count != 2 || input.first_hit_target != 0 ||
            trace.targets[0] != 0 || trace.targets[1] != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(&sections[0], 0, sizeof sections[0]);
    sections[0].init = 4;
    sections[0].weapon_mode = 0;
    sections[0].target_slots = (StarsU16)((1U << 1) | (1U << 2));
    sections[0].battle_rating = 5;
    sections[0].base_damage = 20L;
    sections[0].active = 1;
    sections[0].beam_passes = 1;
    sections[0].range_enabled = 1U;
    sections[0].target_range = 1U;
    input.attacker_coordinate = 0x00U;
    targets[0].battle_coordinate = 0x00U;
    targets[1].battle_coordinate = 0x22U;
    targets[0].battle_rating = 99;
    input.section_count = 1;
    input.target_count = 2;
    if (!stars_f_attack(&input) || trace.calls != 1 ||
            trace.targets[0] != 0 || input.hit_count != 1)
        return 1;
    targets[0].battle_rating = 3;
    memset(&trace, 0, sizeof trace);
    memset(&sections[0], 0, sizeof sections[0]);
    sections[0].init = 4;
    sections[0].weapon_mode = 0;
    sections[0].target_mode_masks[0] = (StarsU16)(1U << 1);
    sections[0].target_mode_masks[1] = (StarsU16)(1U << 2);
    sections[0].battle_rating = 5;
    sections[0].base_damage = 20L;
    sections[0].active = 1;
    sections[0].beam_passes = 2;
    input.section_count = 1;
    input.target_count = 3;
    if (!stars_f_attack(&input) || trace.calls != 2 ||
            input.hit_count != 2 || trace.targets[0] != 1 ||
            trace.targets[1] != 0)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(&sections[0], 0, sizeof sections[0]);
    sections[0].init = 4;
    sections[0].weapon_mode = 1;
    sections[0].target_slot = 2;
    sections[0].battle_rating = 5;
    sections[0].base_damage = 40L;
    sections[0].active = 1;
    sections[0].beam_passes = 1;
    input.target_count = 2;
    if (!stars_f_attack(&input) || trace.calls != 1 ||
            trace.damage != 20L || trace.target != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(&sections[0], 0, sizeof sections[0]);
    sections[0].init = -1;
    sections[0].init_base = 60;
    sections[0].init_delta = 7;
    sections[0].weapon_mode = 0;
    sections[0].target_slot = 2;
    sections[0].battle_rating = 5;
    sections[0].base_damage = 20L;
    sections[0].active = 1;
    input.init = 63;
    if (!stars_f_attack(&input) || trace.calls != 1 ||
            trace.damage != 10L || trace.target != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(&sections[0], 0, sizeof sections[0]);
    sections[0].init = -1;
    sections[0].init_base = 0;
    sections[0].init_delta = -1;
    sections[0].weapon_mode = 0;
    sections[0].target_slot = 2;
    sections[0].battle_rating = 5;
    sections[0].base_damage = 20L;
    sections[0].active = 1;
    input.init = 63;
    if (!stars_f_attack(&input) || trace.calls != 1 ||
            trace.damage != 10L || trace.target != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(&sections[0], 0, sizeof sections[0]);
    sections[0].init = 4;
    sections[0].weapon_mode = 2;
    sections[0].target_slot = 3;
    sections[0].battle_rating = 5;
    sections[0].torpedo_count = 10L;
    sections[0].torpedo_base_percent = 100;
    sections[0].torpedo_passes = 0;
    sections[0].torpedo_weapon_flags = 4;
    sections[0].torpedo_shield_damage = 1L;
    sections[0].active = 1;
    input.init = 4;
    input.target_count = 3;
    if (!stars_f_attack(&input) || trace.calls != 1 ||
            input.hit_count != 1 || trace.target != 2)
        return 1;
    memset(&trace, 0, sizeof trace);
    sections[0].torpedo_hull_damage = 1L;
    trace.reject_hull = 1;
    if (!stars_f_attack(&input) || trace.dispatches != 2 ||
            trace.calls != 1 ||
            input.hit_count != 2 || trace.mode != 4)
        return 7;
    puts("F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS");
    return 0;
}
