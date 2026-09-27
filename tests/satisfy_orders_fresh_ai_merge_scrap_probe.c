#include "core.h"
#include "common.h"
#include "SATISFYORD.H"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MergeScrapTrace {
    StarsAiShdefFleet fleets[2];
    StarsU8 raw_fleets[2][124];
    StarsU8 order[18];
    StarsU8 designs[16 * 147];
    StarsU8 planet[60];
    StarsProduceItem production_queue[4];
    StarsI32 mineral_stock[3];
    StarsFleetState fleet_storage[4];
    StarsFleetState *fleet_items[4];
    StarsU16 fleet_count;
    StarsInvalidateReportRecord fleet_report;
    StarsMineralMinedInput mining_input;
    StarsMineralMinedOps mining_ops;
    StarsProducePlanet production_planet;
    StarsSatisfyPackedInput satisfy_input;
    StarsI16 satisfy_passes[4];
    StarsI16 satisfy_pass_count;
    StarsU8 tech_battle[6];
    StarsU8 tech_trader[12];
    StarsTechLearnPlayer tech_player;
    StarsU16 tech_goto;
    StarsU16 game_flags;
    StarsI16 present[4];
    StarsI16 checks;
    StarsI16 merge_calls;
    StarsI16 delete_calls;
    StarsI16 cargo_calls;
    StarsI16 lookup_calls;
    StarsI16 invalidate_calls;
    StarsI16 balance_lookup_calls;
    StarsI16 deleted_id;
    StarsI16 destination_id;
    StarsI16 message_count;
    StarsI16 message_recipient[2];
    StarsI16 message_code[2];
    StarsI16 message_payload[2][8];
    StarsI16 tech_mark_calls;
    StarsI16 tech_lookup_calls;
    StarsI16 tech_learn_calls;
    StarsI16 tech_random_calls;
    StarsI16 tech_random_value;
    StarsI16 remove_message_calls;
    StarsI16 production_queue_calls;
    StarsI16 production_finish_calls;
    StarsI16 production_changed;
    StarsI16 planet_desirability;
    StarsI16 mining_draw[3];
    StarsI16 mining_draw_count;
    StarsI16 mining_failures;
    StarsI16 production_build_calls;
    StarsI16 production_object_calls;
    StarsI16 failures;
} MergeScrapTrace;

#define CHECK(test) do { ++trace->checks; if (!(test)) { \
    fprintf(stderr, "probe assertion failed at line %d\n", __LINE__); \
    ++trace->failures; } } while (0)

static void put_word(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 get_word(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1U] << 8));
}

static void put_long(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    put_word(raw, offset, (StarsU16)value);
    put_word(raw, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

static StarsU32 get_long(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU32)get_word(raw, offset) |
        ((StarsU32)get_word(raw, (StarsU16)(offset + 2U)) << 16);
}

static const StarsU8 *part_record(StarsU16 group, StarsU16 item)
{
    StarsLookupPartInput lookup;
    StarsI16 status;
    memset(&lookup, 0, sizeof lookup);
    lookup.group = group;
    lookup.item = item;
    lookup.player = -1;
    status = stars_lookup_part(&lookup);
    if (status < 0) {
        fprintf(stderr, "part reject group=%u item=%u status=%d\n",
            group, item, status);
        return (const StarsU8 *)0;
    }
    {
        const StarsU8 *record = stars_lookup_part_record(&lookup,
            retained_part_segment,
            (StarsU16)sizeof retained_part_segment);
        if (!record)
            fprintf(stderr, "part map fail group=%u item=%u off=%u stride=%u size=%u\n",
                group, item, lookup.table_offset, lookup.table_stride,
                (StarsU16)sizeof retained_part_segment);
        return record;
    }
}

static StarsU16 read_hull(void *opaque, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    const StarsU8 *record;
    (void)opaque;
    if (capacity < 143U) return 0U;
    record = part_record(0x4000U, id);
    if (!record) { fprintf(stderr, "hull lookup fail %u\n", id); return 0U; }
    memcpy(out, record, 143U);
    return 1U;
}

static StarsI16 read_part(void *opaque, StarsU16 group, StarsU16 item,
    StarsU8 *out, StarsU16 capacity)
{
    const StarsU8 *record;
    (void)opaque;
    if (capacity < 8U) return 0;
    record = part_record(group, item);
    if (!record) { fprintf(stderr, "part lookup callback fail %u/%u\n", group, item); return 0; }
    memset(out, 0, 8U);
    out[4] = record[40];
    out[5] = record[41];
    return 1;
}

static StarsI16 read_design_name(void *opaque, const StarsU8 *input,
    StarsU16 length, StarsU8 *out, StarsU16 *out_length)
{
    StarsI16 capacity = (StarsI16)*out_length;
    (void)opaque;
    if (!stars_decompress_user_string(retained_decode_prefix, input,
            (StarsI16)length, out, &capacity)) return 0;
    *out_length = (StarsU16)strlen((const char *)out);
    return 1;
}

static StarsI16 part_technology(void *opaque, StarsU16 group,
    StarsU16 subtype, StarsU8 technology[6])
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsLookupPartInput lookup;
    const StarsU8 *record;
    memset(&lookup, 0, sizeof lookup);
    lookup.group = group;
    lookup.item = subtype;
    lookup.player = -1;
    if (stars_lookup_part(&lookup) < 0)
        return 0;
    record = stars_lookup_part_record(&lookup, retained_part_segment,
        (StarsU16)sizeof retained_part_segment);
    if (!record)
        return 0;
    memcpy(technology, record + 2U, 6U);
    ++trace->tech_lookup_calls;
    return 1;
}

