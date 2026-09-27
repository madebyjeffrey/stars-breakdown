#include "core.h"

#define TRANSFER_DLG_WM_PAINT 0x000fU
#define TRANSFER_DLG_WM_ERASE_BACKGROUND 0x0014U
#define TRANSFER_DLG_WM_INIT_DIALOG 0x0110U
#define TRANSFER_DLG_WM_COMMAND 0x0111U
#define TRANSFER_DLG_WM_LBUTTONDOWN 0x0201U
#define TRANSFER_DLG_WM_LBUTTONDBLCLK 0x0203U
#define TRANSFER_DLG_ID_OK 1U
#define TRANSFER_DLG_ID_CANCEL 2U
#define TRANSFER_DLG_ID_HELP 0x0076U
#define TRANSFER_DLG_TITLE_STRING 0x0367
#define TRANSFER_DLG_HELP_SMALL 0x0433U
#define TRANSFER_DLG_HELP_LARGE 0x0438U

static void transfer_reposition_control(StarsTransferDlgInput *input,
    StarsU16 hwnd, StarsRect *client, StarsI16 delta_y, StarsI16 delta_x,
    StarsU16 id)
{
    StarsU16 control;
    StarsRect rect;
    StarsPoint point;
    const StarsTransferDlgOps *ops = input->ops;
    control = ops->get_dialog_item ? ops->get_dialog_item(input->context,
        hwnd, id) : id;
    if (ops->get_window_rect)
        ops->get_window_rect(input->context, control, &rect);
    point.x = (StarsI16)rect.left; point.y = (StarsI16)rect.top;
    if (ops->screen_to_client)
        ops->screen_to_client(input->context, hwnd, &point);
    point.x = (StarsI16)(point.x - delta_x);
    point.y = (StarsI16)(point.y + delta_y);
    if (ops->set_window_pos)
        ops->set_window_pos(input->context, control, 0U, point.x, point.y,
            0, 0, 1U);
    (void)client;
}

/* NE 11:5686-5A16. Dialog wrapper for transfer controls, including the
 * large-hull layout adjustment and its exact accept/cancel/tutor paths. */
StarsI16 stars_transfer_dlg(StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam, StarsTransferDlgInput *input)
{
    const StarsTransferDlgOps *ops;
    StarsRect client;
    StarsU8 text[128];
    StarsI16 half_height;
    StarsI16 delta_y;
    StarsI16 delta_x;
    StarsI16 metric;
    StarsU16 dc;
    StarsPoint point;
    StarsU16 control;
    StarsI16 help_id;

    if (!input || !input->ops)
        return 0;
    ops = input->ops;
    if (message == TRANSFER_DLG_WM_PAINT) {
        dc = ops->begin_paint ? ops->begin_paint(input->context, hwnd) : 0U;
        if (ops->get_client_rect)
            ops->get_client_rect(input->context, hwnd, &client);
        if (ops->draw)
            ops->draw(input->context, hwnd, dc, &client, -1);
        if (ops->end_paint)
            ops->end_paint(input->context, hwnd, dc);
        return 1;
    }
    if (message == TRANSFER_DLG_WM_ERASE_BACKGROUND) {
        if (ops->get_client_rect)
            ops->get_client_rect(input->context, hwnd, &client);
        if (ops->fill_rect)
            ops->fill_rect(input->context, wparam, &client,
                input->button_face_brush);
        return 1;
    }
    if (message == TRANSFER_DLG_WM_INIT_DIALOG) {
        if (ops->sticky_position)
            ops->sticky_position(input->context, hwnd, input->sticky_point, 1);
        if (ops->get_client_rect)
            ops->get_client_rect(input->context, hwnd, &client);
        if (input->mode == 1U) {
            text[0] = 0;
            if (ops->get_string)
                (void)ops->get_string(input->context, TRANSFER_DLG_TITLE_STRING,
                    text, (StarsU16)sizeof text);
            if (ops->set_window_text)
                ops->set_window_text(input->context, hwnd, text);
            if (input->valid_hulls > 10) {
                half_height = (StarsI16)(client.bottom / 2U);
                client.bottom = (StarsU16)(client.bottom + half_height);
                if (ops->set_window_pos)
                    ops->set_window_pos(input->context, hwnd, 0U, 0, 0,
                        (StarsI16)client.right, (StarsI16)client.bottom, 6U);
                if (ops->get_client_rect)
                    ops->get_client_rect(input->context, hwnd, &client);
                metric = ops->get_system_metric ?
                    ops->get_system_metric(input->context, 4U) : 0;
                delta_y = (StarsI16)(half_height - (metric + 2));
                metric = ops->get_system_metric ?
                    ops->get_system_metric(input->context, 7U) : 0;
                delta_x = (StarsI16)(metric + 4);
                transfer_reposition_control(input, hwnd, &client, delta_y,
                    delta_x, input->control_ids[0]);
                transfer_reposition_control(input, hwnd, &client, delta_y,
                    delta_x, input->control_ids[1]);
                transfer_reposition_control(input, hwnd, &client, delta_y,
                    delta_x, input->control_ids[2]);
            }
        }
        if (ops->setup_buttons)
            ops->setup_buttons(input->context, &client);
        if (input->tutor_active && ops->advance_tutor)
            ops->advance_tutor(input->context);
        return 1;
    }
    if (message == TRANSFER_DLG_WM_COMMAND) {
        if (wparam == TRANSFER_DLG_ID_OK ||
                wparam == TRANSFER_DLG_ID_CANCEL) {
            if (ops->sticky_position)
                ops->sticky_position(input->context, hwnd, input->sticky_point, 0);
            if (ops->end_dialog)
                ops->end_dialog(input->context, hwnd,
                    (StarsI16)(wparam == TRANSFER_DLG_ID_OK));
            if (input->tutor_active && ops->advance_tutor)
                ops->advance_tutor(input->context);
            return 1;
        }
        if (wparam == TRANSFER_DLG_ID_HELP) {
            help_id = input->mode == 1U ? TRANSFER_DLG_HELP_LARGE :
                TRANSFER_DLG_HELP_SMALL;
            if (ops->win_help)
                ops->win_help(input->context, hwnd, input->help_file, 1,
                    (StarsU16)help_id);
            return 1;
        }
    } else if (message == TRANSFER_DLG_WM_LBUTTONDOWN ||
            message == TRANSFER_DLG_WM_LBUTTONDBLCLK) {
        point.x = (StarsI16)lparam;
        point.y = (StarsI16)(lparam >> 16);
        return ops->track ? ops->track(input->context, hwnd, point, wparam) : 0;
    }
    (void)control;
    return 0;
}
