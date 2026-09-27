#include "core.h"
#include <stdio.h>
#include <string.h>

/* Recovered _xfer fields used by the FTrackXfer boundary.  The probe keeps
 * the records byte-oriented so the test cannot accidentally use a host-sized
 * pointer or long when checking the Win16 layout. */
#define XFER_BYTES 128
#define XFER_ID 0
#define XFER_GROBJ 2
#define FLEET_IPLAYER 6
#define FLEET_CARGO 80
#define THING_TURN 20

typedef struct PackedTrackContext {
    unsigned char source[XFER_BYTES];
    unsigned char other[XFER_BYTES];
    unsigned char thing[XFER_BYTES];
    unsigned char *active;
    unsigned char *other_active;
    StarsI16 source_side;
    StarsI16 transfer_both;
    StarsI16 captures;
    StarsI16 moves;
    StarsI16 tracks;
    StarsI16 supplies;
    StarsI16 draws;
    StarsI16 updates;
    StarsI16 move_x;
    StarsI32 last_delta;
} PackedTrackContext;

static void put16(unsigned char *raw, int offset, unsigned short value)
{
    raw[offset] = (unsigned char)(value & 0xffU);
    raw[offset + 1] = (unsigned char)(value >> 8);
}

static unsigned short get16(const unsigned char *raw, int offset)
{
    return (unsigned short)(raw[offset] |
        ((unsigned short)raw[offset + 1] << 8));
}

static void put32(unsigned char *raw, int offset, unsigned long value)
{
    put16(raw, offset, (unsigned short)(value & 0xffffUL));
    put16(raw, offset + 2, (unsigned short)(value >> 16));
}

static StarsI32 get32(const unsigned char *raw, int offset)
{
    unsigned long low;
    unsigned long high;
    low = (unsigned long)get16(raw, offset);
    high = (unsigned long)get16(raw, offset + 2);
    return (StarsI32)((high << 16) | low);
}

static StarsI16 hit(void *context, StarsI16 button, const StarsPoint *point)
{
    (void)context;
    return button == point->x;
}

static void capture(void *context, StarsU16 hwnd)
{
    (void)hwnd;
    ++((PackedTrackContext *)context)->captures;
}

static StarsI16 move(void *context, StarsPoint *point)
{
    PackedTrackContext *packed;
    packed = (PackedTrackContext *)context;
    ++packed->moves;
    if (packed->moves == 1) {
        point->x = packed->move_x;
        return 1;
    }
    return 0;
}

static StarsI32 quantity(void *context, StarsI16 kind, StarsI16 side)
{
    PackedTrackContext *packed;
    (void)kind;
    (void)side;
    packed = (PackedTrackContext *)context;
    /* This is the packed fleet's available-stat fixture. */
    return get32(packed->active, FLEET_CARGO + 12);
}

static StarsI32 change_cargo(void *context, StarsI16 kind, StarsI32 delta)
{
    PackedTrackContext *packed;
    (void)kind;
    (void)delta;
    packed = (PackedTrackContext *)context;
    return get32(packed->active, FLEET_CARGO + 8);
}

static StarsI32 supply(void *context, StarsI16 kind, StarsI32 delta)
{
    PackedTrackContext *packed;
    StarsI32 current;
    StarsI32 other_current;
    (void)kind;
    packed = (PackedTrackContext *)context;
    ++packed->supplies;
    packed->last_delta = delta;
    current = get32(packed->active, FLEET_CARGO + 8);
    put32(packed->active, FLEET_CARGO + 8,
        (unsigned long)(current + delta));
    if (packed->transfer_both) {
        other_current = get32(packed->other_active, FLEET_CARGO + 8);
        put32(packed->other_active, FLEET_CARGO + 8,
            (unsigned long)(other_current - delta));
    }
    return 1;
}

static StarsI16 track_button(void *context, StarsI16 button)
{
    PackedTrackContext *packed;
    (void)button;
    packed = (PackedTrackContext *)context;
    ++packed->tracks;
    return packed->tracks == 1;
}

static void draw(void *context, StarsU16 hdc, StarsI16 kind)
{
    (void)hdc;
    (void)kind;
    ++((PackedTrackContext *)context)->draws;
}

static void update(void *context)
{
    ++((PackedTrackContext *)context)->updates;
}

static void install(StarsTrackXferOps *ops, PackedTrackContext *packed)
{
    memset(ops, 0, sizeof *ops);
    ops->hit_test = hit;
    ops->set_capture = capture;
    ops->release_capture = capture;
    ops->mouse_move = move;
    ops->quantity = quantity;
    ops->change_cargo = change_cargo;
    ops->supply = supply;
    ops->track_button = track_button;
    ops->draw_dialog = draw;
    ops->update_buttons = update;
    ops->context = packed;
}

