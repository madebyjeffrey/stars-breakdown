#include "core.h"

static StarsU16 draw_popup_packed_get16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static StarsU32 draw_popup_packed_get32(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU32)draw_popup_packed_get16(record, offset) |
        ((StarsU32)draw_popup_packed_get16(record,
            (StarsU16)(offset + 2U)) << 16);
}

static StarsI16 draw_popup_packed_geti16(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsI16)draw_popup_packed_get16(record, offset);
}

static void draw_popup_payload_zero(StarsDrawPopupPayload *payload)
{
    StarsI16 index;

    for (index = 0; index < STARS_DRAW_POPUP_DATA_SIZE; ++index)
        payload->raw[index] = 0;
    for (index = 0; index < 5; ++index)
        payload->rgi[index] = 0;
    for (index = 0; index < 8; ++index)
        payload->part[index] = 0;
    payload->gr_popup = 0;
    payload->id_planet = 0;
    payload->planet_var = 0;
    payload->planet_value = 0;
    payload->planet_minimum = 0;
    payload->planet_maximum = 0;
    payload->player_value = 0;
    payload->player_minimum = 0;
    payload->player_maximum = 0;
    payload->id_plan = 0;
    payload->c_max = 0;
    payload->c_current = 0;
    payload->c_operate = 0;
    payload->factory = 0;
    payload->dx_out = 0;
    payload->psz_near = 0;
    payload->player = 0;
    payload->fleet_far = 0;
    payload->red_damage = 0;
    payload->dx_damage = 0;
    payload->grbit = 0;
    payload->shdef_far = 0;
    payload->show_damage = 0;
    payload->hide_counts = 0;
    payload->token = 0;
    payload->summary = 0;
    payload->token_index = 0;
}

static void draw_popup_payload_load(const StarsU8 *record,
    StarsDrawPopupPayload *payload)
{
    StarsI16 index;

    draw_popup_payload_zero(payload);
    for (index = 0; index < STARS_DRAW_POPUP_DATA_SIZE; ++index)
        payload->raw[index] = record[index];
    payload->gr_popup = draw_popup_packed_geti16(record, 0);
    for (index = 0; index < 5; ++index)
        payload->rgi[index] = (StarsI32)draw_popup_packed_get32(record,
            (StarsU16)(2U + index * 4));
    payload->id_planet = draw_popup_packed_geti16(record, 2);
    payload->planet_var = draw_popup_packed_geti16(record, 4);
    payload->planet_value = draw_popup_packed_geti16(record, 6);
    payload->planet_minimum = draw_popup_packed_geti16(record, 8);
    payload->planet_maximum = draw_popup_packed_geti16(record, 10);
    payload->player_value = draw_popup_packed_geti16(record, 12);
    payload->player_minimum = draw_popup_packed_geti16(record, 14);
    payload->player_maximum = draw_popup_packed_geti16(record, 16);
    payload->id_plan = draw_popup_packed_geti16(record, 2);
    payload->c_max = draw_popup_packed_geti16(record, 4);
    payload->c_current = draw_popup_packed_geti16(record, 6);
    payload->c_operate = draw_popup_packed_geti16(record, 8);
    payload->factory = draw_popup_packed_geti16(record, 10);
    payload->dx_out = draw_popup_packed_geti16(record, 2);
    payload->psz_near = draw_popup_packed_get32(record, 4);
    payload->player = draw_popup_packed_geti16(record, 2);
    payload->fleet_far = draw_popup_packed_get32(record, 2);
    payload->red_damage = draw_popup_packed_geti16(record, 6);
    payload->dx_damage = draw_popup_packed_geti16(record, 8);
    payload->grbit = draw_popup_packed_get16(record, 10);
    for (index = 0; index < 8; ++index)
        payload->part[index] = record[2 + index];
    payload->shdef_far = draw_popup_packed_get32(record, 2);
    payload->show_damage = draw_popup_packed_geti16(record, 6);
    payload->hide_counts = draw_popup_packed_geti16(record, 8);
    payload->token = draw_popup_packed_geti16(record, 10);
    payload->summary = draw_popup_packed_geti16(record, 12);
    payload->token_index = draw_popup_packed_geti16(record, 14);
}

