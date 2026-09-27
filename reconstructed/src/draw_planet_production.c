#include "core.h"
#include <string.h>

#define PLANET_PRODUCTION_TILE_RESET 0x0800U
#define PLANET_PRODUCTION_EMPTY_LEFT (-5)
#define PLANET_PRODUCTION_EMPTY_TOP (-5)
#define PLANET_PRODUCTION_EMPTY_RIGHT (-6)
#define PLANET_PRODUCTION_EMPTY_BOTTOM (-6)

static void planet_production_hide(StarsPlanetProductionInput *input)
{
    StarsI16 index;

    if (input->ops->show_window) {
        input->ops->show_window(input->ops->context, input->list_window, 0);
        for (index = 0; index < 2; ++index)
            input->ops->show_window(input->ops->context, input->buttons[index], 0);
    }
    if (input->reference_rect) {
        input->reference_rect->left = PLANET_PRODUCTION_EMPTY_LEFT;
        input->reference_rect->top = PLANET_PRODUCTION_EMPTY_TOP;
        input->reference_rect->right = PLANET_PRODUCTION_EMPTY_RIGHT;
        input->reference_rect->bottom = PLANET_PRODUCTION_EMPTY_BOTTOM;
    }
}

/* Genuine semantic reconstruction of DrawPlanetProduction, NE 0A:2C38-310B.
 * The common tile frame, list placement, ETA/name rows, production buttons,
 * enablement, and reference rectangle are kept as typed operations. */
void stars_draw_planet_production(StarsPlanetProductionInput *input)
{
    StarsRect rect;
    StarsRect button_rect;
    StarsI16 left;
    StarsI16 right;
    StarsI16 y;
    StarsI16 line;
    StarsI16 rows;
    StarsI16 width;
    StarsI16 index;
    StarsI16 selected;
    StarsI16 list_width;
    StarsI16 list_height;
    StarsI16 target_height;
    StarsU16 position_flags;
    StarsRect list_rect;
    const StarsU8 *title;
    const StarsU8 *label;

    if (!input || !input->tile || !input->data || !input->ops ||
        !input->ops->draw_tile)
        return;
    if ((input->tile->flags & PLANET_PRODUCTION_TILE_RESET) != 0U) {
        planet_production_hide(input);
        input->tile->flags = (StarsU16)(input->tile->flags &
            (StarsU16)~PLANET_PRODUCTION_TILE_RESET);
    }
    title = input->ops->compressed_string ? input->ops->compressed_string(
        input->ops->context, 0x32e) : (const StarsU8 *)0;
    rect.left = rect.top = rect.right = rect.bottom = 0;
    if (!input->ops->draw_tile(input->ops->context, input->hdc, input->tile,
            title, &rect)) {
        planet_production_hide(input);
        return;
    }
    left = (StarsI16)(rect.left + 4);
    right = (StarsI16)(rect.right - 4);
    line = input->line_height > 0 ? input->line_height : 1;
    rows = input->compact_rows ? 3 : 5;
    target_height = (StarsI16)((line + 2) * rows);
    list_width = 0;
    list_height = 0;
    if (input->ops->get_client_rect) {
        input->ops->get_client_rect(input->ops->context, input->list_window,
            &list_rect);
        list_width = (StarsI16)(list_rect.right - list_rect.left);
        list_height = (StarsI16)(list_rect.bottom - list_rect.top);
    }
    position_flags = 0x14;
    if (list_width == (StarsI16)(right - left) &&
        target_height >= list_height &&
        (StarsI16)(target_height - list_height) < line)
        position_flags = 0x15;
    if (input->ops->set_window_pos)
        input->ops->set_window_pos(input->ops->context, input->list_window, 0,
            left, (StarsI16)(rect.top + 4), (StarsI16)(right - left),
            target_height, position_flags);
    if (input->ops->show_window)
        input->ops->show_window(input->ops->context, input->list_window, 5);
    if (input->ops->get_client_rect) {
        input->ops->get_client_rect(input->ops->context, input->list_window,
            &list_rect);
        list_height = (StarsI16)(list_rect.bottom - list_rect.top);
    }
    if (list_height <= 0)
        list_height = target_height;
    selected = input->ops->selection ? input->ops->selection(
        input->ops->context, input->list_window) : 0;
    y = (StarsI16)(rect.top + 4 + list_height + 4);
    if (input->data->eta[0] != 0U) {
        if (input->ops->select_font)
            input->ops->select_font(input->ops->context, input->hdc,
                input->bold_font);
        label = input->ops->compressed_string ?
            input->ops->compressed_string(input->ops->context, 0x218) :
            (const StarsU8 *)0;
        if (label && input->ops->text_out)
            input->ops->text_out(input->ops->context, input->hdc, left, y,
                label, (StarsI16)strlen((const char *)label));
        width = input->ops->text_extent && label ? input->ops->text_extent(
            input->ops->context, input->hdc, label,
            (StarsI16)strlen((const char *)label)) : 0;
        if (input->ops->select_font)
            input->ops->select_font(input->ops->context, input->hdc,
                input->normal_font);
        if (input->ops->right_text)
            input->ops->right_text(input->ops->context, input->hdc, right, y,
                input->data->eta, (StarsI16)strlen((const char *)input->data->eta),
                width);
    }
    y = (StarsI16)(y + line);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->bold_font);
    label = input->ops->compressed_string ?
        input->ops->compressed_string(input->ops->context, 0x4e9) :
        (const StarsU8 *)0;
    if (label && input->ops->text_out)
        input->ops->text_out(input->ops->context, input->hdc, left, y, label,
            (StarsI16)strlen((const char *)label));
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->normal_font);
    if (input->ops->right_text)
        input->ops->right_text(input->ops->context, input->hdc, right, y,
            input->data->planet_name,
            (StarsI16)strlen((const char *)input->data->planet_name), width);
    y = (StarsI16)(y + line);
    width = (StarsI16)((right - left - 16) / 3);
    button_rect.top = (StarsU16)y;
    button_rect.bottom = (StarsU16)(y + line + line / 2);
    for (index = 0; index < 2; ++index) {
        button_rect.left = (StarsU16)(left + index * (width + 8));
        button_rect.right = (StarsU16)(button_rect.left + width);
        if (input->ops->set_window_pos)
            input->ops->set_window_pos(input->ops->context, input->buttons[index],
                0, button_rect.left, button_rect.top, width,
                (StarsI16)(line + line / 2), 0x14);
        if (input->ops->show_window)
            input->ops->show_window(input->ops->context, input->buttons[index], 5);
    }
    if (input->ops->draw_button && input->reference_rect) {
        input->reference_rect->left = button_rect.right;
        input->reference_rect->top = button_rect.top;
        input->reference_rect->right = (StarsU16)(button_rect.right + width);
        input->reference_rect->bottom = button_rect.bottom;
        if (input->ops->enable_window)
            input->ops->enable_window(input->ops->context,
                input->buttons[1], (StarsU16)(input->data->queue_count != 0));
        input->ops->draw_button(input->ops->context, input->hdc,
            input->reference_rect, 8, input->data->queue_count != 0);
    }
    (void)selected;
}
