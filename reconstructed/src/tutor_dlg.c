#include "core.h"

#define TUTOR_WM_DESTROY 2U
#define TUTOR_WM_PAINT 0x000fU
#define TUTOR_WM_ERASE_BACKGROUND 0x0014U
#define TUTOR_WM_CHAR 0x0102U
#define TUTOR_WM_INIT_DIALOG 0x0110U
#define TUTOR_WM_COMMAND 0x0111U
#define TUTOR_ID_PANIC 0x09c7U
#define TUTOR_PANIC_DIALOG 0x09c8U
#define TUTOR_ID_HIDE 2U
#define TUTOR_ID_HELP 0x0076U
#define TUTOR_ID_MENU 0x09c5U
#define TUTOR_ID_PANIC_RESULT 0x0466U
#define TUTOR_HELP_COMMAND 0x0439U
#define TUTOR_ALERT_VISIBLE 0x051aU
#define TUTOR_ALERT_TYPE 0x40
#define TUTOR_MENU_INDEX 5U
#define TUTOR_VISIBLE_BIT 0x4000U
#define TUTOR_PANIC_BIT 0x0200U

/* NE 32:0000-0269.  The dialog procedure owns the tutor window state and
 * routes drawing, menu, panic, help, and frame-message work to explicit
 * Win16 service boundaries. */
StarsI16 stars_tutor_dlg(const StarsTutorDialogInput *input)
{
    StarsRect rect;
    StarsU16 menu;
    StarsU32 panic_proc;
    StarsI16 result;
    const StarsU8 *message;

    if (!input || !input->ops || !input->tutor_window ||
            !input->tutor_flags)
        return 0;
    if (input->message == TUTOR_WM_DESTROY) {
        if (input->ops->sticky_position && input->sticky_point)
            input->ops->sticky_position(input->context, input->hwnd,
                input->sticky_point, 0);
        *input->tutor_window = 0;
        menu = input->ops->get_sub_menu ?
            input->ops->get_sub_menu(input->context, input->frame_window,
                TUTOR_MENU_INDEX) : 0;
        if (input->ops->check_menu)
            input->ops->check_menu(input->context, menu, TUTOR_ID_MENU, 0);
        if (input->ops->end_dialog)
            input->ops->end_dialog(input->context, input->hwnd, 1);
        if (input->ops->end_tutor)
            input->ops->end_tutor(input->context, 1);
        return 1;
    }
    if (input->message == TUTOR_WM_PAINT) {
        if (input->ops->draw_text)
            input->ops->draw_text(input->context, input->hwnd);
        return 1;
    }
    if (input->message == TUTOR_WM_ERASE_BACKGROUND) {
        if (input->ops->get_client_rect && input->ops->fill_rect) {
            input->ops->get_client_rect(input->context, input->hwnd, &rect);
            input->ops->fill_rect(input->context, input->wparam, &rect,
                input->button_face_brush);
        }
        return 1;
    }
    if (input->message == TUTOR_WM_CHAR) {
        if (input->ops->post_message)
            input->ops->post_message(input->context, input->frame_window,
                TUTOR_WM_CHAR, input->wparam, input->lparam);
        return 0;
    }
    if (input->message == TUTOR_WM_INIT_DIALOG) {
        *input->tutor_window = input->hwnd;
        if (input->ops->set_window_pos)
            input->ops->set_window_pos(input->context, input->hwnd,
                0xffffU, 0, 0, 0, 0, 3U);
        if (input->ops->sticky_position && input->sticky_point)
            input->ops->sticky_position(input->context, input->hwnd,
                input->sticky_point, 1);
        return 1;
    }
    if (input->message != TUTOR_WM_COMMAND)
        return 0;

    if (input->wparam == TUTOR_ID_PANIC) {
        panic_proc = 0UL;
        if (input->ops->make_proc)
            panic_proc = input->ops->make_proc(input->context,
                input->instance);
        result = input->ops->dialog_box ? input->ops->dialog_box(
            input->context, input->instance, TUTOR_PANIC_DIALOG,
            input->hwnd, panic_proc) : 0;
        if (input->ops->free_proc)
            input->ops->free_proc(input->context, panic_proc);
        if (result != 0) {
            *input->tutor_flags = (StarsU16)((*input->tutor_flags &
                (StarsU16)~TUTOR_PANIC_BIT) |
                (result == 0x09ca ? TUTOR_PANIC_BIT : 0));
            if (input->ops->post_message)
                input->ops->post_message(input->context, input->frame_window,
                    TUTOR_ID_PANIC_RESULT, (StarsU16)result, 0);
        }
        return 0;
    }
    if (input->wparam == TUTOR_ID_HIDE) {
        if (input->ops->show_tutor)
            input->ops->show_tutor(input->context, 0);
        if ((*input->tutor_flags & TUTOR_VISIBLE_BIT) != 0U) {
            message = input->ops->format_ids ?
                input->ops->format_ids(input->context,
                    TUTOR_ALERT_VISIBLE) : (const StarsU8 *)0;
            if (input->ops->alert)
                (void)input->ops->alert(input->context, message,
                    TUTOR_ALERT_TYPE);
            *input->tutor_flags = (StarsU16)(*input->tutor_flags &
                (StarsU16)~TUTOR_VISIBLE_BIT);
        }
        return 1;
    }
    if (input->wparam == TUTOR_ID_HELP) {
        if (input->ops->win_help)
            input->ops->win_help(input->context, input->hwnd,
                input->help_file, 1U, input->tutor_help_id);
        return 1;
    }
    return 0;
}
