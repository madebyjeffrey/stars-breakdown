#include "core.h"
#include <string.h>

/* NE 1A:59F0-5C06. Paint one inventory list item, preserving the selected
 * fill, palette realization, 26-column tile lookup, text state restoration,
 * and focus framing of the original owner-draw callback. */
void stars_draw_dlg_lb_entire_item(const StarsDrawDlgLBEntireItemInput *input)
{
    StarsRect rect;
    StarsU16 stock;
    StarsU16 tile;
    StarsI16 source_y;
    StarsU32 old_color;
    StarsU16 old_mode;
    StarsU16 text_length;
    StarsU32 color;

    if (!input || !input->fill_rect || !input->text)
        return;
    rect = input->item_rect;
    stock = (input->item_state & 0x0010U) ? 4U : 0U;
    if (input->get_stock_object)
        stock = input->get_stock_object(input->context, stock);
    input->fill_rect(input->context, input->hdc, &input->item_rect, stock);
    rect.left = (StarsI16)(rect.left + 2);
    rect.right = (StarsI16)(rect.right - 2);
    rect.top = (StarsI16)(rect.top + 1);
    rect.bottom = (StarsI16)(rect.bottom - 1);
    if (input->send_message)
        input->send_message(input->context, input->hwnd_item, 0x040aU,
            input->item_id, 0x112057a4UL);
    if (input->select_palette)
        input->select_palette(input->context, input->hdc, input->palette);
    if (input->realize_palette)
        input->realize_palette(input->context, input->hdc);
    tile = (StarsU16)((input->glyph_x - 'A') +
        (input->glyph_y - 'A') * 26U);
    source_y = (StarsI16)((3 - ((tile >> 3) & 3U)) * 64U);
    if (input->dib_blt)
        input->dib_blt(input->context, input->hdc, rect.left, rect.top,
            64, 64, tile >> 5, (StarsI16)((tile & 7U) * 64U), source_y,
            64, 64, 0xcc0020UL);
    color = input->window_color;
    if ((input->item_state & 0x0010U) == 0U && color == 0U)
        color = 0x0000ffffUL;
    old_color = input->set_text_color ? input->set_text_color(
        input->context, input->hdc, color) : 0U;
    old_mode = input->set_bk_mode ? input->set_bk_mode(
        input->context, input->hdc, 1U) : 0U;
    text_length = (StarsU16)strlen((const char *)input->text);
    if (input->text_out)
        input->text_out(input->context, input->hdc,
            (StarsI16)(rect.left + 0x42),
            (StarsI16)(rect.top + 0x20 - input->arial8_height / 2),
            input->text, text_length);
    if (input->set_text_color)
        input->set_text_color(input->context, input->hdc, old_color);
    if (input->set_bk_mode)
        input->set_bk_mode(input->context, input->hdc, old_mode);
    if (input->handle_focus)
        input->handle_focus(input->context, &input->item_rect,
            (StarsI16)(input->inflate + 2));
}
