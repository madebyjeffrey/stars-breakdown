#include "core.h"
#include <string.h>

static StarsI16 vc_extent(const StarsDrawVcReportInput *input, StarsU16 dc,
    const StarsU8 *text, StarsU16 length)
{
    if (!input->text_extent || !text)
        return 0;
    return input->text_extent(input->context, dc, text, length);
}

static StarsI16 vc_string(const StarsDrawVcReportInput *input, StarsI16 id,
    StarsU8 *work, StarsU16 capacity)
{
    StarsI16 length;

    work[0] = 0;
    if (!input->get_string)
        return 0;
    length = input->get_string(input->context, id, work, capacity);
    if (length < 0)
        length = 0;
    if ((StarsU16)length >= capacity)
        length = (StarsI16)(capacity - 1U);
    work[length] = 0;
    return length;
}

static void vc_append_suffix(StarsU8 *work, StarsU16 capacity,
    const StarsU8 *suffix, StarsI16 *length)
{
    StarsU16 suffix_length;
    StarsU16 available;

    if (!suffix || !length || *length < 0)
        return;
    suffix_length = (StarsU16)strlen((const char *)suffix);
    available = (StarsU16)(capacity - 1U - (StarsU16)*length);
    if (suffix_length > available)
        suffix_length = available;
    if (suffix_length != 0U) {
        memcpy(work + *length, suffix, suffix_length);
        *length = (StarsI16)(*length + suffix_length);
        work[*length] = 0;
    }
}

static StarsI16 vc_player_is_dead(const StarsDrawVcPlayerState *player)
{
    return (StarsI16)(((player->detection_flags & 0x0100U) != 0U) &&
        player->dead_flags != 0U);
}

static void vc_set_color(const StarsDrawVcReportInput *input, StarsU16 dc,
    StarsU32 color)
{
    if (input->set_text_color)
        input->set_text_color(input->context, dc, color);
}

static void vc_icon(const StarsDrawVcReportInput *input, StarsU16 dc,
    StarsU16 mono_dc, StarsI16 x, StarsI16 y, StarsU32 rop)
{
    if (input->bit_blt)
        input->bit_blt(input->context, dc, x, y, 0x0e, 0x0c, mono_dc,
            0, 0, rop);
}

/* NE 22:168E-1E0C. Draw the victory report's player names, score markers,
 * and two-column value table. The old Win16 services are represented by
 * callbacks; all report arithmetic and dispatch remains ordinary C. */
