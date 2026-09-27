#include "core.h"
#include <string.h>

static StarsI16 score_text_width(const StarsInitScoreDlgInput *input,
    StarsU16 dc, const StarsU8 *text)
{
    if (!input->text_extent || !text)
        return 0;
    return input->text_extent(input->context, dc, text,
        (StarsU16)strlen((const char *)text));
}

/* NE 22:13B6-168E. Size the score dialog, distribute its three report
 * controls, and select the title string for the requested victory report.
 * USER and GDI operations remain explicit host callbacks. */
void stars_init_score_dlg(StarsU16 hwnd, const StarsInitScoreDlgInput *input)
{
    StarsU16 dc;
    StarsU8 digit[2];
    const StarsU8 *text;
    StarsI16 digit_width;
    StarsI16 score_width;
    StarsI16 players;
    StarsI32 width;
    StarsI32 height;
    StarsI16 frame_width;
    StarsI16 frame_height;
    StarsRect window_rect;
    StarsRect client_rect;
    StarsRect child_rect;
    StarsU16 item;
    StarsI16 delta;
    StarsI16 first_x;

    if (!input)
        return;
    dc = input->get_dc ? input->get_dc(input->context, hwnd) : 0;
    if (input->select_object)
        input->select_object(input->context, dc, input->font_arial8);
    digit[0] = (StarsU8)'8';
    digit[1] = 0;
    digit_width = input->text_extent ? input->text_extent(input->context, dc,
        digit, 1) : 0;
    width = 0;
    height = 0;
    if (input->victory_mode == 2) {
        width = 600;
        height = 400;
    } else if (input->victory_mode == 0) {
        text = input->get_string ? input->get_string(input->context, 0x01b5U) : 0;
        score_width = score_text_width(input, dc, text);
        score_width = (StarsI16)(score_width + 8);
        if (input->score_x)
            *input->score_x = score_width;
        players = input->player_count < 4 ? 4 : input->player_count;
        width = (StarsI32)players * digit_width * 5 + score_width + 8;
        height = ((StarsI32)input->line_height * 0x21) / 2 + 0x58;
    } else {
        text = input->get_string ? input->get_string(input->context, 0x03b3U) : 0;
        score_width = score_text_width(input, dc, text);
        score_width = (StarsI16)((StarsU16)(score_width * 3) / 2 +
            digit_width * 6);
        if (input->score_x)
            *input->score_x = score_width;
        players = input->player_count < 4 ? 4 : input->player_count;
        width = ((StarsI32)players * input->line_height * 3) / 2 +
            score_width + 8;
        height = ((StarsI32)input->line_height * 0x21) / 2 + 0x58;
    }
    if (input->release_dc)
        input->release_dc(input->context, hwnd, dc);

    window_rect.left = window_rect.top = window_rect.right = window_rect.bottom = 0;
    client_rect.left = client_rect.top = client_rect.right = client_rect.bottom = 0;
    if (input->get_window_rect)
        input->get_window_rect(input->context, hwnd, &window_rect);
    if (input->get_client_rect)
        input->get_client_rect(input->context, hwnd, &client_rect);
    frame_width = (StarsI16)((StarsI16)(window_rect.right - window_rect.left) -
        (StarsI16)client_rect.right);
    frame_height = (StarsI16)((StarsI16)(window_rect.bottom - window_rect.top) -
        (StarsI16)client_rect.bottom);
    if (input->set_window_pos)
        input->set_window_pos(input->context, hwnd, 0, 0,
            (StarsI16)(frame_width + width),
            (StarsI16)(frame_height + height), 6);

    child_rect.left = child_rect.top = child_rect.right = child_rect.bottom = 0;
    item = input->get_dialog_item ? input->get_dialog_item(input->context,
        hwnd, 2) : 0;
    if (input->get_window_rect)
        input->get_window_rect(input->context, item, &child_rect);
    if (input->map_window_points)
        input->map_window_points(input->context, 0, hwnd, &child_rect, 2);
    if (input->offset_rect)
        input->offset_rect(input->context, &child_rect, 0,
            (StarsI16)(height - 4 - (StarsI16)client_rect.bottom));
    delta = (StarsI16)((StarsI16)client_rect.right -
        (StarsI16)child_rect.left);
    first_x = (StarsI16)((width - (StarsI32)delta * 3) / 4);

    item = input->get_dialog_item ? input->get_dialog_item(input->context,
        hwnd, 0x00c6U) : 0;
    if (input->set_window_pos)
        input->set_window_pos(input->context, item, first_x,
            (StarsI16)client_rect.top, 0, 0, 5);
    item = input->get_dialog_item ? input->get_dialog_item(input->context,
        hwnd, 2) : 0;
    if (input->set_window_pos)
        input->set_window_pos(input->context, item,
            (StarsI16)(first_x * 2 + delta), (StarsI16)client_rect.top,
            0, 0, 5);
    item = input->get_dialog_item ? input->get_dialog_item(input->context,
        hwnd, 0x0076U) : 0;
    if (input->set_window_pos)
        input->set_window_pos(input->context, item,
            (StarsI16)(first_x * 3 + delta * 2), (StarsI16)client_rect.top,
            0, 0, 5);
    text = input->get_string ? input->get_string(input->context,
        (StarsU16)(input->victory_mode + 0x04ba)) : 0;
    if (input->set_window_text)
        input->set_window_text(input->context, hwnd, text);
}
