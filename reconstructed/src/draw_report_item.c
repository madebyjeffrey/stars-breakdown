#include "core.h"
#include <string.h>

enum {
    REPORT_ITEM_PLANET = 0,
    REPORT_ITEM_FLEET = 1,
    REPORT_ITEM_SCAN_FLEET = 2,
    REPORT_ITEM_BATTLE = 3,
    REPORT_ITEM_TRANSPARENT = 1,
    REPORT_ITEM_TEXT_OPAQUE = 4,
    REPORT_ITEM_PLANET_INSTALLATIONS = 3,
    REPORT_ITEM_BATTLE_UNIT_COUNT = 8,
    REPORT_ITEM_PRIMARY_PERCENT = 6,
    REPORT_ITEM_FUEL_COLUMN = 6,
    REPORT_ITEM_MINERAL_COLUMN = 7
};

static const StarsU8 report_digit[] = "8";
static const StarsI16 report_battle_units[REPORT_ITEM_BATTLE_UNIT_COUNT] = {
    0xff, 0xfd, 0xfe, 0x0b, 0x13, 0x23, 0x43, 0x83
};

static StarsI16 report_strlen(const StarsU8 *text)
{
    if (!text) return 0;
    return (StarsI16)strlen((const char *)text);
}

/* This fallback is deliberately small: callers may provide the game's
 * localized formatter, but the C reconstruction remains usable in a host
 * probe without depending on a C library ABI for 16-bit long values. */
static StarsI16 report_decimal(StarsI32 value, StarsU8 *out, StarsU16 cap)
{
    StarsU8 reversed[16];
    StarsU32 magnitude;
    StarsU16 count;
    StarsU16 i;
    StarsU16 pos;
    StarsI16 negative;

    if (!out || cap == 0U) return 0;
    negative = value < 0;
    magnitude = negative ? (StarsU32)(-(value + 1L)) + 1UL :
        (StarsU32)value;
    count = 0;
    do {
        reversed[count++] = (StarsU8)('0' + (magnitude % 10UL));
        magnitude /= 10UL;
    } while (magnitude != 0UL && count < (StarsU16)sizeof(reversed));
    pos = 0;
    if (negative && pos + 1U < cap) out[pos++] = '-';
    i = count;
    while (i != 0U && pos + 1U < cap) out[pos++] = reversed[--i];
    out[pos] = 0;
    return (StarsI16)pos;
}

static StarsI16 report_format(const StarsDrawReportItemInput *input,
    StarsI32 value, StarsU8 *out, StarsU16 cap, StarsI16 percent)
{
    StarsI16 length;

    if (!out || cap == 0U) return 0;
    out[0] = 0;
    if (percent && input->ops->format_percent)
        length = input->ops->format_percent(input->context, value, out, cap);
    else if (!percent && input->ops->format_number)
        length = input->ops->format_number(input->context, value, out, cap);
    else
        length = report_decimal(value, out, cap);
    if (length < 0) length = 0;
    if ((StarsU16)length >= cap) length = (StarsI16)(cap - 1U);
    out[length] = 0;
    if (percent && !input->ops->format_percent &&
            (StarsU16)length + 1U < cap) {
        out[length++] = '%';
        out[length] = 0;
    }
    return length;
}

static const StarsU8 *report_text(const StarsU8 *text,
    const StarsU8 *fallback)
{
    return text ? text : fallback;
}

static void report_color(const StarsDrawReportItemInput *input,
    StarsU16 hdc, StarsU32 color)
{
    if (input->ops->set_text_color)
        input->ops->set_text_color(input->context, hdc, color);
}

static void report_left(const StarsDrawReportItemInput *input, StarsU16 hdc,
    StarsRect *rect, const StarsU8 *text)
{
    StarsI16 length;

    if (!input->ops->ext_text_out || !rect || !text) return;
    length = report_strlen(text);
    input->ops->ext_text_out(input->context, hdc, (StarsI16)rect->left,
        (StarsI16)rect->top, REPORT_ITEM_TEXT_OPAQUE, rect, text,
        (StarsU16)length);
}

static void report_right(const StarsDrawReportItemInput *input, StarsU16 hdc,
    StarsI16 right, StarsI16 top, const StarsU8 *text, StarsI16 offset)
{
    StarsI16 length;

    if (!input->ops->right_text_out || !text) return;
    length = report_strlen(text);
    input->ops->right_text_out(input->context, hdc, right, top, text,
        length, offset);
}

