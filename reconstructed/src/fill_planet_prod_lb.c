#include "core.h"
#include <string.h>

enum {
    PLANET_PROD_CLEAR = 0x405,
    PLANET_PROD_ADD = 0x401,
    PLANET_PROD_NONE_STRING = 0x33c,
    PLANET_PROD_ACTIVE_STRING = 0x33d,
    PLANET_PROD_KIND_SHIFT = 17,
    PLANET_PROD_ITEM_SHIFT = 10,
    PLANET_PROD_KIND_MASK = 7,
    PLANET_PROD_ITEM_MASK = 0x7f,
    PLANET_PROD_COUNT_MASK = 0x3ff,
    PLANET_PROD_SHIP_KIND = 1,
    PLANET_PROD_SPECIAL_ITEM_LIMIT = 7,
    PLANET_PROD_MAX_ETA = 100
};

static StarsU16 production_item_kind(StarsU32 packed)
{
    return (StarsU16)((packed >> PLANET_PROD_KIND_SHIFT) &
        PLANET_PROD_KIND_MASK);
}

static StarsU16 production_item_code(StarsU32 packed)
{
    return (StarsU16)((packed >> PLANET_PROD_ITEM_SHIFT) &
        PLANET_PROD_ITEM_MASK);
}

static StarsU16 production_item_count(StarsU32 packed)
{
    return (StarsU16)(packed & PLANET_PROD_COUNT_MASK);
}

static StarsI16 production_is_ordinary_eta(StarsI16 first, StarsI16 last,
    StarsU32 packed)
{
    StarsU16 kind = production_item_kind(packed);
    StarsU16 item = production_item_code(packed);

    (void)last;

    if (first >= 2 && first <= 99)
        return 1;
    if (first != PLANET_PROD_MAX_ETA)
        return 0;
    return kind == PLANET_PROD_SHIP_KIND &&
        item < PLANET_PROD_SPECIAL_ITEM_LIMIT;
}

static StarsU8 production_status(StarsI16 first, StarsI16 last,
    StarsU32 packed)
{
    if ((first == 0 && last == 0) || (first == -1 && last == -1))
        return '&';
    if (production_is_ordinary_eta(first, last, packed))
        return ' ';
    if (first == 1 && last == 1)
        return '*';
    if (first < PLANET_PROD_MAX_ETA)
        return '#';
    return '!';
}

static void production_decorate_line(StarsU8 *line, StarsU32 packed)
{
    StarsU16 kind = production_item_kind(packed);
    StarsU16 item = production_item_code(packed);

    /* The second byte is a display attribute, not another text character.
     * The original increments it after _WSPRINTF and the renderer consumes
     * the resulting bit flags in DrawProductionItem. */
    if (kind == PLANET_PROD_SHIP_KIND) {
        if (item < PLANET_PROD_SPECIAL_ITEM_LIMIT) {
            line[1] = (StarsU8)(line[1] + 2U);
            if (item == 3U)
                line[5] = '*';
        }
        if (item == 0x0cU || item == 4U || item == 5U)
            line[1] = (StarsU8)(line[1] + 1U);
    }
}

/* NE 000a:6692-6aca. Fill the planet production list or format the first
 * production entry for a direct caller.  The original routine combines the
 * production record with localized text and a Windows listbox; those services
 * are explicit here so the recovered logic is ordinary, testable C. */
void stars_fill_planet_production_list(const StarsFillPlanetProdInput *input)
{
    const StarsFillPlanetProdQueue *queue;
    const StarsU32 *items;
    StarsU16 item_count;
    StarsU16 hwnd;
    StarsI16 listbox_mode;
    StarsU16 i;
    StarsU8 empty_text[128];
    StarsU8 name[128];
    StarsU8 line[128];
    StarsI16 first;
    StarsI16 last;
    StarsU8 marker;
    StarsU32 packed;

    if (!input || !input->work || input->work_capacity == 0U)
        return;
    listbox_mode = input->queue == 0;
    hwnd = input->hwnd != 0U ? input->hwnd : input->default_hwnd;
    queue = listbox_mode ? input->selected_queue : input->queue;
    if (listbox_mode && input->send_message)
        input->send_message(input->context, hwnd, PLANET_PROD_CLEAR, 0);

    if (!queue || queue->item_count == 0U) {
        if (input->get_string)
            input->get_string(input->context, PLANET_PROD_NONE_STRING,
                empty_text, sizeof empty_text);
        else
            empty_text[0] = 0;
        if (listbox_mode) {
            if (input->send_message)
                input->send_message(input->context, hwnd, PLANET_PROD_ADD,
                    empty_text);
        } else if (input->get_string) {
            strncpy((char *)input->work, (const char *)empty_text,
                input->work_capacity - 1U);
            input->work[input->work_capacity - 1U] = 0;
        }
        return;
    }

    items = queue->items;
    item_count = queue->item_count;
    for (i = 0; i < item_count; ++i) {
        packed = items[i];
        if (input->name_item)
            input->name_item(input->context, packed, name, sizeof name);
        else
            name[0] = 0;
        first = 0;
        last = 0;
        if (input->estimate_schedule)
            input->estimate_schedule(input->context, input->planet, items,
                i, &first, &last);
        marker = production_status(first, last, packed);
        if (marker == '&' && !listbox_mode)
            continue;
        if (input->format_line)
            input->format_line(input->context, marker,
                production_item_count(packed), name, line, sizeof line);
        else {
            line[0] = marker;
            line[1] = ' ';
            strncpy((char *)line + 2, (const char *)name,
                sizeof line - 3U);
            line[sizeof line - 1U] = 0;
        }
        production_decorate_line(line, packed);
        if (listbox_mode) {
            if (input->send_message)
                input->send_message(input->context, hwnd, PLANET_PROD_ADD,
                    line);
        } else {
            strncpy((char *)input->work, (const char *)line,
                input->work_capacity - 1U);
            input->work[input->work_capacity - 1U] = 0;
            return;
        }
    }
    if (!listbox_mode && input->get_string)
        input->get_string(input->context, PLANET_PROD_NONE_STRING,
            input->work, input->work_capacity);
}
