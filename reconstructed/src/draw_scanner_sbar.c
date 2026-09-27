#include "core.h"
#include <string.h>

enum {
    SCANNER_BAR_PLANET = 0x0001,
    SCANNER_BAR_FLEET = 0x0002,
    SCANNER_BAR_WAYPOINT = 0x0004,
    SCANNER_BAR_THING = 0x0008,
    SCANNER_BAR_EMPTY = 0x0000,
    SCANNER_BAR_TEXT_EMPTY = 0x0555,
    SCANNER_BAR_TEXT_DISTANCE_SHORT = 0x0556,
    SCANNER_BAR_TEXT_DISTANCE_LONG = 0x0557,
    SCANNER_BAR_TEXT_LOCATION = 0x0558,
    SCANNER_BAR_MARGIN = 4,
    SCANNER_BAR_INSET = 2
};

static StarsI16 bar_length(const StarsU8 *text)
{
    return text ? (StarsI16)strlen((const char *)text) : 0;
}

static StarsI16 bar_intersects(const StarsRect *left, const StarsRect *right)
{
    if (left->right <= right->left || right->right <= left->left)
        return 0;
    if (left->bottom <= right->top || right->bottom <= left->top)
        return 0;
    return 1;
}

static void bar_box(const StarsDrawScannerBarInput *input, StarsU16 hdc,
    StarsRect *box, StarsI16 x, StarsI16 width, StarsI16 redraw)
{
    box->left = x;
    box->right = (StarsI16)(x + width);
    if (input->ops->draw_lock_light)
        input->ops->draw_lock_light(input->context, hdc, box, redraw);
}

static void bar_text(const StarsDrawScannerBarInput *input, StarsU16 hdc,
    const StarsRect *box, const StarsU8 *text)
{
    StarsRect text_box;

    if (!text || !input->ops->text_out) return;
    text_box = *box;
    text_box.left = (StarsI16)(text_box.left + SCANNER_BAR_INSET + 1);
    text_box.top = (StarsI16)(text_box.top + SCANNER_BAR_INSET);
    input->ops->text_out(input->context, hdc, text_box.left, text_box.top,
        text, bar_length(text), &text_box);
}

static const StarsU8 *bar_name(const StarsDrawScannerBarInput *input,
    const StarsDrawScannerBarState *state, StarsU8 *buffer,
    StarsU16 capacity)
{
    const StarsU8 *text;
    StarsI16 id;
    StarsI16 kind;

    if (state->label) return state->label;
    kind = 0;
    id = state->object_id;
    if ((state->object_full_flags & SCANNER_BAR_PLANET) != 0) {
        kind = SCANNER_BAR_PLANET;
        id = state->planet_id;
    } else if ((state->object_full_flags & SCANNER_BAR_FLEET) != 0) {
        kind = SCANNER_BAR_FLEET;
        id = state->fleet_id;
    } else if ((state->object_full_flags & SCANNER_BAR_THING) != 0) {
        kind = SCANNER_BAR_THING;
        id = state->thing_id;
    }
    text = input->ops->get_object_name ? input->ops->get_object_name(
        input->context, kind, id) : 0;
    if (text) return text;
    if (input->ops->get_string) {
        (void)input->ops->get_string(input->context, SCANNER_BAR_TEXT_EMPTY,
            buffer, capacity);
        return buffer;
    }
    return 0;
}

static void bar_draw_coordinate(const StarsDrawScannerBarInput *input,
    StarsU16 hdc, StarsRect *box, StarsPoint point)
{
    StarsU8 text[40];
    StarsI16 length;

    if (!input->ops->format_number) return;
    length = input->ops->format_number(input->context, point.x, text,
        (StarsU16)sizeof text);
    bar_text(input, hdc, box, text);
    (void)length;
}

/* NE 12:62D8-6AFF.  Draw the scanner's two-row status bar.  The original
 * packed _sbar and selection globals are represented by typed state; Win16
 * handles, resource strings, object names, distance text and lock-light
 * painting stay at explicit service callbacks. */
