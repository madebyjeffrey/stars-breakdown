#include "core.h"
#include <string.h>

#define BUILD_DD_RESET 0x40b
#define BUILD_DD_ADD 0x403
#define BUILD_DD_COUNT 0x406
#define BUILD_DD_END 0x40e
#define BUILD_DD_DELETE 0x817
#define BUILD_DD_EDIT 0x818
#define BUILD_DD_MAX 32
#define SHDEF_SIZE 147
#define SHDEF_NAME_OFFSET 8
#define SHDEF_FLAGS_OFFSET 123
#define SHDEF_CEXIST_OFFSET 131
#define SHDEF_NAME_BYTES 32

static StarsU16 fill_build_raw_u16(const StarsU8 *record, StarsI16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static StarsU32 fill_build_raw_u32(const StarsU8 *record, StarsI16 offset)
{
    return (StarsU32)record[offset] |
        ((StarsU32)record[offset + 1] << 8) |
        ((StarsU32)record[offset + 2] << 16) |
        ((StarsU32)record[offset + 3] << 24);
}

static const StarsU8 *fill_build_packed_record(const StarsU8 *records,
    StarsI16 stride, StarsI16 index)
{
    if (!records || stride < SHDEF_SIZE || index < 0)
        return (const StarsU8 *)0;
    return records + (long)stride * (long)index;
}

static StarsI16 fill_build_read_design(const StarsFillBuildDesign *designs,
    const StarsU8 *packed, StarsI16 stride, StarsI16 index,
    StarsU8 *name, StarsI16 name_size, StarsI16 *has_hull,
    StarsI16 *has_components)
{
    const StarsU8 *record;
    StarsI16 length;
    StarsU32 existing;

    record = fill_build_packed_record(packed, stride, index);
    if (record) {
        if (fill_build_raw_u16(record, SHDEF_FLAGS_OFFSET) & 0x0200)
            return 0;
        length = 0;
        while (length < SHDEF_NAME_BYTES &&
                length < (StarsI16)(name_size - 1) &&
                record[SHDEF_NAME_OFFSET + length] != 0) {
            name[length] = record[SHDEF_NAME_OFFSET + length];
            ++length;
        }
        name[length] = 0;
        existing = fill_build_raw_u32(record, SHDEF_CEXIST_OFFSET);
        *has_hull = existing != 0;
        *has_components = existing != 0;
        return 1;
    }
    if (!designs || index < 0)
        return 0;
    if (!designs[index].visible)
        return 0;
    length = (StarsI16)strlen((const char *)designs[index].name);
    if (length > (StarsI16)(name_size - 1))
        length = (StarsI16)(name_size - 1);
    (void)memcpy(name, designs[index].name, (size_t)length);
    name[length] = 0;
    *has_hull = designs[index].has_hull;
    *has_components = designs[index].has_components;
    return 1;
}

static void build_dd_add(const StarsFillBuildInput *input,
    const StarsU8 *text)
{
    if (input->send_message)
        input->send_message(input->context, input->dropdown, BUILD_DD_ADD,
            0, text);
}

static void build_dd_disable_slot_buttons(const StarsFillBuildInput *input)
{
    StarsU16 item;

    if (!input->get_dialog_item || !input->enable_window)
        return;
    item = input->get_dialog_item(input->context, input->slot_dialog,
        BUILD_DD_DELETE);
    input->enable_window(input->context, item, 0);
    item = input->get_dialog_item(input->context, input->slot_dialog,
        BUILD_DD_EDIT);
    input->enable_window(input->context, item, 0);
}

static void build_dd_add_special_parts(const StarsFillBuildInput *input)
{
    StarsI16 index;
    StarsI16 count;
    StarsU8 name[80];

    count = input->starbase_mode ? 5 : 32;
    for (index = 0; index < count; ++index) {
        if (!input->part_name || input->part_name(input->context,
                input->starbase_mode, index, name, (StarsU16)sizeof name) == 0)
            continue;
        build_dd_add(input, name);
    }
}

static void build_dd_add_enemy_designs(const StarsFillBuildInput *input)
{
    StarsI16 player;
    StarsI16 design;
    StarsU8 player_name[48];
    StarsU8 name[64];
    StarsU8 combined[128];
    StarsI16 player_length;
    StarsI16 design_length;
    StarsI16 has_hull;
    StarsI16 has_components;

    if (!input->player_designs && !input->packed_player_designs)
        return;
    for (player = 0; player < input->player_count; ++player) {
        if (player == input->current_player)
            continue;
        for (design = 0; design < input->designs_per_player; ++design) {
            if (!fill_build_read_design(input->player_designs,
                    input->packed_player_designs,
                    input->packed_player_design_stride,
                    player * input->designs_per_player + design,
                    name, (StarsI16)sizeof name, &has_hull,
                    &has_components))
                continue;
            player_length = input->player_name ? input->player_name(
                input->context, player, player_name,
                (StarsU16)sizeof player_name) : 0;
            design_length = (StarsI16)strlen((const char *)name);
            if (player_length > (StarsI16)sizeof combined - 2)
                player_length = (StarsI16)sizeof combined - 2;
            (void)memcpy(combined, player_name, (size_t)player_length);
            combined[player_length] = ':';
            (void)memcpy(combined + player_length + 1, name,
                (size_t)design_length + 1U);
            build_dd_add(input, combined);
        }
    }
}

static void build_dd_add_labels(const StarsFillBuildInput *input)
{
    static const StarsU16 normal_ids[13] = {
        0x052, 0x054, 0x056, 0x058, 0x05a, 0x05c, 0x05e,
        0x060, 0x062, 0x064, 0x066, 0x068, 0x06a
    };
    static const StarsU16 starbase_ids[8] = {
        0x07c, 0x07e, 0x080, 0x082, 0x084, 0x086, 0x088, 0x08a
    };
    const StarsU16 *ids;
    StarsI16 count;
    StarsI16 index;
    const StarsU8 *text;

    ids = input->starbase_mode ? starbase_ids : normal_ids;
    count = input->starbase_mode ? 8 : 13;
    for (index = 0; index < count; ++index) {
        text = input->compressed_string ? input->compressed_string(
            input->context, ids[index]) : (const StarsU8 *)0;
        if (text)
            build_dd_add(input, text);
    }
}

void stars_fill_build_dropdown(const StarsFillBuildInput *input)
{
    StarsU16 item;
    StarsI16 max_designs;
    StarsI16 index;
    StarsI16 any_available;
    StarsI16 first_action_done;
    StarsI16 queued;
    StarsI16 has_hull;
    StarsI16 has_components;
    StarsU8 name[SHDEF_NAME_BYTES];
    StarsU32 count;
    StarsI16 row_height;
    StarsRect rect;

    if (!input)
        return;
    if (input->send_message)
        input->send_message(input->context, input->dropdown, BUILD_DD_RESET,
            0, (const StarsU8 *)0);
    if (input->mode != 0)
        build_dd_disable_slot_buttons(input);
    max_designs = input->starbase_mode ? 10 : 16;
    any_available = 0;
    for (index = 0; index < max_designs; ++index)
        if (fill_build_read_design(input->designs, input->packed_designs,
                input->packed_design_stride, index, name,
                (StarsI16)sizeof name, &has_hull, &has_components)) {
            any_available = 1;
            break;
        }
    item = input->get_dialog_item ? input->get_dialog_item(input->context,
        input->slot_dialog, 0x816) : 0;
    if (input->enable_window)
        input->enable_window(input->context, item,
            (StarsU16)((input->mode <= 2 && any_available) ? 1 : 0));
    if (input->mode == 1) {
        build_dd_add_special_parts(input);
    } else if (input->mode == 2) {
        build_dd_add_enemy_designs(input);
    } else if (input->mode == 3 || input->mode == 4) {
        build_dd_add_labels(input);
    } else if (input->designs || input->packed_designs) {
        first_action_done = 0;
        for (index = 0; index < max_designs; ++index) {
            if (!fill_build_read_design(input->designs, input->packed_designs,
                    input->packed_design_stride, index, name,
                    (StarsI16)sizeof name, &has_hull, &has_components))
                continue;
            build_dd_add(input, name);
            if (!first_action_done) {
                queued = 0;
                if (input->queued)
                    (void)input->queued(input->context, index, &queued);
                if (input->enable_window)
                    input->enable_window(input->context,
                        input->get_dialog_item(input->context,
                            input->slot_dialog, BUILD_DD_EDIT),
                        (StarsU16)(!has_hull && !has_components && !queued));
                if (input->enable_window)
                    input->enable_window(input->context,
                        input->get_dialog_item(input->context,
                            input->slot_dialog, BUILD_DD_DELETE), 1);
                first_action_done = 1;
            }
        }
    }
    count = input->list_count ? (StarsU32)input->list_count(input->context,
        input->dropdown) : 0U;
    if (count > BUILD_DD_MAX)
        count = BUILD_DD_MAX;
    if (input->send_message)
        input->send_message(input->context, input->dropdown, BUILD_DD_COUNT,
            0, (const StarsU8 *)0);
    if (input->get_window_rect && input->set_window_pos) {
        row_height = input->line_height;
        if (row_height < 15)
            --row_height;
        if (row_height < 0)
            row_height = 0;
        rect.left = rect.top = rect.right = rect.bottom = 0;
        input->get_window_rect(input->context, input->dropdown, &rect);
        input->set_window_pos(input->context, input->dropdown, 0, 0, 0,
            input->parent_width - rect.left,
            (StarsI16)((count + 1U) * row_height + 8 +
                (input->line_height > 14 ? 1 : 0)), 6);
    }
    if (input->send_message)
        input->send_message(input->context, input->dropdown, BUILD_DD_END,
            0, (const StarsU8 *)0);
}
