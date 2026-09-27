#include "core.h"
#include <string.h>

#define START_TUTOR_ACTIVE 0x0800U
#define START_TUTOR_VISIBLE 0x4000U
#define START_TUTOR_TURN_DONE 0x0008U
#define START_TUTOR_FINISHED 0x0010U
#define START_TUTOR_PROC_FAILURE 0x001aU
#define START_TUTOR_WORLD_RESOURCE 0x09c6U
#define START_TUTOR_FRAME_COMMAND 0x0111U
#define START_TUTOR_TUTOR_COMMAND 0x0fa1U
#define START_TUTOR_BASE_STRING 0x01e6U
#define START_TUTOR_STARTUP_SUFFIX 0x148aU
#define START_TUTOR_GAME_SUFFIX 0x148eU
#define START_TUTOR_EXISTING_GAME 0x01e8U
#define START_TUTOR_TUTOR_MENU 0x0000U
#define START_TUTOR_SCAN_MASK 0x04e0U

static void start_tutor_show_and_invalidate(StarsStartTutorState *state,
    const StarsStartTutorOps *ops, void *context)
{
    if (ops->show_tutor)
        ops->show_tutor(context, 1);
    if (ops->invalidate_rect)
        ops->invalidate_rect(context, state->tutor_window, 1U);
}

/* NE 32:06B4-0A2F.  Start or resume the tutorial, including its saved-game
 * handoff, title-window transition, scan defaults, tutor-task positioning,
 * and final dialog visibility.  Global records are represented by one typed
 * state object; OS and game-service calls remain explicit callbacks. */
void stars_start_tutor(StarsStartTutorState *state, StarsI16 restart,
    const StarsStartTutorOps *ops, void *context)
{
    StarsI16 metric;
    StarsI16 length;
    StarsI16 open_result;
    StarsI16 task_done;

    if (!state || !ops)
        return;
    if ((state->gd_flags & START_TUTOR_ACTIVE) == 0U) {
        (void)memset(&state->misc_state, 0, sizeof(state->misc_state));
        state->tutor_flags = 0U;
        state->tutor_idt = 0;
        state->tutor_idt_bold = 0;
        state->tutor_idh = 0;
        state->tutor_ids_error = 0;
        state->tutor_window = 0U;
        if (state->tutor_proc == 0UL && ops->make_proc)
            state->tutor_proc = ops->make_proc(context, state->instance);
        if (state->tutor_proc == 0UL) {
            if (ops->alert_ids)
                (void)ops->alert_ids(context, START_TUTOR_PROC_FAILURE, 0x10);
            return;
        }
        state->gd_flags = (StarsU16)(state->gd_flags |
            START_TUTOR_ACTIVE);
        if (ops->save_game_state)
            ops->save_game_state(context);
        if (state->freeing_title == 0U) {
            state->freeing_title = 1U;
            if (ops->destroy_window)
                ops->destroy_window(context, state->title_window);
            state->title_window = 0U;
            if (ops->show_window)
                ops->show_window(context, state->frame_window, 5);
        }
        state->grbit_scan = START_TUTOR_SCAN_MASK;
        metric = ops->get_system_metric ?
            ops->get_system_metric(context, 0U) : 0;
        if (metric < 800)
            state->scan_zoom = 0;
        else if (metric < 0x0400)
            state->scan_zoom = 1;
        else if (metric < 0x0500)
            state->scan_zoom = 2;
        else
            state->scan_zoom = 3;

        if (state->game_lid == 0UL) {
            length = ops->get_string && state->base_name ?
                ops->get_string(context, START_TUTOR_BASE_STRING,
                    state->base_name, state->base_capacity) : 0;
            if (!restart && state->base_name && ops->append_string &&
                    ops->access_file) {
                ops->append_string(context, state->base_name,
                    START_TUTOR_STARTUP_SUFFIX);
                if (ops->access_file(context, state->base_name, 0) != -1) {
                    open_result = ops->alert_ids ? ops->alert_ids(context,
                        START_TUTOR_EXISTING_GAME, 0x2022) : 6;
                    if (open_result != 6) {
                        if ((StarsU16)length < state->base_capacity)
                            state->base_name[(StarsU16)length] = 0;
                        ops->append_string(context, state->base_name,
                            START_TUTOR_GAME_SUFFIX);
                        state->ini_flags = (StarsU16)(state->ini_flags | 1U);
                        open_result = ops->open_game ?
                            ops->open_game(context,
                                state->frame_window, 0) : 0;
                        if (open_result > 0 && ops->create_children)
                            ops->create_children(context);
                        state->ini_flags = (StarsU16)(state->ini_flags &
                            (StarsU16)~1U);
                    }
                }
                if ((StarsU16)length < state->base_capacity)
                    state->base_name[(StarsU16)length] = 0;
            }
            if (state->game_lid == 0UL && ops->create_world)
                ops->create_world(context);
            (void)memset(state->misc_state, 0, sizeof(state->misc_state));
            state->zip_valid = 1U;
            state->gd_bits2 = (StarsU16)(state->gd_bits2 | 0x20U);
            if (ops->initialize_menu)
                ops->initialize_menu(context, START_TUTOR_TUTOR_MENU);
            if (ops->post_message)
                ops->post_message(context, state->frame_window,
                    START_TUTOR_FRAME_COMMAND, START_TUTOR_TUTOR_COMMAND, 0);
            if (state->freeing_title == 0U) {
                state->freeing_title = 1U;
                if (ops->destroy_window)
                    ops->destroy_window(context, state->title_window);
                state->title_window = 0U;
            }
            if (ops->show_window)
                ops->show_window(context, state->frame_window, 5);
        }
        state->tutor_ids_error = -1;
        state->tutor_idt = 0;
        state->tutor_idt_bold = 0;
        state->tutor_flags = (StarsU16)((state->tutor_flags & 0xbbffU) |
            START_TUTOR_VISIBLE);
        do {
            task_done = ops->task_done ? ops->task_done(context) : 1;
            if (!task_done || (state->tutor_flags & START_TUTOR_TURN_DONE))
                break;
            state->tutor_idt = (StarsI16)(state->tutor_idt + 8);
        } while (1);
        if ((state->tutor_flags & START_TUTOR_FINISHED) != 0U) {
            if (ops->end_tutor)
                ops->end_tutor(context, 0);
            return;
        }
        if (state->tutor_window == 0U && ops->create_dialog)
            ops->create_dialog(context, state->instance,
                START_TUTOR_WORLD_RESOURCE, state->frame_window,
                state->tutor_proc);
        if (state->tutor_idt != 0) {
            if ((state->tutor_flags & START_TUTOR_TURN_DONE) == 0U)
                state->tutor_idt = (StarsI16)(state->tutor_idt - 8);
            state->tutor_idt_bold = state->tutor_idt;
            if (ops->advance_tutor)
                ops->advance_tutor(context);
        }
    }
    start_tutor_show_and_invalidate(state, ops, context);
}
