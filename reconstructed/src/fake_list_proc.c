#include "core.h"

static StarsU16 low_word(StarsU32 value)
{ return (StarsU16)value; }

static StarsI16 high_word(StarsU32 value)
{ return (StarsI16)(value >> 16); }

/* Genuine C reconstruction of FakeListProc, NE 1A:6758-694B. */
StarsI32 stars_fake_list_proc(StarsFakeListProcInput *input, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsU32 lparam)
{
    StarsPoint point;
    StarsU32 selected;

    if (!input)
        return 0;
    if (message == 0x20U) {
        point.x = 0;
        point.y = 0;
        if (input->ops && input->ops->get_cursor_pos)
            input->ops->get_cursor_pos(input->context, hwnd, &point);
        if (input->ops && input->ops->screen_to_client)
            input->ops->screen_to_client(input->context, hwnd, &point);
        if (point.x < 0x40) {
            if (input->ops && input->ops->set_cursor)
                input->ops->set_cursor(input->context, input->hand_cursor);
            return 1;
        }
    }
    else if ((message == 0x201U || message == 0x204U) &&
             low_word(lparam) < 0x40U &&
             (input->build_mode == 4U || message == 0x204U)) {
        if (input->ops && input->ops->call_window_proc) {
            input->ops->call_window_proc(input->context, input->real_list_proc,
                hwnd, 0x201U, (StarsU32)wparam | ((StarsU32)low_word(lparam) << 16));
            input->ops->call_window_proc(input->context, input->real_list_proc,
                hwnd, 0x202U, (StarsU32)wparam | ((StarsU32)low_word(lparam) << 16));
        }
        if (message != 0x204U && input->build_mode == 4U) {
            if (input->ops && input->ops->track_slot)
                input->ops->track_slot(input->context, hwnd,
                    low_word(lparam), high_word(lparam), wparam, 1, 0);
            return 0;
        }
        selected = input->ops && input->ops->send_message ?
            input->ops->send_message(input->context, hwnd, 0x409U, 0, 0) :
            0xffffffffUL;
        if ((StarsU16)selected != 0xffffU) {
            if (input->ops && input->ops->send_message)
                input->ops->send_message(input->context, hwnd, 0x40aU,
                    (StarsU16)selected, 0x112057a4UL);
            input->global_part[0] = (StarsU8)(1U <<
                ((input->work[0] + 0xbfU) & 0x1fU));
            input->global_part[1] = (StarsU8)(input->work[1] - 0x41U);
            if (input->ops && input->ops->lookup_part)
                input->ops->lookup_part(input->context, input->global_part, 0);
            input->global_popup = 9;
            if (input->ops && input->ops->popup)
                input->ops->popup(input->context, hwnd, low_word(lparam),
                    high_word(lparam));
        }
        return 0;
    }
    if (input->ops && input->ops->call_window_proc)
        return input->ops->call_window_proc(input->context, input->real_list_proc,
            hwnd, message, (StarsU32)wparam | ((StarsU32)low_word(lparam) << 16));
    return 0;
}
