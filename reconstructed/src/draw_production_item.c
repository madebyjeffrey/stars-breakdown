#include "core.h"
#include <string.h>

enum {
    PRODUCTION_LISTBOX_NONE = 0,
    PRODUCTION_LISTBOX_COLORED = 1,
    PRODUCTION_LISTBOX_BUTTON_FACE = 2,
    PRODUCTION_TEXT_BASE_OFFSET = 1,
    PRODUCTION_TEXT_LIST_OFFSET = 6,
    PRODUCTION_TEXT_MARKED_OFFSET = 7,
    PRODUCTION_SUFFIX_WIDTH = 6,
    PRODUCTION_SUFFIX_START = 2,
    PRODUCTION_PERCENT_DENOMINATOR = 100
};

static StarsU16 production_marker_offset(const StarsDrawProductionInput *in,
    StarsI16 *progress, StarsI16 *marked, StarsI16 *percent,
    StarsI16 *italic)
{
    StarsU8 marker = in->text[0];

    *progress = 0;
    *marked = 0;
    *percent = 0;
    *italic = 0;
    if (marker == 'I') {
        *italic = 1;
        return PRODUCTION_TEXT_BASE_OFFSET;
    }
    if (marker == 'P') {
        *progress = 1;
        *percent = (StarsI16)in->text[1];
        *marked = 1;
    } else if (marker == 'Q') {
        *marked = 1;
    }
    if (in->listbox == PRODUCTION_LISTBOX_NONE)
        return PRODUCTION_TEXT_BASE_OFFSET;
    if (*marked)
        return PRODUCTION_TEXT_MARKED_OFFSET;
    if (((StarsI16)in->text[1] - ' ') & 2)
        *italic = 1;
    return PRODUCTION_TEXT_LIST_OFFSET;
}

static StarsU16 production_brush(const StarsDrawProductionInput *in,
    StarsU32 *text_color, StarsI16 *italic, StarsI16 *progress,
    StarsI16 *percent)
{
    StarsU8 marker = in->text[0];
    StarsU16 brush = in->window_brush;

    *italic = marker == 'I';
    *progress = 0;
    *percent = 0;
    if (in->selected) {
        *text_color = in->window_color;
        if (marker == ' ' || marker == 'I') {
            brush = in->window_color == 0UL ?
                (in->ops->get_stock_object ?
                in->ops->get_stock_object(in->context, 0U) : 0U) :
                (in->ops->get_stock_object ?
                in->ops->get_stock_object(in->context, 4U) : 0U);
        } else if (marker == '#') {
            brush = in->blue_brush;
        } else if (marker == '&') {
            brush = in->gray_brush;
        } else if (marker == '*') {
            brush = in->green_brush;
        } else if (marker != 'P' && marker != 'Q') {
            brush = in->red_brush;
        }
    } else {
        if (marker == ' ' || marker == 'I')
            *text_color = in->blank_color;
        else if (marker == '#')
            *text_color = in->hash_color;
        else if (marker == '&')
            *text_color = in->ampersand_color;
        else if (marker == '*')
            *text_color = in->other_color;
        else if (marker == 'P' || marker == 'Q')
            *text_color = in->blank_color;
        else
            *text_color = in->other_color;
        if (marker == ' ' || marker == 'I') {
            brush = in->ops->get_stock_object ?
                in->ops->get_stock_object(in->context,
                in->window_color == 0UL ? 0U : 4U) : 0U;
        } else if (marker == '#') {
            brush = in->blue_brush;
        } else if (marker == '&') {
            brush = in->gray_brush;
        } else if (marker == '*') {
            brush = in->green_brush;
        } else if (marker == 'P' || marker == 'Q') {
            brush = in->window_brush;
        } else {
            brush = in->red_brush;
        }
    }
    if (marker == 'P') {
        *progress = 1;
        *percent = (StarsI16)in->text[1];
    }
    return brush;
}

/* NE 0A:6208-6B1A. Render the production-list marker protocol, including
 * selection colors, progress fill, clipped text, suffix extraction, and
 * restoration of the caller's text state. */