void stars_draw_vc_report(StarsU16 hdc, const StarsDrawVcReportInput *input)
{
    StarsU8 work[128];
    StarsU8 digit[2];
    StarsI16 row;
    StarsI16 half;
    StarsI16 value_index;
    StarsI16 value;
    StarsI16 value_length;
    StarsI16 width;
    StarsI16 name_width;
    StarsI16 x;
    StarsI16 y;
    StarsI16 value_x;
    StarsI16 line_y;
    StarsI16 value_start;
    StarsI16 label_cursor;
    StarsI16 row_value;
    StarsI16 row_base;
    StarsI16 inner;
    StarsI16 i;
    StarsI16 marker_check;
    StarsI16 scaled;
    StarsU16 mono_dc;
    StarsU16 old_bitmap;
    StarsU16 flags;
    const StarsU8 *name;
    const StarsDrawVcPlayerState *player;

    if (!input)
        return;
    if (input->set_background_mode)
        input->set_background_mode(input->context, hdc, 1);
    if (input->select_object)
        input->select_object(input->context, hdc, input->font_arial8_large);
    x = input->score_x;
    for (i = 0; i < input->player_count; ++i) {
        name = input->player_name ? input->player_name(input->context, i) : 0;
        name_width = name ? vc_extent(input, hdc, name,
            (StarsU16)strlen((const char *)name)) : 0;
        player = input->players ? &input->players[i] : 0;
        if (!player || !vc_player_is_dead(player)) {
            if (!player || (player->score_flags & 0x4000U) == 0U)
                vc_set_color(input, hdc, 0);
            else
                vc_set_color(input, hdc, 0x00ff0000UL);
        } else {
            vc_set_color(input, hdc, 0x007f7f7fUL);
        }
        if (input->text_out && name)
            input->text_out(input->context, hdc,
                (StarsI16)(x - name_width + (input->line_height * 3) / 2),
                (StarsI16)(-(input->line_height / 2) - name_width + 0x54),
                name, (StarsU16)strlen((const char *)name));
        x = (StarsI16)(x + (input->line_height * 3) / 2);
    }

    if (input->select_object)
        input->select_object(input->context, hdc, input->font_arial8_small);
    if (input->select_object)
        input->select_object(input->context, hdc, input->brush_button_shadow);
    digit[0] = (StarsU8)'8';
    digit[1] = 0;
    (void)vc_extent(input, hdc, digit, 1);
    mono_dc = input->create_compatible_dc ?
        input->create_compatible_dc(input->context, hdc) : 0;
    old_bitmap = input->select_object && mono_dc ?
        input->select_object(input->context, mono_dc, input->bitmap_mono) : 0;
    vc_set_color(input, hdc, 0);
    if (input->set_background_color)
        input->set_background_color(input->context, hdc, 0x00ffffffUL);
    x = (StarsI16)(input->score_x + 4);
    width = (StarsI16)(((input->line_height * 3) / 2 - 12) / 2);
    y = (StarsI16)(((input->line_height * 3) / 2 - 11) / 2 -
        input->line_height / 4);
    for (i = 0; i < input->player_count; ++i) {
        flags = input->players ? (StarsU16)((input->players[i].score_flags >> 6) & 0xffU) : 0;
        for (inner = 0; inner < 7; ++inner) {
            if ((flags & 1U) != 0U) {
                vc_icon(input, hdc, mono_dc, (StarsI16)(x + width),
                    (StarsI16)(y + 0x58 + (inner * input->line_height * 3) / 2),
                    0x8800c6UL);
                player = input->players ? &input->players[i] : 0;
                marker_check = inner + (inner > 1 ? 1 : 0);
                if ((!player || !vc_player_is_dead(player)) &&
                    input->get_check && input->get_check(input->context,
                        marker_check) != 0) {
                    if (!player || (player->score_flags & 0x4000U) != 0U)
                        vc_set_color(input, hdc, 0x00ff0000UL);
                    else {
                        flags >>= 1;
                        continue;
                    }
                } else {
                    vc_set_color(input, hdc, input->color_button_shadow);
                }
                if (input->set_background_color)
                    input->set_background_color(input->context, hdc, 0);
                vc_icon(input, hdc, mono_dc, (StarsI16)(x + width),
                    (StarsI16)(y + 0x58 + (inner * input->line_height * 3) / 2),
                    0xee0086UL);
                vc_set_color(input, hdc, 0);
                if (input->set_background_color)
                    input->set_background_color(input->context, hdc,
                        0x00ffffffUL);
            }
            flags >>= 1;
        }
        if (input->pat_blt)
            input->pat_blt(input->context, hdc, x,
                (StarsI16)(0x58 - input->line_height / 4), 1,
                (StarsI16)((input->line_height * 0x15) / 2 + 1),
                0xf00021UL);
        x = (StarsI16)(x + (input->line_height * 3) / 2);
    }
    if (input->pat_blt)
        input->pat_blt(input->context, hdc, x,
            (StarsI16)(0x58 - input->line_height / 4), 1,
            (StarsI16)((input->line_height * 0x15) / 2 + 1), 0xf00021UL);
    if (input->select_object && mono_dc)
        input->select_object(input->context, mono_dc, old_bitmap);
    if (input->delete_dc && mono_dc)
        input->delete_dc(input->context, mono_dc);

    line_y = 0x58;
    value_start = 0x3aa;
    value_index = 0;
    for (row = 0; row <= 8; ++row) {
        row_base = value_index;
        for (half = 0; half < 2; ++half) {
            if (half == 0) {
                value_x = 0;
            } else {
                value_start = (StarsI16)(value_start - 3);
                value_x = row < 7 ? (StarsI16)(input->score_x - x) : 8;
                if (row < 8) {
                    if (input->pat_blt)
                        input->pat_blt(input->context, hdc,
                            (StarsI16)(input->score_x + 4),
                            (StarsI16)(line_y - input->line_height / 4),
                            (StarsI16)((input->line_height * input->player_count * 3) / 2),
                            1, 0xf00021UL);
                    if (row == 7)
                        line_y = (StarsI16)(line_y + input->line_height / 2);
                }
            }
            row_value = row_base;
            label_cursor = value_start;
            value_length = vc_string(input, value_start, work,
                (StarsU16)sizeof(work));
            if (half == 1) {
                if (row < 7 && input->get_check &&
                    input->get_check(input->context, row_value) == 0)
                    vc_set_color(input, hdc, 0x007f7f7fUL);
                else
                    vc_set_color(input, hdc, 0);
                if (input->text_out)
                    input->text_out(input->context, hdc, value_x, line_y,
                        work, (StarsU16)value_length);
            }
            x = (StarsI16)(value_x + vc_extent(input, hdc, work,
                (StarsU16)value_length));
            for (inner = 0; inner < 2; ++inner) {
                label_cursor = (StarsI16)(label_cursor + 1);
                if (inner == 1 && row != 1) {
                    label_cursor = (StarsI16)(label_cursor + 1);
                    break;
                }
                value = input->get_value ? input->get_value(input->context,
                    row_value, 0) : 0;
                if (row == 0) {
                    scaled = (StarsI16)(((StarsI32)value * input->plan_max) / 100);
                    value = scaled;
                    label_cursor = 0x3c5;
                }
                value_length = input->format_value ?
                    input->format_value(input->context, value, work,
                        (StarsU16)sizeof(work)) : 0;
                if (value_length < 0)
                    value_length = 0;
                if (row == 3)
                    vc_append_suffix(work, (StarsU16)sizeof(work),
                        input->percent_suffix, &value_length);
                if (half == 1 && input->text_out)
                    input->text_out(input->context, hdc, x, line_y, work,
                        (StarsU16)value_length);
                x = (StarsI16)(x + vc_extent(input, hdc, work,
                    (StarsU16)value_length));
                if (row != 2 && row != 3)
                    x = (StarsI16)(x + 4);
                value_length = vc_string(input, label_cursor, work,
                    (StarsU16)sizeof(work));
                if (half == 1 && input->text_out)
                    input->text_out(input->context, hdc, x, line_y, work,
                        (StarsU16)value_length);
                x = (StarsI16)(x + vc_extent(input, hdc, work,
                    (StarsU16)value_length));
                row_value = (StarsI16)(row_value + 1);
            }
            if (half == 0) {
                value_start = label_cursor;
            } else {
                value_start = label_cursor;
                value_index = row_value;
            }
        }
        line_y = (StarsI16)(line_y + (input->line_height * 3) / 2);
    }
}
