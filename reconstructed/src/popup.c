#include "core.h"

/* Typed semantic reconstruction of Popup, NE 25:0c7c-1938.  Popup owns
 * measurement dispatch, screen-edge placement, and popup-window setup;
 * localized content measurement and packed game records remain callbacks. */

static StarsI16 popup_clamp(StarsI16 coordinate, StarsI16 extent,
    StarsI16 screen_extent)
{
    StarsI16 maximum;

    maximum = (StarsI16)(screen_extent - extent);
    if (coordinate > maximum)
        coordinate = maximum;
    if (coordinate < 0)
        coordinate = 0;
    return coordinate;
}

void stars_popup(StarsPopupInput *input)
{
    const StarsPopupOps *ops;
    StarsPopupMeasureCase measure_case;
    StarsPoint screen_point;
    StarsPoint popup_size;
    StarsU16 hdc;
    StarsU16 old_font;
    StarsU16 popup_window;
    StarsI16 width;
    StarsI16 height;
    StarsI16 metric;

    if (!input || !input->ops)
        return;
    ops = input->ops;
    screen_point.x = input->x;
    screen_point.y = input->y;
    if (ops->client_to_screen)
        ops->client_to_screen(ops->context, input->hwnd, &screen_point);
    hdc = 0;
    old_font = 0;
    if (ops->get_dc)
        hdc = ops->get_dc(ops->context, input->hwnd);
    if (ops->select_font)
        old_font = ops->select_font(ops->context, hdc, input->font_normal);

    measure_case.hwnd = input->hwnd;
    measure_case.hdc = hdc;
    measure_case.popup_kind = input->popup_kind;
    measure_case.line_height = input->line_height;
    measure_case.line_height_large = input->line_height_large;
    measure_case.print_mode = 0;
    measure_case.value1 = 0;
    measure_case.value2 = 0;
    popup_size.x = 0;
    popup_size.y = 0;
    switch (input->popup_kind) {
    case 1:
    case 2:
    case 3:
    case 4:
        if (ops->measure)
            popup_size = ops->measure(ops->context, &measure_case);
        break;
    case 5:
    case 6:
    case 7:
        if (ops->measure)
            popup_size = ops->measure(ops->context, &measure_case);
        popup_size.x = (StarsI16)(popup_size.x + 2);
        popup_size.y = (StarsI16)(popup_size.y + 2);
        break;
    case 10:
        measure_case.value1 = input->planet_id;
        if (ops->measure)
            popup_size = ops->measure(ops->context, &measure_case);
        popup_size.x = (StarsI16)(popup_size.x + 2);
        popup_size.y = (StarsI16)(popup_size.y + 2);
        break;
    case 8:
    case 12:
        measure_case.value1 = 200;
        if (ops->measure)
            popup_size = ops->measure(ops->context, &measure_case);
        popup_size.x = (StarsI16)(popup_size.x + 2);
        popup_size.y = (StarsI16)(popup_size.y + 2);
        break;
    case 9:
        popup_size.x = (StarsI16)((input->line_height < 15 ? 0 : 40) +
            0x158);
        popup_size.y = (StarsI16)(input->line_height_large +
            input->line_height * 12 + 0x4e);
        break;
    case 11:
    case 14:
        if (ops->update_slots)
            ops->update_slots(ops->context);
        popup_size.x = 0x154;
        popup_size.y = (StarsI16)(input->line_height * 7 + 0x13a);
        if (input->popup_kind == 11)
            popup_size.y = (StarsI16)(popup_size.y +
                input->slot_extra_rows * input->line_height);
        break;
    case 13:
        popup_size.x = 0x78;
        popup_size.y = 0x50;
        break;
    default:
        if (ops->measure)
            popup_size = ops->measure(ops->context, &measure_case);
        break;
    }
    width = popup_size.x;
    height = popup_size.y;
    if (ops->select_font)
        ops->select_font(ops->context, hdc, old_font);
    if (ops->release_dc)
        ops->release_dc(ops->context, input->hwnd, hdc);
    metric = 0;
    if (ops->get_system_metric)
        metric = ops->get_system_metric(ops->context, 0);
    screen_point.x = (StarsI16)(screen_point.x - width);
    screen_point.x = popup_clamp(screen_point.x, width, metric);
    metric = 0;
    if (ops->get_system_metric)
        metric = ops->get_system_metric(ops->context, 1);
    screen_point.y = (StarsI16)(screen_point.y - height);
    screen_point.y = popup_clamp(screen_point.y, height, metric);
    popup_window = 0;
    if (ops->create_window)
        popup_window = ops->create_window(ops->context, input->hwnd,
            0x90800000UL,
            screen_point.x, screen_point.y, width, height);
    if (ops->send_message)
        ops->send_message(ops->context, popup_window, 0x30,
            input->font_normal);
    if (ops->set_capture)
        ops->set_capture(ops->context, popup_window);
}