static StarsI16 race_stat(void *opaque, StarsI16 player, StarsI16 stat)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    CHECK(player == 1 && stat == 0x0e);
    return retained_player_race_stat;
}

static StarsI16 race_grbit(void *opaque, StarsI16 player, StarsI16 bit)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    CHECK(player == 1 && bit >= 0 && bit < 32);
    return (StarsI16)((retained_player_race_bits >> bit) & 1UL);
}

static void update_cost(void *opaque, StarsU8 *design)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsShdefRepriceInput input;
    StarsU8 repriced[147];
    memset(&input, 0, sizeof input);
    input.part_segment = retained_part_segment;
    input.part_segment_size = (StarsU16)sizeof retained_part_segment;
    input.player_technology = retained_player_technology;
    input.game_flags = &trace->game_flags;
    input.race_stat = race_stat;
    input.race_grbit = race_grbit;
    input.context = trace;
    input.player = 1;
    if (!stars_reprice_shdef_cost(&input, design, 147U, repriced)) {
        fprintf(stderr, "reprice callback fail\n");
        ++trace->failures;
        return;
    }
    memcpy(design, repriced, sizeof repriced);
}

static StarsI16 balance_lookup_fleet(void *, StarsI16,
    StarsFleetBalanceFleet *);
static StarsI16 balance_max_weight(void *, StarsI16, StarsI16, StarsI16);

static void balance_cargo(void *opaque, StarsU8 *destination,
    StarsU8 *deleted)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsFleetBalanceFleet current[2];
    StarsFleetTransferCargoBalanceInput input;
    StarsU16 side;
    StarsU16 slot;

    memset(current, 0, sizeof current);
    for (side = 0; side < 2U; ++side) {
        const StarsU8 *raw = side == 0U ? destination : deleted;
        current[side].player = (StarsI16)get_word(raw, 2U);
        current[side].flags = get_word(raw, 4U);
        for (slot = 0; slot < 16U; ++slot) {
            current[side].ship_count[slot] =
                (StarsI16)get_word(raw, (StarsU16)(12U + slot * 2U));
            current[side].ship_status[slot] =
                get_word(raw, (StarsU16)(44U + slot * 2U));
        }
        for (slot = 0; slot < 5U; ++slot)
            current[side].cargo[slot] = (StarsI32)get_long(raw,
                (StarsU16)(76U + slot * 4U));
    }
    memset(&input, 0, sizeof input);
    input.new_fleets[0] = &current[0];
    input.new_fleets[1] = &current[1];
    input.fleet_ids[0] = (StarsI16)get_word(destination, 0U);
    input.fleet_ids[1] = (StarsI16)get_word(deleted, 0U);
    input.player = 1;
    input.lookup_fleet = balance_lookup_fleet;
    input.max_weight = balance_max_weight;
    input.context = trace;
    stars_fleet_transfer_cargo_balance(&input);

    for (side = 0; side < 2U; ++side) {
        StarsU8 *raw = side == 0U ? destination : deleted;
        for (slot = 0; slot < 16U; ++slot)
            put_word(raw, (StarsU16)(44U + slot * 2U),
                current[side].ship_status[slot]);
        for (slot = 0; slot < 5U; ++slot)
            put_long(raw, (StarsU16)(76U + slot * 4U),
                (StarsU32)current[side].cargo[slot]);
    }
    ++trace->cargo_calls;
}

static StarsI16 balance_lookup_fleet(void *opaque, StarsI16 fleet_id,
    StarsFleetBalanceFleet *out)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    const StarsU8 *raw;
    StarsU16 slot;
    StarsU16 side;
    if (!out || (fleet_id != 2 && fleet_id != 3))
        return 0;
    side = (StarsU16)(fleet_id - 2);
    raw = trace->raw_fleets[side];
    memset(out, 0, sizeof *out);
    out->player = (StarsI16)get_word(raw, 2U);
    out->flags = get_word(raw, 4U);
    for (slot = 0; slot < 16U; ++slot) {
        out->ship_count[slot] =
            (StarsI16)get_word(raw, (StarsU16)(12U + slot * 2U));
        out->ship_status[slot] =
            get_word(raw, (StarsU16)(44U + slot * 2U));
    }
    for (slot = 0; slot < 5U; ++slot)
        out->cargo[slot] = (StarsI32)get_long(raw,
            (StarsU16)(76U + slot * 4U));
    ++trace->balance_lookup_calls;
    return out->player == 1;
}

