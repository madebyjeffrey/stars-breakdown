#include "core.h"
#include <stdio.h>
#include <string.h>

#define CMD_CLOSE 1
#define CMD_RESEARCH 0x8b
#define CMD_ADD 0x418
#define CMD_MOVE_UP 0x439

typedef struct ProbeState {
    int finished;
    int finish_value;
    int sticky;
    int ended;
    int help;
    int toggles;
    int fills;
    int selections;
    int initializes;
    int navigates;
    int draws;
    int invalidates;
} ProbeState;

static void probe_finish(void *context, StarsI16 value)
{
    ProbeState *state;
    state = (ProbeState *)context;
    state->finished += 1;
    state->finish_value = value;
}

static void probe_sticky(void *context, StarsU16 dialog)
{
    ProbeState *state;
    (void)dialog;
    state = (ProbeState *)context;
    state->sticky += 1;
}

static void probe_end(void *context, StarsU16 dialog, StarsI16 value)
{
    ProbeState *state;
    (void)dialog;
    (void)value;
    state = (ProbeState *)context;
    state->ended += 1;
}

static void probe_help(void *context, StarsU16 dialog, StarsU16 topic)
{
    ProbeState *state;
    (void)dialog;
    (void)topic;
    state = (ProbeState *)context;
    state->help += 1;
}

static void probe_toggle(void *context, StarsI16 value)
{
    ProbeState *state;
    (void)value;
    state = (ProbeState *)context;
    state->toggles += 1;
}

static void probe_fill(void *context, StarsU16 dialog,
    const StarsProductionList *queue)
{
    ProbeState *state;
    (void)dialog;
    (void)queue;
    state = (ProbeState *)context;
    state->fills += 1;
}

static void probe_select(void *context, StarsI16 index, StarsI16 value)
{
    ProbeState *state;
    (void)index;
    (void)value;
    state = (ProbeState *)context;
    state->selections += 1;
}

static void probe_initialize(void *context)
{
    ProbeState *state;
    state = (ProbeState *)context;
    state->initializes += 1;
}

static void probe_navigate(void *context, StarsI16 direction,
    StarsI16 value)
{
    ProbeState *state;
    (void)direction;
    (void)value;
    state = (ProbeState *)context;
    state->navigates += 1;
}

static void probe_draw(void *context, StarsU16 dialog)
{
    ProbeState *state;
    (void)dialog;
    state = (ProbeState *)context;
    state->draws += 1;
}

static void probe_invalidate(void *context, StarsU16 dialog)
{
    ProbeState *state;
    (void)dialog;
    state = (ProbeState *)context;
    state->invalidates += 1;
}

static StarsU32 probe_item(StarsU16 type, StarsU16 number, StarsU16 count)
{
    return ((StarsU32)type << 17) | ((StarsU32)number << 10) | count;
}

static void probe_require(int condition)
{
    if (!condition)
        puts("PROD_COMMAND_HANDLER_SOURCE=FAIL");
}

int main(void)
{
    StarsProductionList queue;
    StarsProductionList catalog;
    StarsProdCommandOps ops;
    StarsProdCommandInput input;
    ProbeState state;

    memset(&queue, 0, sizeof(queue));
    memset(&catalog, 0, sizeof(catalog));
    memset(&state, 0, sizeof(state));
    memset(&ops, 0, sizeof(ops));
    catalog.item_count = 1;
    catalog.items[0] = probe_item(4, 8, 2);
    ops.finish = probe_finish;
    ops.sticky = probe_sticky;
    ops.end_dialog = probe_end;
    ops.help = probe_help;
    ops.toggle_research = probe_toggle;
    ops.fill_queue = probe_fill;
    ops.select_item = probe_select;
    ops.initialize = probe_initialize;
    ops.navigate = probe_navigate;
    ops.draw = probe_draw;
    ops.invalidate = probe_invalidate;
    ops.context = &state;
    input.dialog = 7;
    input.message = 0;
    input.parameter = 0;
    input.selected_index = -1;
    input.shift_down = 0;
    input.control_down = 0;
    input.queue = &queue;
    input.catalog = &catalog;
    input.ops = &ops;

    queue.item_count = 1;
    queue.items[0] = probe_item(2, 3, 4);
    input.message = CMD_ADD;
    input.parameter = 0;
    input.selected_index = 0;
    stars_prod_command_handler(&input);
    probe_require(queue.item_count == 2 && state.selections == 1);

    input.message = CMD_ADD + 1;
    input.parameter = 1;
    stars_prod_command_handler(&input);
    probe_require((queue.items[0] & 0x3ffUL) == 3);

    input.message = CMD_MOVE_UP;
    input.selected_index = 1;
    stars_prod_command_handler(&input);
    probe_require(queue.items[0] == probe_item(2, 3, 4));

    input.message = CMD_RESEARCH;
    input.parameter = 1;
    stars_prod_command_handler(&input);
    probe_require(state.toggles == 1 && state.fills >= 4);

    input.message = CMD_CLOSE;
    stars_prod_command_handler(&input);
    probe_require(state.finished == 1 && state.finish_value == 1 &&
        state.sticky == 1 && state.ended == 1);

    if (state.finished != 1 || state.finish_value != 1 || state.sticky != 1 ||
            state.ended != 1 || state.toggles != 1 || state.fills < 4 ||
            state.draws < 4 || state.invalidates < 4)
        return 1;
    puts("PROD_COMMAND_HANDLER_SOURCE=PASS");
    return 0;
}