static void draw_popup_text_zero(StarsDrawPopupText *text)
{
    text->popup_kind = 0;
    text->row = 0;
    text->role = 0;
    text->string_id = -1;
    text->format_kind = STARS_DRAW_POPUP_FORMAT_NONE;
    text->value1 = 0;
    text->value2 = 0;
    text->visible = 1;
    text->bold = 0;
    text->text_color = 0;
    text->hull_index = -1;
    text->x = 0;
    text->y = 0;
    text->anchor = STARS_DRAW_POPUP_ANCHOR_LEFT;
    text->font_kind = STARS_DRAW_POPUP_FONT_NORMAL;
    text->background_mode = 2;
}

static void draw_popup_text_position(StarsDrawPopupText *text,
    StarsI16 x, StarsI16 y, StarsI16 anchor, StarsI16 font_kind,
    StarsI16 background_mode)
{
    text->x = x;
    text->y = y;
    text->anchor = anchor;
    text->font_kind = font_kind;
    text->background_mode = background_mode;
    text->bold = font_kind == STARS_DRAW_POPUP_FONT_BOLD;
}

static void draw_popup_emit_text(const StarsDrawPopupOps *ops,
    StarsDrawPopupText *text)
{
    StarsU8 buffer[128];
    StarsI16 length;
    StarsI16 index;

    if (!ops->emit_text)
        return;
    for (index = 0; index < (StarsI16)sizeof buffer; ++index)
        buffer[index] = 0;
    length = 0;
    if (text->format_kind != STARS_DRAW_POPUP_FORMAT_NONE &&
            ops->format_text)
        length = ops->format_text(ops->context, text, buffer,
            (StarsU16)sizeof buffer);
    else if (text->string_id >= 0 && ops->resolve_string)
        length = ops->resolve_string(ops->context, text->string_id,
            buffer, (StarsU16)sizeof buffer);
    if (length < 0)
        length = 0;
    ops->emit_text(ops->context, text, buffer, length);
}

static void draw_popup_emit_string_at(const StarsDrawPopupOps *ops,
    StarsI16 popup_kind, StarsI16 row, StarsI16 role, StarsI16 string_id,
    StarsI16 bold, StarsI16 x, StarsI16 y, StarsI16 anchor,
    StarsI16 font_kind, StarsI16 background_mode)
{
    StarsDrawPopupText text;

    draw_popup_text_zero(&text);
    text.popup_kind = popup_kind;
    text.row = row;
    text.role = role;
    text.string_id = string_id;
    draw_popup_text_position(&text, x, y, anchor, font_kind,
        background_mode);
    text.bold = bold;
    draw_popup_emit_text(ops, &text);
}

static void draw_popup_emit_value_at(const StarsDrawPopupOps *ops,
    StarsI16 popup_kind, StarsI16 row, StarsI16 role, StarsI16 format_kind,
    StarsI32 value1, StarsI32 value2, StarsI16 bold, StarsI16 color,
    StarsI16 x, StarsI16 y, StarsI16 anchor, StarsI16 font_kind,
    StarsI16 background_mode)
{
    StarsDrawPopupText text;

    draw_popup_text_zero(&text);
    text.popup_kind = popup_kind;
    text.row = row;
    text.role = role;
    text.format_kind = format_kind;
    text.value1 = value1;
    text.value2 = value2;
    text.text_color = color;
    draw_popup_text_position(&text, x, y, anchor, font_kind,
        background_mode);
    text.bold = bold;
    draw_popup_emit_text(ops, &text);
}