static StarsI16 balance_max_weight(void *opaque, StarsI16 player,
    StarsI16 design_id, StarsI16 statistic)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsShipComponentStat components[16];
    StarsShipDesignStat design;
    StarsHullStat hull;
    StarsU8 hull_record[143];
    StarsU16 count;
    StarsU16 index;
    if (player != 1 || statistic < 1 || statistic > 2) {
        ++trace->failures;
        return 0;
    }
    if (design_id != 2)
        return 0;
    count = retained_shdef_record[6];
    if (count > 16U || !read_hull(opaque, retained_shdef_record[2],
            hull_record, (StarsU16)sizeof hull_record)) {
        ++trace->failures;
        return 0;
    }
    for (index = 0; index < count; ++index) {
        StarsU16 offset = (StarsU16)(17U + index * 4U);
        components[index].group = get_word(retained_shdef_record, offset);
        components[index].packed_count_type = get_word(
            retained_shdef_record, (StarsU16)(offset + 2U));
    }
    design.component_count = count;
    design.components = components;
    hull.fuel_max = get_word(hull_record, 50U);
    hull.cargo_max = get_word(hull_record, 52U);
    return stars_wt_max_ship_design_stat(&design, &hull, statistic);
}

static void lookup_fleet(void *opaque, StarsI16 player, StarsU8 *fleet)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsFleetState working;
    StarsI16 fleet_id;
    memset(&working, 0, sizeof working);
    memcpy(working.bytes, fleet, sizeof working.bytes);
    fleet_id = (StarsI16)get_word(fleet, 0U);
    if (player != -1 || fleet_id < 0 || fleet_id >= 4 ||
            !stars_lookup_fleet(trace->fleet_items, trace->fleet_count,
                -1, &working, 0) ||
            memcmp(trace->fleet_items[fleet_id]->bytes, fleet,
                sizeof working.bytes) != 0)
        ++trace->failures;
    ++trace->lookup_calls;
}

static void delete_fleet(void *opaque, StarsI16 deleted_id,
    StarsI16 reason, StarsI16 destination_id)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    if (reason != 2 || deleted_id < 0 || deleted_id >= 4 ||
            destination_id != 2 ||
            !stars_delete_fleet(trace->fleet_items, &trace->fleet_count,
                deleted_id, reason, destination_id, 0, 0))
        ++trace->failures;
    else
        trace->present[(StarsU16)deleted_id] = 0;
    trace->deleted_id = deleted_id;
    trace->destination_id = destination_id;
    ++trace->delete_calls;
}

static void invalidate_report(void *opaque, StarsI16 player,
    StarsI16 report_kind)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsInvalidateReportInput input;
    StarsInvalidateReportRecord *current = &trace->fleet_report;
    memset(&input, 0, sizeof input);
    input.report = player;
    input.reload = report_kind;
    input.current = &current;
    input.fleet_report = &trace->fleet_report;
    input.ai_mode = 0;
    input.game_view_flags = 0U;
    stars_invalidate_report(&input);
    if (player != 1 || report_kind != 2 ||
            current != &trace->fleet_report ||
            trace->fleet_report.cached != 0 ||
            (input.game_view_flags & 0x0200U) == 0U)
        ++trace->failures;
    ++trace->invalidate_calls;
}

static void merge_selected_fleets(void *opaque,
    StarsAiShdefFleet *destination, StarsAiShdefFleet *deleted)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsMergeFleetInput input;
    StarsU16 slot;
    StarsI16 destination_index;
    StarsI16 deleted_index;

    destination_index = (StarsI16)(destination - trace->fleets);
    deleted_index = (StarsI16)(deleted - trace->fleets);
    if (destination_index != 0 || deleted_index != 1)
        ++trace->failures;
    memset(&input, 0, sizeof input);
    input.destination = trace->raw_fleets[(StarsU16)destination_index];
    input.deleted = trace->raw_fleets[(StarsU16)deleted_index];
    input.no_delete = 0;
    input.cargo_balance = balance_cargo;
    input.lookup = lookup_fleet;
    input.delete_fleet = delete_fleet;
    input.invalidate_report = invalidate_report;
    input.context = trace;
    stars_merge_two_fleets(&input);
    for (slot = 0; slot < 16U; ++slot) {
        destination->ship_counts[slot] = get_word(
            trace->raw_fleets[(StarsU16)destination_index],
            (StarsU16)(12U + slot * 2U));
        deleted->ship_counts[slot] = get_word(
            trace->raw_fleets[(StarsU16)deleted_index],
            (StarsU16)(12U + slot * 2U));
    }
    deleted->player = -1;
    ++trace->merge_calls;
}

static void raw_message(void *opaque, StarsI16 recipient,
    StarsI16 code, const StarsI16 payload[8])
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsI16 i;
    if (trace->message_count >= 2) {
        ++trace->failures;
        return;
    }
    trace->message_recipient[trace->message_count] = recipient;
    trace->message_code[trace->message_count] = code;
    for (i = 0; i < 8; ++i)
        trace->message_payload[trace->message_count][i] = payload[i];
    ++trace->message_count;
}

static StarsI16 tech_random(void *opaque, StarsI16 limit);

static StarsI16 learn_technology(void *opaque, StarsI16 player,
    StarsU16 *message_object)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsTechLearnInput input;
    StarsI16 index;
    StarsI16 result;
    ++trace->tech_learn_calls;
    if (player != 1 || *message_object != 24U) ++trace->failures;
    memset(&input, 0, sizeof input);
    for (index = 0; index < 6; ++index)
        trace->tech_player.tech_level[index] =
            retained_player_technology[index];
    input.player = &trace->tech_player;
    input.player_id = player;
    input.message_id = -1;
    input.goto_id = &trace->tech_goto;
    input.random = tech_random;
    input.context = trace;
    /* Use recovered action-run draw 11. Random(100)=43 takes the original
     * no-learn return in ITechLearnATech. */
    result = stars_itech_learn_a_tech(&input);
    if (result != 0 || *message_object != 24U || trace->tech_goto != 0U)
        ++trace->failures;
    return result;
}

