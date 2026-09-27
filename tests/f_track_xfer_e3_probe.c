#include "core.h"
#include <stdio.h>
#include <string.h>

#define RECORD_BYTES 128
#define RECORD_ID 0
#define RECORD_GROUP 2
#define FLEET_MINERAL_BASE 88

#define MSG_MOUSEMOVE 0x0200
#define MSG_LBUTTONUP 0x0202
#define MSG_LBUTTONDOWN 0x0201

typedef struct Event {
    StarsU16 message;
    StarsI16 x;
    StarsI16 y;
} Event;

typedef struct E3Context {
    StarsU8 records[2][RECORD_BYTES];
    StarsCargoState states[2];
    StarsTransferEndpoint endpoints[2];
    StarsChgCargoOps cargo_ops;
    Event events[4];
    StarsI16 event_count;
    StarsI16 event_index;
    StarsU16 window;
    StarsI16 captures;
    StarsI16 moves;
    StarsI16 tracks;
    StarsI16 supplies;
    StarsI16 direct_moves;
    StarsI16 draws;
    StarsI16 updates;
    StarsI16 lookups;
    StarsI16 stores;
    StarsI16 last_draw_kind;
    StarsI32 last_delta;
} E3Context;

static void put16(StarsU8 *record, StarsI16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[offset + 1] = (StarsU8)(value >> 8);
}

static StarsU16 get16(const StarsU8 *record, StarsI16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[offset + 1] << 8));
}

static void put32(StarsU8 *record, StarsI16 offset, StarsU32 value)
{
    put16(record, offset, (StarsU16)value);
    put16(record, (StarsI16)(offset + 2), (StarsU16)(value >> 16));
}

static StarsU32 get32(const StarsU8 *record, StarsI16 offset)
{
    return (StarsU32)get16(record, offset) |
        ((StarsU32)get16(record, (StarsI16)(offset + 2)) << 16);
}

static void clear_state(StarsCargoState *state, StarsU32 amount)
{
    StarsI16 i;

    memset(state, 0, sizeof *state);
    state->fleet_flags = 7U;
    state->owner = 1U;
    state->minerals[2] = amount;
    for (i = 0; i < 16; ++i)
        state->ship_counts[i] = 0U;
}

static void sync_record(E3Context *context, StarsI16 index)
{
    StarsI16 i;

    for (i = 0; i < 5; ++i)
        put32(context->records[index],
            (StarsI16)(FLEET_MINERAL_BASE + i * 4),
            context->states[index].minerals[i]);
    put16(context->records[index], 6, context->states[index].owner);
}

static void load_record(E3Context *context, StarsI16 index)
{
    StarsI16 i;

    for (i = 0; i < 5; ++i)
        context->states[index].minerals[i] = get32(context->records[index],
            (StarsI16)(FLEET_MINERAL_BASE + i * 4));
    context->states[index].owner = get16(context->records[index], 6);
    context->states[index].fleet_flags = 7U;
}

static void reset(E3Context *context, StarsU32 first, StarsU32 second)
{
    StarsU16 window;

    window = context->window;
    memset(context, 0, sizeof *context);
    context->window = window;
    clear_state(&context->states[0], first);
    clear_state(&context->states[1], second);
    put16(context->records[0], RECORD_ID, 17U);
    put16(context->records[0], RECORD_GROUP, 7U);
    put16(context->records[1], RECORD_ID, 23U);
    put16(context->records[1], RECORD_GROUP, 7U);
    sync_record(context, 0);
    sync_record(context, 1);
    load_record(context, 0);
    load_record(context, 1);
    context->endpoints[0].group = 7;
    context->endpoints[0].id = 17U;
    context->endpoints[1].group = 7;
    context->endpoints[1].id = 23U;
}

static StarsI16 find_record(E3Context *context, StarsI16 group, StarsU16 id)
{
    StarsI16 i;

    for (i = 0; i < 2; ++i)
        if ((StarsI16)get16(context->records[i], RECORD_GROUP) == group &&
                get16(context->records[i], RECORD_ID) == id)
            return i;
    return -1;
}

