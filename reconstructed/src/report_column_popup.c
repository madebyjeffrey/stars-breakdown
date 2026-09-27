#include "core.h"
#include <string.h>

static StarsI16 popup_string(const StarsReportColumnPopupInput *input,
    StarsI16 id, StarsU8 *out, StarsU16 capacity)
{
    StarsI16 length;

    out[0] = 0;
    if (!input->get_string)
        return 0;
    length = input->get_string(input->context, id, out, capacity);
    if (length < 0)
        length = 0;
    if ((StarsU16)length >= capacity)
        length = (StarsI16)(capacity - 1U);
    out[length] = 0;
    return length;
}

static void popup_copy(char *out, StarsU16 capacity, const char *text)
{
    if (capacity == 0U)
        return;
    strncpy(out, text ? text : "", capacity - 1U);
    out[capacity - 1U] = 0;
}

static void popup_append(char *out, StarsU16 capacity, const char *text)
{
    StarsU16 used;
    StarsU16 available;

    if (capacity == 0U || !text)
        return;
    used = (StarsU16)strlen(out);
    available = (StarsU16)(capacity - 1U - used);
    strncat(out, text, available);
}

static void popup_menu_text(const StarsReportColumnPopupInput *input,
    StarsI16 string_id, const char *header, StarsU8 *out)
{
    StarsU8 localized[50];

    popup_string(input, string_id, localized, (StarsU16)sizeof(localized));
    popup_copy((char *)out, 50U, header);
    popup_append((char *)out, 50U, (const char *)localized);
}

/* NE 22:74D4-7AF5. Build and display a report-column menu, then apply its
 * sort or visibility selection. The original menu/resource and USER calls
 * are explicit callbacks; the report state transition remains typed C. */
void stars_report_column_popup(StarsI16 x, StarsI16 y,
    const StarsReportColumnPopupInput *input)
{
    StarsU8 storage[32][50];
    const StarsU8 *items[32];
    StarsI16 field_index[32];
    StarsI16 menu_count;
    StarsI16 sort_count;
    StarsI16 field_start;
    StarsI16 hide_index;
    StarsI16 special_count;
    StarsI16 selection;
    StarsI16 i;
    StarsI16 id;
    StarsU16 dc;
    StarsU32 bit;
    StarsU8 header[50];
    StarsU8 localized[50];
    const StarsU8 *extra;

    if (!input || !input->state)
        return;
    dc = input->get_dc ? input->get_dc(input->context,
        input->report_window) : input->hdc;
    if (input->get_header)
        input->get_header(input->context, input->state->report, input->column,
            dc, header, (StarsU16)sizeof(header));
    else
        header[0] = 0;
    menu_count = 0;
    sort_count = 0;
    field_start = 0;
    hide_index = -1;
    special_count = 0;
    for (i = 0; i < 2 && menu_count < 32; ++i) {
        popup_menu_text(input, (StarsI16)(0x46d + i), (const char *)header,
            storage[menu_count]);
        items[menu_count] = storage[menu_count];
        ++menu_count;
    }
    sort_count = menu_count;
    if ((input->state->report == 0 &&
            (input->column == 0x0b || input->column == 9 || input->column == 10)) ||
        (input->state->report == 1 && input->column == 7)) {
        if (menu_count < 32) {
            items[menu_count] = 0;
            ++menu_count;
        }
        special_count = (StarsI16)(3 + (input->state->report == 1));
        for (i = 0; i < special_count && menu_count < 32; ++i) {
            if (input->get_sort_label)
                input->get_sort_label(input->context, input->state->report,
                    i, storage[menu_count], 50U);
            else {
                id = (StarsI16)(0x471 + i);
                popup_string(input, id, localized, (StarsU16)sizeof(localized));
                popup_copy((char *)storage[menu_count], 50U,
                    (const char *)localized);
            }
            items[menu_count] = storage[menu_count];
            ++menu_count;
        }
        if (menu_count < 32) {
            items[menu_count] = 0;
            ++menu_count;
        }
        extra = input->get_compressed_string ?
            input->get_compressed_string(input->context, 0x55f) : 0;
        popup_copy((char *)storage[menu_count], 50U, (const char *)extra);
        items[menu_count] = storage[menu_count];
        ++menu_count;
        sort_count = menu_count;
    }
    if (menu_count < 32) {
        items[menu_count] = 0;
        ++menu_count;
    }
    sort_count = menu_count;
    hide_index = -1;
    if (input->column != 0 && menu_count < 32) {
        hide_index = menu_count;
        popup_string(input, 0x46f, localized, (StarsU16)sizeof(localized));
        popup_copy((char *)storage[menu_count], 50U, (const char *)header);
        popup_append((char *)storage[menu_count], 50U,
            (const char *)localized);
        popup_string(input, 0x470, localized, (StarsU16)sizeof(localized));
        popup_append((char *)storage[menu_count], 50U,
            (const char *)localized);
        items[menu_count] = storage[menu_count];
        ++menu_count;
        if (menu_count < 32) {
            items[menu_count] = 0;
            ++menu_count;
        }
    }
    field_start = menu_count;
    for (i = 0; i < input->state->field_count && menu_count < 32; ++i) {
        bit = (StarsU32)1UL << (i & 31);
        if ((input->state->visible_mask & bit) == 0U) {
            if (input->get_header)
                input->get_header(input->context, input->state->report, i, dc,
                    header, (StarsU16)sizeof(header));
            popup_string(input, 0x471, localized, (StarsU16)sizeof(localized));
            popup_copy((char *)storage[menu_count], 50U, (const char *)localized);
            popup_append((char *)storage[menu_count], 50U, (const char *)header);
            items[menu_count] = storage[menu_count];
            field_index[menu_count] = i;
            ++menu_count;
        }
    }
    if (menu_count == field_start)
        --menu_count;
    if (input->release_dc)
        input->release_dc(input->context, input->report_window, dc);
    selection = input->popup_menu ? input->popup_menu(input->context,
        input->report_window, x, y, (StarsU16)menu_count, items,
        input->right_button) : -1;
    if (selection < 0)
        return;
    if (selection < sort_count) {
        input->state->sort_column = input->column;
        if (special_count == 0) {
            input->state->ascending = (StarsI16)(selection == 0);
        } else {
            input->state->subsort = (StarsI16)((selection - 2) %
                (special_count + 3));
            if (input->state->subsort > special_count)
                input->state->subsort = special_count;
            input->state->ascending = (StarsI16)(selection < special_count + 2);
        }
        if (input->sort_cache)
            input->sort_cache(input->context, input->state->report,
                input->column);
    } else if (selection == hide_index) {
        input->state->visible_mask &= ~((StarsU32)1UL <<
            (input->column & 31));
        if (input->set_scroll_bar)
            input->set_scroll_bar(input->context);
    } else if (selection >= field_start && selection < menu_count) {
        input->state->visible_mask |= (StarsU32)1UL <<
            (field_index[selection] & 31);
        if (input->set_scroll_bar)
            input->set_scroll_bar(input->context);
    }
    if (input->invalidate)
        input->invalidate(input->context, input->report_window);
}
