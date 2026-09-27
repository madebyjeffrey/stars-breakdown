#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TORPEDO_TARGET_NATIVE
StarsI16 stars_dz_from_brc_brc(StarsU8 first, StarsU8 second)
{
    StarsI16 dx;
    StarsI16 dy;

    dx = (StarsI16)(first & 0x0fU) - (StarsI16)(second & 0x0fU);
    dy = (StarsI16)(first >> 4) - (StarsI16)(second >> 4);
    if (dx < 0) dx = (StarsI16)-dx;
    if (dy < 0) dy = (StarsI16)-dy;
    return dx > dy ? dx : dy;
}
#endif

static StarsU8 segment2[25256];

static StarsI16 read_segment(const char *path)
{
    FILE *file;
    size_t count;
    int failed;

    file = fopen(path, "rb");
    if (!file)
        return 0;
    count = fread(segment2, 1U, sizeof segment2, file);
    failed = ferror(file);
    if (!failed && fgetc(file) != EOF)
        failed = 1;
    if (fclose(file) != 0)
        failed = 1;
    return (StarsI16)(!failed && count == sizeof segment2);
}

typedef struct RawTorpedoTrace {
    StarsI16 calls;
    StarsI16 lookup_calls;
    StarsU16 lookup_group;
    StarsU16 lookup_item;
    StarsU16 lookup_count;
    StarsI16 shield_calls;
    StarsI16 hull_calls;
    StarsU16 flags[2];
    StarsI16 shields_only[2];
    StarsI32 beam_damage[2];
    StarsI32 torpedo_damage[2];
    StarsU32 packets_before;
    StarsU32 packets_after;
    StarsI16 typed_calls;
    StarsI16 random_calls;
    StarsI16 record_lookup_calls;
    StarsU16 record_offset;
} RawTorpedoTrace;

static StarsI16 lookup_torpedo_part(void *context, StarsU16 group,
    StarsU16 item, StarsU16 count, StarsAttackSection *section)
{
    RawTorpedoTrace *trace;

    trace = (RawTorpedoTrace *)context;
    ++trace->lookup_calls;
    trace->lookup_group = group;
    trace->lookup_item = item;
    trace->lookup_count = count;
    if (group != 0x0020U || item > 11U || count != 1U)
        return 0;
    section->active = 1U;
    section->raw_torpedo_power = 16U;
    return 1;
}

static StarsI16 torpedo_tech_status(void *context, StarsU32 component)
{
    (void)context;
    (void)component;
    return 1;
}

static const StarsU8 *lookup_torpedo_record(void *context, StarsU16 group,
    StarsU16 item, StarsU16 *record_size)
{
    RawTorpedoTrace *trace;
    StarsLookupPartInput part;

    trace = (RawTorpedoTrace *)context;
    ++trace->record_lookup_calls;
    memset(&part, 0, sizeof part);
    part.group = group;
    part.item = item;
    part.player = -1;
    part.tech_status = torpedo_tech_status;
    part.context = context;
    if (!stars_lookup_part(&part))
        return (const StarsU8 *)0;
    trace->record_offset = part.table_offset;
    *record_size = part.table_stride;
    return stars_lookup_part_record(&part, segment2,
        (StarsU16)sizeof segment2);
}

static StarsI16 selective_random_draw(void *context, StarsI16 limit)
{
    RawTorpedoTrace *trace;

    trace = (RawTorpedoTrace *)context;
    ++trace->random_calls;
    (void)limit;
    return trace->random_calls == 2 ? 99 : 0;
}

