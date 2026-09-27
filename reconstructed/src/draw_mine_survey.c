#include "core.h"

#define MINE_SURVEY_KIND_NONE 0
#define MINE_SURVEY_KIND_FLEET 2
#define MINE_SURVEY_KIND_PLANET 1
#define MINE_SURVEY_KIND_THING 8
#define MINE_SURVEY_FULL_PLANET 4
#define MINE_SURVEY_FULL_FLEET 2
#define MINE_SURVEY_FULL_THING 8
#define MINE_SURVEY_FULL_PLANET_DETAIL 1
#define MINE_SURVEY_OPAQUE 2U
#define MINE_SURVEY_UNKNOWN_ID (-1)
#define MINE_SURVEY_THING_ID_MASK 0x01ffU
#define MINE_SURVEY_THING_OWNER_SHIFT 9
#define MINE_SURVEY_THING_KIND_SHIFT 13
#define MINE_SURVEY_THING_KIND_MASK 0x0007U
#define MINE_SURVEY_THING_DESTINATION_MASK 0x03ffU
#define MINE_SURVEY_THING_RANGE_SHIFT 10
#define MINE_SURVEY_THING_RANGE_MASK 0x000fU
#define MINE_SURVEY_FLEET_BYTES 124U
#define MINE_SURVEY_PLANET_BYTES 56U
#define MINE_SURVEY_THING_BYTES 18U

static StarsU16 mine_survey_read_u16(const StarsU8 *record, StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[(StarsU16)(offset + 1U)] << 8));
}

static StarsI16 mine_survey_read_i16(const StarsU8 *record, StarsU16 offset)
{
    return (StarsI16)mine_survey_read_u16(record, offset);
}

static StarsI32 mine_survey_read_i32(const StarsU8 *record, StarsU16 offset)
{
    StarsU32 value;

    value = (StarsU32)record[offset];
    value |= (StarsU32)record[(StarsU16)(offset + 1U)] << 8;
    value |= (StarsU32)record[(StarsU16)(offset + 2U)] << 16;
    value |= (StarsU32)record[(StarsU16)(offset + 3U)] << 24;
    return (StarsI32)value;
}

static void mine_survey_decode_packed_fleet(const StarsU8 *record,
    StarsU16 counts[16])
{
    StarsI16 index;

    for (index = 0; index < 16; ++index)
        counts[index] = mine_survey_read_u16(record,
            (StarsU16)(12U + (StarsU16)(index * 2)));
}

static void mine_survey_decode_packed_planet(const StarsU8 *record,
    StarsI16 survey_turn, StarsI16 current_turn,
    StarsDrawMineSurveyPlanetInfo *planet)
{
    StarsI16 index;

    planet->owner = mine_survey_read_i16(record, 2);
    planet->survey_detail = record[4];
    planet->survey_turn = survey_turn;
    planet->current_turn = current_turn;
    for (index = 0; index < 3; ++index) {
        planet->mineral_concentration[index] = record[(StarsU16)(9 + index)];
        planet->environment[index] = (StarsI16)record[(StarsU16)(12 + index)];
        planet->original_environment[index] =
            (StarsI16)record[(StarsU16)(15 + index)];
        planet->minerals[index] = mine_survey_read_i32(record,
            (StarsU16)(28 + (StarsU16)(index * 4)));
    }
}

static void mine_survey_decode_packed_thing(const StarsU8 *record,
    StarsDrawMineSurveyThingInfo *thing)
{
    StarsI16 index;
    StarsU16 worm;
    StarsU16 range;

    thing->id_full = mine_survey_read_u16(record, 0);
    thing->point.x = mine_survey_read_i16(record, 2);
    thing->point.y = mine_survey_read_i16(record, 4);
    thing->mine_count = mine_survey_read_u16(record, 6);
    thing->mine_type = record[12];
    thing->destination_word = 0;
    worm = mine_survey_read_u16(record, 6);
    range = (StarsU16)((worm >> 2) & 0x03ffU);
    if (((StarsU16)(thing->id_full >> MINE_SURVEY_THING_KIND_SHIFT) &
            MINE_SURVEY_THING_KIND_MASK) == 1U)
        thing->destination_word = (StarsU16)(
            (mine_survey_read_u16(record, 12) &
                MINE_SURVEY_THING_DESTINATION_MASK) |
            (StarsU16)((range & MINE_SURVEY_THING_RANGE_MASK) <<
                MINE_SURVEY_THING_RANGE_SHIFT));
    thing->visibility_mask = (StarsU32)mine_survey_read_u16(record, 14);
    for (index = 0; index < 3; ++index)
        thing->amounts[index] = 0;
}