static StarsCargoState *lookup(void *opaque, StarsI16 group, StarsU16 id)
{
    E3Context *context;
    StarsI16 index;

    context = (E3Context *)opaque;
    ++context->lookups;
    index = find_record(context, group, id);
    if (index < 0)
        return (StarsCargoState *)0;
    load_record(context, index);
    return &context->states[index];
}

static void store(void *opaque, StarsI16 group, StarsU16 id,
    const StarsCargoState *state)
{
    E3Context *context;
    StarsI16 index;

    context = (E3Context *)opaque;
    index = find_record(context, group, id);
    if (index < 0)
        return;
    context->states[index] = *state;
    sync_record(context, index);
    ++context->stores;
}

static StarsU32 cargo_free(void *opaque, const StarsCargoState *state)
{
    (void)opaque;
    (void)state;
    return 200UL;
}

static StarsI32 change(void *opaque, StarsI16 group, StarsU16 id,
    StarsI16 supply, StarsI32 amount, void *object)
{
    E3Context *context = (E3Context *)opaque;
    (void)object;
    return stars_change_cargo(group, id, supply, amount, (StarsCargoState *)0,
        &context->cargo_ops, context);
}

static StarsI32 supply(void *opaque, StarsI16 kind, StarsI32 amount)
{
    E3Context *context = (E3Context *)opaque;
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
    E3Context *context = (E3Context *)opaque;
#ifdef TARGET_NATIVE
    SetCapture((HWND)hwnd);
#else
    (void)hwnd;
#endif
    ++context->captures;
}

static void release_capture(void *opaque, StarsU16 hwnd)
{
    E3Context *context = (E3Context *)opaque;
#ifdef TARGET_NATIVE
    ReleaseCapture();
#else
    (void)hwnd;
#endif
    ++context->captures;
}

static StarsI16 next_event(E3Context *context, StarsU16 *message,
    StarsI16 *x, StarsI16 *y)
{
#ifdef TARGET_NATIVE
    MSG msg;

    while (PeekMessage(&msg, (HWND)0, 0U, 0xffffU, PM_REMOVE)) {
        if (msg.message == WM_MOUSEMOVE || msg.message == WM_LBUTTONUP ||
                msg.message == WM_LBUTTONDOWN) {
            *message = (StarsU16)msg.message;
            *x = (StarsI16)LOWORD(msg.lParam);
            *y = (StarsI16)HIWORD(msg.lParam);
            return 1;
        }
    }
    return 0;
#else
    Event *event;

    if (context->event_index >= context->event_count)
        return 0;
    event = &context->events[context->event_index++];
    *message = event->message;
    *x = event->x;
    *y = event->y;
    return 1;
#endif
}

static StarsI16 move(void *opaque, StarsPoint *point)
{
    E3Context *context = (E3Context *)opaque;
    StarsU16 message;
    StarsI16 x;
    StarsI16 y;

    ++context->moves;
    if (!next_event(context, &message, &x, &y))
        return 0;
    point->x = x;
    point->y = y;
    return message == MSG_MOUSEMOVE;
}

static StarsI32 quantity(void *opaque, StarsI16 kind, StarsI16 side)
{
    E3Context *context = (E3Context *)opaque;
    (void)kind;
    (void)side;
    return stars_change_cargo(7, 23U, 2, 0L, (StarsCargoState *)0,
        &context->cargo_ops, context);
}

static StarsI32 current(void *opaque, StarsI16 kind, StarsI32 ignored)
{
    E3Context *context = (E3Context *)opaque;
    (void)kind;
    (void)ignored;
    return stars_change_cargo(7, 17U, 2, 0L, (StarsCargoState *)0,
        &context->cargo_ops, context);
}

static StarsI16 track_button(void *opaque, StarsI16 button)
{
    E3Context *context = (E3Context *)opaque;
    StarsU16 message;
    StarsI16 x;
    StarsI16 y;
    (void)button;

    ++context->tracks;
    if (!next_event(context, &message, &x, &y))
        return 0;
    return message == MSG_LBUTTONDOWN;
}

