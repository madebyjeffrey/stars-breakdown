#include "core.h"

enum {
    DECORATE_MESSAGE_SCAN_BYTES = 0x31,
    DECORATE_CURRENT_MESSAGE_MIN = 0,
    DECORATE_FILTER_ICON_SOURCE_X = 0x29,
    DECORATE_FILTER_ICON_SOURCE_Y = 0x45,
    DECORATE_FILTERED_ICON_SOURCE_X = 0x1a,
    DECORATE_FILTERED_ICON_SOURCE_Y = 0x36,
    DECORATE_MESSAGE_ICON_SOURCE_Y = 0x1c,
    DECORATE_MESSAGE_ICON_FILTERED_SOURCE_Y = 0x2a,
    DECORATE_SOUND_ICON_SOURCE_Y = 0x38,
    DECORATE_MESSAGE_ICON_WIDTH = 15,
    DECORATE_MESSAGE_ICON_HEIGHT = 14,
    DECORATE_FILTER_ICON_SIZE = 15,
    DECORATE_SOUND_ICON_WIDTH = 15,
    DECORATE_SOUND_ICON_HEIGHT = 9,
    DECORATE_SOUND_BUTTON_WIDTH = 17,
    DECORATE_SOUND_BUTTON_HEIGHT = 11
};

#define DECORATE_ICON_ROP_MASK 0x008800c6UL
#define DECORATE_ICON_ROP_PAINT 0x00ee0086UL
#define DECORATE_SOUND_ROP 0x00cc0020UL
#define DECORATE_BUTTON_ROP 0x00f00021UL

static StarsI16 decorate_bit_is_set(const StarsU8 *bits, StarsU16 bytes,
    StarsU16 id)
{
    StarsU16 byte_index = (StarsU16)(id >> 3);
    if (!bits || byte_index >= bytes) return 0;
    return (StarsI16)((bits[byte_index] >> (id & 7U)) & 1U);
}

static void decorate_bitmap(const StarsDecorateMsgTitleOps *ops, void *context,
    StarsU16 hdc, StarsU16 source_dc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsI16 source_y, StarsU32 raster)
{
    if (ops && ops->bit_blt)
        ops->bit_blt(context, hdc, x, y, width, height, source_dc, 0,
            source_y, raster);
}

static void decorate_filter_toggle(StarsDecorateMsgTitleInput *input,
    StarsU16 source_dc)
{
    const StarsDecorateMsgTitleOps *ops = input->ops;
    StarsI16 title_height = (StarsI16)(input->rect.bottom - input->rect.top);
    StarsI16 button_x = (StarsI16)(input->rect.right - title_height);
    StarsI16 icon_y;
    StarsI16 icon_x;
    StarsI16 source_x;
    StarsI16 source_y;
    StarsU16 old_brush;

    if (!ops) return;
    if (input->view_filtered) {
        source_x = DECORATE_FILTERED_ICON_SOURCE_X;
        source_y = DECORATE_FILTERED_ICON_SOURCE_Y;
    } else {
        source_x = DECORATE_FILTER_ICON_SOURCE_X;
        source_y = DECORATE_FILTER_ICON_SOURCE_Y;
    }
    old_brush = ops->select_object ? ops->select_object(input->context,
        input->hdc, input->shadow_brush) : 0;
    if (ops->pat_blt)
        ops->pat_blt(input->context, input->hdc, (StarsI16)(button_x - 1),
            input->rect.top, 1, title_height, DECORATE_BUTTON_ROP);
    if (ops->select_object)
        ops->select_object(input->context, input->hdc, input->highlight_brush);
    if (ops->pat_blt)
        ops->pat_blt(input->context, input->hdc, button_x, input->rect.top,
            1, title_height, DECORATE_BUTTON_ROP);
    if (ops->select_object)
        ops->select_object(input->context, input->hdc, old_brush);

    icon_y = (StarsI16)(((title_height - DECORATE_FILTER_ICON_SIZE) / 2) +
        input->rect.top);
    icon_x = (StarsI16)(input->rect.right -
        (input->rect.bottom - icon_y));
    if (ops->select_object)
        ops->select_object(input->context, source_dc, input->mono_bitmap);
    decorate_bitmap(ops, input->context, input->hdc, source_dc, icon_x, icon_y,
        DECORATE_FILTER_ICON_SIZE, DECORATE_FILTER_ICON_SIZE, source_y,
        DECORATE_ICON_ROP_MASK);
    if (ops->select_object)
        ops->select_object(input->context, source_dc, input->message_bitmap);
    if (ops->bit_blt)
        ops->bit_blt(input->context, input->hdc, icon_x, icon_y,
            DECORATE_FILTER_ICON_SIZE, DECORATE_FILTER_ICON_SIZE, source_dc,
            0, source_x, DECORATE_ICON_ROP_PAINT);
}

/* Genuine semantic reconstruction of DecorateMsgTitleBar, NE 7:799C-7D8B.
 * The original uses a compatible monochrome DC as a mask, then paints the
 * message, filter-toggle, and sound icons from two bitmaps.  The callbacks
 * retain that GDI sequencing while the input exposes the message/filter
 * state that controls which decoration is visible. */
