#include "core.h"

/* Genuine C reconstruction of DumpFleets, NE 34:9530-a1c1.  The report
 * formatter and stream are callback-owned; this routine computes the typed
 * fleet row and preserves the detailed-report visibility gate. */

static void dump_fleet_ship_totals(const StarsDumpFleet *fleet,
    StarsDumpFleetRow *row)
{
    StarsI16 index;
    StarsI16 ship_class;

    row->ship_other_total = 0L;
    for (index = 0; index < 4; ++index)
        row->ship_class_total[index] = 0L;
    for (index = 0; index < 16; ++index)
        if (fleet->ship_class[index] >= 2 && fleet->ship_class[index] <= 5) {
            ship_class = (StarsI16)(fleet->ship_class[index] - 2);
            row->ship_class_total[ship_class] += fleet->ship_count[index];
        } else {
            row->ship_other_total += fleet->ship_count[index];
        }
}

static void dump_fleet_copy_cargo(StarsI32 *destination,
    const StarsI32 *source)
{
    StarsI16 index;

    for (index = 0; index < 5; ++index)
        destination[index] = source[index];
}

static StarsU16 dump_fleets_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static StarsU32 dump_fleets_packed_get32(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU32)dump_fleets_packed_get16(record, offset) |
        ((StarsU32)dump_fleets_packed_get16(record,
            (StarsU16)(offset + 2)) << 16);
}

static void dump_fleets_packed_load(const StarsU8 *record,
    StarsDumpFleet *fleet, const StarsDumpFleetsPackedInput *input)
{
    StarsI16 index;

    fleet->id = (StarsI16)(dump_fleets_packed_get16(record, 0) & 0x01ffU);
    fleet->owner = (StarsI16)dump_fleets_packed_get16(record, 2);
    fleet->flags = dump_fleets_packed_get16(record, 4);
    fleet->planet_id = (StarsI16)dump_fleets_packed_get16(record, 6);
    fleet->x = (StarsI16)dump_fleets_packed_get16(record, 8);
    fleet->y = (StarsI16)dump_fleets_packed_get16(record, 10);
    for (index = 0; index < 16; ++index) {
        fleet->ship_count[index] = (StarsI16)dump_fleets_packed_get16(
            record, (StarsU16)(12 + index * 2));
        fleet->ship_class[index] = index == 0 ? 2 :
            (index == 4 ? 4 : 0);
    }
    for (index = 0; index < 5; ++index)
        fleet->cargo[index] = (StarsI32)dump_fleets_packed_get32(record,
            (StarsU16)(76 + index * 4));
    fleet->player_number = (StarsI16)dump_fleets_packed_get16(record, 2) + 1;
    fleet->eta = input->eta;
    fleet->warp = (StarsI16)(dump_fleets_packed_get16(record, 118) & 0x000fU);
    fleet->weight = (StarsI32)dump_fleets_packed_get32(record, 44);
    fleet->cloak_percent = input->cloak_percent;
    fleet->scanner_range = input->scanner_range;
    fleet->scanner_detail = input->scanner_detail;
    fleet->task_id = (StarsI16)dump_fleets_packed_get16(record, 98);
    fleet->destination_id = input->destination_id;
    fleet->destination_visible = input->destination_visible;
    fleet->destination_requires_detail = input->destination_requires_detail;
    fleet->plan_id = input->plan_id;
    fleet->mines = input->mines;
    fleet->mine_sweep = input->mine_sweep;
    fleet->mine_laying = input->mine_laying;
    fleet->terraforming = input->terraforming;
}

static void dump_fleet_make_row(const StarsDumpFleetsInput *input,
    const StarsDumpFleet *fleet, StarsDumpFleetRow *row)
{
    StarsI16 index;

    row->id = fleet->id;
    row->owner = fleet->owner;
    row->x = fleet->x;
    row->y = fleet->y;
    row->planet_id = fleet->planet_id;
    row->destination_id = fleet->destination_id;
    row->destination_visible = fleet->destination_visible &&
        (!fleet->destination_requires_detail || input->detailed);
    row->plan_id = fleet->plan_id;
    row->ship_total = 0L;
    dump_fleet_ship_totals(fleet, row);
    row->ship_total = row->ship_other_total;
    for (index = 0; index < 4; ++index)
        row->ship_total += row->ship_class_total[index];
    dump_fleet_copy_cargo(row->cargo, fleet->cargo);
    row->player_number = input->detailed ? fleet->player_number : 0;
    row->eta = input->detailed ? fleet->eta : 0;
    row->warp = input->detailed ? fleet->warp : 0;
    row->weight = input->detailed ? fleet->weight : 0L;
    row->cloak_percent = input->detailed ? fleet->cloak_percent : 0;
    row->scanner_range = input->detailed ? fleet->scanner_range : 0;
    row->scanner_detail = input->detailed ? fleet->scanner_detail : 0;
    row->task_id = input->detailed ? fleet->task_id : 0;
    row->mines = input->detailed ? fleet->mines : 0L;
    row->mine_sweep = input->detailed ? fleet->mine_sweep : 0L;
    row->mine_laying = input->detailed ? fleet->mine_laying : 0L;
    row->terraforming = input->detailed ? fleet->terraforming : 0L;
}

