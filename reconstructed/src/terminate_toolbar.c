/* Genuine C reconstruction of TerminateToolbarFocus (NE 14:1680-17ea). */
#include "core.h"
#include <stdio.h>

void stars_terminate_toolbar_focus(StarsTerminateToolbarFocusInput *input)
{
    StarsU8 text[20];
    StarsU16 i;
    StarsI16 value;

    if (!input) return;
    input->grbits2 = (StarsU16)((input->grbits2 & 0xffbfU) | 0x0040U);
    value = 0;
    if (input->cancel) {
        value = input->radar_view;
    } else {
        text[0] = 0;
        if (input->get_window_text)
            input->get_window_text(input->context, input->radar, text, 20U);
        for (i = 0U; i < 19U && text[i] >= (StarsU8)'0' &&
                 text[i] <= (StarsU8)'9'; ++i)
            value = (StarsI16)(value * 10 + (StarsI16)(text[i] - (StarsU8)'0'));
        if (text[i] != 0 && text[i] != (StarsU8)'%') value = 0;
    }
    if (value < 2) value = 2;
    else if (value > 100) value = 100;
    if (input->send_message)
        input->send_message(input->context, input->radar, 0x040eU,
            (StarsU16)((100 - value) / 10), 0UL);
    sprintf((char *)text, "%d%%", (int)value);
    if (input->set_window_text)
        input->set_window_text(input->context, input->radar, text);
    if (value != input->radar_view) {
        input->radar_view = value;
        if ((input->scan_flags & 0x0020U) == 0U && input->invalidate_rect)
            input->invalidate_rect(input->context, input->toolbar);
        input->scan_flags = (StarsU16)(input->scan_flags | 0x0020U);
        if (input->invalidate_rect)
            input->invalidate_rect(input->context, input->scanner);
    }
    if (input->set_focus)
        input->set_focus(input->context, input->frame);
}