void stars_decorate_msg_title_bar(StarsDecorateMsgTitleInput *input)
{
    const StarsDecorateMsgTitleOps *ops;
    StarsU16 source_dc;
    StarsU16 old_object;
    StarsU32 old_text_color = 0;
    StarsU32 old_background_color = 0;
    StarsI16 colors_changed = 0;
    StarsI16 has_filterable_message = 0;
    StarsI16 message_id;
    StarsU16 byte_index;
    StarsI16 title_height;
    StarsI16 icon_y;

    if (!input || !input->ops) return;
    ops = input->ops;
    source_dc = ops->create_compatible_dc ?
        ops->create_compatible_dc(input->context, input->hdc) : 0;
    if (!source_dc) return;
    old_object = ops->select_object ? ops->select_object(input->context,
        source_dc, input->mono_bitmap) : 0;
    title_height = (StarsI16)(input->rect.bottom - input->rect.top);

    if (!input->view_filtered && input->current_message >=
            DECORATE_CURRENT_MESSAGE_MIN &&
            input->current_message < input->message_count &&
            ops->message_id) {
        message_id = ops->message_id(input->context,
            input->current_message);
        if (!decorate_bit_is_set(input->filtered_bits,
                input->message_bit_bytes, (StarsU16)message_id)) {
            icon_y = (StarsI16)(((title_height - 11) / 2) +
                input->rect.top);
            old_text_color = ops->set_text_color ? ops->set_text_color(
                input->context, input->hdc, 0UL) : 0UL;
            old_background_color = ops->set_background_color ?
                ops->set_background_color(input->context, input->hdc,
                0x00ffffffUL) : 0UL;
            colors_changed = 1;
            decorate_bitmap(ops, input->context, input->hdc, source_dc,
                icon_y, (StarsI16)(icon_y - 3),
                DECORATE_MESSAGE_ICON_WIDTH, DECORATE_MESSAGE_ICON_HEIGHT,
                DECORATE_MESSAGE_ICON_SOURCE_Y, DECORATE_ICON_ROP_MASK);
            if (ops->select_object)
                ops->select_object(input->context, source_dc,
                    input->message_bitmap);
            if (ops->bit_blt)
                ops->bit_blt(input->context, input->hdc, icon_y,
                    (StarsI16)(icon_y - 3), DECORATE_MESSAGE_ICON_WIDTH,
                    DECORATE_MESSAGE_ICON_HEIGHT, source_dc, 0,
                    DECORATE_MESSAGE_ICON_SOURCE_Y, DECORATE_ICON_ROP_PAINT);
        } else {
            icon_y = (StarsI16)(((title_height - 11) / 2) +
                input->rect.top);
            old_text_color = ops->set_text_color ? ops->set_text_color(
                input->context, input->hdc, 0UL) : 0UL;
            old_background_color = ops->set_background_color ?
                ops->set_background_color(input->context, input->hdc,
                0x00ffffffUL) : 0UL;
            colors_changed = 1;
            decorate_bitmap(ops, input->context, input->hdc, source_dc,
                (StarsI16)(icon_y - 1), icon_y, 14, 12,
                DECORATE_MESSAGE_ICON_FILTERED_SOURCE_Y,
                DECORATE_ICON_ROP_MASK);
            if (ops->select_object)
                ops->select_object(input->context, source_dc,
                    input->message_bitmap);
            if (ops->bit_blt)
                ops->bit_blt(input->context, input->hdc,
                    (StarsI16)(icon_y - 1), icon_y, 14, 12, source_dc, 0,
                    DECORATE_MESSAGE_ICON_FILTERED_SOURCE_Y,
                    DECORATE_ICON_ROP_PAINT);
        }
    }

    /* The 49-byte scan mirrors the original bitfield span, including empty
     * bytes between message families.  A toggle is useful only if some sent
     * message is currently included by the filter. */
    for (byte_index = 0U; byte_index < DECORATE_MESSAGE_SCAN_BYTES;
            ++byte_index)
        if (input->sent_bits && input->filtered_bits &&
                byte_index < input->message_bit_bytes &&
                (input->sent_bits[byte_index] &
                input->filtered_bits[byte_index]) != 0U) {
            has_filterable_message = 1;
            break;
        }
    if (!has_filterable_message)
        input->view_filtered = 0;
    else
        decorate_filter_toggle(input, source_dc);

    if (colors_changed) {
        if (ops->set_text_color)
            ops->set_text_color(input->context, input->hdc, old_text_color);
        if (ops->set_background_color)
            ops->set_background_color(input->context, input->hdc,
                old_background_color);
    }
    if ((input->game_flags & STARS_DECORATE_SOUND_DISABLED) == 0U) {
        if (ops->select_object)
            ops->select_object(input->context, source_dc,
                input->message_bitmap);
        icon_y = (StarsI16)(((title_height - 7) / 2) + input->rect.top);
        if (ops->pat_blt)
            ops->pat_blt(input->context, input->hdc,
                (StarsI16)(input->rect.right - 0x2e),
                (StarsI16)(icon_y - 1), DECORATE_SOUND_BUTTON_WIDTH,
                DECORATE_SOUND_BUTTON_HEIGHT, 0x00000042UL);
        if (ops->bit_blt)
            ops->bit_blt(input->context, input->hdc,
                (StarsI16)(input->rect.right - 0x2d), icon_y,
                DECORATE_SOUND_ICON_WIDTH, DECORATE_SOUND_ICON_HEIGHT,
                source_dc, 0, DECORATE_SOUND_ICON_SOURCE_Y,
                DECORATE_SOUND_ROP);
    }
    if (ops->select_object)
        ops->select_object(input->context, source_dc, old_object);
    if (ops->delete_dc)
        ops->delete_dc(input->context, source_dc);
}
