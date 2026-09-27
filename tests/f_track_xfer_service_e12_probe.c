#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ServiceContext {
    StarsCargoState states[2];
    StarsTransferEndpoint endpoints[2];
    StarsI16 captures;
    StarsI16 moves;
    StarsI16 supplies;
    StarsI16 tracks;
    StarsI16 draws;
    StarsI16 updates;
    StarsI32 last_delta;
    StarsI16 move_x;
} ServiceContext;

static void clear_state(StarsCargoState *state, StarsU32 amount)
{
    StarsI16 index;
    for (index = 0; index < 5; ++index)
        state->minerals[index] = 0UL;
    for (index = 0; index < 3; ++index)
        state->cargo[index] = 0U;
    state->thing_capacity = 0U;
    state->fleet_flags = 7U;
    state->owner = 1U;
    for (index = 0; index < 16; ++index)
        state->ship_counts[index] = 0U;
    state->minerals[2] = amount;
}

static void reset(ServiceContext *context, StarsU32 first, StarsU32 second)
{
    clear_state(&context->states[0], first);
    clear_state(&context->states[1], second);
    context->endpoints[0].group = 1;
    context->endpoints[0].id = 10U;
    context->endpoints[0].object = &context->states[0];
    context->endpoints[1].group = 1;
    context->endpoints[1].id = 20U;
    context->endpoints[1].object = &context->states[1];
    context->captures = 0;
    context->moves = 0;
    context->supplies = 0;
    context->tracks = 0;
    context->draws = 0;
    context->updates = 0;
    context->last_delta = 0L;
    context->move_x = 60;
}

static StarsI32 change(void *opaque, StarsI16 group, StarsU16 id,
    StarsI16 supply, StarsI32 amount, void *object)
{
    ServiceContext *context = (ServiceContext *)opaque;
    StarsCargoState *state = (StarsCargoState *)object;
    (void)context;
    (void)id;
    return stars_change_cargo(group, id, supply, amount, state, 0, 0);
}

static StarsI32 supply(void *opaque, StarsI16 kind, StarsI32 amount)
{
    ServiceContext *context = (ServiceContext *)opaque;
    ++context->supplies;
    context->last_delta = amount;
    return stars_xfer_supply(kind, amount, context->endpoints, change,
        context);
}

static StarsI16 hit(void *opaque, StarsI16 button, const StarsPoint *point)
{
    (void)opaque;
    return button == point->x;
}

static void capture(void *opaque, StarsU16 hwnd)
{
    (void)hwnd;
    ++((ServiceContext *)opaque)->captures;
}

static StarsI16 move(void *opaque, StarsPoint *point)
{
    ServiceContext *context = (ServiceContext *)opaque;
    ++context->moves;
    if (context->moves == 1) {
        point->x = context->move_x;
        point->y = 0;
        return 1;
    }
    return 0;
}

static StarsI32 quantity(void *opaque, StarsI16 kind, StarsI16 side)
{
    ServiceContext *context = (ServiceContext *)opaque;
    (void)kind;
    (void)side;
    return stars_change_cargo(1, 20U, 2, 0L,
        &context->states[1], 0, 0);
}

static StarsI32 current(void *opaque, StarsI16 kind, StarsI32 ignored)
{
    ServiceContext *context = (ServiceContext *)opaque;
    (void)kind;
    (void)ignored;
    return stars_change_cargo(1, 10U, 2, 0L,
        &context->states[0], 0, 0);
}

static StarsI16 track_button(void *opaque, StarsI16 button)
{
    ServiceContext *context = (ServiceContext *)opaque;
    (void)button;
    ++context->tracks;
    return context->tracks == 1;
}

static void draw(void *opaque, StarsU16 hdc, StarsI16 kind)
{
    (void)hdc;
    (void)kind;
    ++((ServiceContext *)opaque)->draws;
}

static void update(void *opaque)
{
    ++((ServiceContext *)opaque)->updates;
}

static void install(StarsTrackXferOps *ops, ServiceContext *context)
{
    memset(ops, 0, sizeof *ops);
    ops->hit_test = hit;
    ops->set_capture = capture;
    ops->release_capture = capture;
    ops->mouse_move = move;
    ops->quantity = quantity;
    ops->change_cargo = current;
    ops->supply = supply;
    ops->track_button = track_button;
    ops->draw_dialog = draw;
    ops->update_buttons = update;
    ops->context = context;
}

static StarsI16 expect(StarsI16 condition)
{
    return condition ? 0 : 1;
}

static void setup_input(StarsTrackXferInput *input,
    const StarsTrackXferOps *ops)
{
    input->hwnd = 1U;
    input->hdc = 2U;
    input->point.x = 0;
    input->point.y = 0;
    input->button_count = 1;
    input->source_mode = 0;
    input->source_side = 0;
    input->cargo_kind = 2;
    input->owner_matches = 1;
    input->drag_rect.left = 10;
    input->drag_rect.top = 0;
    input->drag_rect.right = 110;
    input->drag_rect.bottom = 20;
    input->drag_rect_valid = 1;
    input->ops = ops;
}

int main(void)
{
    ServiceContext context;
    StarsTrackXferInput input;
    StarsTrackXferOps ops;
    StarsI16 failures;
    StarsI32 conserved;

    reset(&context, 10UL, 100UL);
    install(&ops, &context);
    setup_input(&input, &ops);
    input.total_cargo = 100L;
    input.other_cargo = 10L;
    if (stars_track_xfer(&input) != 1)
        return 1;
    conserved = (StarsI32)context.states[0].minerals[2] +
        (StarsI32)context.states[1].minerals[2];
    failures = 0;
    failures += expect(context.supplies == 1 && context.last_delta == 41L);
    failures += expect(context.states[0].minerals[2] == 51UL &&
        context.states[1].minerals[2] == 59UL);
    failures += expect(conserved == 110L && context.captures == 2 &&
        context.moves == 2 && context.draws == 2 && context.updates == 1);

    reset(&context, 10UL, 100UL);
    install(&ops, &context);
    setup_input(&input, &ops);
    input.source_side = 1;
    input.total_cargo = 100L;
    input.other_cargo = 10L;
    stars_track_xfer(&input);
    failures += expect(context.last_delta == -41L &&
        context.states[0].minerals[2] == 0UL &&
        context.states[1].minerals[2] == 110UL);
    failures += expect(context.supplies == 1 && context.captures == 2);

    reset(&context, 20UL, 5UL);
    install(&ops, &context);
    memset(&input, 0, sizeof input);
    input.point.x = 0;
    input.button_count = 1;
    input.source_mode = 1;
    input.cargo_kind = 2;
    input.fkb = 4;
    input.md_xfer_dialog = 1;
    input.other_cargo = 15L;
    input.ops = &ops;
    stars_track_xfer(&input);
    failures += expect(context.tracks == 2 && context.last_delta == 10L);
    failures += expect(context.states[0].minerals[2] == 25UL &&
        context.states[1].minerals[2] == 0UL && context.supplies == 1);
    failures += expect(context.draws == 2 && context.updates == 1);

#ifndef TARGET_NATIVE
    printf("F_TRACK_XFER_SERVICE_E12_CHECKS=12 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