static void mine_survey_copy_planet_summary(
    const StarsDrawMineSurveyPlanetInfo *source,
    StarsDrawMineSurveyPlanetSummary *summary)
{
    StarsI16 index;

    summary->owner = source->owner;
    summary->owner_known = source->owner != MINE_SURVEY_UNKNOWN_ID;
    summary->survey_detail = (StarsI16)source->survey_detail;
    summary->detailed_rows = source->survey_detail > 2U;
    summary->turns_since_survey = (StarsI16)(source->current_turn -
        source->survey_turn);
    for (index = 0; index < 3; ++index) {
        summary->environment[index] = source->environment[index];
        summary->original_environment[index] =
            source->original_environment[index];
        summary->mineral_concentration[index] =
            source->mineral_concentration[index];
        summary->minerals[index] = source->minerals[index];
    }
}

static void mine_survey_decode_thing(
    const StarsDrawMineSurveyThingInfo *source,
    StarsDrawMineSurveyThingSummary *summary)
{
    StarsI16 index;
    StarsU16 kind;

    kind = (StarsU16)((source->id_full >> MINE_SURVEY_THING_KIND_SHIFT) &
        MINE_SURVEY_THING_KIND_MASK);
    summary->kind = (StarsI16)kind;
    summary->id = (StarsI16)(source->id_full & MINE_SURVEY_THING_ID_MASK);
    summary->owner = (StarsI16)((source->id_full >>
        MINE_SURVEY_THING_OWNER_SHIFT) & 0x000fU);
    summary->point = source->point;
    summary->destination_known = 0;
    summary->destination_id = MINE_SURVEY_UNKNOWN_ID;
    summary->range_code = 0;
    if (kind == 1U) {
        summary->destination_id = (StarsI16)(source->destination_word &
            MINE_SURVEY_THING_DESTINATION_MASK);
        summary->range_code = (StarsI16)((source->destination_word >>
            MINE_SURVEY_THING_RANGE_SHIFT) & MINE_SURVEY_THING_RANGE_MASK);
        summary->destination_known = summary->range_code != 0;
    }
    summary->mine_type = (StarsI16)source->mine_type;
    summary->mine_count = (StarsI16)source->mine_count;
    summary->visibility_mask = source->visibility_mask;
    for (index = 0; index < 3; ++index)
        summary->amounts[index] = source->amounts[index];
}

static StarsI16 mine_survey_normalize_kind(const StarsDrawMineSurveyInput *input)
{
    if (input->selected_kind != MINE_SURVEY_FULL_PLANET)
        return input->selected_kind;
    if ((input->selected_full_kind & MINE_SURVEY_FULL_PLANET_DETAIL) != 0)
        return MINE_SURVEY_KIND_PLANET;
    if ((input->selected_full_kind & MINE_SURVEY_FULL_FLEET) != 0)
        return MINE_SURVEY_KIND_FLEET;
    if ((input->selected_full_kind & MINE_SURVEY_FULL_THING) != 0)
        return MINE_SURVEY_KIND_THING;
    return MINE_SURVEY_KIND_NONE;
}

