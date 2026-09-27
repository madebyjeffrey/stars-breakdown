/* Semantic reconstruction of VerifyTurns. */
#include "core.h"

static void release(void *memory, const StarsVerifyTurnsOps *ops,
    void *context)
{
    if (memory && ops->free_memory) ops->free_memory(context, memory, 6U);
}

/* NE 5:6686-6996. Log status codes are mapped to the original rgOut values;
 * allocations and file/log services are supplied by the host boundary. */
void stars_verify_turns(StarsVerifyTurnsState *state,
    const StarsVerifyTurnsOps *ops, void *context)
{
    void *coldrop;
    void *xferfull;
    void *plan_extra;
    void *turn_serial;
    StarsI16 saved_id;
    StarsU32 saved_tick;
    StarsU16 i;
    StarsI16 old_output;
    StarsI16 log_code;
    StarsI16 check_result;
    StarsI16 load_result;
    StarsI16 run_result;
    StarsI16 changed;

    if (!state || !ops || !ops->allocate) return;
    coldrop = ops->allocate(context, 12000UL, 6U);
    xferfull = ops->allocate(context, 25000UL, 6U);
    plan_extra = ops->allocate(context, (StarsU32)state->plan_count * 2UL,
        6U);
    turn_serial = ops->allocate(context,
        (StarsU32)state->player_count * 16UL, 6U);
    state->col_drop = 0U;
    state->xfer_full = 0U;
    state->message_index = 0U;
    saved_id = state->id_player;
    saved_tick = state->tick_last;
    for (i = 0U; i < state->player_count && i < 16U; ++i) {
        state->tick_last = saved_tick;
        old_output = state->output[i];
        log_code = 0;
        if (!(state->player_flags[i] & 0x0200U)) {
            check_result = ops->check_log ? ops->check_log(context,
                (StarsI16)i, &log_code) : 0;
            if (!check_result) {
                if (!log_code) {
                    if (state->player_dead[i]) state->output[i] = -1;
                    else state->output[i] =
                        (state->global_flags & 0x0080U) ? 2 : 1;
                } else if (log_code == 0x1c) state->output[i] = 4;
                else if (log_code == 0x1d) state->output[i] = 5;
                else state->output[i] = 3;
            } else {
                load_result = ops->load_log ? ops->load_log(context,
                    (StarsI16)i) : 0;
                if (load_result) {
                    run_result = ops->run_log ? ops->run_log(context) : 0;
                    state->output[i] = run_result ? 0 : 3;
                } else state->output[i] = 0;
            }
        } else state->output[i] = 0;
        changed = (state->output[i] != old_output);
        if ((saved_tick == 0UL || changed) && ops->get_tick)
            state->tick_last = ops->get_tick(context);
        saved_tick = state->tick_last;
    }
    release(plan_extra, ops, context);
    release(turn_serial, ops, context);
    release(coldrop, ops, context);
    release(xferfull, ops, context);
    state->id_player = saved_id;
}