static void report_mineral(const StarsDrawReportItemInput *input,
    StarsU16 hdc, StarsI16 x, StarsI16 y, StarsI16 mineral, StarsI32 value)
{
    StarsU8 text[32];
    StarsI16 length;

    if (input->ops->draw_mineral) {
        input->ops->draw_mineral(input->context, hdc, x, y, mineral, value);
        return;
    }
    length = report_format(input, value, text, (StarsU16)sizeof(text), 0);
    if (input->ops->right_text_out)
        input->ops->right_text_out(input->context, hdc, x, y, text,
            length, 0);
}

static void report_percent(const StarsDrawReportItemInput *input,
    StarsU16 hdc, StarsI16 right, StarsI16 top, StarsI32 value,
    StarsI16 color_mode)
{
    StarsU8 text[32];
    StarsI16 length;

    if (color_mode > 0)
        report_color(input, hdc, color_mode == 1 ? input->color_disabled :
            input->color_warning);
    length = report_format(input, value, text, (StarsU16)sizeof(text), 1);
    if (input->ops->right_text_out)
        input->ops->right_text_out(input->context, hdc, right, top, text,
            length, 0);
}

static void report_coordinate(const StarsDrawReportItemInput *input,
    const StarsReportItemFleet *fleet, StarsU8 *text, StarsU16 cap)
{
    StarsU8 x[16];
    StarsU8 y[16];
    StarsI16 lx;
    StarsI16 ly;
    StarsU16 pos;

    lx = report_format(input, fleet->position.x, x, (StarsU16)sizeof(x), 0);
    ly = report_format(input, fleet->position.y, y, (StarsU16)sizeof(y), 0);
    pos = 0;
    if (pos + 1U < cap) text[pos++] = '(';
    if ((StarsU16)lx < cap - pos) {
        memcpy(text + pos, x, (size_t)lx);
        pos = (StarsU16)(pos + lx);
    }
    if (pos + 2U < cap) {
        text[pos++] = ',';
        text[pos++] = ' ';
    }
    if ((StarsU16)ly < cap - pos) {
        memcpy(text + pos, y, (size_t)ly);
        pos = (StarsU16)(pos + ly);
    }
    if (pos + 1U < cap) text[pos++] = ')';
    if (pos >= cap) pos = (StarsU16)(cap - 1U);
    text[pos] = 0;
}

static const StarsReportItemPlanet *report_planet_at(
    const StarsDrawReportItemInput *input, StarsI16 row)
{
    if (input->ops->planet_at)
        return input->ops->planet_at(input->context, row);
    return 0;
}

static const StarsReportItemFleet *report_fleet_at(
    const StarsDrawReportItemInput *input, StarsI16 report, StarsI16 row)
{
    if (input->ops->fleet_at)
        return input->ops->fleet_at(input->context, report, row);
    return 0;
}

static const StarsReportItemBattle *report_battle(
    const StarsDrawReportItemInput *input, StarsI16 row)
{
    if (input->ops->battle_at)
        return input->ops->battle_at(input->context, row);
    return 0;
}

static StarsI16 report_width(const StarsDrawReportItemInput *input,
    StarsU16 hdc)
{
    if (!input->ops->text_extent) return 0;
    return input->ops->text_extent(input->context, hdc, report_digit, 1U);
}

static void report_setup(const StarsDrawReportItemInput *input, StarsU16 hdc)
{
    if (input->ops->select_object)
        input->ops->select_object(input->context, hdc, input->font);
    report_color(input, hdc, input->color_black);
    if (input->ops->set_background_mode)
        input->ops->set_background_mode(input->context, hdc,
            REPORT_ITEM_TRANSPARENT);
}

static void report_planet_status(const StarsDrawReportItemInput *input,
    StarsU16 hdc, const StarsRect *rect, const StarsReportItemPlanet *planet)
{
    StarsI16 height;
    StarsI16 i;
    StarsRect bar;

    if (!planet->inhabited || !input->ops->fill_rect) return;
    height = (StarsI16)(((StarsI16)rect->bottom - (StarsI16)rect->top) /
        REPORT_ITEM_PLANET_INSTALLATIONS - 1);
    if (height < 1) return;
    bar.right = (StarsU16)((StarsI16)rect->right - height);
    for (i = 0; i < REPORT_ITEM_PLANET_INSTALLATIONS; ++i) {
        bar.left = bar.right;
        bar.top = (StarsU16)((StarsI16)rect->top + i * (height + 1));
        bar.bottom = (StarsU16)((StarsI16)bar.top + height);
        input->ops->fill_rect(input->context, hdc, &bar,
            planet->brush_status[i] ? planet->brush_status[i] :
            (i == 0 ? input->brush_planet_cargo :
                (i == 1 ? input->brush_planet_warp :
                    input->brush_planet_gate)));
    }
}

