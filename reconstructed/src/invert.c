/* Semantic reconstruction of InvertPaneBorder. */
#include "core.h"

static StarsI16 absolute_value(StarsI16 value)
{ return value < 0 ? (StarsI16)(0 - value) : value; }

static void pat_blt(const StarsInvertOps *ops, void *context, StarsU16 dc,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height)
{
    if (ops->pat_blt)
        ops->pat_blt(context, dc, x, y, width, height, 0x005a0049UL);
}

static StarsI16 clamp_x(const StarsInvertState *state, StarsI16 x,
    StarsI16 width_limit)
{
    if (state->x_top + x < 198)
        x = (StarsI16)(198 - state->x_top);
    if (state->dx - state->x_top - x < width_limit)
        x = (StarsI16)(state->dx - state->x_top - width_limit);
    if (state->x_top + x > 396)
        x = (StarsI16)(396 - state->x_top);
    return x;
}

/* NE 5:1E3C-24AF. The point returned by cases 2 and 4 intentionally carries
 * only the vertical delta, matching the original packed POINT return. */
StarsInvertPoint stars_invert_pane_border(StarsInvertState *state,
    StarsU16 dc, StarsI16 gr_sel, StarsInvertPoint point,
    const StarsInvertPoint *previous, const StarsInvertOps *ops, void *context)
{
    StarsI16 old_x;
    StarsI16 width_limit;
    StarsI16 top_limit;
    StarsI16 vertical_span;
    StarsI16 lower_edge;
    StarsI16 direction;
    StarsI16 top_height;
    StarsI16 delta_x;
    StarsI16 strip_width;
    StarsI16 draw_x;
    StarsI16 available;

    if (!state || !ops) return point;
    old_x = point.x;
    if (previous && gr_sel != 1) {
        StarsInvertPoint delta;
        delta.x = (StarsI16)(point.x - previous->x);
        delta.y = (StarsI16)(point.y - previous->y);
        (void)stars_invert_pane_border(state, dc, gr_sel, delta, 0,
            ops, context);
    }
    if (state->layout == 0 ||
        (state->layout != 1 && state->layout != 2)) {
        width_limit = 100; top_limit = 50;
        vertical_span = (StarsI16)(state->y2 - state->y1);
        lower_edge = (StarsI16)(vertical_span - 8);
        direction = 1;
        top_height = (StarsI16)((state->dy_arial8 * 13) / 2 + 10);
    } else {
        width_limit = 200; top_limit = 100;
        vertical_span = (StarsI16)(state->dy - state->y1);
        lower_edge = state->y2;
        direction = -1;
        top_height = 100;
    }
    vertical_span = (StarsI16)(vertical_span - 8);
    point.x = clamp_x(state, point.x, width_limit);
    if (gr_sel & 2) {
        if (state->y1 + point.y < top_limit)
            point.y = (StarsI16)(top_limit - state->y1);
        else if (vertical_span - point.y < top_height)
            point.y = (StarsI16)(vertical_span - top_height);
    }
    if (gr_sel & 4) {
        if (lower_edge + point.y < top_height)
            point.y = (StarsI16)((top_height - lower_edge) * direction);
        else {
            available = (StarsI16)(state->dy - state->y2 - 8);
            if (available - point.y < state->dy_arial8 * 13 - 36)
                point.y = (StarsI16)(available -
                    (state->dy_arial8 * 13 - 36));
        }
    }
    if (previous) {
        delta_x = (StarsI16)(old_x - previous->x);
        delta_x = clamp_x(state, delta_x, width_limit);
        strip_width = (StarsI16)(point.x - delta_x);
    } else strip_width = 0;
    switch (gr_sel) {
    case 2:
        pat_blt(ops, context, dc, 0, (StarsI16)(state->y1 + point.y + 1),
            (StarsI16)(state->x_top + 1), 6);
        point.x = 0;
        break;
    case 3:
        pat_blt(ops, context, dc, (StarsI16)(state->x_top + point.x + 1),
            0, 6, state->dy);
        pat_blt(ops, context, dc, 0,
            (StarsI16)(state->y1 + point.y + 1),
            (StarsI16)(state->x_top + point.x + 1), 6);
        point.x = point.x;
        break;
    case 4:
        pat_blt(ops, context, dc, state->layout == 0 ? 0 :
            (StarsI16)(state->x_top + 7),
            (StarsI16)(state->y2 + point.y + 1),
            state->layout == 0 ? (StarsI16)(state->x_top + 1) :
            (StarsI16)(state->dx - state->x_top - 7), 6);
        point.x = 0;
        break;
    case 5:
        pat_blt(ops, context, dc, (StarsI16)(state->x_top + point.x + 1),
            0, 6, state->dy);
        pat_blt(ops, context, dc, state->layout == 0 ? 0 :
            (StarsI16)(state->x_top + point.x + 7),
            (StarsI16)(state->y2 + point.y + 1),
            state->layout == 0 ? (StarsI16)(state->x_top + point.x + 1) :
            (StarsI16)(state->dx - point.x - state->x_top - 7), 6);
        break;
    case 7:
        pat_blt(ops, context, dc, (StarsI16)(state->x_top + point.x + 1),
            0, 6, state->dy);
        pat_blt(ops, context, dc, 0,
            (StarsI16)(state->y1 + point.y + 1),
            (StarsI16)(state->x_top + point.x + 1), 6);
        pat_blt(ops, context, dc, (StarsI16)(state->x_top + point.x + 7),
            (StarsI16)(state->y2 + point.y + 1),
            (StarsI16)(state->dx - point.x - state->x_top - 7), 6);
        break;
    default:
        if (!previous || absolute_value(strip_width) > 5) {
            if (previous)
                pat_blt(ops, context, dc,
                    (StarsI16)(state->x_top + point.x - strip_width + 1),
                    0, 6, state->dy);
            pat_blt(ops, context, dc,
                (StarsI16)(state->x_top + point.x + 1), 0, 6, state->dy);
        } else if (strip_width) {
            if (strip_width < 0) {
                draw_x = (StarsI16)(state->x_top + point.x);
                strip_width = (StarsI16)(0 - strip_width);
            } else draw_x = (StarsI16)(state->x_top + point.x - strip_width);
            pat_blt(ops, context, dc, (StarsI16)(draw_x + 1), 0,
                strip_width, state->dy);
            pat_blt(ops, context, dc, (StarsI16)(draw_x + 7), 0,
                strip_width, state->dy);
        }
        break;
    }
    return point;
}
