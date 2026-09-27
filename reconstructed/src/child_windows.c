/* Semantic reconstruction of CreateChildWindows. */
#include "core.h"

static StarsU16 create_child(const StarsCreateChildOps *ops, void *context,
    const StarsU8 *class_name, StarsI16 x, StarsI16 y, StarsI16 width,
    StarsI16 height, StarsU16 parent, StarsU16 instance)
{
    if (!ops->create_window) return 0U;
    return ops->create_window(context, class_name, x, y, width, height,
        parent, instance);
}

static void invalidate_child(const StarsCreateChildOps *ops, void *context,
    StarsU16 hwnd)
{
    if (ops->invalidate && hwnd) ops->invalidate(context, hwnd);
}

/* NE 5:038C-06D6. The title formatter owns localized resource lookup; this
 * routine owns the player/no-player branch and the exact child-window life
 * cycle observed in the original Win16 code. */
void stars_create_child_windows(StarsCreateChildState *state,
    const StarsCreateChildOps *ops, void *context)
{
    StarsU8 title[128];

    if (!state || !ops || !ops->make_title || !ops->set_window_text)
        return;
    ops->make_title(context, state->player, state->base_path, title,
        (StarsU16)sizeof(title));
    ops->set_window_text(context, state->frame, title);
    if (state->player == (StarsI16)-1) return;

    if (!state->scanner) {
        state->scanner = create_child(ops, context, state->scan_class,
            (StarsI16)-200, (StarsI16)-200, 10, 10, state->frame,
            state->instance);
    } else {
        invalidate_child(ops, context, state->scanner);
        state->y_scan_top = 1000;
        state->x_scan_top = 1000;
        if (ops->set_scan_scroll_bars)
            ops->set_scan_scroll_bars(context, state->scanner);
    }

    if (!state->mine) {
        state->mine = create_child(ops, context, state->mine_class,
            (StarsI16)-500, (StarsI16)-500, state->mine_width,
            state->mine_height, state->frame, state->instance);
    } else invalidate_child(ops, context, state->mine);

    if (!state->planet) {
        state->planet = create_child(ops, context, state->planet_class,
            (StarsI16)-500, (StarsI16)-500, 10, 10, state->frame,
            state->instance);
    } else invalidate_child(ops, context, state->planet);

    if (!state->toolbar) {
        state->toolbar = create_child(ops, context, state->toolbar_class,
            (StarsI16)-500, (StarsI16)-500, 10, 10, state->frame,
            state->instance);
    } else invalidate_child(ops, context, state->toolbar);

    if (state->message && ops->destroy_window)
        ops->destroy_window(context, state->message);
    state->message = create_child(ops, context, state->message_class,
        (StarsI16)-500, (StarsI16)-500, 10, 10, state->frame, state->instance);
    if (ops->refit_frame_children) ops->refit_frame_children(context);
}
