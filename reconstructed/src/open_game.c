/* Semantic reconstruction of FOpenGame. */
#include <string.h>
#include "core.h"

static char *last_separator(char *text)
{
    char *slash;
    char *backslash;

    slash = strrchr(text, '/');
    backslash = strrchr(text, '\\');
    if (!slash) return backslash;
    if (!backslash) return slash;
    return slash > backslash ? slash : backslash;
}

static void reset_selection(StarsOpenGameState *state)
{
    state->selection_object = 0;
    state->selection_waypoint = -1;
    state->selection_id = -1;
}

/* NE 5:58F4. File dialog and persistence services remain callbacks; path
 * handling, startup mode cleanup, and all success/failure dispatch are C. */
StarsI16 stars_open_game(StarsOpenGameState *state, StarsU16 hwnd,
    StarsI16 race_only, const StarsOpenGameOps *ops, void *context)
{
    StarsI16 result;
    StarsI16 check_password;
    StarsI16 prefix_length;
    char *separator;
    char *filename;
    char filter[512];

    if (!state || !ops) return 0;
    state->selected_path[0] = 0;
    if ((state->ini_flags & 1U) == 0U) {
        if (ops->open_dialog) {
            filter[0] = 0;
            check_password = ops->open_dialog(context, hwnd,
                (StarsU8 *)filter, (StarsU16)(race_only ? 0x317U : 0x318U));
            (void)check_password;
            if (state->selected_path[0] == 0) return 0;
        } else return 0;
        prefix_length = 0;
    } else {
        strcpy(state->selected_path, state->startup_path);
        separator = last_separator(state->base_path);
        if (separator) *separator = 0;
        filename = last_separator(state->selected_path);
        if (!filename) {
            if (ops->set_work_from_date) ops->set_work_from_date(context, 2, -1);
            strcpy(state->selected_path, state->startup_path);
            filename = last_separator(state->selected_path);
        }
        prefix_length = filename ?
            (StarsI16)(filename - state->selected_path + 1) : 0;
        state->file_error_silent = 1;
    }
    result = ops->was_race_file ? ops->was_race_file(context,
        (const StarsU8 *)(state->selected_path + prefix_length),
        (StarsI16)(race_only == 0)) : 0;
    if (race_only != 0) {
        if (result > 0)
            strcpy(state->race_path, state->selected_path + prefix_length);
        return result;
    }
    if (result == 0) {
        if (prefix_length > 0)
            state->selected_path[prefix_length - 1] = 0;
        if (ops->destroy_game) ops->destroy_game(context);
        strcpy(state->base_path, state->selected_path);
        filename = state->selected_path + prefix_length;
        result = ops->load_game ? ops->load_game(context,
            (const StarsU8 *)state->selected_path,
            (const StarsU8 *)filename) : 0;
        if (result == 0) {
            if (state->ini_flags & 1U) {
                state->ini_flags = (StarsU16)(state->ini_flags & 0xfffeU);
                state->file_error_silent = 0;
            }
            return 0;
        }
        if (state->ini_flags & 1U) {
            state->file_error_silent = 0;
            state->ini_flags = (StarsU16)(state->ini_flags & 0xfffeU);
            separator = last_separator(state->selected_path);
            if (separator) {
                *separator = 0;
                strcpy(state->startup_path, state->selected_path);
            }
            if (state->player == -1 && (state->ini_flags & 8U) == 0U)
                state->global_flags = (StarsU16)(state->global_flags | 4U);
            if (state->game_lid != state->ini_lid ||
                state->ini_turn < state->game_turn)
                state->ini_flags = (StarsU16)(state->ini_flags & 0xffefU);
        }
        reset_selection(state);
        state->orders_visible = 0;
        if (ops->create_children) ops->create_children(context);
        if (state->player == -1) {
            if (state->title != 0U && state->freeing_title == 0U) {
                state->freeing_title = 1U;
                if (ops->destroy_window)
                    ops->destroy_window(context, state->title);
                state->title = 0U;
            }
            if (ops->bring_up_host) ops->bring_up_host(context);
            return 0;
        }
        if (ops->send_message)
            ops->send_message(context, state->frame, 0x111U, 0xfa1U, 0UL);
        state->ini_flags = (StarsU16)(state->ini_flags & 0xfe1fU);
        if (state->planet_count != 0U && ops->find_select)
            ops->find_select(context);
        return 1;
    }
    if ((state->ini_flags & 1U) && state->serial_number == 0UL)
        result = -1;
    state->file_error_silent = 0;
    state->ini_flags = (StarsU16)(state->ini_flags & 0xfffeU);
    if (result != -1) {
        if (ops->race_wizard) ops->race_wizard(context, hwnd);
        result = 0;
    }
    return result;
}