static StarsI16 draw_popup_measure_string(const StarsDrawPopupOps *ops,
    StarsI16 popup_kind, StarsI16 role, StarsI16 string_id,
    StarsI16 bold)
{
    StarsDrawPopupText text;

    if (!ops->measure_text)
        return 0;
    draw_popup_text_zero(&text);
    text.popup_kind = popup_kind;
    text.role = role;
    text.string_id = string_id;
    draw_popup_text_position(&text, 0, 0, STARS_DRAW_POPUP_ANCHOR_LEFT,
        bold ? STARS_DRAW_POPUP_FONT_BOLD : STARS_DRAW_POPUP_FONT_NORMAL, 2);
    text.bold = bold;
    return ops->measure_text(ops->context, &text);
}

static StarsI16 draw_popup_measure_value(const StarsDrawPopupOps *ops,
    StarsI16 popup_kind, StarsI16 role, StarsI16 format_kind,
    StarsI32 value1, StarsI32 value2, StarsI16 bold)
{
    StarsDrawPopupText text;

    if (!ops->measure_text)
        return 0;
    draw_popup_text_zero(&text);
    text.popup_kind = popup_kind;
    text.role = role;
    text.format_kind = format_kind;
    text.value1 = value1;
    text.value2 = value2;
    draw_popup_text_position(&text, 0, 0, STARS_DRAW_POPUP_ANCHOR_LEFT,
        bold ? STARS_DRAW_POPUP_FONT_BOLD : STARS_DRAW_POPUP_FONT_NORMAL, 2);
    text.bold = bold;
    return ops->measure_text(ops->context, &text);
}

