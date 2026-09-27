#include "core.h"

/* Genuine C reconstruction of AdvanceTutor, NE 20:0A30-0C01. */
void stars_advance_tutor(StarsAdvanceTutorState *state,
    const StarsAdvanceTutorOps *ops, void *context)
{
    StarsI16 old_bold;
    StarsI16 task_done;
    StarsI16 redraw;
    StarsU8 title[52];
    StarsRect rect;
    const StarsU8 *format;

    if (!state)
        return;
    redraw = 0;
    state->flags = (StarsU16)(state->flags & (StarsU16)~4U);
    old_bold = state->idt_bold;
    task_done = ops && ops->task_done ? ops->task_done(context) : 0;
    redraw = (StarsI16)(old_bold != state->idt_bold);
    if (task_done != 0) {
        if ((state->flags & 8U) == 0U) {
            do {
                state->idt = (StarsI16)(state->idt + 8);
                state->ids_error = -1;
                state->flags = (StarsU16)((state->flags & (StarsU16)~0x0420U) |
                    0x0020U);
                state->idt_bold = state->idt;
                task_done = ops && ops->task_done ?
                    ops->task_done(context) : 0;
                if (task_done == 0)
                    break;
            } while ((state->flags & 8U) == 0U);
            redraw = 1;
            state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
        } else {
            state->idh = (StarsI16)0x0db6;
        }
        if (state->idt >= 640 || (state->flags & 0x10U) != 0U) {
            if (state->ids_error != 0x020a && ops && ops->tutor_error)
                ops->tutor_error(context, 0x020a);
            if (ops && ops->end_tutor)
                ops->end_tutor(context, 0);
            return;
        }
    }
    if (!redraw)
        return;
    format = ops && ops->title_format ? ops->title_format(context) :
        (const StarsU8 *)0;
    if (format && ops && ops->format_title)
        ops->format_title(context, title, format,
            (StarsI16)(state->idt / 8 + 1));
    if (ops && ops->set_window_text)
        ops->set_window_text(context, state->window, title);
    if (ops && ops->show_tutor)
        ops->show_tutor(context, 1);
    rect.left = rect.top = rect.right = rect.bottom = 0;
    if (ops && ops->get_window_rect)
        ops->get_window_rect(context, state->window, &rect);
    if (ops && ops->screen_to_client) {
        ops->screen_to_client(context, state->window,
            (StarsPoint *)&rect.left);
        ops->screen_to_client(context, state->window,
            (StarsPoint *)&rect.right);
    }
    if (ops && ops->expand_rect)
        ops->expand_rect(context, &rect, (StarsI16)-state->arial8_height,
            (StarsI16)(-2 * state->arial8_height));
    if (ops && ops->invalidate_rect)
        ops->invalidate_rect(context, state->window, &rect, 1);
}
