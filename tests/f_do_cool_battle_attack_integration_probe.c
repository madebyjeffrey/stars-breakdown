#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU8 packed_tokens[2][29];
    StarsU8 dgroup[0x190];
    StarsU8 hull_record[123];
    StarsCoolBattleToken *tokens;
    StarsI16 attack_calls;
    StarsI16 damage_calls;
    StarsI16 move_calls;
    StarsI16 move_events;
    StarsI16 attack_events;
    StarsI16 header_calls;
    StarsI16 report_calls;
    StarsI16 release_calls;
    StarsI16 last_attacker;
    StarsI16 last_target;
    StarsI32 last_damage;
    StarsI16 attack_index;
    StarsI16 attack_owner;
    StarsI16 attack_initiative;
    StarsU16 attack_players;
    StarsU8 attack_object_kind;
    StarsU8 attack_coordinate;
    StarsU8 target_coordinate;
    StarsI16 part_lookup_calls;
    StarsU16 part_group;
    StarsU16 part_item;
    StarsU16 part_count;
    StarsI16 hull_resolve_calls;
    StarsU16 hull_segment;
    StarsU16 hull_offset;
    StarsCoolBattleReportSummary summary;
} Trace;

static void put_word(StarsU8 *bytes, StarsI16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)(value & 0xffU);
    bytes[offset + 1] = (StarsU8)(value >> 8);
}

static StarsU16 get_word(const StarsU8 *bytes, StarsI16 offset)
{
    return (StarsU16)(bytes[offset] |
        ((StarsU16)bytes[offset + 1] << 8));
}

static StarsI16 allocate(void *context, StarsU16 bytes,
    StarsI16 allocation_type)
{
    (void)context;
    (void)bytes;
    (void)allocation_type;
    return 1;
}

static StarsI16 initialize(void *context, const StarsCoolBattleFleet *fleets,
    StarsI16 fleet_count, StarsI16 player_count, StarsU16 player_mask,
    StarsCoolBattleToken *tokens, StarsI16 token_capacity,
    StarsI16 *token_count, StarsI16 *round_count, StarsI16 *initiative_min,
    StarsI16 *initiative_max)
{
    Trace *trace = (Trace *)context;
    (void)fleets;
    (void)player_count;
    (void)player_mask;
    if (!tokens || token_capacity < 2 || fleet_count != 2)
        return 0;
    memset(tokens, 0, (size_t)token_capacity * sizeof *tokens);
    tokens[0].id = 100;
    tokens[0].player = 0;
    tokens[0].active = 1;
    tokens[0].count = 1;
    tokens[0].object_kind = 1U;
    tokens[0].init_min = 2;
    tokens[0].init_max = 2;
    tokens[0].battle_coordinate = 0x10U;
    tokens[1].id = 101;
    tokens[1].player = 1;
    tokens[1].active = 1;
    tokens[1].count = 1;
    tokens[1].object_kind = 2U;
    tokens[1].init_min = 2;
    tokens[1].init_max = 2;
    tokens[1].battle_coordinate = 0x20U;
    trace->tokens = tokens;
    *token_count = 2;
    *round_count = 0;
    *initiative_min = 2;
    *initiative_max = 2;
    return 1;
}

static StarsI16 move_token(void *context, StarsI16 round,
    StarsCoolBattleToken *token)
{
    Trace *trace = (Trace *)context;
    (void)round;
    (void)token;
    ++trace->move_calls;
    return 1;
}

static StarsI16 initiative_available(void *context, StarsI16 initiative)
{
    (void)context;
    return initiative == 2;
}

static StarsI16 damage_token(void *context, StarsI16 attacker,
    StarsI16 target, StarsI16 section, StarsI32 amount, StarsI16 mode)
{
    Trace *trace = (Trace *)context;
    (void)section;
    (void)mode;
    ++trace->damage_calls;
    trace->last_attacker = attacker;
    trace->last_target = target;
    trace->last_damage = amount;
    if (trace->tokens && target >= 0 && target < 2) {
        trace->tokens[target].count = 0;
        trace->tokens[target].active = 0;
    }
    return 1;
}

static StarsI16 lookup_part(void *context, StarsU16 group, StarsU16 item,
    StarsU16 count, StarsAttackSection *section)
{
    Trace *trace = (Trace *)context;
    ++trace->part_lookup_calls;
    trace->part_group = group;
    trace->part_item = item;
    trace->part_count = count;
    if (group != 0x10U || item != 7U || count != 2U)
        return 0;
    section->init = 2;
    section->base_damage = 5L;
    section->active = 1U;
    section->beam_passes = 1;
    return 1;
}