static void report_planet_capacity(const StarsDrawReportItemInput *input,
    StarsU16 hdc, const StarsRect *rect, StarsI16 current, StarsI16 maximum)
{
    StarsI16 color_mode;

    color_mode = current < maximum ? 0 : (current == maximum ? 1 : 2);
    report_percent(input, hdc, (StarsI16)rect->right, (StarsI16)rect->top,
        current, color_mode);
}

static void report_planet(const StarsDrawReportItemInput *input, StarsU16 hdc,
    StarsRect *rect, StarsI16 row, StarsI16 column)
{
    const StarsReportItemPlanet *planet;
    const StarsU8 *text;
    StarsU8 work[48];
    StarsI16 width;
    StarsI16 label_width;
    StarsI16 i;
    StarsI16 value;

    planet = report_planet_at(input, row);
    if (!planet) return;
    report_setup(input, hdc);
    width = report_width(input, hdc);
    switch (column) {
    case 0:
        if (input->selection_kind == 1 &&
                input->selected_planet == planet->id)
            report_color(input, hdc, input->color_selected);
        text = report_text(planet->name, input->double_dash);
        report_left(input, hdc, rect, text);
        report_planet_status(input, hdc, rect, planet);
        return;
    case 1:
        text = planet->inhabited ? planet->owner_name : input->double_dash;
        report_left(input, hdc, rect, report_text(text, input->double_dash));
        return;
    case 2:
        if (planet->max_population <= planet->population)
            report_color(input, hdc, input->color_disabled);
        value = (StarsI16)(planet->population * 100L);
        report_mineral(input, hdc, (StarsI16)rect->right,
            (StarsI16)rect->top, -1, value);
        return;
    case 3:
        report_percent(input, hdc, (StarsI16)rect->right,
            (StarsI16)rect->top, planet->capacity_percent, 0);
        return;
    case 4:
        label_width = planet->desirability_label && input->ops->text_extent ?
            input->ops->text_extent(input->context, hdc,
                planet->desirability_label,
                (StarsU16)report_strlen(planet->desirability_label)) : 0;
        if (planet->desirability < 0)
            report_color(input, hdc, input->color_disabled);
        else if (planet->desirability < 11)
            report_color(input, hdc, input->color_warning);
        report_percent(input, hdc, (StarsI16)rect->left + label_width,
            (StarsI16)rect->top, planet->desirability, 0);
        if (planet->optimal_value > planet->desirability)
            report_percent(input, hdc, (StarsI16)rect->right,
                (StarsI16)rect->top, planet->optimal_value, 0);
        return;
    case 5:
        if (input->ops->draw_production)
            input->ops->draw_production(input->context, hdc, rect, planet);
        else
            report_left(input, hdc, rect,
                report_text(planet->production_name, input->double_dash));
        return;
    case 6:
    case 7:
    case 8:
        value = planet->current_installations[column - 6];
        if (column == 8 && value < 1) {
            work[0] = '-'; work[1] = '-'; work[2] = '-'; work[3] = 0;
            report_right(input, hdc, (StarsI16)rect->right,
                (StarsI16)rect->top, work, 0);
        } else if (column == 8) {
            report_percent(input, hdc, (StarsI16)rect->right,
                (StarsI16)rect->top, planet->defense_survival_percent, 0);
        } else {
            report_planet_capacity(input, hdc, rect, value,
                planet->operable_installations[column - 6]);
        }
        return;
    case 9:
    case 10:
        i = 0;
        if (column == 9) {
            for (i = 0; i < 3; ++i)
                report_mineral(input, hdc,
                    (StarsI16)((StarsI16)rect->left + width * (4 + i * 5)),
                    (StarsI16)rect->top, i, planet->mineral_weight[i]);
        } else {
            for (i = 0; i < 3; ++i)
                report_mineral(input, hdc,
                    (StarsI16)((StarsI16)rect->left + width * (3 + i * 4)),
                    (StarsI16)rect->top, i, planet->estimated_minerals[i]);
        }
        return;
    case 11:
        for (i = 0; i < 3; ++i)
            report_mineral(input, hdc,
                (StarsI16)((StarsI16)rect->left + width * (3 + i * 4)),
                (StarsI16)rect->top, i, planet->mineral_concentration[i]);
        return;
    case 12:
        report_mineral(input, hdc, (StarsI16)rect->right,
            (StarsI16)rect->top, -1, planet->resources);
        report_percent(input, hdc, (StarsI16)rect->right,
            (StarsI16)rect->top, planet->research_percent, 0);
        return;
    case 13:
    case 14:
        i = (StarsI16)(column - 13);
        text = planet->next_name[i];
        report_left(input, hdc, rect, report_text(text, input->double_dash));
        return;
    default:
        return;
    }
}