void stars_draw_scanner_sbar(StarsU16 hdc, const StarsRect *paint_rect,
    StarsDrawScannerBarInput *input)
{
    StarsRect client;
    StarsRect row;
    StarsRect clip;
    StarsRect box;
    StarsRect text_box;
    const StarsDrawScannerBarState *state;
    const StarsU8 *text;
    const StarsU8 *distance;
    StarsU16 old_brush;
    StarsU16 old_mode;
    StarsU32 old_background;
    StarsU32 old_text;
    StarsI16 acquired;
    StarsI16 x;
    StarsI16 width;
    StarsI16 id;
    StarsI16 kind;
    StarsU8 buffer[160];

    if (!input || !input->ops) return;
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->context, input->scanner_window,
            &client);
    else {
        client.left = 0;
        client.top = 0;
        client.right = 320;
        client.bottom = (StarsI16)(input->scrollbar_height * 2);
    }
    row.left = client.left;
    row.right = client.right;
    row.bottom = client.bottom;
    row.top = (StarsI16)(row.bottom - input->scrollbar_height);
    if (paint_rect && !bar_intersects(paint_rect, &row)) return;
    acquired = 0;
    if (hdc == 0 && input->ops->get_dc) {
        hdc = input->ops->get_dc(input->context, input->scanner_window);
        acquired = 1;
    }
    if (hdc == 0) return;
    old_mode = input->ops->set_background_mode ?
        input->ops->set_background_mode(input->context, hdc, 1) : 0;
    old_background = input->ops->set_background_color ?
        input->ops->set_background_color(input->context, hdc,
            input->button_face_color) : 0;
    old_text = input->ops->set_text_color ? input->ops->set_text_color(
        input->context, hdc, input->button_text_color) : 0;
    old_brush = input->button_face_brush;
    if (input->ops->select_object)
        input->ops->select_object(input->context, hdc, old_brush);
    if (input->full_redraw && input->ops->fill_background)
        input->ops->fill_background(input->context, hdc, &row,
            input->full_redraw);
    row.bottom = (StarsI16)(row.bottom - input->scrollbar_height / 2);
    clip.left = (StarsI16)(row.left + SCANNER_BAR_MARGIN);
    clip.top = (StarsI16)(row.top + SCANNER_BAR_MARGIN);
    clip.right = row.right;
    clip.bottom = (StarsI16)(row.bottom - SCANNER_BAR_MARGIN);
    x = clip.left;
    state = input->bar ? input->bar : &input->selection;
    width = input->ops->text_extent ? input->ops->text_extent(input->context,
        hdc, (const StarsU8 *)"planet", 6) + 6 : 42;
    bar_box(input, hdc, &box, x, width, input->full_redraw);
    if ((state->object_flags & SCANNER_BAR_PLANET) != 0) {
        id = state->planet_id + 1;
        if (input->ops->format_number) {
            (void)input->ops->format_number(input->context, id, buffer,
                (StarsU16)sizeof buffer);
            bar_text(input, hdc, &box, buffer);
        }
    } else if ((state->object_flags & SCANNER_BAR_WAYPOINT) != 0) {
        if (input->ops->format_number) {
            (void)input->ops->format_number(input->context, state->waypoint,
                buffer, (StarsU16)sizeof buffer);
            bar_text(input, hdc, &box, buffer);
        }
    }
    x = (StarsI16)(box.right + SCANNER_BAR_MARGIN);
    width = input->ops->text_extent ? input->ops->text_extent(input->context,
        hdc, (const StarsU8 *)"object", 6) + 6 : 42;
    bar_box(input, hdc, &box, x, width, input->full_redraw);
    text = bar_name(input, state, buffer, (StarsU16)sizeof buffer);
    bar_text(input, hdc, &box, text);
    x = (StarsI16)(box.right + SCANNER_BAR_MARGIN);
    width = input->ops->text_extent ? input->ops->text_extent(input->context,
        hdc, (const StarsU8 *)"0000", 4) + 6 : 34;
    bar_box(input, hdc, &box, x, width, input->full_redraw);
    if (state->point.x > 0) bar_draw_coordinate(input, hdc, &box,
        state->point);
    x = (StarsI16)(box.right + SCANNER_BAR_MARGIN);
    width = input->ops->text_extent ? input->ops->text_extent(input->context,
        hdc, (const StarsU8 *)"0000", 4) + 6 : 34;
    bar_box(input, hdc, &box, x, width, input->full_redraw);
    if (state->point.y > 0) {
        if (input->ops->format_number) {
            (void)input->ops->format_number(input->context, state->point.y,
                buffer, (StarsU16)sizeof buffer);
            bar_text(input, hdc, &box, buffer);
        }
    }
    x = (StarsI16)(box.right + SCANNER_BAR_MARGIN);
    clip.right = (StarsI16)(row.right - SCANNER_BAR_MARGIN);
    bar_box(input, hdc, &box, x, (StarsI16)(clip.right - x),
        input->full_redraw);
    text_box = box;
    text_box.left = (StarsI16)(text_box.left - 2);
    text_box.right = (StarsI16)(text_box.right + 2);
    text = bar_name(input, state, buffer, (StarsU16)sizeof buffer);
    if (state->label) bar_text(input, hdc, &text_box, text);
    kind = (StarsI16)(state->object_full_flags & 0x000f);
    distance = 0;
    if (state->point.x != state->target.x || state->point.y != state->target.y) {
        if (input->ops->get_distance)
            distance = input->ops->get_distance(input->context, state->point,
                state->target);
        if (distance) bar_text(input, hdc, &text_box, distance);
        if (input->ops->get_location) {
            text = input->ops->get_location(input->context, kind,
                state->object_id, state->target);
            if (text) bar_text(input, hdc, &text_box, text);
        }
    }
    if (input->ops->select_object)
        input->ops->select_object(input->context, hdc, old_brush);
    if (input->ops->set_background_mode)
        (void)input->ops->set_background_mode(input->context, hdc, old_mode);
    if (input->ops->set_text_color)
        (void)input->ops->set_text_color(input->context, hdc, old_text);
    if (input->ops->set_background_color)
        (void)input->ops->set_background_color(input->context, hdc,
            old_background);
    if (acquired && input->ops->release_dc)
        input->ops->release_dc(input->context, input->scanner_window, hdc);
}
