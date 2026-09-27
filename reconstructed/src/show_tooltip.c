#include "core.h"

static StarsU16 tooltip_rect_equal(const StarsRect *first,
    const StarsRect *second)
{
    return (StarsU16)(first->left == second->left &&
        first->top == second->top && first->right == second->right &&
        first->bottom == second->bottom);
}

/* Genuine C reconstruction of ShowTooltip, NE 14:17EA-19DD. */
void stars_show_tooltip(StarsI16 id, const StarsRect *rectangle,
    StarsTooltipState *state, const StarsTooltipOps *ops)
{
    StarsU16 was_visible;
    StarsU16 dc;
    StarsU16 old_font;
    StarsI16 length;
    StarsU32 extent;
    StarsU32 deadline;
    StarsU32 now;
    StarsU16 show;
    StarsI16 width;
    StarsU8 text[80];

    if (!state || !ops)
        return;
    was_visible = (StarsU16)(state->window != 0U && ops->is_visible &&
        ops->is_visible(ops->context, state->window) != 0U);
    if (id < 0 || !rectangle) {
        if (state->window && ops->destroy_window)
            ops->destroy_window(ops->context, state->window);
        if (was_visible && ops->get_tick)
            state->last_tick = ops->get_tick(ops->context);
        return;
    }
    if (id == state->current_id &&
        tooltip_rect_equal(rectangle, &state->rectangle))
        return;
    state->current_id = id;
    length = ops->get_string ? ops->get_string(ops->context, id, text) : 0;
    state->rectangle = *rectangle;
    width = 0;
    if (ops->get_dc && ops->text_extent) {
        dc = ops->get_dc(ops->context, 0);
        old_font = ops->select_object ?
            ops->select_object(ops->context, dc, ops->font) : 0;
        extent = ops->text_extent(ops->context, dc, text, length);
        width = (StarsI16)(extent & 0xffffUL);
        state->width = width;
        if (ops->select_object)
            ops->select_object(ops->context, dc, old_font);
        if (ops->release_dc)
            ops->release_dc(ops->context, 0, dc);
    }
    if (!state->window) {
        if (ops->create_window)
            state->window = ops->create_window(ops->context,
                (StarsI16)(width + 6), (StarsI16)(ops->font_height + 6));
    } else if (ops->invalidate_window) {
        ops->invalidate_window(ops->context, state->window, 1);
    }
    if (ops->set_window_pos)
        ops->set_window_pos(ops->context, state->window, 0, 0,
            (StarsI16)(width + 6), (StarsI16)(ops->font_height + 6),
            0x0216U);
    deadline = state->last_tick + 400UL;
    now = ops->get_tick ? ops->get_tick(ops->context) : deadline;
    show = (StarsU16)(now <= deadline || was_visible);
    if (ops->window_proc)
        ops->window_proc(ops->context, state->window, 0x05f3U, show, 0UL);
}