static StarsI32 dialog_transfer(void *opaque, StarsI16 kind,
    StarsI32 amount)
{
    E3Context *context = (E3Context *)opaque;
    StarsI32 accepted;
    (void)kind;

    ++context->direct_moves;
    accepted = amount;
    if (accepted > (StarsI32)context->states[1].minerals[2])
        accepted = (StarsI32)context->states[1].minerals[2];
    if (accepted > 0x7ffeL - (StarsI32)context->states[0].minerals[2])
        accepted = 1L;
    context->states[0].minerals[2] += (StarsU32)accepted;
    context->states[1].minerals[2] -= (StarsU32)accepted;
    sync_record(context, 0);
    sync_record(context, 1);
    return accepted;
}

static void draw(void *opaque, StarsU16 hdc, StarsI16 kind)
{
    E3Context *context = (E3Context *)opaque;
    (void)hdc;
    context->last_draw_kind = kind;
    ++context->draws;
}

static void update(void *opaque)
{
    E3Context *context = (E3Context *)opaque;
#ifdef TARGET_NATIVE
    EnableWindow((HWND)context->window, TRUE);
#endif
    ++context->updates;
}

static void install(StarsTrackXferOps *ops, E3Context *context)
{
    memset(ops, 0, sizeof *ops);
    context->cargo_ops.lookup = lookup;
    context->cargo_ops.store = store;
    context->cargo_ops.cargo_free = cargo_free;
    context->cargo_ops.fuel_free = cargo_free;
    ops->hit_test = hit;
    ops->set_capture = capture;
    ops->release_capture = release_capture;
    ops->mouse_move = move;
    ops->quantity = quantity;
    ops->change_cargo = current;
    ops->supply = supply;
    ops->dialog_transfer = dialog_transfer;
    ops->track_button = track_button;
    ops->draw_dialog = draw;
    ops->update_buttons = update;
    ops->context = context;
}

static void post_drag(E3Context *context)
{
#ifdef TARGET_NATIVE
    PostAppMessage(GetCurrentTask(), WM_MOUSEMOVE, 0U,
        MAKELONG(60, 0));
    PostAppMessage(GetCurrentTask(), WM_LBUTTONUP, 0U,
        MAKELONG(60, 0));
#else
    context->event_count = 2;
    context->event_index = 0;
    context->events[0].message = MSG_MOUSEMOVE;
    context->events[0].x = 60;
    context->events[0].y = 0;
    context->events[1].message = MSG_LBUTTONUP;
    context->events[1].x = 60;
    context->events[1].y = 0;
#endif
}

static void post_button(E3Context *context)
{
#ifdef TARGET_NATIVE
    PostAppMessage(GetCurrentTask(), WM_LBUTTONDOWN, 0U,
        MAKELONG(0, 0));
    PostAppMessage(GetCurrentTask(), WM_LBUTTONUP, 0U,
        MAKELONG(0, 0));
#else
    context->event_count = 2;
    context->event_index = 0;
    context->events[0].message = MSG_LBUTTONDOWN;
    context->events[0].x = 0;
    context->events[0].y = 0;
    context->events[1].message = MSG_LBUTTONUP;
    context->events[1].x = 0;
    context->events[1].y = 0;
#endif
}