static void mine_survey_emit_patterns(const StarsDrawMineSurveyInput *input,
    StarsU16 memory_dc, StarsI16 kind, StarsI16 object_kind)
{
    static const StarsDrawMineSurveyPattern fleet_patterns[] = {
        { 0, 0, 70, 2, 0xf00021UL, 0 },
        { 0, 2, 2, 68, 0xf00021UL, 0 },
        { 16, 68, 2, 38, 0xf00021UL, 0 },
        { 2, 68, 15, 1, 0xf00021UL, 1 },
        { 1, 69, 15, 1, 0xf00021UL, 1 },
        { 52, 68, 18, 2, 0xf00021UL, 1 },
        { 68, 2, 2, 66, 0xf00021UL, 1 },
        { 69, 1, 1, 1, 0xf00021UL, 1 },
        { 17, 104, 37, 2, 0xf00021UL, 1 },
        { 16, 105, 1, 1, 0xf00021UL, 1 },
        { 52, 70, 2, 34, 0xf00021UL, 1 },
        { 2, 2, 66, 66, 0x42UL, 1 },
        { 18, 68, 34, 36, 0x42UL, 1 }
    };
    static const StarsDrawMineSurveyPattern thing_patterns[] = {
        { 0, 0, 70, 2, 0xf00021UL, 0 },
        { 0, 2, 2, 68, 0xf00021UL, 0 },
        { 8, 68, 2, 38, 0xf00021UL, 0 },
        { 1, 68, 15, 1, 0xf00021UL, 1 },
        { 1, 69, 15, 1, 0xf00021UL, 1 },
        { 26, 68, 18, 2, 0xf00021UL, 1 },
        { 34, 2, 2, 66, 0xf00021UL, 1 },
        { 69, 1, 1, 1, 0xf00021UL, 1 },
        { 17, 104, 37, 2, 0xf00021UL, 1 },
        { 8, 105, 1, 1, 0xf00021UL, 1 },
        { 26, 70, 2, 34, 0xf00021UL, 1 },
        { 1, 2, 66, 66, 0x42UL, 1 },
        { 9, 68, 34, 36, 0x42UL, 1 }
    };
    static const StarsDrawMineSurveyPattern thing_compact_patterns[] = {
        { 0, 0, 70, 2, 0xf00021UL, 0 },
        { 0, 2, 2, 68, 0xf00021UL, 0 },
        { 1, 68, 68, 1, 0xf00021UL, 1 },
        { 1, 69, 69, 1, 0xf00021UL, 1 },
        { 34, 2, 2, 66, 0xf00021UL, 1 },
        { 69, 1, 1, 1, 0xf00021UL, 1 },
        { 1, 2, 66, 66, 0x42UL, 1 }
    };
    const StarsDrawMineSurveyPattern *patterns;
    StarsI16 count;
    StarsI16 index;
    StarsI16 base_x;
    StarsI16 base_y;

    if (!input->ops->draw_panel_pattern || kind == MINE_SURVEY_KIND_NONE)
        return;
    if (kind == MINE_SURVEY_KIND_FLEET) {
        patterns = fleet_patterns;
        count = (StarsI16)(sizeof fleet_patterns / sizeof fleet_patterns[0]);
    } else if (kind == MINE_SURVEY_KIND_THING &&
            (object_kind == 2 || object_kind == 3)) {
        patterns = thing_compact_patterns;
        count = (StarsI16)(sizeof thing_compact_patterns /
            sizeof thing_compact_patterns[0]);
    } else {
        patterns = thing_patterns;
        count = (StarsI16)(sizeof thing_patterns / sizeof thing_patterns[0]);
    }
    base_x = (StarsI16)(input->rect.left + 6);
    base_y = (StarsI16)(input->rect.top + 6);
    for (index = 0; index < count; ++index) {
        StarsDrawMineSurveyPattern pattern;
        pattern = patterns[index];
        pattern.x = (StarsI16)(pattern.x + base_x);
        pattern.y = (StarsI16)(pattern.y + base_y);
        input->ops->draw_panel_pattern(input->ops->context, memory_dc,
            &pattern);
    }
}

/* Genuine bounded C reconstruction of DrawMineSurvey, NE 06:065A-37AB.
 * The panel-specific mineral/environment and packed-record rendering is kept
 * behind typed callbacks until those shared data records are fully recovered. */