static void report_primary(const StarsDrawReportItemInput *input,
    StarsU16 hdc, StarsRect *rect, const StarsReportItemFleet *fleet,
    StarsI16 width)
{
    StarsRect name_rect;
    StarsU8 work[32];
    StarsI16 name_right;
    StarsI16 extent;

    name_rect = *rect;
    name_right = (StarsI16)rect->right;
    name_rect.right = (StarsU16)(name_right - width * REPORT_ITEM_PRIMARY_PERCENT);
    report_left(input, hdc, &name_rect,
        report_text(fleet->primary_name, input->double_dash));
    report_percent(input, hdc, name_right, (StarsI16)rect->top,
        fleet->primary_count, 0);
    if (fleet->primary_count >= 2 && input->ops->text_extent &&
            input->ops->text_out) {
        work[0] = '+'; work[1] = 0;
        extent = input->ops->text_extent(input->context, hdc, work, 1U);
        input->ops->text_out(input->context, hdc, name_right - extent,
            (StarsI16)rect->top, work, 1U);
    }
}

static StarsI32 report_ship_total(const StarsReportItemFleet *fleet)
{
    StarsI32 total;
    StarsI16 i;

    total = 0;
    for (i = 0; i < STARS_REPORT_ITEM_SHIP_TYPES; ++i)
        total += fleet->ship_counts[i];
    return total;
}

static void report_fleet(const StarsDrawReportItemInput *input, StarsU16 hdc,
    StarsRect *rect, StarsI16 report, StarsI16 row, StarsI16 column)
{
    const StarsReportItemFleet *fleet;
    const StarsU8 *text;
    StarsU8 work[64];
    StarsI16 width;
    StarsI16 i;
    StarsI16 selected;

    fleet = report_fleet_at(input, report, row);
    if (!fleet) return;
    report_setup(input, hdc);
    width = report_width(input, hdc);
    selected = (StarsI16)(input->selection_kind == 2 &&
        (report == REPORT_ITEM_FLEET ? input->selected_fleet == fleet->id :
            input->selected_scan_fleet == fleet->id));
    switch (column) {
    case 0:
        if (selected) report_color(input, hdc, input->color_selected);
        report_left(input, hdc, rect, report_text(fleet->name,
            input->double_dash));
        return;
    case 1:
        report_format(input, (StarsI32)fleet->id + 1L, work,
            (StarsU16)sizeof(work), 0);
        report_right(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, work, 0);
        return;
    case 2:
        if (fleet->location_name)
            text = fleet->location_name;
        else {
            report_coordinate(input, fleet, work, (StarsU16)sizeof(work));
            text = work;
        }
        report_left(input, hdc, rect, text);
        return;
    case 3:
        report_left(input, hdc, rect,
            report_text(fleet->destination_name, input->double_dash));
        return;
    case 4:
        report_right(input, hdc, (StarsI16)rect->right,
            (StarsI16)rect->top,
            report_text(fleet->eta_name, input->double_dash), 0);
        return;
    case 5:
        if (fleet->task_color != 0xffffU)
            report_color(input, hdc, fleet->task_color);
        report_left(input, hdc, rect,
            report_text(fleet->task_name, input->double_dash));
        return;
    case 6:
        report_mineral(input, hdc, (StarsI16)((StarsI16)rect->left + width * 4),
            (StarsI16)rect->top, (StarsI16)-fleet->fuel_safe, fleet->fuel);
        return;
    case 7:
        for (i = 0; i < 4; ++i)
            report_mineral(input, hdc,
                (StarsI16)((StarsI16)rect->left + width * (4 + i * 5)),
                (StarsI16)rect->top, i, fleet->minerals[i]);
        return;
    case 8:
        report_primary(input, hdc, rect, fleet, width);
        return;
    case 9:
        if (fleet->cloak_percent == 0)
            text = input->double_dash;
        else {
            report_format(input, fleet->cloak_percent, work,
                (StarsU16)sizeof(work), 1);
            text = work;
        }
        report_right(input, hdc, (StarsI16)rect->right - 5,
            (StarsI16)rect->top, text, 0);
        return;
    case 10:
        report_left(input, hdc, rect,
            report_text(fleet->plan_name, input->double_dash));
        return;
    case 11:
        report_mineral(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, -1, fleet->weight);
        return;
    default:
        break;
    }

    if (report != REPORT_ITEM_SCAN_FLEET) return;
    switch (column) {
    case 3:
        if (!fleet->scanner_visible)
            text = input->double_dash;
        else {
            report_format(input, fleet->scanner_percent, work,
                (StarsU16)sizeof(work), 1);
            text = work;
        }
        report_right(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, text, 0);
        return;
    case 4:
        report_mineral(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, -1, fleet->minerals[0]);
        return;
    case 5:
        report_primary(input, hdc, rect, fleet, width);
        return;
    case 6:
        report_mineral(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, -1, report_ship_total(fleet));
        return;
    case 7:
        report_mineral(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, -1, fleet->combat_ship_count);
        return;
    case 8:
    case 9:
    case 10:
    case 11:
        report_mineral(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, -1,
            fleet->ship_category_count[column - 8]);
        return;
    default:
        return;
    }
}

