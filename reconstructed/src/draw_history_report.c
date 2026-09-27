#include "core.h"
#include <string.h>

static StarsI16 history_string(const StarsDrawHistoryReportInput *input,
    StarsI16 id, StarsU8 *work, StarsU16 capacity)
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

static StarsI16 history_active(const StarsDrawHistoryReportInput *input,
    StarsI16 player)
{
    return (StarsI16)(input->series && input->series[player].points &&
        input->series[player].point_count != 0U);
}

static StarsI32 history_value(const StarsDrawHistoryReportInput *input,
    StarsI16 player, StarsI16 point)
{
    if (input->get_value)
        return input->get_value(input->context, player, point);
    return input->series[player].points[point].value;
}

static void history_expand(const StarsDrawHistoryReportInput *input,
    StarsRect *rect, StarsI16 dx, StarsI16 dy)
{
    if (input->expand_rect)
        input->expand_rect(input->context, rect, dx, dy);
    else {
        rect->left = (StarsU16)((StarsI16)rect->left - dx);
        rect->right = (StarsU16)((StarsI16)rect->right + dx);
        rect->top = (StarsU16)((StarsI16)rect->top - dy);
        rect->bottom = (StarsU16)((StarsI16)rect->bottom + dy);
    }
}

static StarsI16 history_format(const StarsDrawHistoryReportInput *input,
    StarsI32 value, StarsU8 *work, StarsU16 capacity, StarsI16 long_value)
{
    StarsI16 length;

    work[0] = 0;
    length = long_value && input->format_long ?
        input->format_long(input->context, value, work, capacity) :
        (!long_value && input->format_short ?
            input->format_short(input->context, value, work, capacity) : 0);
    if (length < 0)
        length = 0;
    if ((StarsU16)length >= capacity)
        length = (StarsI16)(capacity - 1U);
    work[length] = 0;
    return length;
}

static StarsI32 history_axis_range(StarsI16 turn)
{
    StarsI32 range;

    range = ((StarsI32)turn + 4) / 5 * 5;
    if (range < 100) {
        if (range > 50)
            range = (range + 5) / 10 * 10;
    } else {
        range = 100;
    }
    return range;
}

/* NE 22:2494-2F93. Draw the history graph, including its title, axes, grid,
 * player legend, and series. Graph records and all Win16 drawing services are
 * explicit host inputs/callbacks. */