static const StarsU8 *resolve_far_hull(void *context, StarsU16 segment,
    StarsU16 offset)
{
    Trace *trace = (Trace *)context;
    ++trace->hull_resolve_calls;
    trace->hull_segment = segment;
    trace->hull_offset = offset;
    if ((segment == 0x1234U && offset == 0x2126U) ||
            (segment == 0x2345U && offset == 0x12dfU))
        return trace->hull_record;
    return (const StarsU8 *)0;
}

static StarsI16 attack_token(void *context, StarsI16 round,
    StarsI16 initiative, StarsI16 index, StarsCoolBattleToken *token,
    StarsU16 surviving)
{
    Trace *trace = (Trace *)context;
    StarsAttackSection section;
    StarsAttackToken targets[2];
    StarsAttackInput attack;
    StarsAttackPackedPartSlot slots[1];
    StarsI16 target_index;
    StarsI16 target;

    (void)round;
    ++trace->attack_calls;
    target = -1;
    for (target_index = 0; target_index < 2; ++target_index)
        if (target_index != index && trace->tokens[target_index].active) {
            target = target_index;
            break;
        }
    if (target < 0)
        return 1;
    memset(&section, 0, sizeof section);
    memset(targets, 0, sizeof targets);
    memset(&attack, 0, sizeof attack);
    section.init = initiative;
    /* Deliberately conflict with the resolved raw _hs bytes below. */
    section.part_group = 0x20U;
    section.part_item = 4U;
    section.part_count = 1U;
    section.weapon_mode = 0;
    section.target_slot = target;
    section.battle_rating = 0;
    section.base_damage = 5L;
    section.active = 1;
    section.beam_passes = 1;
    for (target_index = 0; target_index < 2; ++target_index) {
        targets[target_index].owner = trace->tokens[target_index].player;
        targets[target_index].battle_coordinate =
            trace->tokens[target_index].battle_coordinate;
        targets[target_index].active_flags = trace->tokens[target_index].active;
        targets[target_index].target_mask = (StarsU16)(1U << target_index);
        targets[target_index].battle_rating = 0;
        targets[target_index].beam_defense_percent = 100;
    }
    if (!stars_attack_bind_cool_battle_global_hull(&attack, index,
            initiative, surviving, token, trace->dgroup, resolve_far_hull,
            trace, slots, 16U))
        return 0;
    trace->attack_index = attack.attacker_index;
    trace->attack_owner = attack.attacker_owner;
    trace->attack_initiative = attack.init;
    trace->attack_players = attack.attack_players;
    trace->attack_object_kind = attack.attacker_object_kind;
    trace->attack_coordinate = attack.attacker_coordinate;
    trace->target_coordinate = targets[target].battle_coordinate;
    attack.section_count = 1;
    attack.sections = &section;
    attack.target_count = 2;
    attack.targets = targets;
    attack.ops.damage_token = damage_token;
    attack.ops.lookup_part = lookup_part;
    attack.ops.context = trace;
    return stars_f_attack(&attack) ? 1 : 0;
}

static StarsI16 write_header(void *context,
    const StarsCoolBattleHeader *header)
{
    (void)header;
    ++((Trace *)context)->header_calls;
    return 1;
}

static StarsI16 write_event(void *context,
    const StarsCoolBattleEvent *event)
{
    Trace *trace = (Trace *)context;
    if (event->kind == 1)
        ++trace->move_events;
    if (event->kind == 2)
        ++trace->attack_events;
    return 1;
}

static StarsI16 send_report(void *context,
    const StarsCoolBattleReportSummary *summary)
{
    Trace *trace = (Trace *)context;
    ++trace->report_calls;
    trace->summary = *summary;
    return 1;
}

static void release(void *context)
{
    ++((Trace *)context)->release_calls;
}

