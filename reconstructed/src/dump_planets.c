#include "core.h"

/* Genuine C reconstruction of DumpPlanets, NE 34:86e4-952f.  The original
 * writes tab-separated text through a stream.  This version keeps the report
 * data typed and lets the report host format fields and own the stream. */

static void dump_planet_copy_i16(StarsI16 *destination,
    const StarsI16 *source, StarsI16 count)
{
    StarsI16 index;

    for (index = 0; index < count; ++index)
        destination[index] = source[index];
}

static void dump_planet_copy_i32(StarsI32 *destination,
    const StarsI32 *source, StarsI16 count)
{
    StarsI16 index;

    for (index = 0; index < count; ++index)
        destination[index] = source[index];
}

static StarsU16 dump_planets_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static StarsU32 dump_planets_packed_get32(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU32)dump_planets_packed_get16(record, offset) |
        ((StarsU32)dump_planets_packed_get16(record,
            (StarsU16)(offset + 2)) << 16);
}

static void dump_planets_packed_load(const StarsU8 *record,
    StarsDumpPlanet *planet, StarsI32 resources, StarsI32 survival_percent,
    StarsI16 scanner_detail)
{
    StarsI16 index;
    StarsU16 packed_population;
    StarsU32 weights;

    planet->id = (StarsI16)dump_planets_packed_get16(record, 0);
    planet->owner = (StarsI16)dump_planets_packed_get16(record, 2);
    planet->flags = dump_planets_packed_get16(record, 4);
    planet->turn = (StarsI16)dump_planets_packed_get16(record, 50);
    for (index = 0; index < 3; ++index) {
        planet->mineral_concentration[index] = (StarsI32)record[9 + index];
        planet->environment[index] = (StarsI16)record[12 + index];
        planet->environment_original[index] = (StarsI16)record[15 + index];
        planet->mineral_rate[index] = (StarsI32)dump_planets_packed_get32(
            record, (StarsU16)(28 + index * 4));
    }
    packed_population = dump_planets_packed_get16(record, 18);
    weights = dump_planets_packed_get32(record, 20);
    planet->population = (StarsI32)(packed_population & 0x0fffU);
    planet->production = (StarsI32)((weights >> 20) & 0x0fffUL);
    planet->resources = resources;
    planet->survival_percent = survival_percent;
    planet->scanner_range = (StarsI16)((dump_planets_packed_get32(record,
        24) >> 12) & 0x1fUL);
    planet->scanner_detail = scanner_detail;
    packed_population = dump_planets_packed_get16(record, 46);
    planet->starbase_id = (StarsI16)(packed_population & 0x03ffU);
    planet->starbase_level = (StarsI16)((packed_population >> 10) & 0x0fU);
    packed_population = dump_planets_packed_get16(record, 48);
    planet->wormhole_id = (StarsI16)(packed_population & 0x03ffU);
    planet->wormhole_distance = (StarsI16)((packed_population >> 10) & 0x3fU);
    planet->mine_count = (StarsI16)((weights >> 8) & 0x0fffUL);
}

static void dump_planet_clear_detailed(StarsDumpPlanetRow *row)
{
    StarsI16 index;

    for (index = 0; index < 3; ++index) {
        row->mineral_concentration[index] = 0L;
        row->mineral_rate[index] = 0L;
        row->environment[index] = 0;
        row->environment_original[index] = 0;
    }
    row->scanner_range = 0;
    row->scanner_detail = 0;
    row->starbase_id = -1;
    row->starbase_level = 0;
    row->wormhole_id = -1;
    row->wormhole_distance = 0;
    row->mine_count = 0;
}

static void dump_planet_clear_unavailable(StarsDumpPlanetRow *row,
    StarsU16 flags)
{
    StarsI16 index;
    StarsI16 kind;

    kind = (StarsI16)(flags & 0xffU);
    if (kind <= 2) {
        for (index = 0; index < 3; ++index) {
            row->mineral_concentration[index] = 0L;
            row->environment[index] = 0;
            row->environment_original[index] = 0;
        }
    }
    if (kind <= 3)
        for (index = 0; index < 3; ++index)
            row->mineral_rate[index] = 0L;
    if (kind != STARS_DUMP_PLANET_FLAG_INHABITED) {
        row->production = 0L;
        row->survival_percent = 0L;
        row->resources = 0L;
    }
    if ((flags & STARS_DUMP_PLANET_FLAG_SCANNED) == 0U) {
        row->scanner_range = 0;
        row->scanner_detail = 0;
        row->starbase_id = -1;
        row->starbase_level = 0;
        row->wormhole_id = -1;
        row->wormhole_distance = 0;
    }
}

