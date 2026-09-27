#include "core.h"

#define RELATIONS_WM_DESTROY 2U
#define RELATIONS_WM_PAINT 0x000fU
#define RELATIONS_WM_ERASE_BACKGROUND 0x0014U
#define RELATIONS_WM_CONTROL_COLOR 0x0019U
#define RELATIONS_WM_INIT_DIALOG 0x0110U
#define RELATIONS_WM_COMMAND 0x0111U
#define RELATIONS_ID_LIST 0x07d3U
#define RELATIONS_ID_FIRST 0x07d4U
#define RELATIONS_ID_LAST 0x07d6U
#define RELATIONS_ID_OK 2U
#define RELATIONS_ID_HELP 0x0076U
#define RELATIONS_RELATION_OFFSET 0x2cU
#define RELATIONS_DIALOG_RESULT_OFFSET 3

static StarsI16 relations_selected_player(StarsRelationsInput *input)
{
    StarsI16 selected;
    if (!input->ops || !input->ops->get_selection) return -1;
    selected = input->ops->get_selection(input->context);
    if (selected >= input->player) ++selected;
    return selected;
}

/* NE 31:0088-04CE. Dialog procedure for editing the current player's
 * relationship table.  The original stores each relationship as a compact
 * byte and excludes the current player from the list; this version keeps the
 * relationship bytes directly and makes the index adjustment explicit. */
StarsI16 stars_relations_dialog(StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsI32 lparam, StarsRelationsInput *input)
{
    StarsI16 selected;
    StarsI16 player;
    StarsU16 control;
    if (!input || !input->relation_values) return 0;
    input->hwnd = hwnd;
    if (message == RELATIONS_WM_DESTROY) {
        if (input->dirty) {
            if (input->ops && input->ops->log_change)
                input->ops->log_change(input->context,
                    (StarsI16)(input->scan_group & 0x000f));
            if (input->ops && input->ops->invalidate_scanner)
                input->ops->invalidate_scanner(input->context,
                    input->scanner_window);
        }
        return 0;
    }
    if (message == RELATIONS_WM_PAINT) {
        if (input->ops && input->ops->paint)
            input->ops->paint(input->context, hwnd);
        return 1;
    }
    if (message == RELATIONS_WM_ERASE_BACKGROUND) {
        if (input->ops && input->ops->fill_background)
            input->ops->fill_background(input->context, wparam);
        return 1;
    }
    if (message == RELATIONS_WM_CONTROL_COLOR) {
        control = (StarsU16)lparam;
        if (control != input->list_control) {
            if (input->ops && input->ops->fill_background)
                input->ops->fill_background(input->context, wparam);
            return (StarsI16)input->background_brush;
        }
        return 0;
    }
    if (message == RELATIONS_WM_INIT_DIALOG) {
        if (input->ops && input->ops->sticky_dialog)
            input->ops->sticky_dialog(input->context, hwnd, 1);
        if (input->ops && input->ops->set_radio) {
            player = input->player;
            control = (StarsU16)(RELATIONS_ID_FIRST +
                (StarsU16)(input->relation_values[player]));
            input->ops->set_radio(input->context, RELATIONS_ID_FIRST,
                RELATIONS_ID_LAST, control);
        }
        if (input->ops && input->ops->add_name)
            for (player = 0; player < input->player_count; ++player)
                if (player != input->player)
                    input->ops->add_name(input->context, player);
        if (input->ops && input->ops->select_row)
            input->ops->select_row(input->context, 0);
        input->dirty = 0;
        if (input->ops && input->ops->fill_background)
            input->ops->fill_background(input->context, wparam);
        return 1;
    }
    if (message != RELATIONS_WM_COMMAND) return 0;
    if (wparam == RELATIONS_ID_OK) {
        if (input->ops && input->ops->sticky_dialog)
            input->ops->sticky_dialog(input->context, hwnd, 0);
        selected = relations_selected_player(input);
        if (input->ops && input->ops->end_dialog)
            input->ops->end_dialog(input->context,
                (StarsI16)(selected + RELATIONS_DIALOG_RESULT_OFFSET));
        return 1;
    }
    if (wparam == RELATIONS_ID_LIST) {
        selected = relations_selected_player(input);
        if (selected >= 0 && selected < input->player_count &&
                input->ops && input->ops->set_radio) {
            control = (StarsU16)(RELATIONS_ID_FIRST +
                input->relation_values[selected]);
            input->ops->set_radio(input->context, RELATIONS_ID_FIRST,
                RELATIONS_ID_LAST, control);
        }
    } else if (wparam >= RELATIONS_ID_FIRST &&
            wparam <= RELATIONS_ID_LAST) {
        selected = relations_selected_player(input);
        if (selected >= 0 && selected < input->player_count) {
            input->relation_values[selected] = (StarsU8)(wparam -
                RELATIONS_ID_FIRST);
            input->dirty = 1;
        }
    } else if (wparam == RELATIONS_ID_HELP) {
        if (input->ops && input->ops->help)
            input->ops->help(input->context, input->help_file);
        return 1;
    }
    return 0;
}
