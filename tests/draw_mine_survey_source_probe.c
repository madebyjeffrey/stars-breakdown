#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 create;
    StarsI16 delete_count;
    StarsI16 fleet;
    StarsI16 planet;
    StarsI16 thing;
    StarsI16 unknown;
    StarsI16 lookup;
    StarsI16 summary;
    StarsI16 planet_details;
    StarsI16 planet_owner_known;
    StarsI16 planet_detailed_rows;
    StarsI16 planet_turns;
    StarsI16 thing_details;
    StarsI16 thing_kind;
    StarsI16 thing_id;
    StarsI16 thing_owner;
    StarsI16 thing_destination_known;
    StarsI16 thing_destination;
    StarsI16 thing_range;
    StarsI16 planet_environment;
    StarsI16 planet_concentration;
    StarsI32 planet_minerals;
    StarsI16 thing_mine_count;
    StarsI16 thing_mine_type;
    StarsI16 thing_point_x;
    StarsU32 total_ships;
    StarsU16 last_mode;
    StarsU32 last_background;
    StarsU32 last_text;
} Trace;

static void put16(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static void put32(StarsU8 *record, StarsU16 offset, StarsU32 value)
{
    record[offset] = (StarsU8)(value & 0xffUL);
    record[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
    record[(StarsU16)(offset + 2U)] = (StarsU8)(value >> 16);
    record[(StarsU16)(offset + 3U)] = (StarsU8)(value >> 24);
}

static StarsU16 create_dc(void *context, StarsU16 hdc)
{
    (void)hdc;
    ++((Trace *)context)->create;
    return 9;
}
static void delete_dc(void *context, StarsU16 hdc)
{
    (void)hdc;
    ++((Trace *)context)->delete_count;
}
static void color(void *context, StarsU16 hdc, StarsU32 value)
{
    (void)hdc;
    if (value == 0x12345678UL)
        ((Trace *)context)->last_background = value;
    else
        ((Trace *)context)->last_text = value;
}
static StarsU32 get_background(void *context, StarsU16 hdc)
{
    (void)context;
    (void)hdc;
    return 0x12345678UL;
}
static StarsU32 get_text(void *context, StarsU16 hdc)
{
    (void)context;
    (void)hdc;
    return 0x87654321UL;
}
static void mode(void *context, StarsU16 hdc, StarsU16 value)
{
    ((Trace *)context)->last_mode = value;
    (void)hdc;
}
static StarsU16 get_mode(void *context, StarsU16 hdc)
{
    (void)context;
    (void)hdc;
    return 7;
}
static StarsI16 fleet(void *context, StarsU16 dc,
    StarsDrawMineSurveyRect *rect, StarsI16 id)
{
    (void)dc;
    (void)rect;
    (void)id;
    ++((Trace *)context)->fleet;
    return 1;
}
static StarsI16 planet(void *context, StarsU16 dc,
    StarsDrawMineSurveyRect *rect, StarsI16 id)
{
    (void)dc;
    (void)rect;
    (void)id;
    ++((Trace *)context)->planet;
    return 0;
}
static void fleet_summary(void *context, StarsU16 dc,
    StarsDrawMineSurveyRect *rect, StarsI16 id, StarsU32 total,
    StarsI16 status, StarsI16 cord)
{
    Trace *trace = (Trace *)context;
    (void)dc;
    (void)rect;
    (void)id;
    (void)status;
    (void)cord;
    ++trace->summary;
    trace->total_ships = total;
}
static StarsI16 lookup_planet(void *context, StarsI16 id)
{
    (void)id;
    ++((Trace *)context)->lookup;
    return ((Trace *)context)->lookup < 2;
}
static StarsI16 thing(void *context, StarsU16 dc,
    StarsDrawMineSurveyRect *rect, StarsI16 id)
{
    (void)dc;
    (void)rect;
    (void)id;
    ++((Trace *)context)->thing;
    return 1;
}
static StarsI16 unknown(void *context, StarsU16 dc,
    StarsDrawMineSurveyRect *rect)
{
    (void)dc;
    (void)rect;
    ++((Trace *)context)->unknown;
    return 1;
}
static void planet_details(void *context, StarsU16 dc,
    StarsDrawMineSurveyRect *rect, StarsI16 id,
    const StarsDrawMineSurveyPlanetSummary *summary)
{
    Trace *trace = (Trace *)context;
    (void)dc;
    (void)rect;
    (void)id;
    ++trace->planet_details;
    trace->planet_owner_known = summary->owner_known;
    trace->planet_detailed_rows = summary->detailed_rows;
    trace->planet_turns = summary->turns_since_survey;
    trace->planet_environment = summary->environment[0];
    trace->planet_concentration = summary->mineral_concentration[0];
    trace->planet_minerals = summary->minerals[0];
}
static void thing_details(void *context, StarsU16 dc,
    StarsDrawMineSurveyRect *rect, StarsI16 id,
    const StarsDrawMineSurveyThingSummary *summary)
{
    Trace *trace = (Trace *)context;
    (void)dc;
    (void)rect;
    (void)id;
    ++trace->thing_details;
    trace->thing_kind = summary->kind;
    trace->thing_id = summary->id;
    trace->thing_owner = summary->owner;
    trace->thing_destination_known = summary->destination_known;
    trace->thing_destination = summary->destination_id;
    trace->thing_range = summary->range_code;
    trace->thing_mine_count = summary->mine_count;
    trace->thing_mine_type = summary->mine_type;
    trace->thing_point_x = summary->point.x;
}

int main(void)
{
    StarsDrawMineSurveyInput input;
    StarsDrawMineSurveyOps ops;
    StarsDrawMineSurveyPlanetInfo planet_info;
    StarsDrawMineSurveyThingInfo thing_info;
    StarsU8 packed_fleet[124];
    StarsU8 packed_planet[56];
    StarsU8 packed_thing[18];
    Trace trace;
    int failures = 0;

#define CHECK(condition) do { \
    if (!(condition)) \
        ++failures; \
} while (0)

    (void)memset(&input, 0, sizeof input);
    (void)memset(&ops, 0, sizeof ops);
    (void)memset(&planet_info, 0, sizeof planet_info);
    (void)memset(&thing_info, 0, sizeof thing_info);
    (void)memset(packed_fleet, 0, sizeof packed_fleet);
    (void)memset(packed_planet, 0, sizeof packed_planet);
    (void)memset(packed_thing, 0, sizeof packed_thing);
    (void)memset(&trace, 0, sizeof trace);
    ops.create_compatible_dc = create_dc;
    ops.delete_dc = delete_dc;
    ops.set_background = color;
    ops.set_text_color = color;
    ops.get_background_mode = get_mode;
    ops.get_background_color = get_background;
    ops.get_text_color = get_text;
    ops.set_background_mode = mode;
    ops.draw_fleet_panel = fleet;
    ops.draw_fleet_summary = fleet_summary;
    ops.draw_planet_panel = planet;
    ops.lookup_planet = lookup_planet;
    ops.draw_thing_panel = thing;
    ops.draw_unknown_planet = unknown;
    ops.draw_planet_details = planet_details;
    ops.draw_thing_details = thing_details;
    ops.context = &trace;
    input.hdc = 3;
    input.selected_kind = 4;
    input.selected_full_kind = 2;
    input.selected_fleet = 6;
    input.selected_planet = 7;
    input.selected_thing = 8;
    planet_info.owner = 4;
    planet_info.survey_detail = 3;
    planet_info.survey_turn = 11;
    planet_info.current_turn = 17;
    input.planet_info = &planet_info;
    {
        static const StarsU16 ship_counts[16] = {
            1, 2, 0, 3, 4, 0, 1, 0,
            0, 0, 2, 0, 0, 1, 0, 5
        };
        input.fleet_ship_counts = ship_counts;
        input.fleet_status = 7;
        input.fleet_cord = 2;
    }
    input.ops = &ops;
    stars_draw_mine_survey(&input);
    CHECK(trace.create == 1 && trace.delete_count == 1 && trace.fleet == 1 &&
        trace.summary == 1 && trace.total_ships == 19U &&
        trace.last_mode == 7 && trace.last_background == 0x12345678UL &&
        trace.last_text == 0x87654321UL);
    input.selected_kind = 1;
    stars_draw_mine_survey(&input);
    CHECK(trace.planet == 1 && trace.unknown == 1 && trace.lookup == 1 &&
        trace.planet_details == 1 && trace.planet_owner_known == 1 &&
        trace.planet_detailed_rows == 1 && trace.planet_turns == 6);
    stars_draw_mine_survey(&input);
    CHECK(trace.planet == 1 && trace.unknown == 2 && trace.lookup == 2);
    input.selected_kind = 8;
    thing_info.id_full = (StarsU16)((1U << 13) | (3U << 9) | 17U);
    thing_info.destination_word = (StarsU16)((6U << 10) | 42U);
    input.thing_info = &thing_info;
    stars_draw_mine_survey(&input);
    CHECK(trace.thing == 1 && trace.thing_details == 1 &&
        trace.thing_kind == 1 && trace.thing_id == 17 &&
        trace.thing_owner == 3 && trace.thing_destination == 42 &&
        trace.thing_range == 6 && trace.thing_destination_known);
    input.selected_kind = 0;
    stars_draw_mine_survey(&input);
    CHECK(trace.planet == 1 && trace.unknown == 2);

    (void)memset(&trace, 0, sizeof trace);
    planet_info.owner = -1;
    planet_info.survey_detail = 2;
    planet_info.survey_turn = 9;
    planet_info.current_turn = 11;
    input.selected_kind = 1;
    input.selected_planet = 7;
    stars_draw_mine_survey(&input);
    CHECK(trace.planet == 1 && trace.lookup == 1 &&
        trace.planet_details == 1 && trace.planet_owner_known == 0 &&
        trace.planet_detailed_rows == 0 && trace.planet_turns == 2 &&
        trace.unknown == 1);

    (void)memset(&trace, 0, sizeof trace);
    thing_info.id_full = (StarsU16)((2U << 13) | (5U << 9) | 31U);
    thing_info.destination_word = (StarsU16)((6U << 10) | 42U);
    input.selected_kind = 8;
    input.selected_thing = 8;
    stars_draw_mine_survey(&input);
    CHECK(trace.thing == 1 && trace.thing_details == 1 &&
        trace.thing_kind == 2 && trace.thing_id == 31 &&
        trace.thing_owner == 5 && !trace.thing_destination_known &&
        trace.thing_destination == -1 && trace.thing_range == 0);

    (void)memset(&trace, 0, sizeof trace);
    input.selected_kind = 8;
    input.selected_thing = -1;
    stars_draw_mine_survey(&input);
    CHECK(trace.thing == 0 && trace.thing_details == 0 &&
        trace.unknown == 0 && trace.create == 1 && trace.delete_count == 1);

    input.ops = (const StarsDrawMineSurveyOps *)0;
    stars_draw_mine_survey(&input);
    CHECK(input.ops == (const StarsDrawMineSurveyOps *)0);

    /* The same panel paths must accept the recovered raw Win16 records, not
     * only pre-decoded host structs. */
    (void)memset(&trace, 0, sizeof trace);
    put16(packed_fleet, 12, 2);
    put16(packed_fleet, 14, 3);
    put16(packed_fleet, 16, 5);
    put16(packed_fleet, 18, 7);
    input.ops = &ops;
    input.fleet_ship_counts = (const StarsU16 *)0;
    input.planet_info = (const StarsDrawMineSurveyPlanetInfo *)0;
    input.thing_info = (const StarsDrawMineSurveyThingInfo *)0;
    input.packed_fleet_record = packed_fleet;
    input.packed_fleet_size = sizeof packed_fleet;
    input.packed_planet_record = packed_planet;
    input.packed_planet_size = sizeof packed_planet;
    input.packed_thing_record = packed_thing;
    input.packed_thing_size = sizeof packed_thing;
    input.selected_kind = 4;
    input.selected_full_kind = 2;
    stars_draw_mine_survey(&input);
    CHECK(trace.summary == 1 && trace.total_ships == 17U);

    put16(packed_planet, 2, 4);
    packed_planet[4] = 3;
    packed_planet[9] = 7;
    packed_planet[12] = 11;
    put32(packed_planet, 28, 1234UL);
    input.packed_survey_turn = 11;
    input.packed_current_turn = 17;
    input.selected_kind = 1;
    input.selected_planet = 7;
    stars_draw_mine_survey(&input);
    CHECK(trace.planet_details == 1 && trace.planet_owner_known == 1 &&
        trace.planet_environment == 11 &&
        trace.planet_concentration == 7 && trace.planet_minerals == 1234L);

    put16(packed_thing, 0, (StarsU16)((1U << 13) | (3U << 9) | 17U));
    put16(packed_thing, 2, 321U);
    put16(packed_thing, 4, 654U);
    put16(packed_thing, 6, (StarsU16)(6U << 2));
    put16(packed_thing, 12, 42U);
    packed_thing[12] = 42U;
    input.selected_kind = 8;
    input.selected_thing = 8;
    stars_draw_mine_survey(&input);
    CHECK(trace.thing_details == 1 && trace.thing_kind == 1 &&
        trace.thing_id == 17 && trace.thing_owner == 3 &&
        trace.thing_destination == 42 && trace.thing_range == 6 &&
        trace.thing_point_x == 321);

    if (failures == 0)
        puts("DRAW_MINE_SURVEY_SOURCE=PASS");
    else
        puts("DRAW_MINE_SURVEY_SOURCE=FAIL");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("DMSR.TXT", "w");
        if (!report)
            return 7;
        fprintf(report,
            "DRAW_MINE_SURVEY_CHECKS=12 FAILURES=%d RESULT=%s\n",
            failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#endif
    return failures != 0;
}
