#include <stdio.h>
#include <string.h>
#include "common.h"

#ifndef FACTOR_NATIVE
StarsI16 stars_dz_from_brc_brc(StarsU8 first, StarsU8 second)
{
    return (StarsI16)(first >= second ? first - second : second - first);
}

#endif

typedef struct Trace {
    StarsI16 lookup_calls;
    StarsU16 group;
    StarsU16 item;
    StarsU16 count;
    StarsU32 component;
    StarsU16 table_segment;
    StarsU16 table_segment_is_current_cs;
    StarsU16 table_offset;
    StarsU16 table_stride;
    const StarsU8 *segment;
    StarsU16 segment_size;
    StarsU16 record_lookup_calls;
    StarsU16 record_offset;
    StarsU16 record_size_override;
    StarsI16 damage_calls;
    StarsU16 damage_class;
    StarsI32 damage;
} Trace;

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

static StarsU32 resolve_component(void *context, StarsU16 group,
    StarsU16 item)
{
    (void)context;
    return ((StarsU32)group << 16) | item;
}

static StarsI16 tech_status(void *context, StarsU32 component)
{
    (void)context;
    (void)component;
    return 1;
}

static StarsI16 lookup_part(void *context, StarsU16 group, StarsU16 item,
    StarsU16 count, StarsAttackSection *section)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->lookup_calls;
    trace->group = group;
    trace->item = item;
    trace->count = count;
    {
        StarsLookupPartInput part;
        StarsI16 status;

        memset(&part, 0, sizeof part);
        part.group = group;
        part.item = item;
        part.player = -1;
        part.resolve = resolve_component;
        part.tech_status = tech_status;
        part.context = context;
        status = stars_lookup_part(&part);
        if (status <= 0)
            return 0;
        trace->component = part.component;
        trace->table_segment = part.table_segment;
        trace->table_segment_is_current_cs =
            part.table_segment_is_current_cs;
        trace->table_offset = part.table_offset;
        trace->table_stride = part.table_stride;
    }
    if (group == 0x10U && item == 7U) {
        section->init = -1;
        section->init_base = 4;
        section->init_delta = 0;
        section->weapon_mode = 0;
        section->target_slot = 1;
        section->battle_rating = 5;
        section->base_damage = 10L;
        section->active = 1U;
        section->beam_passes = 1;
        section->raw_scaling = 1U;
        return 1;
    }
    if (group == 0x20U && item == 4U) {
        section->init = -1;
        section->init_base = 7;
        section->weapon_mode = 2;
        section->target_slot = 1;
        section->battle_rating = 5;
        section->active = 1U;
        section->torpedo_count = 1L;
        section->torpedo_base_percent = 100;
        section->torpedo_passes = 1;
        return 1;
    }
    return 0;
}

static const StarsU8 *lookup_part_record(void *context, StarsU16 group,
    StarsU16 item, StarsU16 *record_size)
{
    Trace *trace;
    StarsLookupPartInput part;

    trace = (Trace *)context;
    ++trace->record_lookup_calls;
    memset(&part, 0, sizeof part);
    part.group = group;
    part.item = item;
    part.player = -1;
    (void)stars_lookup_part(&part);
    trace->record_offset = part.table_offset;
    *record_size = trace->record_size_override ?
        trace->record_size_override : part.table_stride;
    return stars_lookup_part_record(&part, trace->segment,
        trace->segment_size);
}

static StarsI16 damage(void *context, StarsI16 attacker, StarsI16 target,
    StarsI16 section, StarsI32 amount, StarsI16 mode)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)attacker;
    (void)target;
    (void)section;
    (void)mode;
    ++trace->damage_calls;
    trace->damage = amount;
    return 1;
}

static StarsI16 raw_damage(void *context, StarsI16 attacker, StarsI16 target,
    StarsI16 section, StarsI32 *damage_value, StarsI32 base_damage,
    StarsU16 damage_class, StarsI16 mode, StarsU32 *packets)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)attacker;
    (void)target;
    (void)section;
    (void)base_damage;
    (void)mode;
    (void)packets;
    ++trace->damage_calls;
    trace->damage = *damage_value;
    trace->damage_class = damage_class;
    return 1;
}

