#include "core.h"
#include <string.h>

enum {
    SCORE_X_WM_PAINT = 0x000f,
    SCORE_X_WM_ERASE_BACKGROUND = 0x0014,
    SCORE_X_WM_SETCURSOR = 0x0020,
    SCORE_X_WM_INIT_DIALOG = 0x0110,
    SCORE_X_WM_COMMAND = 0x0111,
    SCORE_X_WM_LBUTTONDOWN = 0x0201,
    SCORE_X_WM_RBUTTONDOWN = 0x0204,
    SCORE_X_ID_CANCEL = 2,
    SCORE_X_ID_HELP = 0x76,
    SCORE_X_ID_NEXT_REPORT = 0xc6,
    SCORE_X_HELP_ID = 0x455,
    SCORE_X_HISTORY_TITLE = 0x3c6,
    SCORE_X_HISTORY_FIRST = 0x1b3,
    SCORE_X_HISTORY_COUNT = 8,
    SCORE_X_HISTORY_MENU_CANCEL = -1
};

static void score_x_advance(StarsScoreXInput *input)
{
    if (input->tutor_active && input->ops->advance_tutor)
        input->ops->advance_tutor(input->context);
}

/* Genuine semantic reconstruction of ScoreXDlg, NE 22:0F66-13B6.
 * The dialog chooses one of the score, victory, or history renderers; its
 * history mode exposes a compact eight-entry report selector and its command
 * paths own the dialog/tutor state transitions. */
StarsI16 stars_score_x_dlg(StarsScoreXInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsRect paint;
    StarsPoint point;
    StarsU16 dc;
    StarsU16 i;
    StarsI16 selection;
    StarsU8 names[8][40];
    StarsU16 selected[8];
    StarsU16 mode;
    StarsI16 y;

    if (!input || !input->ops)
        return 0;
    if (message == SCORE_X_WM_PAINT) {
        dc = input->ops->begin_paint ? input->ops->begin_paint(
            input->context, hwnd, &paint) : 0;
        if (input->mode == 2U) {
            if (input->ops->draw_history)
                input->ops->draw_history(input->context, dc, hwnd);
        } else if (input->mode == 0U) {
            if (input->ops->draw_score)
                input->ops->draw_score(input->context, dc, hwnd);
        } else if (input->ops->draw_victory) {
            input->ops->draw_victory(input->context, dc, hwnd);
        }
        if (input->ops->end_paint)
            input->ops->end_paint(input->context, hwnd, &paint);
        return 1;
    }
    if (message == SCORE_X_WM_ERASE_BACKGROUND) {
        if (input->ops->get_client_rect)
            input->ops->get_client_rect(input->context, hwnd, &paint);
        if (input->ops->fill_rect)
            input->ops->fill_rect(input->context, wparam, &paint,
                input->button_face_brush);
        return 1;
    }
    if (message == SCORE_X_WM_SETCURSOR && input->mode == 2U) {
        point.x = point.y = 0;
        if (input->ops->get_cursor_pos)
            input->ops->get_cursor_pos(input->context, &point);
        if (input->ops->screen_to_client)
            input->ops->screen_to_client(input->context, hwnd, &point);
        y = (StarsI16)point.y;
        if (y > 2 && y < (StarsI16)(input->font_height_large +
                input->font_height - 2)) {
            if (input->ops->set_cursor)
                input->ops->set_cursor(input->context, input->hand_cursor);
            return 1;
        }
    } else if (message == SCORE_X_WM_INIT_DIALOG) {
        if (input->ops->init_dialog)
            input->ops->init_dialog(input->context, hwnd, input->mode);
        input->in_dialog = 1;
        input->dialog_window = hwnd;
        if (input->ops->sticky_position)
            input->ops->sticky_position(input->context, hwnd, 1);
        score_x_advance(input);
        return 1;
    } else if (message == SCORE_X_WM_COMMAND) {
        if (wparam == SCORE_X_ID_CANCEL) {
            if (input->ops->sticky_position)
                input->ops->sticky_position(input->context, hwnd, 0);
            if (input->ops->end_dialog)
                input->ops->end_dialog(input->context, hwnd, 0);
            input->in_dialog = 0;
            input->dialog_window = 0;
            score_x_advance(input);
            return 1;
        }
        if (wparam == SCORE_X_ID_HELP) {
            if (input->ops->help)
                input->ops->help(input->context, hwnd, input->help_file, 1,
                    SCORE_X_HELP_ID);
            return 1;
        }
        if (wparam == SCORE_X_ID_NEXT_REPORT) {
            mode = (StarsU16)((input->mode + 1U) % 3U);
            input->mode = mode;
            if (input->ops->invalidate)
                input->ops->invalidate(input->context, hwnd);
            if (input->ops->init_dialog)
                input->ops->init_dialog(input->context, hwnd, mode);
            score_x_advance(input);
            return 1;
        }
    } else if ((message == SCORE_X_WM_LBUTTONDOWN ||
            message == SCORE_X_WM_RBUTTONDOWN) && input->mode == 2U) {
        y = (StarsI16)(StarsU16)(lparam >> 16);
        if (y > 2 && y < (StarsI16)(input->font_height_large +
                input->font_height - 2)) {
            for (i = 0; i < SCORE_X_HISTORY_COUNT; ++i) {
                names[i][0] = 0;
                if (input->ops->get_string)
                    input->ops->get_string(input->context,
                        SCORE_X_HISTORY_FIRST + i, names[i], 40);
                selected[i] = (StarsU16)(i ==
                    (input->history_selection & 0x000fU));
            }
            if (input->ops->popup_menu)
                selection = input->ops->popup_menu(input->context, hwnd,
                    (StarsI16)(StarsU16)lparam, y, SCORE_X_HISTORY_COUNT,
                    (const StarsU8 *)names, selected,
                    (StarsI16)(input->history_selection & 0x000fU),
                    message == SCORE_X_WM_RBUTTONDOWN);
            else
                selection = SCORE_X_HISTORY_MENU_CANCEL;
            if (selection != SCORE_X_HISTORY_MENU_CANCEL) {
                input->history_selection = (StarsU16)((input->history_selection &
                    0xfff0U) | ((StarsU16)selection & 0x000fU));
                input->history_flags = (StarsU16)((input->history_flags &
                    0xff7fU) | 0x0080U);
                if (input->ops->invalidate)
                    input->ops->invalidate(input->context, hwnd);
            }
        }
    }
    return 0;
}
