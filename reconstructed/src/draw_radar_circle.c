#include "core.h"

/* NE 12:4E7C-540B. Accumulate radar discs, discard redundant contained
 * discs, recognize a clip-covering circle, and flush the accumulated set. */

static StarsI32 radar_square(StarsI16 value)
{
    return (StarsI32)value * (StarsI32)value;
}

static StarsI16 radar_contains_clip(const StarsRadarCircleState *state,
    StarsI16 cx, StarsI16 cy, StarsI16 radius)
{
    StarsI16 x[4];
    StarsI16 y[4];
    StarsI16 index;
    StarsI32 distance;

    x[0] = (StarsI16)state->clip.left; x[1] = x[0];
    x[2] = (StarsI16)state->clip.right; x[3] = x[2];
    y[0] = (StarsI16)state->clip.bottom; y[1] = (StarsI16)state->clip.top;
    y[2] = y[0]; y[3] = y[1];
    for (index = 0; index < 4; ++index) {
        if (radar_square(radius) < (StarsI32)(x[index] - cx) ||
            radar_square(radius) < (StarsI32)(y[index] - cy)) return 0;
        distance = radar_square((StarsI16)(x[index] - cx)) +
            radar_square((StarsI16)(y[index] - cy));
        if (radar_square(radius) < distance) return 0;
    }
    return 1;
}

static void radar_ellipse(const StarsRadarCircleState *state,
    const StarsRect *rect)
{
    if (state->ops && state->ops->ellipse)
        state->ops->ellipse(state->context, state->hdc, rect);
}

void stars_draw_radar_circle(StarsRadarCircleState *state,
    const StarsRect *rect)
{
    StarsRect ellipse;
    StarsI16 radius;
    StarsI16 center_x;
    StarsI16 center_y;
    StarsI16 index;
    StarsI16 slot;
    StarsI16 delta;
    StarsI32 distance;
    StarsU32 old_color;

    if (!state || !state->ops) return;
    if (!rect) {
        if (state->hollow && state->ops->set_mode) {
            state->ops->set_mode(state->context, state->hdc, 9);
            old_color = state->ops->set_background ?
                state->ops->set_background(state->context, state->hdc,
                    0x0000ff00UL) : 0;
        } else old_color = 0;
        for (index = 0; index < state->current; ++index) {
            if (state->radius[index] <= 0) continue;
            ellipse.left = (StarsU16)(state->center_x[index] - state->radius[index]);
            ellipse.top = (StarsU16)(state->center_y[index] - state->radius[index]);
            ellipse.right = (StarsU16)(state->center_x[index] + state->radius[index] + 1);
            ellipse.bottom = (StarsU16)(state->center_y[index] + state->radius[index] + 1);
            radar_ellipse(state, &ellipse);
        }
        if (state->hollow && state->ops->set_background)
            state->ops->set_background(state->context, state->hdc, old_color);
        if (state->hollow && state->ops->set_mode)
            state->ops->set_mode(state->context, state->hdc, 15);
        state->covered = 0; state->current = 0;
        return;
    }
    if (state->covered) return;
    if (state->ops->intersect && !state->ops->intersect(state->context,
        rect, &state->clip)) return;
    radius = (StarsI16)(((StarsI16)rect->right - (StarsI16)rect->left) >> 1);
    center_x = (StarsI16)rect->left + radius;
    center_y = (StarsI16)rect->top + radius;
    if (radar_contains_clip(state, center_x, center_y, radius)) {
        state->covered = 1; state->current = 0; return;
    }
    if (state->current != state->capacity) {
        slot = state->current;
        for (index = 0; index < state->current; ++index) {
            if (state->radius[index] < 1) slot = index;
            else {
                distance = radar_square((StarsI16)(center_x - state->center_x[index])) +
                    radar_square((StarsI16)(center_y - state->center_y[index]));
                if (state->radius[index] < radius) {
                    delta = (StarsI16)(radius - state->radius[index]);
                    if (distance < radar_square(delta)) {
                        state->radius[index] = 0; slot = index;
                    }
                } else if (radius < state->radius[index]) {
                    delta = (StarsI16)(state->radius[index] - radius);
                    if (distance < radar_square(delta)) return;
                } else if (center_x == state->center_x[index] &&
                    center_y == state->center_y[index]) return;
            }
        }
        if (slot >= state->current) {
            state->center_x[slot] = center_x; state->center_y[slot] = center_y;
            state->radius[slot] = radius; ++state->current;
            return;
        }
        return;
    }
    if (state->hollow && state->ops->set_mode) {
        state->ops->set_mode(state->context, state->hdc, 9);
        old_color = state->ops->set_background ?
            state->ops->set_background(state->context, state->hdc,
                0x0000ff00UL) : 0;
    } else old_color = 0;
    ellipse = *rect;
    radar_ellipse(state, &ellipse);
    if (state->hollow && state->ops->set_background)
        state->ops->set_background(state->context, state->hdc, old_color);
    if (state->hollow && state->ops->set_mode)
        state->ops->set_mode(state->context, state->hdc, 15);
}
