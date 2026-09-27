#include "core.h"

/* Genuine C reconstruction of InvalidateReport, NE 22:7AF6-7CD6. */
void stars_invalidate_report(StarsInvalidateReportInput *input)
{
    StarsInvalidateReportRecord *saved_report;
    void *saved_ids;
    StarsRect client;
    StarsI16 body_top;
    StarsI16 body_bottom;
    StarsI16 restore_current;
    StarsI16 selected_reload;

    if (!input || !input->current)
        return;
    if ((input->game_view_flags & 2U) != 0U || input->ai_mode != 0)
        return;

    restore_current = 0;
    selected_reload = 0;
    if (*input->current == (StarsInvalidateReportRecord *)0 ||
            input->report != (*input->current)->report) {
        if (*input->current == (StarsInvalidateReportRecord *)0) {
            selected_reload = input->reload;
            *input->current = input->report == 0 ? input->planet_report :
                input->fleet_report;
        } else if (input->reload != 0 &&
                (*input->current)->report != input->report) {
            saved_report = *input->current;
            saved_ids = input->report_ids ? *input->report_ids : (void *)0;
            restore_current = 1;
            *input->current = input->report == 0 ? input->planet_report :
                input->fleet_report;
        }
    } else {
        client.left = client.top = client.right = client.bottom = 0;
        if (input->ops && input->ops->get_client_rect)
            input->ops->get_client_rect(input->context,
                input->report_window, &client);
        /* The original computes the report body rectangle here for the
         * non-full invalidation case; the resulting locals are not used by
         * this procedure, but retaining the calculation documents the ABI. */
        if (input->reload != 2) {
            body_top = (StarsI16)(input->arial8_height + 6);
            body_bottom = (StarsI16)((input->arial8_height + 4) *
                (*input->current)->rows_visible + body_top);
            (void)body_top;
            (void)body_bottom;
        }
        if (input->ops && input->ops->invalidate_rect)
            input->ops->invalidate_rect(input->context,
                input->report_window, &client,
                (StarsI16)(input->reload == 2));
        input->game_view_flags = (StarsU16)((input->game_view_flags &
            (StarsU16)~0x0200U) | 0x0200U);
    }

    if (!*input->current)
        return;
    (*input->current)->cached = 0;
    if (input->reload != 0 && input->ops && input->ops->sort_cache)
        input->ops->sort_cache(input->context, (*input->current)->report,
            (*input->current)->sort_column);

    if (selected_reload == 0) {
        if (!restore_current) {
            if (input->reload != 0 && input->report_window != 0 &&
                    input->report == (*input->current)->report) {
                if (input->ops && input->ops->set_scroll_range)
                    input->ops->set_scroll_range(input->context,
                        (*input->current)->vertical_scroll, 2, 0,
                        (StarsI16)((*input->current)->rows -
                            (*input->current)->rows_visible), 0);
                if (input->ops && input->ops->invalidate_rect)
                    input->ops->invalidate_rect(input->context,
                        input->report_window, (const StarsRect *)0, 1);
            }
        } else {
            *input->current = saved_report;
            if (input->report_ids)
                *input->report_ids = saved_ids;
        }
    } else {
        *input->current = (StarsInvalidateReportRecord *)0;
    }
}