static void setup_input(StarsTrackXferInput *input,
    const StarsTrackXferOps *ops, E3Context *context)
{
    memset(input, 0, sizeof *input);
    input->hwnd = context->window;
    input->hdc = 2U;
    input->point.x = 0;
    input->button_count = 1;
    input->cargo_kind = 2;
    input->owner_matches = 1;
    input->drag_rect.left = 10;
    input->drag_rect.right = 110;
    input->drag_rect_valid = 1;
    input->ops = ops;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    E3Context context;
    StarsTrackXferInput input;
    StarsTrackXferOps ops;
    StarsI16 checks;
    StarsI16 failures;

    memset(&context, 0, sizeof context);
#ifdef TARGET_NATIVE
    context.window = (StarsU16)GetDesktopWindow();
    if (!context.window) {
        FILE *debug;
        debug = fopen("C:\\FTXE3\\FTXE3D.TXT", "a");
        if (debug) {
            fputs("CREATE_WINDOW_FAILED\n", debug);
            fclose(debug);
        }
        return 2;
    }
#else
    context.window = 1U;
#endif
    checks = 0;
    failures = 0;

    reset(&context, 10UL, 100UL);
    install(&ops, &context);
    setup_input(&input, &ops, &context);
    input.total_cargo = 100L;
    input.other_cargo = 10L;
    post_drag(&context);
    failures += check(stars_track_xfer(&input) == 1, &checks);
    failures += check(get32(context.records[0], FLEET_MINERAL_BASE + 8) ==
        51UL && get32(context.records[1], FLEET_MINERAL_BASE + 8) == 59UL,
        &checks);
    failures += check(context.lookups == 5 && context.stores == 2 &&
        context.supplies == 1 && context.captures == 2 &&
        context.moves == 2 && context.draws == 2 && context.updates == 1,
        &checks);
#ifdef TARGET_NATIVE
    {
        FILE *debug;
        debug = fopen("C:\\FTXE3\\FTXE3D.TXT", "w");
        if (debug) {
            fprintf(debug,
                "D1=%lu,%lu L=%d S=%d Q=%d C=%d M=%d D=%d U=%d\\n",
                (unsigned long)get32(context.records[0], FLEET_MINERAL_BASE + 8),
                (unsigned long)get32(context.records[1], FLEET_MINERAL_BASE + 8),
                (int)context.lookups, (int)context.stores,
                (int)context.supplies, (int)context.captures,
                (int)context.moves, (int)context.draws, (int)context.updates);
            fclose(debug);
        }
    }
#endif

    reset(&context, 20UL, 10UL);
    install(&ops, &context);
    setup_input(&input, &ops, &context);
    input.source_mode = 1;
    input.md_xfer_dialog = 1;
    input.fkb = 4;
    input.button_index = 3;
    input.other_cargo = 7L;
    post_button(&context);
    failures += check(stars_track_xfer(&input) == 1, &checks);
    failures += check(get32(context.records[0], FLEET_MINERAL_BASE + 8) ==
        27UL && get32(context.records[1], FLEET_MINERAL_BASE + 8) == 3UL,
        &checks);
    failures += check(context.direct_moves == 1 && context.supplies == 0 &&
        context.tracks == 2 && context.draws == 2 && context.updates == 1,
        &checks);
#ifdef TARGET_NATIVE
    {
        FILE *debug;
        debug = fopen("C:\\FTXE3\\FTXE3D.TXT", "a");
        if (debug) {
            fprintf(debug,
                "D2=%lu,%lu X=%d Q=%d T=%d D=%d U=%d FAIL=%d\\n",
                (unsigned long)get32(context.records[0], FLEET_MINERAL_BASE + 8),
                (unsigned long)get32(context.records[1], FLEET_MINERAL_BASE + 8),
                (int)context.direct_moves, (int)context.supplies,
                (int)context.tracks, (int)context.draws,
                (int)context.updates, (int)failures);
            fclose(debug);
        }
    }
#endif

    reset(&context, 10UL, 100UL);
    install(&ops, &context);
    setup_input(&input, &ops, &context);
    input.owner_matches = 0;
    post_drag(&context);
    failures += check(stars_track_xfer(&input) == 1, &checks);
    failures += check(get32(context.records[0], FLEET_MINERAL_BASE + 8) ==
        10UL && get32(context.records[1], FLEET_MINERAL_BASE + 8) == 100UL &&
        context.captures == 0 && context.supplies == 0, &checks);

#ifdef TARGET_NATIVE
    {
        FILE *debug;
        debug = fopen("C:\\FTXE3\\FTXE3D.TXT", "a");
        if (debug) {
            fprintf(debug,
                "D3=%lu,%lu C=%d Q=%d FAIL=%d\\n",
                (unsigned long)get32(context.records[0],
                    FLEET_MINERAL_BASE + 8),
                (unsigned long)get32(context.records[1],
                    FLEET_MINERAL_BASE + 8), (int)context.captures,
                (int)context.supplies, (int)failures);
            fclose(debug);
        }
    }
    /* The desktop window is owned by Win98; leave it untouched. */
#else
    printf("F_TRACK_XFER_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