static void dump_planet_make_row(const StarsDumpPlanetsInput *input,
    const StarsDumpPlanet *planet, StarsDumpPlanetRow *row)
{
    row->id = planet->id;
    row->owner = planet->owner;
    row->owner_visible = planet->owner < 0 ||
        (planet->flags & STARS_DUMP_PLANET_FLAG_SCANNED) != 0U;
    row->turns_since_visit = (StarsI16)(input->game_turn - planet->turn);
    row->flags = planet->flags;
    row->population = planet->population;
    row->production = planet->production;
    row->survival_percent = planet->survival_percent;
    row->resources = planet->resources;
    dump_planet_copy_i32(row->mineral_concentration,
        planet->mineral_concentration, 3);
    dump_planet_copy_i32(row->mineral_rate, planet->mineral_rate, 3);
    dump_planet_copy_i16(row->environment, planet->environment, 3);
    dump_planet_copy_i16(row->environment_original,
        planet->environment_original, 3);
    row->scanner_range = planet->scanner_range;
    row->scanner_detail = planet->scanner_detail;
    row->starbase_id = planet->starbase_id;
    row->starbase_level = planet->starbase_level;
    row->wormhole_id = planet->wormhole_id;
    row->wormhole_distance = planet->wormhole_distance;
    row->mine_count = planet->mine_count;
    if (!input->detailed) {
        dump_planet_clear_detailed(row);
    } else {
        dump_planet_clear_unavailable(row, planet->flags);
    }
    if (!input->detailed) {
        dump_planet_clear_unavailable(row, planet->flags);
    }
}

StarsI16 stars_dump_planets(StarsDumpPlanetsInput *input)
{
    StarsDumpPlanetRow row;
    StarsI16 index;
    StarsI16 header_count;

    if (!input || !input->file_name || !input->planets ||
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
    for (index = 0; index < (StarsI16)input->planet_count; ++index) {
        dump_planet_make_row(input, &input->planets[index], &row);
        if (input->write_row) {
            if (!input->write_row(input->context, &row)) {
                input->close_report(input->context);
                return 0;
            }
        } else if (input->format_text_field && input->write_text_value) {
            StarsI16 field;
            StarsI16 text_length;
            StarsU8 text[256];

            /* The detailed report adds environment/scanner/starbase fields
             * at the original report-service edge.  The twenty-column
             * normal export is the stable serialized boundary currently
             * exposed by this coordinator. */
            for (field = 0; field < STARS_DUMP_PLANET_TEXT_FIELD_COUNT;
                    ++field) {
                text_length = input->format_text_field(input->context,
                    field, &row, text, (StarsU16)sizeof text);
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

            for (field = 0; field < STARS_DUMP_PLANET_TEXT_FIELD_COUNT;
                    ++field)
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

StarsI16 stars_dump_planets_packed(StarsDumpPlanetsPackedInput *input)
{
    StarsDumpPlanet planet;
    StarsDumpPlanetsInput report;
    StarsI16 index;

    if (!input || !input->planet_record || input->planet_size <
            STARS_DUMP_PLANET_RECORD_SIZE)
        return 0;
    planet.id = 0;
    planet.owner = 0;
    planet.flags = 0;
    planet.turn = 0;
    planet.population = 0L;
    planet.production = 0L;
    planet.survival_percent = 0L;
    planet.resources = 0L;
    planet.scanner_range = 0;
    planet.scanner_detail = 0;
    planet.starbase_id = 0;
    planet.starbase_level = 0;
    planet.wormhole_id = 0;
    planet.wormhole_distance = 0;
    planet.mine_count = 0;
    for (index = 0; index < 3; ++index) {
        planet.environment[index] = 0;
        planet.environment_original[index] = 0;
        planet.mineral_concentration[index] = 0L;
        planet.mineral_rate[index] = 0L;
    }
    dump_planets_packed_load(input->planet_record, &planet,
        input->resources, input->survival_percent, input->scanner_detail);
    if (input->resolve_services)
        input->resolve_services(input->report.context, input->planet_record,
            input->planet_size, &planet);
    report = input->report;
    report.planets = &planet;
    report.planet_count = 1;
    return stars_dump_planets(&report);
}