static void draw_popup_emit_case1(const StarsDrawPopupOps *ops,
    const StarsDrawPopupCase *popup, StarsI16 line_height)
{
    StarsI16 suffix;
    StarsI16 low;
    StarsI16 high;
    StarsI16 center;
    StarsI16 label_x;
    StarsI16 title_width;

    center = (StarsI16)(popup->client_rect.right >> 1);
    title_width = draw_popup_measure_value(ops, popup->popup_kind,
        STARS_DRAW_POPUP_TEXT_TITLE, STARS_DRAW_POPUP_FORMAT_LONG,
        popup->payload.rgi[0], 0, 1);
    label_x = (StarsI16)(title_width + 4);

    draw_popup_emit_value_at(ops, popup->popup_kind, 0,
        STARS_DRAW_POPUP_TEXT_TITLE, STARS_DRAW_POPUP_FORMAT_LONG,
        popup->payload.rgi[0], 0, 1, 0, center, 4,
        STARS_DRAW_POPUP_ANCHOR_CENTER, STARS_DRAW_POPUP_FONT_BOLD, 2);
    draw_popup_emit_string_at(ops, popup->popup_kind, 1,
        STARS_DRAW_POPUP_TEXT_LABEL, 0x265, 0, label_x,
        (StarsI16)(line_height + 4), STARS_DRAW_POPUP_ANCHOR_RIGHT,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
    if (popup->payload.planet_maximum < 0)
        draw_popup_emit_string_at(ops, popup->popup_kind, 1,
            STARS_DRAW_POPUP_TEXT_VALUE, 0x254, 0, label_x,
            (StarsI16)(line_height + 4), STARS_DRAW_POPUP_ANCHOR_LEFT,
            STARS_DRAW_POPUP_FONT_NORMAL, 2);
    else
        draw_popup_emit_value_at(ops, popup->popup_kind, 1,
            STARS_DRAW_POPUP_TEXT_VALUE,
            STARS_DRAW_POPUP_FORMAT_I16_PAIR,
            popup->payload.planet_maximum,
            popup->payload.player_value, 0, 0, label_x,
            (StarsI16)(line_height + 4), STARS_DRAW_POPUP_ANCHOR_LEFT,
            STARS_DRAW_POPUP_FONT_NORMAL, 2);
    draw_popup_emit_string_at(ops, popup->popup_kind, 2,
        STARS_DRAW_POPUP_TEXT_LABEL, 0x264, 0, label_x,
        (StarsI16)(line_height * 2 + 4), STARS_DRAW_POPUP_ANCHOR_RIGHT,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
    if (popup->payload.player_maximum < 0 ||
            (popup->payload.player_maximum < 1 &&
             popup->payload.player_minimum == 0))
        draw_popup_emit_string_at(ops, popup->popup_kind, 2,
            STARS_DRAW_POPUP_TEXT_VALUE, 0x254, 0, label_x,
            (StarsI16)(line_height * 2 + 4), STARS_DRAW_POPUP_ANCHOR_LEFT,
            STARS_DRAW_POPUP_FONT_NORMAL, 2);
    else {
        draw_popup_emit_value_at(ops, popup->popup_kind, 2,
            STARS_DRAW_POPUP_TEXT_VALUE,
            STARS_DRAW_POPUP_FORMAT_I16_PAIR,
            popup->payload.player_minimum,
            popup->payload.player_maximum, 0, 0, label_x,
            (StarsI16)(line_height * 2 + 4), STARS_DRAW_POPUP_ANCHOR_LEFT,
            STARS_DRAW_POPUP_FONT_NORMAL, 2);
        if (popup->payload.planet_value != 0 ||
                popup->payload.planet_minimum != 0) {
            suffix = (popup->payload.player_maximum < 0 ||
                (popup->payload.player_maximum < 1 &&
                 (StarsU16)popup->payload.player_minimum < 30U)) ?
                (StarsI16)0x51f : (StarsI16)0x51e;
            draw_popup_emit_string_at(ops, popup->popup_kind, 2,
                STARS_DRAW_POPUP_TEXT_VALUE, suffix, 0, label_x,
                (StarsI16)(line_height * 2 + 4),
                STARS_DRAW_POPUP_ANCHOR_LEFT,
                STARS_DRAW_POPUP_FONT_NORMAL, 2);
        }
    }
    low = (StarsI16)((StarsU32)popup->payload.rgi[4] & 0xffffUL);
    high = (StarsI16)(((StarsU32)popup->payload.rgi[4] >> 16) & 0xffffUL);
    if (high >= 0) {
        draw_popup_emit_string_at(ops, popup->popup_kind, 3,
            STARS_DRAW_POPUP_TEXT_LABEL, 0x266, 0, label_x,
            (StarsI16)(line_height * 3 + 4), STARS_DRAW_POPUP_ANCHOR_RIGHT,
            STARS_DRAW_POPUP_FONT_NORMAL, 2);
        draw_popup_emit_value_at(ops, popup->popup_kind, 3,
            STARS_DRAW_POPUP_TEXT_VALUE,
            STARS_DRAW_POPUP_FORMAT_LONG_PAIR, low, high, 0, 0, label_x,
            (StarsI16)(line_height * 3 + 4), STARS_DRAW_POPUP_ANCHOR_LEFT,
            STARS_DRAW_POPUP_FONT_NORMAL, 2);
    }
}

static void draw_popup_emit_case2(const StarsDrawPopupOps *ops,
    const StarsDrawPopupCase *popup, StarsI16 line_height)
{
    StarsI16 center;

    center = (StarsI16)(popup->client_rect.right >> 1);
    draw_popup_emit_value_at(ops, popup->popup_kind, 0,
        STARS_DRAW_POPUP_TEXT_PLANET_NAME, STARS_DRAW_POPUP_FORMAT_I16,
        popup->payload.id_planet, 0, 0, 0, center, 4,
        STARS_DRAW_POPUP_ANCHOR_CENTER, STARS_DRAW_POPUP_FONT_NORMAL, 2);
    draw_popup_emit_string_at(ops, popup->popup_kind, 1,
        STARS_DRAW_POPUP_TEXT_LABEL, 0x262, 0, center,
        (StarsI16)(line_height + 4), STARS_DRAW_POPUP_ANCHOR_CENTER,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
    draw_popup_emit_value_at(ops, popup->popup_kind, 1,
        STARS_DRAW_POPUP_TEXT_FLEET_NAME, STARS_DRAW_POPUP_FORMAT_LONG,
        (StarsI32)popup->payload.fleet_far, 0, 0, 0, center,
        (StarsI16)(line_height + 4), STARS_DRAW_POPUP_ANCHOR_CENTER,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
}

static StarsI16 draw_popup_hull_count(const StarsU8 *record,
    StarsI16 hull)
{
    return (StarsI16)draw_popup_packed_get16(record,
        (StarsU16)(12U + hull * 2));
}

static StarsI16 draw_popup_hull_damage(const StarsU8 *record,
    StarsI16 hull)
{
    return (StarsI16)draw_popup_packed_get16(record,
        (StarsU16)(44U + hull * 2));
}

static void draw_popup_emit_case3(const StarsDrawPopupOps *ops,
    const StarsDrawPopupInput *input, const StarsDrawPopupCase *popup,
    StarsI16 line_height)
{
    StarsI16 hull;
    StarsI16 row;
    StarsI16 count;
    StarsI16 damage;
    StarsI16 visible;
    StarsI16 color;
    StarsI16 tech;
    StarsI32 percent;
    StarsI16 damage_level;
    StarsU8 name[64];
    StarsI16 name_length;
    StarsI16 right;
    StarsI16 separator_x;
    StarsI16 row_y;

    right = (StarsI16)(popup->client_rect.right - 4);
    if (popup->client_rect.bottom - popup->client_rect.top <
            (StarsU16)(line_height * 2)) {
        draw_popup_emit_string_at(ops, popup->popup_kind, 0,
            STARS_DRAW_POPUP_TEXT_VALUE, 0x263, 0, 4, 4,
            STARS_DRAW_POPUP_ANCHOR_LEFT, STARS_DRAW_POPUP_FONT_NORMAL, 2);
        return;
    }
    draw_popup_emit_string_at(ops, popup->popup_kind, 0,
        STARS_DRAW_POPUP_TEXT_LABEL, 0x25b, 1, 4, 4,
        STARS_DRAW_POPUP_ANCHOR_LEFT, STARS_DRAW_POPUP_FONT_BOLD, 2);
    if (popup->payload.planet_minimum != 0) {
        separator_x = (StarsI16)(right - popup->payload.red_damage);
        draw_popup_emit_string_at(ops, popup->popup_kind, 0,
            STARS_DRAW_POPUP_TEXT_SEPARATOR, 0xbf4, 0, separator_x, 4,
            STARS_DRAW_POPUP_ANCHOR_RIGHT, STARS_DRAW_POPUP_FONT_NORMAL, 2);
        draw_popup_emit_string_at(ops, popup->popup_kind, 0,
            STARS_DRAW_POPUP_TEXT_LABEL, 0x347, 0, right, 4,
            STARS_DRAW_POPUP_ANCHOR_RIGHT, STARS_DRAW_POPUP_FONT_NORMAL, 2);
        if (ops->draw_separator)
            ops->draw_separator(ops->context, popup->hdc, 4,
                (StarsI16)(line_height + 2),
                (StarsI16)(popup->client_rect.right - 8), 1, 0x42);
    }
    if (!input->packed_fleet_record || input->packed_fleet_size < 124U)
        return;
    row = 1;
    tech = 0;
    if (input->packed_shdef_record && input->packed_shdef_size >= 147U)
        tech = (StarsI16)((draw_popup_packed_get16(
            input->packed_shdef_record, 0) >> 9) & 0x0fU);
    for (hull = 0; hull < 16; ++hull) {
        count = draw_popup_hull_count(input->packed_fleet_record, hull);
        if (count <= 0)
            continue;
        visible = 1;
        if (popup->payload.planet_maximum != 0 &&
                popup->payload.planet_maximum != 0xff) {
            visible = ops->hull_visible ?
                ops->hull_visible(ops->context, hull,
                    popup->payload.player, popup->payload.grbit) : 0;
        }
        if (!visible)
            continue;
        damage = draw_popup_hull_damage(input->packed_fleet_record, hull);
        color = popup->payload.planet_value != 0 && damage != 0 ? 0xff : 0;
        name_length = 0;
        for (visible = 0; visible < (StarsI16)sizeof name; ++visible)
            name[visible] = 0;
        if (ops->decorate_hull)
            name_length = ops->decorate_hull(ops->context, tech, hull,
                name, (StarsU16)sizeof name);
        if (name_length < 0)
            name_length = 0;
        {
            StarsDrawPopupText text;
            draw_popup_text_zero(&text);
            text.popup_kind = popup->popup_kind;
            text.row = row;
            text.role = STARS_DRAW_POPUP_TEXT_HULL_NAME;
            text.value1 = count;
            text.value2 = damage;
            text.text_color = color;
            text.hull_index = hull;
            row_y = (StarsI16)(line_height * row + 4);
            draw_popup_text_position(&text, 4, row_y,
                STARS_DRAW_POPUP_ANCHOR_LEFT,
                STARS_DRAW_POPUP_FONT_NORMAL, 2);
            if (ops->emit_text)
                ops->emit_text(ops->context, &text, name, name_length);
        }
        draw_popup_emit_value_at(ops, popup->popup_kind, row,
            STARS_DRAW_POPUP_TEXT_VALUE, STARS_DRAW_POPUP_FORMAT_I16,
            count, 0, 0, color, right,
            (StarsI16)(line_height * row + 4),
            STARS_DRAW_POPUP_ANCHOR_RIGHT, STARS_DRAW_POPUP_FONT_NORMAL, 2);
        if (popup->payload.planet_value != 0 && damage != 0) {
            percent = ((StarsI32)(damage & 0x7f) * (StarsI32)count) /
                100L;
            if (percent < 1)
                percent = 1;
            damage_level = (StarsI16)((StarsU16)damage / 0x280U);
            if (damage_level == 0)
                damage_level = 1;
            draw_popup_emit_value_at(ops, popup->popup_kind, row,
                STARS_DRAW_POPUP_TEXT_VALUE,
                STARS_DRAW_POPUP_FORMAT_PERCENT_PAIR,
                percent, damage_level, 0, color, right,
                (StarsI16)(line_height * row + 4),
                STARS_DRAW_POPUP_ANCHOR_RIGHT,
                STARS_DRAW_POPUP_FONT_NORMAL, 2);
        }
        ++row;
    }
    if (popup->payload.planet_value != 0 && ops->set_text_color)
        (void)ops->set_text_color(ops->context, popup->hdc, 0);
}

static void draw_popup_emit_case4(const StarsDrawPopupOps *ops,
    const StarsDrawPopupInput *input, const StarsDrawPopupCase *popup,
    StarsI16 line_height)
{
    StarsI16 label_x;

    if (!input->coordinate_valid)
        return;
    label_x = (StarsI16)(draw_popup_measure_string(ops, popup->popup_kind,
        STARS_DRAW_POPUP_TEXT_LABEL, 0x25c, 1) + 4);
    draw_popup_emit_string_at(ops, popup->popup_kind, 0,
        STARS_DRAW_POPUP_TEXT_LABEL, 0x25c, 1, label_x, 4,
        STARS_DRAW_POPUP_ANCHOR_RIGHT, STARS_DRAW_POPUP_FONT_BOLD, 2);
    draw_popup_emit_value_at(ops, popup->popup_kind, 0,
        STARS_DRAW_POPUP_TEXT_PLANET_NAME,
        STARS_DRAW_POPUP_FORMAT_I16, input->coordinate_planet, 0, 0, 0,
        label_x, 4, STARS_DRAW_POPUP_ANCHOR_LEFT,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
    draw_popup_emit_string_at(ops, popup->popup_kind, 1,
        STARS_DRAW_POPUP_TEXT_LABEL, 0x261, 0, label_x,
        (StarsI16)(line_height + 4), STARS_DRAW_POPUP_ANCHOR_RIGHT,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
    draw_popup_emit_value_at(ops, popup->popup_kind, 1,
        STARS_DRAW_POPUP_TEXT_VALUE, STARS_DRAW_POPUP_FORMAT_I16,
        input->coordinate_planet + 1, 0, 0, 0, label_x,
        (StarsI16)(line_height + 4), STARS_DRAW_POPUP_ANCHOR_LEFT,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
    draw_popup_emit_string_at(ops, popup->popup_kind, 2,
        STARS_DRAW_POPUP_TEXT_LABEL, 0x260, 0, label_x,
        (StarsI16)(line_height * 2 + 4), STARS_DRAW_POPUP_ANCHOR_RIGHT,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
    draw_popup_emit_value_at(ops, popup->popup_kind, 2,
        STARS_DRAW_POPUP_TEXT_VALUE, STARS_DRAW_POPUP_FORMAT_I16,
        input->coordinate_x, 0, 0, 0, label_x,
        (StarsI16)(line_height * 2 + 4), STARS_DRAW_POPUP_ANCHOR_LEFT,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
    draw_popup_emit_string_at(ops, popup->popup_kind, 3,
        STARS_DRAW_POPUP_TEXT_LABEL, 0x25f, 0, label_x,
        (StarsI16)(line_height * 3 + 4), STARS_DRAW_POPUP_ANCHOR_RIGHT,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
    draw_popup_emit_value_at(ops, popup->popup_kind, 3,
        STARS_DRAW_POPUP_TEXT_VALUE, STARS_DRAW_POPUP_FORMAT_I16,
        input->coordinate_y, 0, 0, 0, label_x,
        (StarsI16)(line_height * 3 + 4), STARS_DRAW_POPUP_ANCHOR_LEFT,
        STARS_DRAW_POPUP_FONT_NORMAL, 2);
}

static void draw_popup_emit_content(const StarsDrawPopupOps *ops,
    const StarsDrawPopupInput *input, const StarsDrawPopupCase *popup,
    StarsI16 line_height)
{
    switch (popup->popup_kind) {
    case 1:
        draw_popup_emit_case1(ops, popup, line_height);
        break;
    case 2:
        draw_popup_emit_case2(ops, popup, line_height);
        break;
    case 3:
        draw_popup_emit_case3(ops, input, popup, line_height);
        break;
    case 4:
        draw_popup_emit_case4(ops, input, popup, line_height);
        break;
    default:
        break;
    }
}

/* Typed semantic reconstruction of DrawPopup, NE 25:01c0-0c7c.
 * The dispatcher owns the Win16 state discipline, the complete 22-byte
 * _popupdata union payload, and geometry derived from the client rectangle.
 * Localization and detailed GDI text rendering stay behind typed per-kind
 * callbacks. */

static void draw_popup_case(const StarsDrawPopupOps *ops,
    StarsDrawPopupCase *popup, StarsDrawPopupDrawCase draw_case)
{
    if (draw_case)
        draw_case(ops->context, popup);
}

void stars_draw_popup(StarsDrawPopupInput *input)
{
    const StarsDrawPopupOps *ops;
    StarsDrawPopupCase popup;
    StarsRect client;
    StarsU32 old_background;
    StarsU32 old_text;
    StarsU16 old_mode;
    StarsU16 old_font;
    StarsI16 line_height;

    if (!input || !input->ops)
        return;
    ops = input->ops;
    client.left = 0;
    client.top = 0;
    client.right = 0;
    client.bottom = 0;
    old_background = 0;
    old_text = 0;
    old_mode = 0;
    old_font = 0;
    line_height = input->arial8_height > 0 ? input->arial8_height : 8;
    if (input->packed_popupdata && input->packed_popupdata_size >= 22U)
        input->popup_kind = (StarsI16)draw_popup_packed_get16(
            input->packed_popupdata, 0);
    if (ops->set_background_color)
        old_background = ops->set_background_color(ops->context, input->hdc,
            0xffffffUL);
    if (ops->set_text_color)
        old_text = ops->set_text_color(ops->context, input->hdc, 0);
    if (ops->set_background_mode)
        old_mode = ops->set_background_mode(ops->context, input->hdc, 2);
    if (ops->select_font)
        old_font = ops->select_font(ops->context, input->hdc,
            input->font_bold);
    if (ops->get_client_rect)
        ops->get_client_rect(ops->context, input->hwnd, &client);

    popup.hwnd = input->hwnd;
    popup.hdc = input->hdc;
    popup.popup_kind = input->popup_kind;
    popup.client_rect = client;
    popup.slot_rect = client;
    popup.value1 = 0;
    popup.value2 = 0;
    popup.starbase_mode = input->slot_starbase_mode;
    draw_popup_payload_zero(&popup.payload);
    if (input->packed_popupdata && input->packed_popupdata_size >= 22U)
        draw_popup_payload_load(input->packed_popupdata, &popup.payload);

    draw_popup_emit_content(ops, input, &popup, line_height);

    switch (input->popup_kind) {
    case 1:
        draw_popup_case(ops, &popup, ops->draw_mineral);
        break;
    case 2:
        draw_popup_case(ops, &popup, ops->draw_fleet_name);
        break;
    case 3:
        draw_popup_case(ops, &popup, ops->draw_fleet);
        break;
    case 4:
        draw_popup_case(ops, &popup, ops->draw_coordinates);
        break;
    case 5:
        popup.value1 = 1;
        draw_popup_case(ops, &popup, ops->draw_planet_state);
        break;
    case 6:
        popup.value1 = (StarsI16)(client.right >> 1);
        popup.value2 = 1;
        draw_popup_case(ops, &popup, ops->draw_zip_order);
        break;
    case 7:
        popup.value1 = 1;
        draw_popup_case(ops, &popup, ops->draw_planet_population);
        break;
    case 8:
        popup.value1 = (StarsI16)client.right;
        popup.value2 = 1;
        draw_popup_case(ops, &popup, ops->draw_factory_mine);
        break;
    case 9:
        popup.value1 = (StarsI16)client.right;
        popup.value2 = (StarsI16)client.bottom;
        draw_popup_case(ops, &popup, ops->draw_component);
        break;
    case 10:
        popup.value1 = (StarsI16)client.right;
        popup.value2 = 1;
        draw_popup_case(ops, &popup, ops->draw_string);
        break;
    case 11:
    case 14:
        popup.value1 = -1;
        popup.slot_rect.top = (StarsU16)(line_height + 0x132);
        popup.slot_rect.left = (StarsU16)(popup.slot_rect.left + 6);
        popup.slot_rect.right = client.right;
        popup.slot_rect.bottom = client.bottom;
        if (ops->select_font)
            (void)ops->select_font(ops->context, input->hdc,
                input->font_bold);
        if (ops->set_background_mode)
            (void)ops->set_background_mode(ops->context, input->hdc, 1);
        draw_popup_case(ops, &popup, ops->draw_slot_hull);
        break;
    case 12:
        popup.value1 = (StarsI16)client.right;
        popup.value2 = 1;
        draw_popup_case(ops, &popup, ops->draw_resource);
        break;
    default:
        draw_popup_case(ops, &popup, ops->draw_default);
        break;
    }

    if (ops->select_font)
        ops->select_font(ops->context, input->hdc, old_font);
    if (ops->set_background_mode)
        ops->set_background_mode(ops->context, input->hdc, old_mode);
    if (ops->set_text_color)
        ops->set_text_color(ops->context, input->hdc, old_text);
    if (ops->set_background_color)
        ops->set_background_color(ops->context, input->hdc, old_background);
}