static StarsI16 tech_random(void *opaque, StarsI16 limit)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    ++trace->tech_random_calls;
    if (limit != 100 || trace->tech_random_value < 0 ||
            trace->tech_random_value >= limit)
        ++trace->failures;
    return trace->tech_random_value;
}

static void remove_message(void *opaque, StarsI16 player, StarsI16 code,
    StarsU16 object)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    if (player != 1 || code != 0x4e || object != 0x8002U)
        ++trace->failures;
    ++trace->remove_message_calls;
}

static void mark_techs(void *opaque, const StarsU8 *design,
    StarsI16 player)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsMarkTechsInput input;
    if (design != trace->designs + 2U * 147U || player != 1)
        ++trace->failures;
    memset(&input, 0, sizeof input);
    memcpy(input.tech_battle, trace->tech_battle,
        sizeof input.tech_battle);
    memcpy(input.tech_trader, trace->tech_trader,
        sizeof input.tech_trader);
    input.lookup = part_technology;
    input.context = trace;
    if (!stars_mark_techs_seen_shdef(design, 147U, &input))
        ++trace->failures;
    memcpy(trace->tech_battle, input.tech_battle,
        sizeof trace->tech_battle);
    memcpy(trace->tech_trader, input.tech_trader,
        sizeof trace->tech_trader);
    ++trace->tech_mark_calls;
}

static void check_planet_input(MergeScrapTrace *trace)
{
    StarsReadPlanetInput input;
    StarsReadPlanetState state;
    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    input.id_player_word = get_word(retained_planet_body, 0U);
    input.flags_word = get_word(retained_planet_body, 2U);
    input.header_flags = retained_planet_header_flags;
    input.game_turn = retained_planet_turn;
    input.payload = retained_planet_body + 4U;
    input.payload_size = (StarsU16)(sizeof retained_planet_body - 4U);
    CHECK(stars_read_planet(&input, &state, 1, 0, 0) == 1);
    CHECK(state.id == 24 && state.player == 1 && state.flags == 0x0f07U);
    if (state.flags != 0x0f07U)
        fprintf(stderr, "planet flags parsed=0x%04x\n", state.flags);
    CHECK(state.minerals[0] == 532U && state.minerals[1] == 956U &&
        state.minerals[2] == 500U && state.minerals[3] == 250U);
    put_word(trace->planet, 0U, (StarsU16)state.id);
    put_word(trace->planet, 2U, (StarsU16)state.player);
    put_word(trace->planet, 4U, state.flags);
    put_long(trace->planet, 0x1cU, state.minerals[0]);
    put_long(trace->planet, 0x20U, state.minerals[1]);
    put_long(trace->planet, 0x24U, state.minerals[2]);
    put_long(trace->planet, 0x28U, state.minerals[3]);
}

static void check_design_input(MergeScrapTrace *trace)
{
    StarsReadShdefOps ops;
    memset(&ops, 0, sizeof ops);
    ops.lookup_hull = read_hull;
    ops.lookup_part = read_part;
    ops.update_cost = update_cost;
    ops.decode_string = read_design_name;
    CHECK(stars_read_shdef(retained_shdef_record,
        (StarsU16)sizeof retained_shdef_record, trace->designs,
        (StarsU16)sizeof trace->designs, 1, &ops, trace) == 1);
    CHECK(get_word(trace->designs + 2U * 147U, 0U) == 20U);
    CHECK(trace->designs[2U * 147U + 0x7aU] == 2U);
    CHECK(get_word(trace->designs + 2U * 147U, 0x2aU) == 123U);
    CHECK(get_word(trace->designs + 2U * 147U, 0x2cU) == 41U);
    CHECK(get_word(trace->designs + 2U * 147U, 0x2eU) == 0U);
    CHECK(get_word(trace->designs + 2U * 147U, 0x30U) == 12U);
}

static void check_at(MergeScrapTrace *trace, StarsI16 condition,
    int line)
{
    ++trace->checks;
    if (!condition) {
        fprintf(stderr, "probe assertion failed at line %d (check %d)\n",
            line, trace->checks);
        ++trace->failures;
    }
}

#define check(trace, condition) check_at((trace), (condition), __LINE__)

static StarsI16 accept_turin_merge(void *opaque, StarsI16 mask)
{
    (void)opaque;
    (void)mask;
    return 1;
}

static void satisfy_order_pass(void *opaque, StarsI16 pass)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;

    if (trace->satisfy_pass_count < 4)
        trace->satisfy_passes[trace->satisfy_pass_count] = pass;
    ++trace->satisfy_pass_count;
    trace->satisfy_input.pass = pass;
    if (!stars_satisfy_orders_packed(&trace->satisfy_input))
        ++trace->failures;
}

static StarsI16 planet_desirability(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 player)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsU8 data[65536];
    StarsI16 result;

    if (planet->id != 24 || player != 1)
        return -1;
    memset(data, 0, sizeof data);
    memcpy(data + 12U, retained_planet_environment, 3U);
    memcpy(data + 0x59a2U + 192U + 16U,
        retained_player_environment, 9U);
    result = stars_planet_desirability(data, 0U, player);
    trace->planet_desirability = result;
    return result;
}

