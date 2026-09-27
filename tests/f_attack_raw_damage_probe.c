#include "core.h"
#include <stdio.h>
#include <string.h>

#ifndef RAW_NATIVE
StarsI16 stars_dz_from_brc_brc(StarsU8 first, StarsU8 second)
{
    (void)first;
    (void)second;
    return 0;
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
#endif

typedef struct RawDamageTrace {
    StarsDamageApplyFleet fleet;
    StarsAttackToken *target_views;
    StarsI16 target_count;
    StarsI16 lookup_calls;
    StarsU16 lookup_group;
    StarsU16 lookup_item;
    StarsU16 lookup_count;
    StarsI16 raw_calls;
    StarsI32 raw_amount_before[4];
    StarsI32 raw_amount_after[4];
    StarsI16 raw_target[4];
    StarsU16 raw_weapon_flags;
    StarsI16 killed;
    StarsI16 shield_records;
    StarsI16 hull_records;
    StarsU32 shield_damage;
    StarsU32 hull_damage;
    StarsI16 use_live_token;
    StarsI16 target_reads;
} RawDamageTrace;

static StarsI16 lookup_raw_part(void *context, StarsU16 group,
    StarsU16 item, StarsU16 count, StarsAttackSection *section)
{
    RawDamageTrace *trace = (RawDamageTrace *)context;

    ++trace->lookup_calls;
    trace->lookup_group = group;
    trace->lookup_item = item;
    trace->lookup_count = count;
    if (group != 0x0010U || (item != 15U && item != 21U) || count != 2U)
        return 0;
    section->target_slots = 2U;
    section->battle_rating = 5;
    section->base_damage = 100L;
    section->raw_scaling = 1U;
    section->raw_capacity_percent = 50U;
    /* Deliberately wrong: production FAttack must take dp from _beam. */
    section->raw_part_dp = 50U;
    section->raw_distance = 10U;
    section->active = 1U;
    section->beam_passes = 1;
    return 1;
}

static StarsDamageApplyFleet *find_fleet(void *context, StarsI16 id)
{
    RawDamageTrace *trace = (RawDamageTrace *)context;
    (void)id;
    return &trace->fleet;
}

static StarsI16 read_live_target(void *context, StarsI16 target_index,
    StarsAttackToken *target)
{
    RawDamageTrace *trace = (RawDamageTrace *)context;

    ++trace->target_reads;
    if (target_index < 0 || target_index >= trace->target_count ||
            !trace->target_views || !target)
        return 0;
    *target = trace->target_views[target_index];
    return 1;
}

static void kill_ships(void *context, StarsDamageApplyToken *token,
    StarsI16 count, StarsDamageApplyFleet *fleet)
{
    RawDamageTrace *trace = (RawDamageTrace *)context;
    (void)token;
    (void)fleet;
    trace->killed = (StarsI16)(trace->killed + count);
}

static void record_damage(void *context, StarsI16 id, StarsU16 flags,
    StarsU32 damage, StarsU16 record_flags)
{
    RawDamageTrace *trace = (RawDamageTrace *)context;
    (void)id;
    (void)flags;
    (void)record_flags;
    if (trace->shield_records == 0) {
        ++trace->shield_records;
        trace->shield_damage = damage;
    } else {
        ++trace->hull_records;
        trace->hull_damage = damage;
    }
}

static StarsI16 apply_raw_damage(void *context, StarsI16 attacker,
    StarsI16 target, StarsI16 section, StarsI32 *amount,
    StarsI32 torpedo_damage, StarsU16 weapon_flags, StarsI16 shields_only,
    StarsU32 *torpedo_packets)
{
    RawDamageTrace *trace = (RawDamageTrace *)context;
    StarsDamageApplyToken local_token;
    StarsDamageApplyToken *token;
    StarsFDamageTokInput damage;
    StarsU32 beam;
    StarsI16 applied;
    StarsI16 call_index;

    (void)attacker;
    (void)section;
    call_index = trace->raw_calls;
    if (call_index < 4) {
        trace->raw_amount_before[call_index] = *amount;
        trace->raw_target[call_index] = target;
    }
    ++trace->raw_calls;
    trace->raw_weapon_flags = weapon_flags;
    token = &local_token;
    if (trace->use_live_token) {
        if (target < 0 || target >= trace->target_count ||
                !trace->target_views)
            return 0;
        memset(token, 0, sizeof *token);
        token->id = 17;
        token->ship_count = trace->target_views[target].ship_count;
        token->dp_shield = (StarsU32)
            trace->target_views[target].shield_points;
        token->hull_damage = (StarsU32)
            trace->target_views[target].hull_dp;
    } else {
        memset(token, 0, sizeof *token);
        token->id = 17;
        token->ship_count = 2U;
        token->dp_shield = 3U;
        token->hull_damage = 10U;
    }
    beam = (StarsU32)*amount;
    memset(&damage, 0, sizeof damage);
    damage.token = token;
    damage.beam_damage = &beam;
    damage.torpedo_damage = (StarsU32)torpedo_damage;
    damage.weapon_flags = weapon_flags;
    damage.shields_only = shields_only;
    damage.torpedo_packets = torpedo_packets;
    damage.find_fleet = find_fleet;
    damage.kill_ships = kill_ships;
    damage.record = record_damage;
    damage.context = trace;
    applied = stars_f_damage_tok(&damage);
    trace->fleet.ship_count = token->ship_count;
    if (trace->use_live_token && trace->target_views) {
        trace->target_views[target].ship_count = token->ship_count;
        trace->target_views[target].shield_points = token->dp_shield;
    }
    *amount = (StarsI32)beam;
    if (call_index < 4)
        trace->raw_amount_after[call_index] = *amount;
    return applied;
}

static StarsI16 ignore_typed_damage(void *context, StarsI16 attacker,
    StarsI16 target, StarsI16 section, StarsI32 amount, StarsI16 mode)
{
    (void)context;
    (void)attacker;
    (void)target;
    (void)section;
    (void)amount;
    (void)mode;
    return 0;
}

int main(void)
{
    StarsAttackSection section;
    StarsAttackToken target;
    StarsAttackInput input;
    RawDamageTrace trace;
    StarsU8 battle_record[6];
    StarsAttackPackedPartSlot packed_part;
    StarsI16 failures;

    memset(&section, 0, sizeof section);
    memset(&target, 0, sizeof target);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    memset(battle_record, 0, sizeof battle_record);
    section.init = 4;
    section.part_group = 0x0010U;
    section.part_item = 15U;
    section.part_count = 2U;
    target.owner = 1;
    target.active_flags = 1U;
    target.target_mask = 2U;
    target.battle_rating = 5;
    target.beam_defense_percent = 80U;
    target.resource_cost = 10L;
    target.ship_count = 2U;
    target.shield_points = 3L;
    target.hull_dp = 10L;
    input.attacker_index = 7;
    input.attacker_owner = 0;
    input.init = 4;
    input.attack_players = 0x0002U;
    input.section_count = 1;
    input.sections = &section;
    input.target_count = 1;
    input.targets = &target;
    packed_part.group = 0x0010U;
    packed_part.item = 15U;
    packed_part.count = 2U;
    input.packed_parts = &packed_part;
    input.packed_part_count = 1U;
    input.ops.lookup_part = lookup_raw_part;
    input.ops.context = &trace;
    input.ops.damage_token = ignore_typed_damage;
    input.ops.raw_damage_token = apply_raw_damage;
    input.battle_record = battle_record;
    failures = 0;
    if (!stars_f_attack(&input)) failures |= 1;
    if (trace.lookup_calls != 1 || trace.lookup_group != 0x0010U ||
            trace.lookup_item != 15U || trace.lookup_count != 2U)
        failures |= 16;
    if (section.raw_scaling != 0U || section.base_damage != 0L)
        failures |= 32;
    if (trace.raw_calls != 1 || trace.killed != 2 ||
            trace.fleet.ship_count != 0U) failures |= 2;
    if (trace.raw_weapon_flags != 1U) failures |= 64;
    if (trace.shield_records != 1 || trace.shield_damage != 6UL ||
            trace.hull_records != 1 || trace.hull_damage != 34UL) failures |= 4;
    if (input.hit_count != 1 || input.first_hit_target != 0 ||
            battle_record[2] != 1U || battle_record[5] != 0U) failures |= 8;
    memset(&trace, 0, sizeof trace);
    packed_part.item = 21U;
    if (!stars_f_attack(&input) || trace.raw_calls != 1 ||
            trace.raw_weapon_flags != 2U || trace.lookup_item != 21U)
        failures |= 128;

    memset(&target, 0, sizeof target);
    memset(&trace, 0, sizeof trace);
    memset(battle_record, 0, sizeof battle_record);
    target.owner = 1;
    target.active_flags = 1U;
    target.target_mask = 2U;
    target.battle_rating = 5;
    target.beam_defense_percent = 80U;
    target.resource_cost = 10L;
    target.ship_count = 2U;
    target.shield_points = 3L;
    target.hull_dp = 10L;
    trace.target_views = &target;
    trace.target_count = 1;
    trace.use_live_token = 1;
    input.attacker_ship_count = 1U;
    input.ops.context = &trace;
    input.ops.read_target = read_live_target;
    input.hit_count = 0;
    input.first_hit_target = -1;
    input.first_hit_section = -1;
    input.battle_record = battle_record;
    if (!stars_f_attack(&input) || trace.raw_calls != 1 ||
            trace.target_reads < 2 || target.ship_count != 0U ||
            target.shield_points != 0L || input.hit_count != 1)
        failures |= 256;

    memset(&target, 0, sizeof target);
    memset(&trace, 0, sizeof trace);
    memset(battle_record, 0, sizeof battle_record);
    {
        StarsAttackToken targets[2];

        memset(targets, 0, sizeof targets);
        targets[0].owner = 1;
        targets[0].active_flags = 1U;
        targets[0].target_mask = 2U;
        targets[0].battle_rating = 5;
        targets[0].beam_defense_percent = 80U;
        targets[0].resource_cost = 10L;
        targets[0].ship_count = 2U;
        targets[0].shield_points = 3L;
        targets[0].hull_dp = 10L;
        targets[1] = targets[0];
        targets[1].target_mask = 1U;
        trace.target_views = targets;
        trace.target_count = 2;
        trace.use_live_token = 1;
        input.ops.context = &trace;
        input.ops.read_target = read_live_target;
        input.targets = targets;
        input.target_count = 2;
        input.attacker_ship_count = 1U;
        input.hit_count = 0;
        input.first_hit_target = -1;
        input.first_hit_section = -1;
        input.battle_record = battle_record;
        section.target_mode_masks[0] = 2U;
        section.target_mode_masks[1] = 1U;
        section.raw_weapon_flags = 1U;
        if (stars_f_attack(&input)) failures |= 512;
        if (trace.raw_calls != 2) failures |= 1024;
        if (trace.target_reads < 8) failures |= 2048;
        if (targets[0].ship_count != 2U || targets[1].ship_count != 2U ||
                targets[0].shield_points != 0L ||
                targets[1].shield_points != 0L)
            failures |= 4096;
        if (input.hit_count != 0 || input.first_hit_target != -1)
            failures |= 8192;
        if (trace.raw_target[0] != 1 || trace.raw_target[1] != 0 ||
                trace.raw_amount_before[0] != 163L ||
                trace.raw_amount_after[0] != 157L ||
                trace.raw_amount_before[1] != 156L ||
                trace.raw_amount_after[1] != 150L)
            failures |= 16384;
        section.raw_weapon_flags = 0U;

        memset(targets, 0, sizeof targets);
        targets[0].owner = 1;
        targets[0].active_flags = 1U;
        targets[0].target_mask = 2U;
        targets[0].battle_rating = 5;
        targets[0].beam_defense_percent = 80U;
        targets[0].resource_cost = 10L;
        targets[0].ship_count = 2U;
        targets[0].shield_points = 3L;
        targets[0].hull_dp = 10L;
        targets[1] = targets[0];
        targets[1].target_mask = 1U;
        memset(&trace, 0, sizeof trace);
        trace.target_views = targets;
        trace.target_count = 2;
        trace.use_live_token = 1;
        input.attacker_ship_count = 1000U;
        input.ops.context = &trace;
        input.targets = targets;
        input.hit_count = 0;
        input.first_hit_target = -1;
        input.first_hit_section = -1;
        input.battle_record = battle_record;
        section.raw_weapon_flags = 1U;
        if (stars_f_attack(&input) || trace.raw_calls != 2 ||
                trace.raw_target[0] != 1 || trace.raw_target[1] != 0 ||
                trace.raw_amount_before[0] != 163200L ||
                trace.raw_amount_after[0] != 163194L ||
                trace.raw_amount_before[1] != 163193L ||
                trace.raw_amount_after[1] != 163187L ||
                targets[0].shield_points != 0L ||
                targets[1].shield_points != 0L)
            failures |= 32768;
        section.raw_weapon_flags = 0U;
    }
    {
        StarsAttackToken hull_target;

        memset(&hull_target, 0, sizeof hull_target);
        memset(&trace, 0, sizeof trace);
        memset(battle_record, 0, sizeof battle_record);
        hull_target.owner = 1;
        hull_target.active_flags = 1U;
        hull_target.target_mask = 2U;
        hull_target.battle_rating = 5;
        hull_target.beam_defense_percent = 80U;
        hull_target.resource_cost = 10L;
        hull_target.ship_count = 2U;
        hull_target.shield_points = 3L;
        hull_target.hull_dp = 10L;
        trace.target_views = &hull_target;
        trace.target_count = 1;
        trace.use_live_token = 1;
        input.attacker_ship_count = 1U;
        input.ops.context = &trace;
        input.ops.read_target = read_live_target;
        input.targets = &hull_target;
        input.target_count = 1;
        input.hit_count = 0;
        input.first_hit_target = -1;
        input.first_hit_section = -1;
        input.battle_record = battle_record;
        section.raw_weapon_flags = 0U;
        if (!stars_f_attack(&input) || trace.raw_calls != 1 ||
                trace.killed != 2 || hull_target.ship_count != 0U ||
                hull_target.shield_points != 0L || input.hit_count != 1 ||
                input.first_hit_target != 0 ||
                trace.raw_amount_before[0] <= 0L ||
                trace.raw_amount_after[0] != 0L ||
                trace.shield_records != 1 || trace.shield_damage != 6U ||
                trace.hull_records != 1 || trace.hull_damage < 20U)
            failures |= 65536;
    }
#ifndef RAW_NATIVE
    printf("F_ATTACK_RAW_DAMAGE_CHECKS=15 FAILURES=%d RESULT=%s\n",
        (int)failures, failures ? "FAIL" : "PASS");
#endif
    return failures;
}