void stars_draw_history_report(StarsU16 hdc,
    const StarsDrawHistoryReportInput *input)
{
    StarsRect graph;
    StarsRect title_rect;
    StarsRect diamond_rect;
    StarsU8 work[128];
    StarsI16 title_length;
    StarsI16 length;
    StarsI16 range_step;
    StarsI16 tick_count;
    StarsI16 tick;
    StarsI16 player;
    StarsI16 point;
    StarsI16 drawn;
    StarsI16 width;
    StarsI16 height;
    StarsI16 left;
    StarsI16 top;
    StarsI16 right;
    StarsI16 bottom;
    StarsI16 x;
    StarsI16 y;
    StarsI16 previous_x;
    StarsI16 previous_y;
    StarsI16 pen;
    StarsI16 old_pen;
    StarsI16 name_y;
    StarsI32 base_turn;
    StarsI32 axis_range;
    StarsI32 maximum;
    StarsI32 value;
    StarsI32 grid_value;
    StarsI32 turn;
    StarsI32 scaled;
    StarsU32 color;
    const StarsU8 *name;

    if (!input)
        return;
    if (input->set_background_mode)
        input->set_background_mode(input->context, hdc, 1);
    graph.left = graph.top = graph.right = graph.bottom = 0;
    if (input->get_client_rect)
        input->get_client_rect(input->context, input->score_dialog, &graph);
    history_expand(input, &graph, -0x30, 0);
    graph.top = (StarsU16)(input->arial10_height + input->arial8_height);
    graph.bottom = (StarsU16)((StarsI16)graph.bottom -
        input->arial8_height * 4);
    title_rect.left = graph.left;
    title_rect.right = graph.right;
    title_rect.top = (StarsU16)(input->arial8_height / 2);
    title_rect.bottom = (StarsU16)((StarsI16)title_rect.top +
        input->arial10_height);
    title_length = history_string(input, 0x3c6, work, (StarsU16)sizeof(work));
    length = history_string(input, (StarsI16)(input->report_type + 0x1b3),
        work + title_length, (StarsU16)(sizeof(work) - title_length));
    if (length > 0)
        work[title_length + length - 1] = 0;
    if (input->select_object)
        input->select_object(input->context, hdc, input->font_arial10);
    if (input->set_text_color)
        input->set_text_color(input->context, hdc, input->color_button_text);
    if (input->center_rect_text)
        input->center_rect_text(input->context, hdc, &title_rect, work,
            (StarsI16)strlen((const char *)work));
    diamond_rect.left = 0x0f;
    diamond_rect.right = (StarsU16)(input->arial10_height + 0x0f);
    diamond_rect.top = title_rect.top;
    diamond_rect.bottom = title_rect.bottom;
    if (input->set_rect)
        input->set_rect(input->context, &diamond_rect, 0x0f,
            (StarsI16)title_rect.top, input->arial10_height + 0x0f,
            (StarsI16)title_rect.top + input->arial10_height);
    if (input->draw_diamond)
        input->draw_diamond(input->context, hdc, &diamond_rect,
            input->brush_blue);
    width = (StarsI16)((StarsI16)graph.right - (StarsI16)graph.left);
    height = (StarsI16)((StarsI16)graph.bottom - (StarsI16)graph.top);
    if (input->pat_blt)
        input->pat_blt(input->context, hdc, (StarsI16)graph.left,
            (StarsI16)graph.top, width, height, 0x42UL);
    if (input->select_object)
        input->select_object(input->context, hdc,
            input->brush_button_hilite);
    if (input->pat_blt) {
        input->pat_blt(input->context, hdc, (StarsI16)graph.left - 1,
            (StarsI16)graph.bottom, width + 3, 1, 0xf00021UL);
        input->pat_blt(input->context, hdc, (StarsI16)graph.left - 2,
            (StarsI16)graph.bottom + 1, width + 4, 1, 0xf00021UL);
        input->pat_blt(input->context, hdc, (StarsI16)graph.right,
            (StarsI16)graph.top - 1, 1, height + 3, 0xf00021UL);
        input->pat_blt(input->context, hdc, (StarsI16)graph.right + 1,
            (StarsI16)graph.top - 2, 1, height + 4, 0xf00021UL);
    }
    if (input->select_object)
        input->select_object(input->context, hdc,
            input->brush_button_shadow);
    if (input->pat_blt) {
        input->pat_blt(input->context, hdc, (StarsI16)graph.left - 2,
            (StarsI16)graph.top - 2, width + 3, 1, 0xf00021UL);
        input->pat_blt(input->context, hdc, (StarsI16)graph.left - 2,
            (StarsI16)graph.top - 1, width + 2, 1, 0xf00021UL);
        input->pat_blt(input->context, hdc, (StarsI16)graph.left - 2,
            (StarsI16)graph.top - 2, 1, height + 3, 0xf00021UL);
        input->pat_blt(input->context, hdc, (StarsI16)graph.left - 1,
            (StarsI16)graph.top - 2, 1, height + 2, 0xf00021UL);
    }
    width -= 4;
    height -= 4;
    history_expand(input, &graph, -2, -2);
    left = (StarsI16)graph.left;
    top = (StarsI16)graph.top;
    right = (StarsI16)graph.right;
    bottom = (StarsI16)graph.bottom;
    width = right - left;
    height = bottom - top;
    base_turn = input->game_turn < 0x65 ? 0 : input->game_turn - 100;
    axis_range = history_axis_range(input->game_turn);
    if (axis_range < 0x33)
        range_step = 5;
    else
        range_step = 10;
    tick_count = (StarsI16)(axis_range / range_step);
    if (input->select_object)
        input->select_object(input->context, hdc, input->font_arial8);
    if (tick_count > 0) {
        for (tick = 0; tick <= tick_count; ++tick) {
            x = (StarsI16)(width * tick / tick_count + left);
            value = base_turn + 0x960L + (StarsI32)tick * range_step;
            length = history_format(input, value, work, (StarsU16)sizeof(work), 0);
            if (input->center_text)
                input->center_text(input->context, hdc, x,
                    bottom + 6, work, length);
            if (tick > 0 && tick < tick_count && input->pat_blt)
                input->pat_blt(input->context, hdc, x, top - 2, 1,
                    height + 4, 0xf00021UL);
        }
    }
    maximum = -1;
    for (player = 0; player < input->player_count; ++player) {
        if (!history_active(input, player))
            continue;
        for (point = 0; point < (StarsI16)input->series[player].point_count;
                ++point) {
            value = history_value(input, player, point);
            if (maximum < value)
                maximum = value;
        }
    }
    if (maximum >= 0) {
        if (maximum < 5)
            maximum = 5;
        if (maximum < 13) grid_value = 1;
        else if (maximum < 26) grid_value = 2;
        else if (maximum < 61) grid_value = 5;
        else if (maximum < 121) grid_value = 10;
        else if (maximum < 301) grid_value = 25;
        else if (maximum < 601) grid_value = 50;
        else if (maximum < 1201) grid_value = 100;
        else if (maximum < 6001) grid_value = 500;
        else if (maximum < 12001) grid_value = 1000;
        else grid_value = maximum / 12 / 500 * 500;
        for (value = grid_value; value < maximum; value += grid_value) {
            scaled = (StarsI32)height * value / maximum;
            y = (StarsI16)(bottom - scaled);
            if (y < top + input->arial8_height / 2)
                break;
            length = history_format(input, value, work,
                (StarsU16)sizeof(work), 1);
            if (input->right_text)
                input->right_text(input->context, hdc, left - 6,
                    y - input->arial8_height / 2, work, length, 0);
            if (input->pat_blt)
                input->pat_blt(input->context, hdc, left - 2, y,
                    width + 4, 1, 0xf00021UL);
        }
        name_y = (StarsI16)(top + 6);
        for (player = 0; player < input->player_count; ++player) {
            if (!history_active(input, player))
                continue;
            name = input->player_name ? input->player_name(input->context,
                player) : 0;
            color = player == input->current_player ? 0x00ffffffUL :
                input->series[player].color;
            if (input->set_text_color)
                input->set_text_color(input->context, hdc, color);
            if (input->text_out && name)
                input->text_out(input->context, hdc, left + 6, name_y, name,
                    (StarsU16)strlen((const char *)name));
            name_y = (StarsI16)(name_y + input->arial8_height);
        }
        for (player = 0; player < input->player_count; ++player) {
            if (!history_active(input, player))
                continue;
            color = player == input->current_player ? 0x00ffffffUL :
                input->series[player].color;
            pen = input->create_pen ? (StarsI16)input->create_pen(
                input->context, 0, 1, color) : 0;
            old_pen = input->select_object && pen ? (StarsI16)
                input->select_object(input->context, hdc, (StarsU16)pen) : 0;
            drawn = 0;
            for (point = 0; point < (StarsI16)input->series[player].point_count;
                    ++point) {
                turn = input->series[player].points[point].turn;
                if (turn < base_turn || turn > base_turn + axis_range)
                    continue;
                scaled = (turn - base_turn) * width / axis_range;
                x = (StarsI16)(left + scaled);
                value = history_value(input, player, point);
                scaled = value * height / maximum;
                y = (StarsI16)(bottom - scaled);
                if (drawn == 0) {
                    if (input->move_to)
                        input->move_to(input->context, hdc, x, y);
                } else if (input->line_to) {
                    input->line_to(input->context, hdc, x, y);
                }
                previous_x = x;
                previous_y = y;
                drawn = (StarsI16)(drawn + 1);
            }
            if (drawn == 1 && input->set_pixel)
                input->set_pixel(input->context, hdc, previous_x, previous_y,
                    color);
            if (input->select_object && old_pen)
                input->select_object(input->context, hdc, (StarsU16)old_pen);
            if (input->delete_object && pen)
                input->delete_object(input->context, (StarsU16)pen);
        }
    }
}