static StarsI16 queue_production(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 design, StarsI16 quantity)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsProduceItem *item;

    if (planet->id != 24 || planet->owner != 1 || design != 0 ||
            quantity != 1 || trace->production_queue_calls >= 4)
        return 0;
    item = &trace->production_queue[trace->production_queue_calls];
    memset(item, 0, sizeof *item);
    item->type = design;
    item->object_kind = 2;
    item->quantity = quantity;
    item->active = 1;
    ++trace->production_queue_calls;
    return 1;
}

static StarsI16 finish_production(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 changed)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;

    if (planet->id != 24 || planet->owner != 1)
        return 0;
    ++trace->production_finish_calls;
    trace->production_changed = changed;
    return 1;
}

static StarsI16 mining_random(void *opaque, StarsI16 limit)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsI16 index = trace->mining_draw_count++;

    if (limit != 100 || index < 0 || index >= 3) {
        ++trace->mining_failures;
        return 99;
    }
    return trace->mining_draw[index];
}

static void mine_for_production(void *opaque)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;

    stars_est_minerals_mined(&trace->mining_input);
    trace->production_planet.mineral[0] = trace->mineral_stock[0];
    trace->production_planet.mineral[1] = trace->mineral_stock[1];
    trace->production_planet.mineral[2] = trace->mineral_stock[2];
}

static StarsI32 resources_at_planet(void *opaque,
    const StarsProducePlanet *planet, StarsI16 owner)
{
    (void)opaque;
    (void)owner;
    return planet->resources;
}

static StarsI16 production_capacity(void *opaque,
    const StarsProducePlanet *planet, StarsI16 type, StarsI16 operating)
{
    (void)opaque;
    (void)planet;
    (void)type;
    (void)operating;
    return 3;
}

static StarsI16 production_costs(void *opaque,
    const StarsProducePlanet *planet, const StarsProduceItem *item,
    StarsI32 result[4])
{
    StarsI16 index;
    (void)opaque;
    (void)planet;
    (void)item;
    for (index = 0; index < 4; ++index)
        result[index] = retained_production_costs[index];
    return 1;
}

static StarsI16 production_build_capacity(void *opaque,
    const StarsProducePlanet *planet, StarsI16 type, StarsI16 operating)
{
    return production_capacity(opaque, planet, type, operating);
}

static StarsI16 production_build_object(void *opaque,
    StarsProducePlanet *planet, StarsI16 object_kind, StarsI16 type,
    StarsI16 count, StarsI32 minerals[3])
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    (void)planet;
    (void)object_kind;
    (void)type;
    (void)count;
    (void)minerals;
    ++trace->production_object_calls;
    return 1;
}

static StarsI16 production_build_item(void *opaque,
    StarsProducePlanet *planet, StarsProduceItem *item,
    StarsProduceItem *partial, StarsI32 resources[4], StarsI16 *status)
{
    MergeScrapTrace *trace = (MergeScrapTrace *)opaque;
    StarsBuildProductionOps build_ops;
    StarsBuildProductionInput build_input;
    StarsI16 index;
    StarsI16 built;

    memset(&build_ops, 0, sizeof build_ops);
    memset(&build_input, 0, sizeof build_input);
    build_ops.costs = production_costs;
    build_ops.capacity = production_build_capacity;
    build_ops.build_object = production_build_object;
    build_ops.context = opaque;
    build_input.planet = planet;
    build_input.item = item;
    build_input.partial = partial;
    build_input.status = status;
    build_input.ops = &build_ops;
    for (index = 0; index < 4; ++index)
        build_input.resources[index] = resources[index];
    built = stars_build_production_item(&build_input);
    for (index = 0; index < 4; ++index)
        resources[index] = build_input.resources[index];
    ++trace->production_build_calls;
    return built;
}

