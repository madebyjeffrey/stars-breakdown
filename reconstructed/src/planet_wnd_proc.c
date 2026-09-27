#include "core.h"

/* Typed semantic reconstruction of PlanetWndProc, NE 0a:0000-0d15.
 * Win16 packed transfer records and the original game drawing helpers remain
 * callback-owned. USER/GDI message services are explicit typed callbacks so
 * the recovered WM_CREATE and message boundaries remain visible. */

static StarsI16 planet_button(const StarsPlanetWndProcInput *input,
    StarsU16 handle)
{
    StarsI16 i;

    for (i = 0; i < 13; ++i)
        if (input->button_handles[i] == handle)
            return i;
    return -1;
}

static StarsI16 planet_notify(StarsI32 l_param)
{
    return (StarsI16)((StarsU32)l_param >> 16);
}

static void planet_transfer(StarsPlanetWndProcInput *input, StarsI16 mode)
{
    StarsI16 selection;
    StarsI16 object_id;
    StarsI16 object_kind;

    if (!input->ops->get_dropdown_selection || !input->ops->find_transfer ||
            !input->ops->transfer)
        return;
    selection = input->ops->get_dropdown_selection(input->ops->context);
    if (selection < 0)
        return;
    if (input->ops->find_transfer(input->ops->context, input->selected_planet,
            selection, &object_id, &object_kind))
        input->ops->transfer(input->ops->context, input->selected_planet,
            mode, object_id, object_kind);
}

static void planet_char_navigation(StarsPlanetWndProcInput *input)
{
    if (input->ops->try_select_adj_planet &&
            input->ops->try_select_adj_planet(input->ops->context,
                input->selected_planet, input->selected_planet))
        return;
    if (input->ops->select_adj_starbase)
        input->ops->select_adj_starbase(input->ops->context,
            input->selected_planet, 1);
}

static StarsU16 planet_create(const StarsPlanetWndLayoutOps *ops,
    StarsU16 parent, StarsU16 class_kind, StarsU16 title_token,
    const StarsU8 *title, StarsU32 style, StarsI16 height)
{
    if (!ops->create_window)
        return 0;
    return ops->create_window(ops->context, class_kind, title_token, title,
        style, 100, 100, 200, height, parent);
}

static void planet_font(const StarsPlanetWndLayoutOps *ops, StarsU16 hwnd,
    StarsU16 font)
{
    if (ops->set_font && hwnd)
        ops->set_font(ops->context, hwnd, font);
}

static const StarsU8 *planet_string(const StarsPlanetWndLayoutOps *ops,
    StarsU16 id)
{
    if (!ops->get_compressed_string)
        return (const StarsU8 *)0;
    return ops->get_compressed_string(ops->context, id);
}

void stars_planet_wnd_create_layout(StarsU16 hwnd,
    StarsPlanetWndProcInput *input)
{
    const StarsPlanetWndLayoutOps *ops;
    StarsPlanetWndLayoutState *state;
    StarsRect rect;
    StarsU16 index;
    StarsU16 font;
    StarsI32 real_proc;

    if (!input || !input->layout_ops || !input->layout_state)
        return;
    ops = input->layout_ops;
    state = input->layout_state;
    font = ops->font_ariel8_bold;
    for (index = 0; index < 3; ++index) {
        state->order_dropdowns[index] = planet_create(ops, hwnd, 1, 0x0876,
            (const StarsU8 *)0, index == 1 ? 0x40200213UL : 0x40200003UL,
            index == 2 ? 0xa0 : 0x50);
        planet_font(ops, state->order_dropdowns[index], font);
    }
    for (index = 0; index < 10; ++index)
        (void)planet_string(ops, (StarsU16)(0x63 + index));
    state->order_edit = planet_create(ops, hwnd, 2, 0, (const StarsU8 *)0,
        0x40800002UL, 0x32);
    if (ops->set_edit_limit && state->order_edit)
        ops->set_edit_limit(ops->context, state->order_edit, 4);
    planet_font(ops, state->order_edit, font);
    if (ops->get_window_long && state->order_edit)
        real_proc = ops->get_window_long(ops->context, state->order_edit, -4);
    else
        real_proc = 0;
    state->real_edit_proc = real_proc;
    if (ops->set_window_long && state->order_edit)
        ops->set_window_long(ops->context, state->order_edit, -4,
            ops->fake_edit_proc);
    state->battle_dropdown = planet_create(ops, hwnd, 1, 0x087c,
        (const StarsU8 *)0, 0x40200003UL, 0x50);
    planet_font(ops, state->battle_dropdown, font);
    state->ship_dropdown = planet_create(ops, hwnd, 1, 0x0885,
        (const StarsU8 *)0, 0x40200213UL, 0x50);
    planet_font(ops, state->ship_dropdown, font);
    rect.left = rect.top = rect.right = rect.bottom = 0;
    if (ops->get_client_rect && state->ship_dropdown)
        ops->get_client_rect(ops->context, state->ship_dropdown, &rect);
    state->ship_dropdown_height = rect.bottom;
    state->ship_list = planet_create(ops, hwnd, 3, 0x088c,
        (const StarsU8 *)0, 0x40a01001UL, 0x50);
    planet_font(ops, state->ship_list, font);
    state->fleet_component_list = planet_create(ops, hwnd, 3, 0x0893,
        (const StarsU8 *)0, 0x40a00051UL, 0x50);
    planet_font(ops, state->fleet_component_list, ops->font_ariel8);
    state->planet_production_list = planet_create(ops, hwnd, 3, 0x089f,
        (const StarsU8 *)0, 0x40a00051UL, 0x50);
    planet_font(ops, state->planet_production_list, ops->font_ariel8);
    for (index = 0; index < 13; ++index) {
        state->buttons[index] = planet_create(ops, hwnd, 4,
            (StarsU16)(0x19f + index),
            planet_string(ops, (StarsU16)(0x19f + index)), 0x40000000UL,
            (StarsI16)(ops->ariel8_height << 1));
        planet_font(ops, state->buttons[index], font);
    }
    state->report_combo = planet_create(ops, hwnd, 4, 799,
        planet_string(ops, 799), 0x40000003UL, ops->ariel8_height);
    planet_font(ops, state->report_combo, font);
    input->ship_dropdown = state->ship_dropdown;
    input->planet_production_list = state->planet_production_list;
    input->rep_combo = state->report_combo;
    for (index = 0; index < 13; ++index)
        input->button_handles[index] = state->buttons[index];
}