static StarsI16 dump_fleet_text_field_count(StarsI16 detailed)
{
    /* Normal: name, coordinates, planet, destination, plan, ship total,
     * five cargo values.  Detailed mode appends player/route services and
     * the four recovered hull-class buckets plus the other bucket. */
    return detailed ? 29 : 12;
}

StarsI16 stars_dump_fleets(StarsDumpFleetsInput *input)
{
    StarsDumpFleetRow row;
    StarsI16 index;
    StarsI16 header_count;

    if (!input || !input->file_name || !input->fleets ||
            !input->open_report || !input->write_header ||
            (!input->write_row &&
                ((!input->write_text_field || !input->write_text_end) &&
                 (!input->format_text_field || !input->write_text_value ||
                  !input->write_text_end))) || !input->close_report ||
            !input->game_active || input->viewer_player < 0)
        return 0;
    if (!input->open_report(input->context, input->file_name,
            input->detailed))
        return 0;
    header_count = input->detailed ?
        STARS_DUMP_PLANET_HEADER_COUNT_DETAILED :
        STARS_DUMP_PLANET_HEADER_COUNT;
    for (index = 0; index < header_count; ++index) {
        if (!input->write_header(input->context, index)) {
            input->close_report(input->context);
            return 0;
        }
    }
    for (index = 0; index < (StarsI16)input->fleet_count; ++index) {
        dump_fleet_make_row(input, &input->fleets[index], &row);
        if (input->write_row) {
            if (!input->write_row(input->context, &row)) {
                input->close_report(input->context);
                return 0;
            }
        } else if (input->format_text_field && input->write_text_value) {
            StarsI16 field;
            StarsI16 field_count;
            StarsI16 text_length;
            StarsU8 text[256];

            field_count = dump_fleet_text_field_count(input->detailed);
            for (field = 0; field < field_count; ++field) {
                text_length = input->format_text_field(input->context, field,
                    &row, text, (StarsU16)sizeof text);
                if (text_length < 0 || text_length >= (StarsI16)sizeof text ||
                        !input->write_text_value(input->context, field, text,
                            (StarsU16)text_length)) {
                    input->close_report(input->context);
                    return 0;
                }
            }
            if (!input->write_text_end(input->context)) {
                input->close_report(input->context);
                return 0;
            }
        } else {
            StarsI16 field;
            StarsI16 field_count;

            field_count = dump_fleet_text_field_count(input->detailed);
            for (field = 0; field < field_count; ++field)
                if (!input->write_text_field(input->context, field, &row)) {
                    input->close_report(input->context);
                    return 0;
                }
            if (!input->write_text_end(input->context)) {
                input->close_report(input->context);
                return 0;
            }
        }
    }
    input->close_report(input->context);
    return 1;
}

StarsI16 stars_dump_fleets_packed(StarsDumpFleetsPackedInput *input)
{
    StarsDumpFleet fleet;
    StarsDumpFleetsInput report;
    StarsI16 index;

    if (!input || !input->fleet_record || input->fleet_size <
            STARS_DUMP_FLEET_RECORD_SIZE)
        return 0;
    fleet.id = 0;
    fleet.owner = 0;
    fleet.x = 0;
    fleet.y = 0;
    fleet.planet_id = 0;
    fleet.flags = 0;
    fleet.player_number = 0;
    fleet.eta = 0;
    fleet.warp = 0;
    fleet.weight = 0L;
    fleet.cloak_percent = 0;
    fleet.scanner_range = 0;
    fleet.scanner_detail = 0;
    fleet.task_id = 0;
    fleet.mines = 0L;
    fleet.mine_sweep = 0L;
    fleet.mine_laying = 0L;
    fleet.terraforming = 0L;
    fleet.destination_id = 0;
    fleet.destination_visible = 0;
    fleet.destination_requires_detail = 0;
    fleet.plan_id = 0;
    for (index = 0; index < 16; ++index) {
        fleet.ship_count[index] = 0;
        fleet.ship_class[index] = 0;
    }
    for (index = 0; index < 5; ++index)
        fleet.cargo[index] = 0L;
    dump_fleets_packed_load(input->fleet_record, &fleet, input);
    if (input->resolve_services)
        input->resolve_services(input->report.context, input->fleet_record,
            input->fleet_size, &fleet);
    report = input->report;
    report.fleets = &fleet;
    report.fleet_count = 1;
    return stars_dump_fleets(&report);
}