static StarsI16 raw_torpedo_damage(void *context, StarsI16 attacker,
    StarsI16 target, StarsI16 section, StarsI32 *beam_damage,
    StarsI32 torpedo_damage, StarsU16 weapon_flags, StarsI16 shields_only,
    StarsU32 *torpedo_packets)
{
    RawTorpedoTrace *trace;
    StarsI16 index;

    trace = (RawTorpedoTrace *)context;
    index = trace->calls;
    if (index < 2) {
        trace->beam_damage[index] = *beam_damage;
        trace->torpedo_damage[index] = torpedo_damage;
        trace->flags[index] = weapon_flags;
        trace->shields_only[index] = shields_only;
    }
    ++trace->calls;
    (void)attacker;
    (void)target;
    (void)section;
    if (shields_only) {
        ++trace->shield_calls;
        return 1;
    }
    ++trace->hull_calls;
    trace->packets_before = torpedo_packets ? *torpedo_packets : 0UL;
    if (torpedo_packets)
        --*torpedo_packets;
    trace->packets_after = torpedo_packets ? *torpedo_packets : 0UL;
    *beam_damage = 0L;
    return 0;
}

static StarsI16 typed_damage_must_not_run(void *context,
    StarsI16 attacker, StarsI16 target, StarsI16 section, StarsI32 damage,
    StarsI16 mode)
{
    RawTorpedoTrace *trace;
    trace = (RawTorpedoTrace *)context;
    ++trace->typed_calls;
    (void)attacker;
    (void)target;
    (void)section;
    (void)damage;
    (void)mode;
    return 0;
}