static void report_battle_row(const StarsDrawReportItemInput *input,
    StarsU16 hdc, StarsRect *rect, StarsI16 row, StarsI16 column)
{
    const StarsReportItemBattle *battle;
    const StarsU8 *text;
    StarsU8 work[64];
    StarsI16 i;
    StarsI16 length;
    StarsI16 unit;
    StarsI32 value;
    StarsU8 kind[2];

    battle = report_battle(input, row);
    if (!battle) return;
    report_setup(input, hdc);
    switch (column) {
    case 0:
        text = battle->location_name;
        if (!text) {
            StarsReportItemFleet temporary;
            memset(&temporary, 0, sizeof temporary);
            temporary.position = battle->position;
            report_coordinate(input, &temporary, work,
                (StarsU16)sizeof(work));
            text = work;
        }
        if (battle->position.x == input->selected_scan_point.x &&
                battle->position.y == input->selected_scan_point.y)
            report_color(input, hdc, input->color_selected);
        report_left(input, hdc, rect, text);
        return;
    case 1:
        value = battle->units[5];
        if (value != 0L) kind[0] = 'O';
        else if (battle->units[6] != 0L) kind[0] = 'T';
        else kind[0] = ' ';
        kind[1] = 0;
        length = 1;
        if (input->ops->center_text)
            input->ops->center_text(input->context, hdc,
                ((StarsI16)rect->right - (StarsI16)rect->left) / 2 +
                    (StarsI16)rect->left, (StarsI16)rect->top, kind,
                (StarsU16)length);
        return;
    case 2:
        report_mineral(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, -1, battle->cplr);
        return;
    case 0x0b:
    case 0x0c:
        report_mineral(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, -1, battle->kills[column == 0x0b]);
        return;
    case 0x0d:
    case 0x0e:
        unit = column == 0x0d ? 0xfd : 0xfe;
        value = battle->units[unit] - battle->kills[column == 0x0d];
        report_mineral(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, -1, value);
        return;
    default:
        break;
    }
    if (column >= 3 && column <= 10) {
        i = (StarsI16)(column - 3);
        report_mineral(input, hdc, (StarsI16)rect->right - 2,
            (StarsI16)rect->top, -1,
            battle->units[report_battle_units[i]]);
    }
}

/* NE 22:3398-4EBE.  Draw one report cell from a typed planet, fleet, scan
 * fleet, or battle record.  The original four computed jumps are represented
 * by the four explicit renderers above; only Win16 drawing and packed-record
 * resolution cross the callback boundary. */
void stars_draw_report_item(StarsU16 hdc, StarsRect *rect, StarsI16 report,
    StarsI16 row, StarsI16 column, StarsDrawReportItemInput *input)
{
    if (!input || !input->ops || !rect) return;
    if (report == REPORT_ITEM_PLANET)
        report_planet(input, hdc, rect, row, column);
    else if (report == REPORT_ITEM_FLEET || report == REPORT_ITEM_SCAN_FLEET)
        report_fleet(input, hdc, rect, report, row, column);
    else if (report == REPORT_ITEM_BATTLE)
        report_battle_row(input, hdc, rect, row, column);
}
