#include "core.h"
#include <string.h>

#define PRODUCTION_DLG_SOURCE_LIST 0x416
#define PRODUCTION_DLG_PLANET_LIST 0x417
#define PRODUCTION_DLG_DIAMOND_LABEL 0x0d2
#define PRODUCTION_DLG_COST_LABEL 0x33f
#define PRODUCTION_DLG_ETA_FORMAT 0x340
#define PRODUCTION_DLG_MINERAL_ROWS 4

static StarsI16 production_format_number(StarsI32 value, StarsU8 *text,
    StarsU16 capacity)
{
    StarsU8 reversed[32];
    unsigned long magnitude;
    StarsU16 count;
    StarsU16 index;
    StarsI16 negative;

    if (!text || capacity == 0) return 0;
    negative = value < 0;
    magnitude = negative ? (unsigned long)(-(value + 1)) + 1UL :
        (unsigned long)value;
    count = 0;
    do {
        if (count >= (StarsU16)sizeof reversed) return 0;
        reversed[count++] = (StarsU8)('0' + magnitude % 10UL);
        magnitude /= 10UL;
    } while (magnitude != 0UL);
    if (negative) {
        if (count >= (StarsU16)sizeof reversed) return 0;
        reversed[count++] = '-';
    }
    if (count + 1 > capacity) return 0;
    for (index = 0; index < count; ++index)
        text[index] = reversed[count - index - 1];
    text[count] = 0;
    return (StarsI16)count;
}

static void production_draw_costs(StarsDrawProductionDlgInput *input,
    StarsU16 dc, const StarsRect *list_rect, StarsI16 list_kind,
    StarsI16 selected, StarsI16 unit_width)
{
    static const StarsU16 labels[PRODUCTION_DLG_MINERAL_ROWS] = {
        0, 1, 2, 5
    };
    StarsI32 costs[4];
    StarsU8 text[64];
    const StarsU8 *label;
    StarsI16 row;
    StarsI16 label_left;
    StarsI16 right;
    StarsI16 separator_x;
    StarsI16 y;
    StarsI16 length;
    const StarsU32 *items;
    StarsU16 item_count;
    StarsU32 packed;
    StarsI16 have_costs;
    StarsU16 label_id;

    have_costs = 0;
    if (input->ops->get_costs)
        have_costs = input->ops->get_costs(input->ops->context, selected,
            costs);
    if (!have_costs && input->ops->get_packed_costs) {
        items = list_kind == 0 ? input->source_items : input->planet_items;
        item_count = list_kind == 0 ? input->source_item_count :
            input->planet_item_count;
        if (items && selected >= 0 && (StarsU16)selected < item_count) {
            packed = items[selected];
            have_costs = input->ops->get_packed_costs(input->ops->context,
                list_kind, selected, packed, costs);
        }
    }
    if (!have_costs)
        return;
    label_left = (StarsI16)(list_rect->left + 0x14);
    separator_x = (StarsI16)(list_rect->right - 0x14 - unit_width);
    right = (StarsI16)(separator_x - 2);
    y = (StarsI16)(list_rect->bottom + 4);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, dc, input->bold_font);
    label = input->ops->get_label ? input->ops->get_label(
        input->ops->context, PRODUCTION_DLG_COST_LABEL) :
        (const StarsU8 *)0;
    if (label && input->ops->text_out)
        input->ops->text_out(input->ops->context, dc, list_rect->left, y,
            label, (StarsI16)strlen((const char *)label));
    for (row = 0; row < PRODUCTION_DLG_MINERAL_ROWS; ++row) {
        y = (StarsI16)(y + input->line_height);
        label_id = labels[row];
        if (input->mineral_labels && label_id < input->mineral_label_count)
            label = input->mineral_labels[label_id];
        else
            label = input->ops->compressed_string ?
                input->ops->compressed_string(input->ops->context, label_id) :
                (const StarsU8 *)0;
        if (input->ops->get_mineral_color && input->ops->set_text_color)
            input->ops->set_text_color(input->ops->context, dc,
                input->ops->get_mineral_color(input->ops->context,
                    labels[row]));
        if (label && input->ops->text_out)
            input->ops->text_out(input->ops->context, dc, label_left,
                y, label, (StarsI16)strlen((const char *)label));
        if (input->ops->right_text) {
            if (input->ops->select_font)
                input->ops->select_font(input->ops->context, dc,
                    input->normal_font);
            if (input->ops->set_text_color)
                input->ops->set_text_color(input->ops->context, dc,
                    input->text_color);
            length = input->ops->format_number ?
                input->ops->format_number(input->ops->context, costs[row],
                    text, (StarsU16)sizeof text) :
                production_format_number(costs[row], text,
                    (StarsU16)sizeof text);
            input->ops->right_text(input->ops->context, dc, right, y, text,
                length, input->max_mineral_width);
        }
        if (row < 3 && input->ops->text_out) {
            label = input->ops->compressed_string ?
                input->ops->compressed_string(input->ops->context, 0x2fe) :
                (const StarsU8 *)0;
            if (label)
                input->ops->text_out(input->ops->context, dc, separator_x, y,
                    label, 2);
        }
    }
    if (list_kind != 0 && input->ops->get_eta) {
        y = (StarsI16)(y + input->line_height + input->line_height / 2);
        if (input->ops->get_eta_format)
            (void)input->ops->get_eta_format(input->ops->context, 0x340);
        (void)input->ops->get_eta(input->ops->context, selected, y);
    }
}

