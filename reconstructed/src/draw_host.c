/* Semantic reconstruction of DrawHostDialog2. */
#include <stdio.h>
#include <string.h>
#include "core.h"

static const StarsU8 *compressed(const StarsDrawHostOps *ops, void *context,
    StarsU16 id)
{
    return ops->compressed ? ops->compressed(context, id) : (const StarsU8 *)"";
}

static StarsU16 copy_string(const StarsDrawHostOps *ops, void *context,
    StarsI16 id, StarsU8 *output, StarsU16 capacity)
{
    StarsU16 length;

    if (!ops->get_string || capacity == 0U) return 0U;
    length = ops->get_string(context, (StarsU16)id, output, capacity);
    if (length >= capacity) length = (StarsU16)(capacity - 1U);
    output[length] = 0;
    return length;
}

static void append_dead_marker(const StarsDrawHostOps *ops, void *context,
    StarsU8 *output, StarsU16 *length, StarsU16 capacity)
{
    const StarsU8 *marker;
    StarsU16 marker_length;

    marker = compressed(ops, context, 0x403U);
    marker_length = (StarsU16)strlen((const char *)marker);
    if ((StarsU16)(*length + marker_length) >= capacity)
        marker_length = (StarsU16)(capacity - *length - 1U);
    if (marker_length != 0U) {
        memcpy(output + *length, marker, marker_length);
        *length = (StarsU16)(*length + marker_length);
        output[*length] = 0;
    }
}

/* NE 5:6240. Drawing is represented by callbacks, while all selection,
 * per-player formatting, dead-marker, and elapsed-time branches are C. */
void stars_draw_host_dialog(StarsDrawHostState *state, StarsU16 hdc_in,
    const StarsDrawHostOps *ops, void *context)
{
    StarsU16 dc;
    StarsU16 old_bk_mode;
    StarsU32 old_bk_color;
    StarsU16 i;
    StarsI16 x_text;
    StarsI16 y;
    StarsI16 extent;
    StarsRect rect;
    StarsU8 work[128];
    StarsU8 status[48];
    const StarsU8 *format;
    const StarsU8 *name;
    StarsU16 length;
    StarsU32 elapsed;
    StarsU32 seconds;
    StarsU32 minutes;
    StarsU32 hours;
    StarsU32 days;
    StarsU16 control;

    if (!state || !ops || !ops->set_bk_mode || !ops->set_bk_color ||
        !ops->select_font || !ops->set_text_color || !ops->set_rect ||
        !ops->offset_rect || !ops->draw_diamond || !ops->right_text ||
        !ops->text_out || !ops->text_extent || !ops->get_dialog_item ||
        !ops->set_window_text) return;
    if (hdc_in == 0U && !ops->get_dc) return;
    dc = hdc_in == 0U ? ops->get_dc(context, state->frame) : hdc_in;
    if (dc == 0U) return;
    old_bk_mode = ops->set_bk_mode(context, dc, 2U);
    old_bk_color = ops->set_bk_color(context, dc, state->button_face);
    ops->select_font(context, dc, state->font);
    format = compressed(ops, context, 0x2f2U);
    extent = ops->text_extent(context, dc, format, 4U);
    x_text = (StarsI16)(state->dy_arial8 + 10 + extent);
    y = 0x30;
    ops->set_rect(context, &rect, 6, 0x30,
        (StarsI16)(state->dy_arial8 + 7),
        (StarsI16)(state->dy_arial8 + 0x31));
    for (i = 0U; i < state->player_count && i < 16U; ++i) {
        ops->draw_diamond(context, dc, &rect, state->blue_brush);
        format = compressed(ops, context, 0x2f1U);
        sprintf((char *)work, (const char *)format, (int)i + 1);
        length = (StarsU16)strlen((const char *)work);
        ops->right_text(context, dc, x_text, y, work, length, 0);
        ops->set_text_color(context, dc,
            state->output[i] < 1 ? 0x7f00UL : 0x7fUL);
        copy_string(ops, context, (StarsI16)(state->output[i] + 0x2cc),
            status, (StarsU16)sizeof(status));
        if ((state->global_flags & 0x40U) == 0U) {
            name = ops->player_name ? ops->player_name(context, (StarsI16)i) :
                (const StarsU8 *)"";
            format = compressed(ops, context, 0x2fdU);
            sprintf((char *)work, (const char *)format,
                (const char *)name, (const char *)status);
        } else {
            format = compressed(ops, context, 0x3ffU);
            sprintf((char *)work, (const char *)format,
                (const char *)status);
        }
        length = (StarsU16)strlen((const char *)work);
        if (state->dead_flags[i] & 0x10U)
            append_dead_marker(ops, context, work, &length,
                (StarsU16)sizeof(work));
        ops->text_out(context, dc, (StarsI16)(x_text + 4), y, work, length);
        ops->set_text_color(context, dc, state->window_text);
        ops->offset_rect(context, &rect, 0,
            (StarsI16)(state->dy_arial8 + 4));
        y = (StarsI16)(y + state->dy_arial8 + 4);
    }
    format = state->turn_format ? state->turn_format : (const StarsU8 *)"%d";
    sprintf((char *)work, (const char *)format, (int)state->turn + 0x961);
    control = ops->get_dialog_item(context, state->frame, 0x7e0U);
    ops->set_window_text(context, control, work);

    elapsed = ops->get_tick ?
        (StarsU32)(ops->get_tick(context) - state->tick_last) : 0UL;
    seconds = elapsed / 1000UL;
    if (seconds < 60UL) {
        format = compressed(ops, context, 0x2f5U);
        sprintf((char *)work, (const char *)format,
            (unsigned long)seconds);
    } else {
        minutes = seconds / 60UL;
        seconds %= 60UL;
        if (minutes < 60UL) {
            format = compressed(ops, context, 0x2f6U);
            sprintf((char *)work, (const char *)format,
                (unsigned long)minutes, (unsigned long)seconds);
        } else {
            hours = minutes / 60UL;
            minutes %= 60UL;
            if (hours < 24UL) {
                format = compressed(ops, context, 0x2f7U);
                sprintf((char *)work, (const char *)format,
                    (unsigned long)hours, (unsigned long)minutes,
                    (unsigned long)seconds);
            } else {
                days = hours / 24UL;
                hours %= 24UL;
                format = compressed(ops, context, 0x2f8U);
                sprintf((char *)work, (const char *)format,
                    (unsigned long)days, (unsigned long)hours,
                    (unsigned long)minutes, (unsigned long)seconds);
            }
        }
    }
    control = ops->get_dialog_item(context, state->frame, 0x7e1U);
    ops->set_window_text(context, control, work);
    ops->set_bk_mode(context, dc, old_bk_mode);
    ops->set_bk_color(context, dc, old_bk_color);
    if (hdc_in == 0U && ops->release_dc)
        ops->release_dc(context, state->frame, dc);
}