int f_attack_raw_torpedo_dispatch_probe(const char *segment_path)
{
    static const StarsU16 expected_dp[12] = {
        5U, 12U, 26U, 48U, 90U, 169U,
        316U, 60U, 85U, 150U, 280U, 525U
    };
    static const StarsU16 expected_init[12] = {
        0U, 1U, 1U, 2U, 2U, 3U, 4U, 0U, 0U, 1U, 2U, 3U
    };
    static const StarsU16 expected_range[12] = {
        4U, 4U, 4U, 5U, 5U, 5U, 5U, 6U, 5U, 5U, 6U, 6U
    };
    StarsAttackSection section;
    StarsAttackToken target;
    StarsAttackInput input;
    StarsAttackPackedPartSlot packed_part;
    RawTorpedoTrace trace;
    StarsI16 failures;
    StarsU16 item;

    if (!segment_path || !read_segment(segment_path))
        return 2;

    memset(&section, 0, sizeof section);
    memset(&target, 0, sizeof target);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    section.init = -1;
    section.init_base = 4;
    section.part_item = 7U;
    section.weapon_mode = 2;
    section.target_slot = 0;
    section.active = 1U;
    section.torpedo_count = 3L;
    section.torpedo_base_percent = 50;
    section.torpedo_passes = 1;
    section.torpedo_weapon_flags = 4U;
    section.torpedo_shield_damage = 2L;
    section.torpedo_hull_damage = 5L;
    target.owner = 1;
    target.active_flags = 1U;
    target.target_mask = 1U;
    target.resource_cost = 100L;
    target.shield_points = 1L;
    target.hull_dp = 100L;
    target.ship_count = 1U;
    input.attacker_owner = 0;
    input.init = 4;
    input.attack_players = 2U;
    input.section_count = 1;
    input.sections = &section;
    input.target_count = 1;
    input.targets = &target;
    packed_part.group = 0x0020U;
    packed_part.item = 8U;
    packed_part.count = 1U;
    input.packed_parts = &packed_part;
    input.packed_part_count = 1U;
    input.ops.damage_token = typed_damage_must_not_run;
    input.ops.lookup_part = lookup_torpedo_part;
    input.ops.lookup_part_record = lookup_torpedo_record;
    input.ops.raw_damage_token = raw_torpedo_damage;
    input.ops.torpedo_random = selective_random_draw;
    input.ops.context = &trace;

    failures = 0;
    for (item = 0U; item < 12U; ++item) {
        if (stars_lookup_torpedo_dp(item) != (StarsI16)expected_dp[item])
            failures |= 1024;
    }
    if (stars_lookup_torpedo_dp(12U) != -1)
        failures |= 2048;
    for (item = 0U; item < 12U; ++item) {
        if (stars_lookup_torpedo_init(item) !=
                (StarsI16)expected_init[item]) failures |= 4096;
        if (stars_lookup_torpedo_range(item) !=
                (StarsI16)expected_range[item]) failures |= 8192;
    }
    if (stars_lookup_torpedo_init(12U) != -1)
        failures |= 16384;
    if (stars_lookup_torpedo_range(12U) != -1)
        failures |= 32768;
    if (!stars_f_attack(&input)) failures |= 1;
    if (trace.calls != 2 || trace.shield_calls != 0 ||
            trace.hull_calls != 2 || trace.typed_calls != 0) failures |= 2;
    if (trace.lookup_calls != 1 || trace.lookup_group != 0x0020U ||
            trace.lookup_item != 8U || trace.lookup_count != 1U ||
            trace.record_lookup_calls != 1 ||
            trace.record_offset != (StarsU16)0x2360U)
        failures |= 256;
    if (trace.beam_damage[0] != 85L || trace.torpedo_damage[0] != 85L ||
            trace.flags[0] != 12U || trace.shields_only[0] != 0)
        failures |= 4;
    if (trace.beam_damage[1] != 42L ||
            trace.torpedo_damage[1] != 42L || trace.flags[1] != 12U ||
            trace.shields_only[1] != 0) failures |= 8;
    if (trace.random_calls != 4 || trace.packets_before != 1UL ||
            trace.packets_after != 0UL)
        failures |= 16;
    if (input.hit_count != 2 || input.first_hit_target != 0 ||
            input.first_hit_section != 0) failures |= 32;

    memset(&trace, 0, sizeof trace);
    input.hit_count = 0;
    input.first_hit_target = -1;
    input.first_hit_section = -1;
    packed_part.item = 7U;
    if (!stars_f_attack(&input) || trace.calls != 2 ||
            trace.beam_damage[0] != 60L ||
            trace.beam_damage[1] != 30L ||
            trace.flags[0] != 4U || trace.flags[1] != 4U ||
            trace.lookup_item != 7U)
        failures |= 64;
    memset(&trace, 0, sizeof trace);
    section.torpedo_base_percent = 50;
    target.hull_dp = 10L;
    if (!stars_f_attack(&input) || trace.calls != 3 ||
            trace.shield_calls != 0 || trace.hull_calls != 3 ||
            trace.beam_damage[0] != 30L ||
            trace.torpedo_damage[0] != 30L ||
            trace.random_calls != 6 || trace.packets_before != 1UL ||
            trace.packets_after != 0UL || input.hit_count != 3)
        failures |= 128;
    memset(&trace, 0, sizeof trace);
    packed_part.item = 8U;
    target.shield_points = 0L;
    target.hull_dp = 100L;
    if (!stars_f_attack(&input) || trace.calls != 3 ||
            trace.beam_damage[0] != 85L ||
            trace.beam_damage[1] != 85L ||
            trace.flags[0] != 12U || trace.flags[1] != 12U ||
            trace.packets_before != 1UL || trace.packets_after != 0UL)
        failures |= 512;

    /* Item 9 has init=1 and dRangeMax=5. With initBase=3 and grobj=1,
     * initiative 4 and inclusive range 6 must reach torpedo dispatch. */
    memset(&trace, 0, sizeof trace);
    section.init = -1;
    section.init_base = 3;
    section.init_delta = 0;
    packed_part.item = 9U;
    input.init = 4;
    input.attacker_object_kind = 1U;
    input.attacker_coordinate = 0U;
    target.battle_coordinate = 6U;
    if (!stars_f_attack(&input) || trace.lookup_item != 9U ||
            trace.calls == 0) failures |= 65536;

    /* One past that same raw boundary is excluded before hit/damage services. */
    memset(&trace, 0, sizeof trace);
    input.hit_count = 0;
    input.first_hit_target = -1;
    input.first_hit_section = -1;
    target.battle_coordinate = 7U;
    if (stars_f_attack(&input) || trace.lookup_item != 9U || trace.calls != 0)
        failures |= 131072;
    return failures;
}

#ifndef RAW_TORPEDO_DISPATCH_NATIVE
int main(int argc, char **argv)
{
    int failures;

    if (argc != 2)
        return 2;
    failures = f_attack_raw_torpedo_dispatch_probe(argv[1]);
    printf("F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
#endif
