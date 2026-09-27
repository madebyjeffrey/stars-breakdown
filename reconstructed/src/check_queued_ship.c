#include "core.h"

/* NE 1A:027C-054F. Warn before editing or deleting a ship design that is
 * already present in production.  Dialog text is formatted by a callback,
 * but the original branch and message parameters remain visible here. */
StarsI16 stars_check_queued_ship(const StarsCheckQueuedShipInput *input)
{
    StarsI16 queued;
    StarsI16 progress;
    StarsI16 message_group;
    StarsU16 title_id;
    StarsU16 focus;
    StarsU8 text[256];

    if (!input || !input->queued_count || !input->format_warning ||
            !input->message_box || !input->set_focus)
        return 1;
    progress = 0;
    queued = input->queued_count(input->context, input->design_id,
        input->edit_mode, &progress);
    if (input->existing_count == 0UL && queued == 0)
        return 1;
    if (input->edit_mode)
        message_group = 0x270;
    else if (input->starbase_mode)
        message_group = 0x26e;
    else
        message_group = 0x26b;
    title_id = (StarsU16)(input->edit_mode + 0x2e6U);
    text[0] = 0;
    input->format_warning(input->context, input->design_id,
        input->existing_count, queued, progress, input->hull_class,
        message_group, text);
    focus = input->get_focus ? input->get_focus(input->context) : 0U;
    if (input->message_box(input->context, focus, text, title_id,
            0x2024U) == 7)
        return 0;
    input->set_focus(input->context, input->hwnd);
    if (input->existing_count == 0UL) {
        if (input->remove_from_queues)
            input->remove_from_queues(input->context, input->design_id,
                input->edit_mode);
    } else {
        if (input->destroy_design)
            input->destroy_design(input->context, input->design_id,
                input->player);
        if (input->invalidate) {
            input->invalidate(input->context, input->scanner_window);
            input->invalidate(input->context, input->message_window);
        }
    }
    return 1;
}
