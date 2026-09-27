#include "core.h"

/* Genuine C reconstruction of SetHScrollBar, NE 22:09E8-0BAD. */
void stars_set_h_scroll_bar(StarsSetHScrollBarInput *input)
{
    StarsRect client;
    StarsI16 metric_width;
    StarsI16 metric_height;
    StarsI16 available;
    StarsI16 hidden_before;
    StarsI16 overflow;
    StarsI16 field;
    StarsU32 mask;
    StarsU16 column_width;
    StarsU16 flags;

    if (!input || !input->view)
        return;
    client.left = client.top = client.right = client.bottom = 0;
    if (input->ops && input->ops->get_client_rect)
        input->ops->get_client_rect(input->context, input->report_window,
            &client);
    metric_width = input->ops && input->ops->get_system_metrics ?
        input->ops->get_system_metrics(input->context, 2) : 0;
    column_width = (StarsU16)input->view->column_width[0] * 2U + 2U;
    available = (StarsI16)(client.right - (StarsI16)column_width - 2 -
        metric_width);
    hidden_before = 0;
    overflow = 0;
    mask = 1UL << (input->view->fields & 0x1fU);
    for (field = (StarsI16)input->view->fields - 1; field > 0; --field) {
        if ((mask & input->view->visible_fields) != 0UL) {
            if ((StarsU16)field < input->view->field_first)
                ++hidden_before;
            available = (StarsI16)(available -
                (StarsI16)input->view->column_width[field] * 2);
            if (available < 0)
                ++overflow;
        }
        mask >>= 1;
    }
    input->view->column_scroll = 0;
    if (overflow == 0) {
        flags = 0x84U;
        input->view->field_first = 1;
        if (input->ops && input->ops->set_scroll_pos)
            input->ops->set_scroll_pos(input->context,
                input->view->hscroll_window, 2, 0, 0);
    }
    else {
        flags = 0x44U;
        if (overflow < hidden_before) {
            hidden_before = 0;
            input->view->field_first = 1;
        }
        if (input->ops && input->ops->set_scroll_pos)
            input->ops->set_scroll_pos(input->context,
                input->view->hscroll_window, 2, hidden_before, 0);
        if (input->ops && input->ops->set_scroll_range)
            input->ops->set_scroll_range(input->context,
                input->view->hscroll_window, 2, 0, overflow, 1);
        input->view->column_scroll = (StarsU16)overflow;
    }
    metric_height = input->ops && input->ops->get_system_metrics ?
        input->ops->get_system_metrics(input->context, 3) : 0;
    if (input->ops && input->ops->set_window_pos)
        input->ops->set_window_pos(input->context, input->view->hscroll_window,
            0, (StarsI16)column_width,
            (StarsI16)(input->arial8_height +
                (input->arial8_height + 4) * input->view->rows_visible + 7),
            (StarsI16)(client.right - metric_width - (StarsI16)column_width),
            metric_height, flags);
}