/* Genuine source reconstruction of DrawProductionDlg, NE 1B:35DC-3AFF.
 * It selects the current source/planet-list item, renders the four cost rows
 * and optional ETA, and records the production-help diamond geometry. */
void stars_draw_production_dlg(StarsDrawProductionDlgInput *input,
    StarsI16 draw_mode)
{
    StarsU16 dc;
    StarsU16 item;
    StarsRect rect;
    StarsI16 list_kind;
    StarsI16 selected;
    StarsI16 width;
    const StarsU8 *label;

    if (!input || !input->ops)
        return;
    dc = input->hdc;
    if (dc == 0 && input->ops->get_dc)
        dc = input->ops->get_dc(input->ops->context, input->hwnd);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, dc, input->normal_font);
    label = input->ops->compressed_string ? input->ops->compressed_string(
        input->ops->context, 0x2fe) : (const StarsU8 *)0;
    width = label && input->ops->text_extent ? input->ops->text_extent(
        input->ops->context, dc, label, 2) : 0;
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, dc, input->bold_font);
    if (input->ops->set_background_color)
        input->ops->set_background_color(input->ops->context, dc,
            input->face_color);
    for (list_kind = 0; list_kind < 2; ++list_kind) {
        item = input->ops->get_dialog_item ? input->ops->get_dialog_item(
            input->ops->context, input->hwnd, list_kind == 0 ?
            PRODUCTION_DLG_SOURCE_LIST : PRODUCTION_DLG_PLANET_LIST) : 0;
        rect.left = rect.top = rect.right = rect.bottom = 0;
        if (input->ops->get_window_rect)
            input->ops->get_window_rect(input->ops->context, item, &rect);
        if (input->ops->screen_to_client)
            input->ops->screen_to_client(input->ops->context, input->hwnd,
                &rect);
        selected = input->ops->selection ? input->ops->selection(
            input->ops->context, item) : -1;
        if (selected >= 0 && (selected != 0 || list_kind != 1))
            production_draw_costs(input, dc, &rect, list_kind,
                (StarsI16)(selected - (list_kind != 0 ? 1 : 0)), width);
    }
    if (input->ops->get_client_rect)
        input->ops->get_client_rect(input->ops->context, input->hwnd, &rect);
    rect.top = (StarsU16)(rect.bottom - (input->line_height * 5 / 2 + 12));
    rect.bottom = (StarsU16)(rect.top + (input->line_height | 1));
    rect.left = 6;
    rect.right = (StarsU16)(rect.left + input->line_height);
    if (input->diamond_rect)
        *input->diamond_rect = rect;
    if (input->ops->draw_diamond)
        input->ops->draw_diamond(input->ops->context, dc, &rect);
    label = input->ops->get_label ? input->ops->get_label(
        input->ops->context, PRODUCTION_DLG_DIAMOND_LABEL) :
        (const StarsU8 *)0;
    if (label && input->ops->text_out)
        input->ops->text_out(input->ops->context, dc,
            (StarsI16)(rect.right + 4), rect.top, label,
            (StarsI16)strlen((const char *)label));
    (void)draw_mode;
    if (input->hdc == 0 && input->ops->release_dc)
        input->ops->release_dc(input->ops->context, input->hwnd, dc);
}