static void reset_trace(PackedTrackContext *packed)
{
    packed->captures = 0;
    packed->moves = 0;
    packed->tracks = 0;
    packed->supplies = 0;
    packed->draws = 0;
    packed->updates = 0;
    packed->last_delta = 0;
}

int main(void)
{
    PackedTrackContext packed;
    StarsTrackXferInput input;
    StarsTrackXferOps ops;
    int checks;
    int failures;

    memset(&packed, 0, sizeof packed);
    memset(&input, 0, sizeof input);
    put16(packed.source, XFER_ID, 17);
    put16(packed.source, XFER_GROBJ, 7);
    put16(packed.source, FLEET_IPLAYER, 1);
    put32(packed.source, FLEET_CARGO + 8, 10);
    put32(packed.source, FLEET_CARGO + 12, 100);
    put16(packed.other, XFER_ID, 23);
    put16(packed.other, XFER_GROBJ, 7);
    put16(packed.other, FLEET_IPLAYER, 1);
    put32(packed.other, FLEET_CARGO + 8, 7);
    put16(packed.thing, XFER_ID, 31);
    put16(packed.thing, XFER_GROBJ, 8);
    put16(packed.thing, THING_TURN, 4);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof packed.source == XFER_BYTES &&
        sizeof packed.other == XFER_BYTES);
    CHECK(get16(packed.source, XFER_ID) == 17U &&
        get16(packed.source, XFER_GROBJ) == 7U);
    CHECK(get16(packed.other, XFER_ID) == 23U &&
        get16(packed.other, XFER_GROBJ) == 7U);
    CHECK(get16(packed.source, FLEET_IPLAYER) == 1U);
    CHECK(get16(packed.other, FLEET_IPLAYER) == 1U);
    CHECK(get32(packed.source, FLEET_CARGO + 8) == 10L &&
        get32(packed.source, FLEET_CARGO + 12) == 100L);
    CHECK(get16(packed.thing, THING_TURN) == 4U);

    reset_trace(&packed);
    packed.active = packed.source;
    packed.other_active = packed.other;
    packed.source_side = 0;
    packed.transfer_both = 0;
    packed.move_x = 60;
    install(&ops, &packed);
    input.point.x = 0;
    input.button_count = 1;
    input.source_mode = 0;
    input.cargo_kind = 2;
    input.owner_matches =
        get16(packed.source, FLEET_IPLAYER) == 1U;
    input.drag_rect.left = 10;
    input.drag_rect.right = 110;
    input.drag_rect_valid = 1;
    input.ops = &ops;
    input.total_cargo = get32(packed.source, FLEET_CARGO + 12);
    input.source_side = packed.source_side;
    if (stars_track_xfer(&input) != 1)
        ++failures;
    ++checks;
    CHECK(packed.captures == 2 && packed.moves == 2);
    CHECK(packed.supplies == 1 && packed.last_delta == 41);
    CHECK(get32(packed.source, FLEET_CARGO + 8) == 51L);
    CHECK(packed.draws == 2 && packed.updates == 1);

    reset_trace(&packed);
    input.owner_matches =
        get16(packed.other, FLEET_IPLAYER) == 1U &&
        get16(packed.source, FLEET_IPLAYER) == 2U;
    if (stars_track_xfer(&input) != 1)
        ++failures;
    ++checks;
    CHECK(packed.captures == 0 && packed.supplies == 0);

    reset_trace(&packed);
    input.object_kind = 8;
    input.cargo_kind = 3;
    input.owner_matches = 1;
    if (stars_track_xfer(&input) != 1)
        ++failures;
    ++checks;
    CHECK(packed.captures == 0 && packed.supplies == 0);

    reset_trace(&packed);
    packed.active = packed.source;
    packed.other_active = packed.other;
    packed.transfer_both = 1;
    memset(&input, 0, sizeof input);
    input.point.x = 0;
    input.button_count = 1;
    input.source_mode = 1;
    input.cargo_kind = 1;
    input.fkb = 4;
    input.md_xfer_dialog = 1;
    input.other_cargo = get32(packed.other, FLEET_CARGO + 8);
    input.ops = &ops;
    if (stars_track_xfer(&input) != 1)
        ++failures;
    ++checks;
    CHECK(packed.tracks == 2 && packed.supplies == 1);
    CHECK(packed.last_delta == 7 &&
        get32(packed.source, FLEET_CARGO + 8) == 58L);
    CHECK(get32(packed.other, FLEET_CARGO + 8) == 0L);
    CHECK(packed.updates == 1 && packed.draws == 2);
    CHECK(checks == 22);
    puts("F_TRACK_XFER_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