int main(void)
{
    StarsCoolBattleFleet fleets[2];
    StarsCoolBattleToken tokens[2];
    StarsCoolBattleInput input;
    StarsCoolBattleOps ops;
    StarsU16 attack_masks[2];
    Trace trace;
    Trace normal_lookup;
    StarsCoolBattleToken normal_token;
    StarsAttackInput normal_attack;
    StarsAttackPackedPartSlot normal_slots[16];

    memset(fleets, 0, sizeof fleets);
    memset(tokens, 0, sizeof tokens);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    put_word(trace.dgroup, 0x0150, 0x2000U);
    put_word(trace.dgroup, 0x0152, 0x1234U);
    trace.hull_record[58] = 0x10U;
    trace.hull_record[60] = 7U;
    trace.hull_record[61] = 2U;
    trace.hull_record[122] = 1U;
    put_word(trace.packed_tokens[0], 0, 100U);
    trace.packed_tokens[0][2] = 0U;
    trace.packed_tokens[0][3] = 1U;
    trace.packed_tokens[0][5] = 0x10U;
    trace.packed_tokens[0][7] = 2U;
    trace.packed_tokens[0][8] = 2U;
    put_word(trace.packed_tokens[0], 19, 1U);
    put_word(trace.packed_tokens[0], 27, 1U);
    put_word(trace.packed_tokens[1], 0, 101U);
    trace.packed_tokens[1][2] = 1U;
    trace.packed_tokens[1][3] = 2U;
    trace.packed_tokens[1][4] = 0x12U;
    trace.packed_tokens[1][5] = 0x20U;
    trace.packed_tokens[1][7] = 2U;
    trace.packed_tokens[1][8] = 2U;
    put_word(trace.packed_tokens[1], 19, 1U);
    put_word(trace.packed_tokens[1], 27, 1U);
    fleets[0].owner = 0;
    fleets[0].planet_id = 12;
    fleets[0].point.x = 20;
    fleets[0].point.y = 30;
    fleets[0].ship_count = 1;
    fleets[1].owner = 1;
    fleets[1].planet_id = 12;
    fleets[1].ship_count = 1;
    attack_masks[0] = 2;
    attack_masks[1] = 1;
    ops.allocate = allocate;
    ops.initialize = initialize;
    ops.move_token = move_token;
    ops.initiative_available = initiative_available;
    ops.attack_token = attack_token;
    ops.write_header = write_header;
    ops.write_event = write_event;
    ops.send_messages_ex = send_report;
    ops.release = release;
    ops.context = &trace;
    input.fleets = fleets;
    input.fleet_count = 2;
    input.player_count = 2;
    input.player_mask = 3;
    input.attack_masks = attack_masks;
    input.tokens = tokens;
    input.token_capacity = 2;
    input.packed_tokens = &trace.packed_tokens[0][0];
    input.packed_token_stride = 29U;
    input.packed_token_capacity = 2U;
    input.battle_id = 0x220;
    input.ops = ops;

    if (!stars_f_do_cool_battle(&input) || trace.attack_calls != 1 ||
            trace.damage_calls != 1 || trace.last_attacker != 1 ||
            trace.last_target != 0 || trace.last_damage != 5L ||
            trace.move_calls != 2 || trace.move_events != 2 ||
            trace.attack_events != 1 || trace.header_calls != 1 ||
            trace.report_calls != 1 || trace.release_calls != 1 ||
            trace.attack_index != 1 || trace.attack_owner != 1 ||
            trace.attack_initiative != 2 || trace.attack_players != 3U ||
            trace.attack_object_kind != 2U ||
            trace.part_lookup_calls != 1 || trace.part_group != 0x10U ||
            trace.part_item != 7U || trace.part_count != 2U ||
            trace.hull_resolve_calls != 1 ||
            trace.hull_segment != 0x1234U ||
            trace.hull_offset != 0x2126U ||
            trace.attack_coordinate != 0x20U ||
            trace.target_coordinate != 0x10U ||
            trace.packed_tokens[0][3] != 1U ||
            trace.packed_tokens[1][3] != 2U ||
            get_word(trace.packed_tokens[0], 19) != 0U ||
            (get_word(trace.packed_tokens[0], 27) & 1U) != 0U ||
            tokens[0].active != 0 || tokens[0].count != 0 ||
            tokens[1].active == 0 || input.surviving_players != 2 ||
            trace.summary.battle_id != 0x220 ||
            trace.summary.round_count != 0)
        return 1;
    memset(&normal_lookup, 0, sizeof normal_lookup);
    memset(&normal_token, 0, sizeof normal_token);
    memset(&normal_attack, 0, sizeof normal_attack);
    memset(normal_slots, 0, sizeof normal_slots);
    normal_token.player = 3;
    normal_token.design = 5;
    put_word(normal_lookup.dgroup, 0x010a, 0x1000U);
    put_word(normal_lookup.dgroup, 0x010c, 0x2345U);
    if (!stars_attack_bind_cool_battle_global_hull(&normal_attack, 0, 2,
            0xffffU, &normal_token, normal_lookup.dgroup, resolve_far_hull,
            &normal_lookup, normal_slots, 16U) ||
            normal_lookup.hull_resolve_calls != 1 ||
            normal_lookup.hull_segment != 0x2345U ||
            normal_lookup.hull_offset != 0x12dfU ||
            normal_attack.packed_part_count != 0U)
        return 1;
    puts("F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=44 FAILURES=0 RESULT=PASS");
    return 0;
}