StarsI32 stars_planet_wnd_proc(StarsU16 hwnd, StarsU16 message,
    StarsU16 w_param, StarsI32 l_param, StarsPlanetWndProcInput *input)
{
    StarsI16 notify;
    StarsI16 button;
    StarsI16 x;
    StarsI16 y;
    StarsI16 double_click;
    StarsU16 child;
    StarsU16 cursor;
    StarsU16 dc;
    StarsI16 draw_action;
    StarsRect rect;
    StarsPoint cursor_point;
    StarsPoint point;

    if (!input || !input->ops)
        return 0;
    if (message == 1) {
        if (input->layout_ops && input->layout_state)
            stars_planet_wnd_create_layout(hwnd, input);
        else if (input->ops->create_controls)
            input->ops->create_controls(input->ops->context, hwnd);
        return 0;
    }
    if (message == 0x0f) {
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->begin_paint)
            dc = input->ops->begin_paint(input->ops->context, hwnd, &rect);
        else
            dc = w_param;
        if (input->ops->paint_planet_ship)
            input->ops->paint_planet_ship(input->ops->context, hwnd, dc);
        if (input->ops->end_paint)
            input->ops->end_paint(input->ops->context, hwnd, &rect);
        return 0;
    }
    if (message == 0x14) {
        if (input->ops->get_client_rect && input->ops->fill_rect) {
            rect.left = rect.top = rect.right = rect.bottom = 0;
            input->ops->get_client_rect(input->ops->context, hwnd, &rect);
            input->ops->fill_rect(input->ops->context, w_param, &rect,
                input->button_face_brush);
        } else if (input->ops->default_window_proc)
            input->ops->default_window_proc(input->ops->context);
        return 1;
    }
    if (message == 0x19) {
        child = (StarsU16)((StarsU32)l_param & 0xffffU);
        if (child == input->rep_combo && input->ops->color_control)
            return input->ops->color_control(input->ops->context, w_param,
                w_param, l_param);
        goto default_proc;
    }
    if (message == 0x20) {
        cursor = 0;
        if (input->ops->get_cursor_pos && input->ops->screen_to_client &&
                input->ops->get_client_rect && input->ops->point_in_rect &&
                input->ops->click_ship_orders &&
                input->ops->click_planet_orders) {
            input->ops->get_cursor_pos(input->ops->context, &cursor_point);
            input->ops->screen_to_client(input->ops->context, hwnd,
                &cursor_point);
            rect.left = rect.top = rect.right = rect.bottom = 0;
            input->ops->get_client_rect(input->ops->context, hwnd, &rect);
            if (input->ops->point_in_rect(input->ops->context, &rect,
                    cursor_point)) {
                cursor = input->ops->click_ship_orders(input->ops->context,
                    cursor_point, 0, 1, 0);
                if (cursor == 0)
                    cursor = input->ops->click_planet_orders(
                        input->ops->context, cursor_point, 0, 1, 0);
            }
        } else if (input->ops->cursor_hit_test)
            cursor = input->ops->cursor_hit_test(input->ops->context, hwnd,
                input->selected_planet, input->selected_fleet);
        if (cursor != 0) {
            if (input->ops->set_cursor_value)
                input->ops->set_cursor_value(input->ops->context, cursor);
            else if (input->ops->set_cursor)
                input->ops->set_cursor(input->ops->context, hwnd);
            return 1;
        }
        goto default_proc;
    }
    if (message == 0x24) {
        if (input->ops->set_minimum_size_exact && l_param != 0) {
            input->ops->set_minimum_size_exact(input->ops->context, l_param,
                (StarsI16)(input->frame_dx * 2 + 0xc6),
                (StarsI16)(input->frame_dy * 2 + 0xc6 +
                    input->title_bar_height));
        } else if (input->ops->set_minimum_size)
            input->ops->set_minimum_size(input->ops->context);
        return 0;
    }
    if (message == 0x2b) {
        draw_action = input->ops->get_draw_item_action ?
            input->ops->get_draw_item_action(input->ops->context, l_param) :
            -2;
        if (draw_action == -1 && input->ops->draw_item_focus)
            input->ops->draw_item_focus(input->ops->context, l_param, -2);
        else if ((draw_action == 1 || draw_action == 2 || draw_action == 4) &&
                input->ops->draw_item_entire)
            input->ops->draw_item_entire(input->ops->context, l_param, -4);
        else if (input->ops->draw_combo_item)
            input->ops->draw_combo_item(input->ops->context, l_param);
        return 1;
    }
    if (message == 0x2c) {
        if (input->ops->measure_combo_item)
            input->ops->measure_combo_item(input->ops->context, l_param,
                (StarsI16)(input->ariel8_height + 2));
        return 1;
    }
    if (message == 0x102) {
        if (w_param == 0x66 || w_param == 0x46)
            planet_char_navigation(input);
        else
            goto default_proc;
        return 0;
    }
    if (message == 0x111) {
        notify = planet_notify(l_param);
        child = (StarsU16)((StarsU32)l_param & 0xffffU);
        if (input->selected_kind == 2 && input->ops->ship_command) {
            input->ops->ship_command(input->ops->context, hwnd, w_param,
                l_param);
            return 0;
        }
        if (child == input->ship_dropdown) {
            if (notify == 1 && input->ops->ship_dropdown_changed)
                input->ops->ship_dropdown_changed(input->ops->context, 1);
            return 0;
        }
        button = planet_button(input, child);
        if (button == 4 && notify == 0) {
            if (input->ops->select_adj_starbase)
                input->ops->select_adj_starbase(input->ops->context,
                    input->selected_planet, input->shift_down ? 1 : 0);
        } else if (button == 5 && notify == 0) {
            if (input->ops->select_adj_planet)
                input->ops->select_adj_planet(input->ops->context,
                    input->shift_down ? 1 : -1, 0);
        } else if (button == 0 && notify == 0) {
            planet_transfer(input, 1);
        } else if (button == 1 && notify == 0) {
            if (input->ops->get_dropdown_selection &&
                    input->ops->find_transfer && input->ops->select_adj_planet) {
                StarsI16 selection;
                StarsI16 object_id;
                StarsI16 object_kind;
                selection = input->ops->get_dropdown_selection(
                    input->ops->context);
                if (selection >= 0 && input->ops->find_transfer(
                        input->ops->context, input->selected_planet,
                        selection, &object_id, &object_kind) && object_kind == 2)
                    input->ops->select_adj_planet(input->ops->context,
                        object_id, 0);
            }
        } else if (button == 2 && notify == 0) {
            if (input->ops->build_ship) {
                point.x = 0x262;
                point.y = 0x1d6;
                input->ops->build_ship(input->ops->context, point);
            }
        } else if (button == 11 && notify == 0) {
            if (input->ops->change_production)
                input->ops->change_production(input->ops->context, 0);
        } else if (button == 12 && notify == 0) {
            if (input->ops->confirm_production &&
                    input->ops->confirm_production(input->ops->context) == 6 &&
                    input->ops->change_production)
                input->ops->change_production(input->ops->context, 1);
        } else if (child == input->planet_production_list && notify == 1) {
            if (input->ops->paint_planet_ship)
                input->ops->paint_planet_ship(input->ops->context, hwnd, 0x40);
            if (input->ops->advance_tutor)
                input->ops->advance_tutor(input->ops->context);
        }
        if (input->ops->set_focus_frame)
            input->ops->set_focus_frame(input->ops->context);
        return 0;
    }
    if (message == 0x201 || message == 0x203 || message == 0x204) {
        x = (StarsI16)((StarsU32)l_param & 0xffffU);
        y = (StarsI16)((StarsU32)l_param >> 16);
        double_click = message == 0x204;
        if (input->ops->set_focus_frame)
            input->ops->set_focus_frame(input->ops->context);
        if (input->ops->planet_click)
            input->ops->planet_click(input->ops->context, x, y, w_param,
                double_click);
        return 0;
    }
    if (message == 0x222) {
        if (input->ops->set_active)
            input->ops->set_active(input->ops->context,
                w_param == 0 ? 0 : hwnd);
        return 0;
    }
default_proc:
    if (input->ops->default_window_proc)
        input->ops->default_window_proc(input->ops->context);
    return 0;
}