void stars_draw_mine_survey(StarsDrawMineSurveyInput *input)
{
    StarsU16 memory_dc;
    StarsU16 old_background_mode;
    StarsU32 old_background_color;
    StarsU32 old_text_color;
    StarsI16 kind;
    StarsI16 drawn;
    StarsI16 slot;
    StarsU32 total_ships;
    StarsU16 packed_fleet_counts[16];
    StarsDrawMineSurveyPlanetInfo packed_planet_info;
    StarsDrawMineSurveyThingInfo packed_thing_info;
    const StarsU16 *fleet_ship_counts;
    const StarsDrawMineSurveyPlanetInfo *planet_info;
    const StarsDrawMineSurveyThingInfo *thing_info;
    StarsI16 thing_kind;

    if (!input || !input->ops)
        return;
    fleet_ship_counts = input->fleet_ship_counts;
    planet_info = input->planet_info;
    thing_info = input->thing_info;
    if (!fleet_ship_counts && input->packed_fleet_record &&
            input->packed_fleet_size >= MINE_SURVEY_FLEET_BYTES) {
        mine_survey_decode_packed_fleet(input->packed_fleet_record,
            packed_fleet_counts);
        fleet_ship_counts = packed_fleet_counts;
    }
    if (!planet_info && input->packed_planet_record &&
            input->packed_planet_size >= MINE_SURVEY_PLANET_BYTES) {
        mine_survey_decode_packed_planet(input->packed_planet_record,
            input->packed_survey_turn, input->packed_current_turn,
            &packed_planet_info);
        planet_info = &packed_planet_info;
    }
    if (!thing_info && input->packed_thing_record &&
            input->packed_thing_size >= MINE_SURVEY_THING_BYTES) {
        mine_survey_decode_packed_thing(input->packed_thing_record,
            &packed_thing_info);
        thing_info = &packed_thing_info;
    }
    old_background_mode = input->ops->get_background_mode ?
        input->ops->get_background_mode(input->ops->context, input->hdc) : 1;
    old_background_color = input->ops->get_background_color ?
        input->ops->get_background_color(input->ops->context, input->hdc) :
        input->base_color;
    old_text_color = input->ops->get_text_color ?
        input->ops->get_text_color(input->ops->context, input->hdc) :
        input->text_color;
    memory_dc = input->ops->create_compatible_dc ?
        input->ops->create_compatible_dc(input->ops->context, input->hdc) : 0;
    if (input->ops->set_background)
        input->ops->set_background(input->ops->context, input->hdc,
            input->base_color);
    if (input->ops->set_text_color)
        input->ops->set_text_color(input->ops->context, input->hdc,
            input->text_color);
    if (input->ops->set_background_mode)
        input->ops->set_background_mode(input->ops->context, input->hdc,
            MINE_SURVEY_OPAQUE);
    kind = mine_survey_normalize_kind(input);
    thing_kind = 0;
    if (thing_info)
        thing_kind = (StarsI16)((thing_info->id_full >>
            MINE_SURVEY_THING_KIND_SHIFT) & MINE_SURVEY_THING_KIND_MASK);
    mine_survey_emit_patterns(input, memory_dc, kind, thing_kind);
    drawn = 0;
    if (kind == MINE_SURVEY_KIND_FLEET && input->selected_fleet !=
            MINE_SURVEY_UNKNOWN_ID) {
        if (input->ops->draw_fleet_panel)
            drawn = input->ops->draw_fleet_panel(input->ops->context,
                memory_dc, &input->rect, input->selected_fleet);
        /* The original fleet branch displays a 16-slot ship total before
         * the status-specific gauges.  Keep that arithmetic in typed C and
         * leave only the actual GDI panel behind the service boundary. */
        if (fleet_ship_counts && input->ops->draw_fleet_summary) {
            total_ships = 0U;
            for (slot = 0; slot < 16; ++slot)
                total_ships += fleet_ship_counts[slot];
            input->ops->draw_fleet_summary(input->ops->context, memory_dc,
                &input->rect, input->selected_fleet, total_ships,
                input->fleet_status, input->fleet_cord);
        }
    }
    else if (kind == MINE_SURVEY_KIND_PLANET && input->selected_planet !=
            MINE_SURVEY_UNKNOWN_ID) {
        /* FLookupPlanet gates the detailed planet branch in the original;
         * keep lookup optional so existing callers can use a fully typed
         * planet-panel adapter without manufacturing packed storage. */
        if (!input->ops->lookup_planet || input->ops->lookup_planet(
                input->ops->context, input->selected_planet)) {
            if (input->ops->draw_planet_panel)
                drawn = input->ops->draw_planet_panel(input->ops->context,
                    memory_dc, &input->rect, input->selected_planet);
            if (planet_info && input->ops->draw_planet_details) {
                StarsDrawMineSurveyPlanetSummary summary;
                mine_survey_copy_planet_summary(planet_info, &summary);
                input->ops->draw_planet_details(input->ops->context,
                    memory_dc, &input->rect, input->selected_planet,
                    &summary);
            }
        }
        if (!drawn && input->ops->draw_unknown_planet)
            (void)input->ops->draw_unknown_planet(input->ops->context,
                memory_dc, &input->rect);
    } else if (kind == MINE_SURVEY_KIND_THING && input->selected_thing !=
            MINE_SURVEY_UNKNOWN_ID) {
        if (input->ops->draw_thing_panel)
            drawn = input->ops->draw_thing_panel(input->ops->context,
                memory_dc, &input->rect, input->selected_thing);
    }
    if (kind == MINE_SURVEY_KIND_THING && input->selected_thing !=
            MINE_SURVEY_UNKNOWN_ID && thing_info &&
            input->ops->draw_thing_details) {
        StarsDrawMineSurveyThingSummary summary;
        mine_survey_decode_thing(thing_info, &summary);
        input->ops->draw_thing_details(input->ops->context, memory_dc,
            &input->rect, input->selected_thing, &summary);
    }
    (void)drawn;
    if (input->ops->set_background_mode)
        input->ops->set_background_mode(input->ops->context, input->hdc,
            old_background_mode);
    if (input->ops->set_text_color)
        input->ops->set_text_color(input->ops->context, input->hdc,
            old_text_color);
    if (input->ops->set_background)
        input->ops->set_background(input->ops->context, input->hdc,
            old_background_color);
    if (input->ops->delete_dc)
        input->ops->delete_dc(input->ops->context, memory_dc);
}