void stars_draw_production_item(const StarsDrawProductionInput *input)
{
    StarsRect rect;
    StarsRect progress_rect;
    StarsU16 brush;
    StarsU16 offset;
    StarsU16 text_length;
    StarsU16 old_mode;
    StarsU16 old_font;
    StarsU32 old_color;
    StarsU32 text_color;
    StarsI16 italic;
    StarsI16 marked;
    StarsI16 progress;
    StarsI16 percent;
    StarsI16 available_width;
    StarsI16 suffix_start;
    StarsI16 suffix_length;
    StarsU8 suffix[32];
    const StarsU8 *body;

    if (!input || !input->text || !input->ops || !input->ops->fill_rect)
        return;
    rect = input->rect;
    brush = production_brush(input, &text_color, &italic, &progress, &percent);
    if (input->listbox == PRODUCTION_LISTBOX_BUTTON_FACE)
        brush = input->button_face_brush;
    input->ops->fill_rect(input->context, input->hdc, &rect, brush);
    if (progress) {
        progress_rect = rect;
        progress_rect.right = (StarsU16)(rect.left +
            ((rect.right - rect.left) * percent) /
            PRODUCTION_PERCENT_DENOMINATOR);
        input->ops->fill_rect(input->context, input->hdc, &progress_rect,
            input->red_brush);
    }
    if (input->inflate && input->ops->inflate_rect)
        input->ops->inflate_rect(input->context, &rect, -2, -1);
    offset = production_marker_offset(input, &progress, &marked, &percent,
        &italic);
    old_color = input->ops->set_text_color ?
        input->ops->set_text_color(input->context, input->hdc, text_color) : 0UL;
    old_mode = input->ops->set_bk_mode ?
        input->ops->set_bk_mode(input->context, input->hdc, 1U) : 0U;
    old_font = 0U;
    if (italic && input->ops->select_font)
        old_font = input->ops->select_font(input->context, input->hdc,
            input->italic_font);
    body = input->text + offset;
    text_length = (StarsU16)strlen((const char *)body);
    available_width = (StarsI16)(rect.right - rect.left);
    while (text_length != 0U && input->ops->text_extent &&
            input->ops->text_extent(input->context, input->hdc, body,
            text_length) > available_width)
        --text_length;
    if (input->ops->text_out)
        input->ops->text_out(input->context, input->hdc, rect.left, rect.top,
            body, text_length);
    if (italic && input->ops->select_font)
        input->ops->select_font(input->context, input->hdc, old_font);
    if (offset >= PRODUCTION_TEXT_LIST_OFFSET && input->ops->right_text) {
        suffix[0] = '\0';
        if ((((StarsI16)input->text[offset - 5U] - ' ') & 2) != 0) {
            if (input->text[offset - 1U] == '*' && input->ops->get_string)
                suffix_length = input->ops->get_string(input->context, 0x0334U,
                    suffix, sizeof suffix);
            else if (input->ops->get_string)
                suffix_length = input->ops->get_string(input->context, 0x0518U,
                    suffix, sizeof suffix);
            else
                suffix_length = 0;
            suffix_length = (StarsI16)strlen((const char *)suffix);
            suffix_start = (StarsI16)(PRODUCTION_SUFFIX_START - marked);
            while (suffix_start < PRODUCTION_SUFFIX_WIDTH &&
                    input->text[suffix_start + marked] == ' ')
                ++suffix_start;
            if (suffix_start < PRODUCTION_SUFFIX_WIDTH) {
                strncpy((char *)suffix + suffix_length,
                    (const char *)input->text + marked + suffix_start,
                    PRODUCTION_SUFFIX_WIDTH - suffix_start);
                suffix_length = (StarsI16)(suffix_length +
                    PRODUCTION_SUFFIX_WIDTH - suffix_start);
                if (!marked && (((StarsI16)input->text[1] - ' ') & 1))
                    suffix[suffix_length - PRODUCTION_SUFFIX_WIDTH] = '%';
            }
        } else {
            suffix_length = 0;
        }
        input->ops->right_text(input->context, input->hdc, rect.right,
            rect.top, suffix, suffix_length, 0);
    }
    if (input->ops->set_text_color)
        input->ops->set_text_color(input->context, input->hdc, old_color);
    if (input->ops->set_bk_mode)
        input->ops->set_bk_mode(input->context, input->hdc, old_mode);
    (void)input->normal_font;
}