int main(int argc, char **argv)
{
    MergeScrapTrace trace;
    StarsTurinTurnFleet turin_fleet;
    StarsTurinTurnPlanet turin_planet;
    StarsTurinDroneAiTurnOps turin_ops;
    StarsTurinDroneAiTurnInput turin_input;
    StarsSatisfyOrdersOps satisfy_ops;
    StarsDoOrdersInput orders_turn;
    StarsOrdersPlanet orders_planet;
    StarsSatisfyScrapPackedState scrap;
    StarsProduceOps produce_ops;
    StarsProduceInput produce_input;
    StarsU16 index;
    StarsU16 slot;
    StarsU16 flags;
    StarsU16 base_flags;
    StarsU16 resource_extra;
    StarsU32 cargo_before;
    StarsU32 cargo_after;
    StarsU8 raw_plord[STARS_AI_PLORD_HEADER_SIZE +
        STARS_AI_PLORD_ORDER_SIZE];
    FILE *report;

    if (argc != 5)
        return 2;
    memset(&trace, 0, sizeof trace);
    memset(&turin_planet, 0, sizeof turin_planet);
    memset(&satisfy_ops, 0, sizeof satisfy_ops);
    memset(&orders_turn, 0, sizeof orders_turn);
    memset(&orders_planet, 0, sizeof orders_planet);
    memset(&scrap, 0, sizeof scrap);
    memset(&produce_ops, 0, sizeof produce_ops);
    memset(&produce_input, 0, sizeof produce_input);
    trace.tech_random_value = (StarsI16)atoi(argv[1]);
    trace.mining_draw[0] = (StarsI16)atoi(argv[2]);
    trace.mining_draw[1] = (StarsI16)atoi(argv[3]);
    trace.mining_draw[2] = (StarsI16)atoi(argv[4]);
    for (index = 0; index < 4U; ++index)
        trace.present[index] = 1;
    check_design_input(&trace);
    check_planet_input(&trace);
    for (index = 0; index < 2U; ++index) {
        trace.fleets[index].player = retained_players[index];
        trace.fleets[index].planet = retained_planets[index];
        trace.fleets[index].point.x = retained_points[index][0];
        trace.fleets[index].point.y = retained_points[index][1];
        trace.fleets[index].mine_value = retained_mine_value;
        trace.fleets[index].ship_counts[2] = retained_slot2_counts[index];
        put_word(trace.raw_fleets[index], 0U, retained_ids[index]);
        put_word(trace.raw_fleets[index], 2U,
            (StarsU16)retained_players[index]);
        put_word(trace.raw_fleets[index], 4U, 0x0907U);
        put_word(trace.raw_fleets[index], 6U,
            (StarsU16)retained_planets[index]);
        put_word(trace.raw_fleets[index], 8U,
            (StarsU16)retained_points[index][0]);
        put_word(trace.raw_fleets[index], 10U,
            (StarsU16)retained_points[index][1]);
        for (slot = 0; slot < 16U; ++slot)
            put_word(trace.raw_fleets[index],
                (StarsU16)(12U + slot * 2U),
                retained_ship_counts[index][slot]);
        for (slot = 0; slot < 5U; ++slot)
            put_long(trace.raw_fleets[index],
                (StarsU16)(76U + slot * 4U),
                retained_cargo[index][slot]);
        trace.raw_fleets[index][98] = 1U;
    }
    for (index = 0; index < 4U; ++index) {
        memset(&trace.fleet_storage[index], 0,
            sizeof trace.fleet_storage[index]);
        put_word(trace.fleet_storage[index].bytes, 0U, index);
        trace.fleet_items[index] = &trace.fleet_storage[index];
    }
    memcpy(trace.fleet_storage[2].bytes, trace.raw_fleets[0],
        sizeof trace.raw_fleets[0]);
    memcpy(trace.fleet_storage[3].bytes, trace.raw_fleets[1],
        sizeof trace.raw_fleets[1]);
    trace.fleet_count = 4U;
    trace.fleet_report.report = 1;
    trace.fleet_report.cached = 1;

    stars_merge_all_shdefs(0x000cU, trace.fleets, 2U, 1,
        merge_selected_fleets, &trace);
    check(&trace, trace.merge_calls == 1);
    check(&trace, trace.deleted_id == 3 && trace.destination_id == 2);
    check(&trace, trace.present[2] == 1 && trace.present[3] == 0);
    check(&trace, get_word(trace.raw_fleets[0], 16U) == 2U);
    check(&trace, get_word(trace.raw_fleets[1], 16U) == 0U);
    check(&trace, trace.cargo_calls == 1 && trace.lookup_calls == 2);
    check(&trace, trace.balance_lookup_calls == 2);
    check(&trace, trace.delete_calls == 1 && trace.invalidate_calls == 1);
    check(&trace, trace.fleet_count == 3U &&
        get_word(trace.fleet_items[2]->bytes, 0U) == 2U &&
        (get_word(trace.fleet_storage[3].bytes, 4U) & 0x0400U) != 0U);
    check(&trace, get_long(trace.raw_fleets[0], 92U) == 420U);
    for (slot = 0; slot < 5U; ++slot) {
        cargo_before = retained_cargo[0][slot] + retained_cargo[1][slot];
        cargo_after = get_long(trace.raw_fleets[0],
            (StarsU16)(76U + slot * 4U)) + get_long(
            trace.raw_fleets[1], (StarsU16)(76U + slot * 4U));
        check(&trace, cargo_after == cargo_before &&
            get_long(trace.raw_fleets[1],
                (StarsU16)(76U + slot * 4U)) == 0U);
    }

    put_word(trace.order, 4U, 24U);
    put_word(trace.order, 6U, 0x1100U);
    memset(raw_plord, 0, sizeof raw_plord);
    raw_plord[3] = 1U;
    memcpy(raw_plord + STARS_AI_PLORD_HEADER_SIZE, trace.order,
        sizeof trace.order);
    memset(&turin_fleet, 0, sizeof turin_fleet);
    turin_fleet.id = 2;
    turin_fleet.owner = 1;
    turin_fleet.planet_id = 24;
    turin_fleet.cord = 1;
    turin_fleet.raw_plord = raw_plord;
    for (slot = 0; slot < 16U; ++slot)
        turin_fleet.ship_counts[slot] = trace.fleets[0].ship_counts[slot];
    memset(&turin_ops, 0, sizeof turin_ops);
    turin_ops.merge_shdefs = accept_turin_merge;
    memset(&turin_input, 0, sizeof turin_input);
    turin_input.turn = 0;
    turin_input.player_id = 1;
    turin_planet.id = 24;
    turin_planet.owner = 1;
    turin_planet.visibility_flags = retained_planet_runtime_flags;
    memcpy(turin_planet.mineral_concentration,
        retained_planet_concentrations,
        sizeof turin_planet.mineral_concentration);
    turin_planet.current_minerals[0] = get_long(trace.planet, 0x1cU);
    turin_planet.current_minerals[1] = get_long(trace.planet, 0x20U);
    turin_planet.current_minerals[2] = get_long(trace.planet, 0x24U);
    turin_planet.current_minerals[3] = get_long(trace.planet, 0x28U);
    turin_planet.production_slot_open = (StarsI16)(
        (retained_planet_runtime_flags & 0x0200U) != 0U &&
        retained_planet_population >= 200UL);
    turin_input.planet_count = 1;
    turin_input.planets = &turin_planet;
    turin_input.plan_max = retained_map_points;
    turin_input.fleet_count = 1;
    turin_input.fleets = &turin_fleet;
    turin_ops.planet_desirability = planet_desirability;
    turin_ops.queue_production = queue_production;
    turin_ops.finish_production = finish_production;
    turin_ops.context = &trace;
    turin_input.ops = &turin_ops;
    check(&trace, stars_do_turin_drone_ai_turn(&turin_input) == 1);
    check(&trace, trace.planet_desirability == 100);
    check(&trace, trace.production_queue_calls == 2 &&
        turin_input.production_hulls_queued == 2);
    check(&trace, trace.production_finish_calls == 1 &&
        trace.production_changed == 1);
    check(&trace, trace.production_queue[0].type == 0 &&
        trace.production_queue[0].object_kind == 2 &&
        trace.production_queue[0].quantity == 1 &&
        trace.production_queue[0].progress == 0);
    check(&trace, trace.production_queue[1].type == 0 &&
        trace.production_queue[1].object_kind == 2 &&
        trace.production_queue[1].quantity == 1 &&
        trace.production_queue[1].progress == 0);
    check(&trace, (get_word(raw_plord, 10U) & 0x000fU) == 5U);
    check(&trace, (get_word(raw_plord, 10U) & 0xfff0U) == 0x1100U);
    /* The raw turn-zero miner branch creates task 5 in production. */
    memcpy(trace.order, raw_plord + STARS_AI_PLORD_HEADER_SIZE,
        sizeof trace.order);
    check(&trace, get_word(trace.order, 6U) == 0x1105U);
    base_flags = (StarsU16)((retained_player_race_bits >> 12) & 1UL);
    resource_extra = 0U;
    scrap.planet_record = trace.planet;
    scrap.planet_size = sizeof trace.planet;
    scrap.design_records[2] = trace.designs + 2U * 147U;
    scrap.design_size = 147U;
    scrap.fleet_race_reprice = base_flags;
    scrap.planet_race_scrap_bonus = (StarsI16)((retained_player_race_bits >> 5) & 1UL);
    if (scrap.planet_race_scrap_bonus)
        scrap.planet_resource_extra = (StarsU16 *)&resource_extra;
    scrap.game_flags = &trace.game_flags;
    satisfy_ops.context = &trace;
    satisfy_ops.raw_message = raw_message;
    satisfy_ops.scrap_learn = learn_technology;
    satisfy_ops.scrap_remove_message = remove_message;
    satisfy_ops.scrap_mark_techs = mark_techs;
    trace.satisfy_input.fleet_record = trace.raw_fleets[0];
    trace.satisfy_input.fleet_size = sizeof trace.raw_fleets[0];
    trace.satisfy_input.order_record = trace.order;
    trace.satisfy_input.order_size = sizeof trace.order;
    trace.satisfy_input.planet_id = 24;
    trace.satisfy_input.scrap_state = &scrap;
    trace.satisfy_input.ops = &satisfy_ops;
    orders_planet.id = 24;
    orders_planet.player = 1;
    orders_planet.flags = get_word(trace.planet, 4U);
    orders_turn.planets = &orders_planet;
    orders_turn.planet_count = 1;
    orders_turn.game_turn = 0;
    orders_turn.satisfy_orders = satisfy_order_pass;
    orders_turn.context = &trace;
    stars_do_orders(0, &orders_turn);
    check(&trace, trace.satisfy_pass_count == 2 &&
        trace.satisfy_passes[0] == 1 && trace.satisfy_passes[1] == 2);
    flags = get_word(trace.raw_fleets[0], 4U);
    check(&trace, (flags & 0x0400U) != 0U);
    check(&trace, (get_word(trace.order, 6U) & 0x1000U) == 0U);
    check(&trace, get_long(trace.planet, 0x1cU) == 597U);
    check(&trace, get_long(trace.planet, 0x20U) == 956U);
    check(&trace, get_long(trace.planet, 0x24U) == 519U);
    check(&trace, get_long(trace.planet, 0x28U) == 250U);
    check(&trace, trace.message_count == 2);
    check(&trace, trace.message_recipient[0] == 1 &&
        trace.message_code[0] == 0x5a);
    check(&trace, trace.message_payload[0][0] == 24 &&
        trace.message_payload[0][1] == 0x0402 &&
        trace.message_payload[0][2] == 84);
    check(&trace, trace.message_recipient[1] == 1 &&
        trace.message_code[1] == 0x141);
    check(&trace, trace.message_payload[1][0] == 24 &&
        trace.message_payload[1][1] == 2);
    check(&trace, trace.tech_mark_calls == 1 &&
        trace.tech_lookup_calls > 0);
    check(&trace, trace.tech_learn_calls == 1 &&
        trace.tech_random_calls == 1 && trace.tech_random_value == 43 &&
        trace.tech_goto == 0U);
    check(&trace, trace.remove_message_calls == 1);

    /* Continue the same current-source slice through mining and Produce.
     * The starting mineral triplet comes directly from packed task-5 scrap. */
    trace.mineral_stock[0] = (StarsI32)get_long(trace.planet, 0x1cU);
    trace.mineral_stock[1] = (StarsI32)get_long(trace.planet, 0x20U);
    trace.mineral_stock[2] = (StarsI32)get_long(trace.planet, 0x24U);
    trace.mining_input.player = 1;
    trace.mining_input.special_race = retained_special_race;
    trace.mining_input.efficiency_tenths = retained_mine_efficiency_tenths;
    trace.mining_input.operating_mines = retained_operating_mines;
    trace.mining_input.c_mines = -1;
    memcpy(trace.mining_input.concentration,
        retained_planet_concentrations,
        sizeof trace.mining_input.concentration);
    memcpy(trace.mining_input.percent_level,
        retained_planet_percent_levels,
        sizeof trace.mining_input.percent_level);
    trace.mining_input.random_rounding = 1;
    trace.mining_input.apply = 1;
    trace.mining_input.mineral_weight = trace.mineral_stock;
    trace.mining_input.ops = &trace.mining_ops;
    trace.mining_ops.random = mining_random;
    trace.mining_ops.context = &trace;
    trace.production_planet.id = 24;
    trace.production_planet.owner = 1;
    trace.production_planet.resources = 32;
    trace.production_planet.research_percent = retained_player_research_percent;
    trace.production_planet.items = trace.production_queue;
    trace.production_planet.item_count = trace.production_queue_calls;
    trace.production_planet.item_capacity = 4;
    produce_ops.context = &trace;
    produce_ops.mine_minerals = mine_for_production;
    produce_ops.resources_at_planet = resources_at_planet;
    produce_ops.max_capacity = production_capacity;
    produce_ops.build_item_with_resources = production_build_item;
    produce_input.planets = &trace.production_planet;
    produce_input.planet_count = 1;
    produce_input.ops = &produce_ops;
    stars_produce(&produce_input);
    stars_do_orders(1, &orders_turn);
    check(&trace, trace.satisfy_pass_count == 4 &&
        trace.satisfy_passes[2] == 3 && trace.satisfy_passes[3] == 4);
    check(&trace, orders_planet.turn == 0 &&
        (orders_planet.flags & 0x2000U) != 0U);
    check(&trace, trace.mining_draw_count == 3 &&
        trace.mining_failures == 0);
    check(&trace, trace.production_build_calls == 1 &&
        trace.production_object_calls == 0);
    check(&trace, trace.production_planet.item_count == 2 &&
        trace.production_queue[0].progress == 81 &&
        trace.production_queue[1].progress == 0);
    check(&trace, trace.production_planet.mineral[0] == 592 &&
        trace.production_planet.mineral[1] ==
            (trace.mining_draw[1] < 70 ? 964 : 963) &&
        trace.production_planet.mineral[2] == 520);
    check(&trace, trace.production_planet.resources == 32 &&
        trace.production_planet.last_year_resources == 4);
    check(&trace, trace.failures == 0);

#ifndef TARGET_NATIVE
    report = stdout;
#else
    report = fopen("C:\\SOE3M\\SOE3MRES.TXT", "w");
    if (!report) return 4;
#endif
    fprintf(report, "SOE3_MERGE_SCRAP_CHECKS=%d FAILURES=%d RESULT=%s "
        "COST=123,41,0,12 PLANET=597,956,519,250 MSG=90,321 "
        "TECH_LOOKUPS=%d TECH_DRAW=%d TECH_RANDOM=%d\n"
        "PASSES=%d,%d,%d,%d TURIN_QUEUE=%d DESIRABILITY=%d "
        "MINE=%ld,%ld,%ld "
        "STOCK=%ld,%ld,%ld PRODUCE=%ld,%ld,%ld "
        "ITEMS=%u ITEM0=%d,%d,%d ITEM1=%d,%d,%d\n",
        trace.checks, trace.failures, trace.failures ? "FAIL" : "PASS",
        trace.tech_lookup_calls, trace.tech_random_value,
        trace.tech_random_calls,
        trace.satisfy_passes[0], trace.satisfy_passes[1],
        trace.satisfy_passes[2], trace.satisfy_passes[3],
        trace.production_queue_calls, trace.planet_desirability,
        (long)trace.mining_input.output[0],
        (long)trace.mining_input.output[1],
        (long)trace.mining_input.output[2],
        (long)trace.mineral_stock[0],
        (long)trace.mineral_stock[1],
        (long)trace.mineral_stock[2],
        (long)trace.production_planet.mineral[0],
        (long)trace.production_planet.mineral[1],
        (long)trace.production_planet.mineral[2],
        (unsigned)trace.production_planet.item_count,
        trace.production_queue[0].type, trace.production_queue[0].object_kind,
        trace.production_queue[0].progress,
        trace.production_queue[1].type, trace.production_queue[1].object_kind,
        trace.production_queue[1].progress);
#ifdef TARGET_NATIVE
    fclose(report);
#endif
    return trace.failures ? 1 : 0;
}