int f_attack_packed_part_probe(const char *segment_path)
{
    StarsAttackSection sections[2];
    StarsAttackPackedPartSlot packed_parts[2];
    StarsU8 raw_slots[2][4];
    StarsAttackToken target;
    StarsAttackInput input;
    Trace trace;
    StarsI16 failures;
    StarsU8 saved_init;
    StarsU8 saved_range;
    StarsU8 saved_dp;
    StarsU16 raw_record;
    StarsU16 torpedo_record;

    if (!segment_path || !read_segment(segment_path))
        return 2;

    memset(sections, 0, sizeof sections);
    memset(packed_parts, 0, sizeof packed_parts);
    memset(raw_slots, 0, sizeof raw_slots);
    memset(&target, 0, sizeof target);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    trace.segment = segment2;
    trace.segment_size = (StarsU16)sizeof segment2;
    failures = 0;
    sections[0].part_group = 0x10U;
    sections[0].part_item = 7U;
    sections[0].part_count = 2U;
    sections[1].part_group = 0x20U;
    sections[1].part_item = 4U;
    sections[1].part_count = 0U;
    sections[1].active = 1U;
    raw_slots[0][0] = 0x10U;
    raw_slots[0][2] = 7U;
    raw_slots[0][3] = 2U;
    raw_slots[1][0] = 0x20U;
    raw_slots[1][2] = 4U;
    raw_slots[1][3] = 0U;
    sections[0].part_group = 0x20U;
    sections[0].part_item = 24U;
    sections[0].part_count = 0U;
    target.owner = 1;
    target.active_flags = 1U;
    target.target_mask = 2U;
    target.battle_rating = 1;
    target.beam_defense_percent = 100U;
    target.battle_coordinate = 1U;
    target.resource_cost = 20L;
    target.ship_count = 1U;
    target.hull_dp = 1L;
    input.attacker_owner = 0;
    /* Item 7's original _beam.init is 5; initBase is 4. */
    input.init = 9;
    input.attack_players = (StarsU16)(1U << 1);
    input.section_count = 2;
    input.sections = sections;
    input.target_count = 1;
    input.targets = &target;
    if (!stars_attack_bind_raw_hull_slots(&input, &raw_slots[0][0], 4U,
            2U, packed_parts, 2U) || packed_parts[0].group != 0x10U ||
            packed_parts[0].item != 7U || packed_parts[0].count != 2U ||
            packed_parts[1].group != 0x20U ||
            packed_parts[1].item != 4U || packed_parts[1].count != 0U)
        ++failures;
    input.ops.damage_token = damage;
    input.ops.raw_damage_token = raw_damage;
    input.ops.lookup_part = lookup_part;
    input.ops.lookup_part_record = lookup_part_record;
    input.ops.context = &trace;
    if (!stars_f_attack(&input)) ++failures;
    if (trace.lookup_calls != 1 || trace.group != 0x10U ||
            trace.item != 7U || trace.count != 2U ||
            trace.component != 0x00100007UL ||
            trace.table_segment != 0U ||
            trace.table_segment_is_current_cs != 1U ||
            trace.table_offset != 0x25f4U ||
            trace.table_stride != 0x3cU ||
            trace.record_lookup_calls != 1U ||
            trace.record_offset != 0x25f4U) ++failures;
    if (trace.damage_calls != 1 || trace.damage != 10L ||
            trace.damage_class != 1U) ++failures;
    if (sections[0].init != 0 || sections[0].base_damage != 0L) ++failures;

    raw_record = 0x25f4U;
    saved_init = segment2[raw_record + 56U];
    segment2[raw_record + 56U] = 4U;
    memset(&trace, 0, sizeof trace);
    trace.segment = segment2;
    trace.segment_size = (StarsU16)sizeof segment2;
    if (stars_f_attack(&input) || trace.record_lookup_calls != 1U ||
            trace.damage_calls != 0) ++failures;
    segment2[raw_record + 56U] = saved_init;

    saved_range = segment2[raw_record + 52U];
    segment2[raw_record + 52U] = 0U;
    memset(&trace, 0, sizeof trace);
    trace.segment = segment2;
    trace.segment_size = (StarsU16)sizeof segment2;
    if (stars_f_attack(&input) || trace.damage_calls != 0) ++failures;
    segment2[raw_record + 52U] = saved_range;

    saved_dp = segment2[raw_record + 54U];
    segment2[raw_record + 54U] = 231U;
    memset(&trace, 0, sizeof trace);
    trace.segment = segment2;
    trace.segment_size = (StarsU16)sizeof segment2;
    if (!stars_f_attack(&input) || trace.damage_calls != 1 ||
            trace.damage_class != 2U) ++failures;
    segment2[raw_record + 54U] = saved_dp;

    memset(&trace, 0, sizeof trace);
    trace.segment = segment2;
    trace.segment_size = (StarsU16)sizeof segment2;
    trace.record_size_override = 57U;
    if (stars_f_attack(&input) || trace.damage_calls != 0) ++failures;

    memset(&trace, 0, sizeof trace);
    raw_slots[0][2] = 24U;
    if (!stars_attack_bind_raw_hull_slots(&input, &raw_slots[0][0], 4U,
            2U, packed_parts, 2U)) ++failures;
    if (stars_f_attack(&input) || trace.lookup_calls != 1 ||
            trace.damage_calls != 0) ++failures;
    memset(&trace, 0, sizeof trace);
    raw_slots[0][2] = 7U;
    raw_slots[0][3] = 0U;
    if (!stars_attack_bind_raw_hull_slots(&input, &raw_slots[0][0], 4U,
            2U, packed_parts, 2U)) ++failures;
    if (stars_f_attack(&input) || trace.lookup_calls != 0 ||
            trace.damage_calls != 0) ++failures;

    if (stars_attack_bind_raw_hull_slots(&input, &raw_slots[0][0], 3U,
            2U, packed_parts, 2U)) ++failures;
    if (stars_attack_bind_raw_hull_slots(&input, &raw_slots[0][0], 4U,
            2U, packed_parts, 1U)) ++failures;

    raw_slots[0][2] = 24U;
    if (!stars_attack_bind_raw_hull_slots(&input, &raw_slots[0][0], 4U,
            2U, packed_parts, 2U)) ++failures;

    /* Torpedo item 4 has raw range=5, dp=90, and init=2. With initBase=7,
     * initiative 9 dispatches one packet as 45 hull damage (dp/2). */
    torpedo_record = (StarsU16)(0x2180U + 4U * 0x3cU);
    sections[0].part_group = 0x20U;
    sections[0].part_item = 4U;
    sections[0].part_count = 1U;
    raw_slots[0][0] = 0x20U;
    raw_slots[0][2] = 4U;
    raw_slots[0][3] = 1U;
    input.init = 9;
    input.attacker_coordinate = 0U;
    target.battle_coordinate = 1U;
    memset(&trace, 0, sizeof trace);
    trace.segment = segment2;
    trace.segment_size = (StarsU16)sizeof segment2;
    if (!stars_attack_bind_raw_hull_slots(&input, &raw_slots[0][0], 4U,
            2U, packed_parts, 2U) || !stars_f_attack(&input) ||
            trace.record_lookup_calls != 1U || trace.record_offset !=
                torpedo_record || trace.damage_calls != 1 ||
            trace.damage != 45L) ++failures;

    saved_dp = segment2[torpedo_record + 54U];
    segment2[torpedo_record + 54U] = 120U;
    memset(&trace, 0, sizeof trace);
    trace.segment = segment2;
    trace.segment_size = (StarsU16)sizeof segment2;
    if (!stars_f_attack(&input) || trace.damage_calls != 1 ||
            trace.damage != 60L) ++failures;
    segment2[torpedo_record + 54U] = saved_dp;

    saved_range = segment2[torpedo_record + 52U];
    segment2[torpedo_record + 52U] = 0U;
    memset(&trace, 0, sizeof trace);
    trace.segment = segment2;
    trace.segment_size = (StarsU16)sizeof segment2;
    if (stars_f_attack(&input) || trace.damage_calls != 0) ++failures;
    segment2[torpedo_record + 52U] = saved_range;

    saved_init = segment2[torpedo_record + 56U];
    segment2[torpedo_record + 56U] = 1U;
    memset(&trace, 0, sizeof trace);
    trace.segment = segment2;
    trace.segment_size = (StarsU16)sizeof segment2;
    if (stars_f_attack(&input) || trace.damage_calls != 0) ++failures;
    segment2[torpedo_record + 56U] = saved_init;
    return failures != 0;
}

#ifndef FACTOR_NATIVE
int main(int argc, char **argv)
{
    int failures;

    if (argc != 2)
        return 2;
    failures = f_attack_packed_part_probe(argv[1]);
    printf("F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
    return failures != 0;
}
#endif
