#include <stdio.h>
#include <string.h>
#include "core.h"

#define FLEET_STRIDE 124
#define THING_STRIDE 18

typedef struct Trace {
    StarsU8 fleets[FLEET_STRIDE * 3];
    StarsU8 things[THING_STRIDE * 2];
    StarsU8 transfer_record[128];
    StarsI16 selection;
    StarsI16 find_calls;
    StarsI16 transfer_calls;
    StarsI16 select_calls;
    StarsI16 last_planet;
    StarsI16 last_mode;
    StarsI16 last_id;
    StarsI16 last_kind;
    StarsI16 last_selected_id;
    StarsI16 focus_calls;
} Trace;

static StarsU16 get_word(const StarsU8 *record, StarsU16 offset)
{
    return (StarsU16)(record[offset] |
        ((StarsU16)record[(StarsU16)(offset + 1)] << 8));
}

static StarsI16 get_signed_word(const StarsU8 *record, StarsU16 offset)
{
    return (StarsI16)get_word(record, offset);
}

static void put_word(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[(StarsU16)(offset + 1)] = (StarsU8)(value >> 8);
}

/* Host copy of the already reconstructed FLookupOrbitingXfer contract.  The
 * PlanetWndProc probe uses it as the packed-record service boundary. */
static StarsI16 lookup_transfer(Trace *trace, StarsI16 planet,
    StarsI16 nth, StarsI16 id_skip, StarsI16 *id, StarsI16 *kind)
{
    StarsI16 index;
    const StarsU8 *record;
    StarsU16 full_id;

    for (index = 0; index < 3; ++index) {
        record = trace->fleets + index * FLEET_STRIDE;
        if (get_signed_word(record, 6) != planet ||
                get_signed_word(record, 0) == id_skip)
            continue;
        if (nth == 0) {
            memcpy(trace->transfer_record + 4, record, FLEET_STRIDE);
            *id = get_signed_word(record, 0);
            *kind = 2;
            put_word(trace->transfer_record, 0, get_word(record, 0));
            put_word(trace->transfer_record, 2, 2);
            return 1;
        }
        --nth;
    }
    for (index = 0; index < 2; ++index) {
        record = trace->things + index * THING_STRIDE;
        full_id = get_word(record, 0);
        if (((full_id >> 13) & 7) != 1)
            continue;
        if (nth == 0) {
            memcpy(trace->transfer_record + 4, record, THING_STRIDE);
            *id = (StarsI16)full_id;
            *kind = 8;
            put_word(trace->transfer_record, 0, full_id);
            put_word(trace->transfer_record, 2, 8);
            return 1;
        }
        --nth;
    }
    return 0;
}

static StarsI16 get_selection(void *context)
{
    return ((Trace *)context)->selection;
}

static StarsI16 find_transfer(void *context, StarsI16 planet,
    StarsI16 selection, StarsI16 *id, StarsI16 *kind)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->find_calls;
    return lookup_transfer(trace, planet, selection, -1, id, kind);
}

static void transfer(void *context, StarsI16 planet, StarsI16 mode,
    StarsI16 id, StarsI16 kind)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->transfer_calls;
    trace->last_planet = planet;
    trace->last_mode = mode;
    trace->last_id = id;
    trace->last_kind = kind;
}

static void select_planet(void *context, StarsI16 id, StarsI16 mode)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)mode;
    ++trace->select_calls;
    trace->last_selected_id = id;
}

static void focus(void *context)
{
    ++((Trace *)context)->focus_calls;
}

static StarsI32 notify(StarsU16 handle, StarsI16 code)
{
    return (StarsI32)(((StarsU32)(StarsU16)code << 16) | handle);
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsPlanetWndProcOps ops;
    StarsPlanetWndProcInput input;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsU16 thing_id;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    ops.get_dropdown_selection = get_selection;
    ops.find_transfer = find_transfer;
    ops.transfer = transfer;
    ops.select_adj_planet = select_planet;
    ops.set_focus_frame = focus;
    ops.context = &trace;
    input.button_handles[0] = 100;
    input.button_handles[1] = 101;
    input.selected_planet = 10;
    input.ops = &ops;
    put_word(trace.fleets + 0 * FLEET_STRIDE, 0, 40);
    put_word(trace.fleets + 0 * FLEET_STRIDE, 6, 10);
    put_word(trace.fleets + 1 * FLEET_STRIDE, 0, 41);
    put_word(trace.fleets + 1 * FLEET_STRIDE, 6, 99);
    put_word(trace.fleets + 2 * FLEET_STRIDE, 0, 42);
    put_word(trace.fleets + 2 * FLEET_STRIDE, 6, 10);
    thing_id = (StarsU16)(7 | (1U << 13));
    put_word(trace.things, 0, thing_id);
    put_word(trace.things, 2, 30);
    put_word(trace.things, 4, 40);
    put_word(trace.things + THING_STRIDE, 0, (StarsU16)(9 | (2U << 13)));
    checks = 0;
    failures = 0;
    trace.selection = 0;
    failures += check(stars_planet_wnd_proc(77, 0x111, 55,
        notify(100, 0), &input) == 0, &checks);
    failures += check(trace.find_calls == 1 && trace.transfer_calls == 1 &&
        trace.last_planet == 10 && trace.last_mode == 1 &&
        trace.last_id == 40 && trace.last_kind == 2, &checks);
    failures += check(get_word(trace.transfer_record, 0) == 40 &&
        get_word(trace.transfer_record, 2) == 2 &&
        get_word(trace.transfer_record + 4, 6) == 10, &checks);
    trace.selection = 1;
    failures += check(stars_planet_wnd_proc(77, 0x111, 55,
        notify(101, 0), &input) == 0, &checks);
    failures += check(trace.find_calls == 2 && trace.select_calls == 1 &&
        trace.last_selected_id == 42, &checks);
    trace.selection = 2;
    failures += check(stars_planet_wnd_proc(77, 0x111, 55,
        notify(100, 0), &input) == 0, &checks);
    failures += check(trace.transfer_calls == 2 && trace.last_id ==
        (StarsI16)thing_id && trace.last_kind == 8 &&
        get_word(trace.transfer_record, 2) == 8 &&
        get_word(trace.transfer_record + 4, 0) == thing_id, &checks);
    failures += check(stars_planet_wnd_proc(77, 0x111, 55,
        notify(101, 0), &input) == 0 && trace.select_calls == 1, &checks);
    trace.selection = 3;
    failures += check(stars_planet_wnd_proc(77, 0x111, 55,
        notify(100, 0), &input) == 0 && trace.find_calls == 5 &&
        trace.transfer_calls == 2, &checks);
    trace.selection = -1;
    failures += check(stars_planet_wnd_proc(77, 0x111, 55,
        notify(100, 0), &input) == 0 && trace.find_calls == 5, &checks);
    failures += check(trace.focus_calls == 6, &checks);
    failures += check(get_word(trace.fleets, 0) == 40 &&
        get_word(trace.fleets + 2 * FLEET_STRIDE, 0) == 42 &&
        get_word(trace.fleets + 2 * FLEET_STRIDE, 6) == 10 &&
        get_word(trace.things, 0) == thing_id, &checks);
#ifndef TARGET_NATIVE
    printf("PLANET_WND_PROC_TRANSFER_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
#ifdef TARGET_NATIVE
    {
        FILE *report;

        report = fopen("C:\\PWTE2\\PWTE2.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "PLANET_WND_PROC_TRANSFER_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#endif
    return failures == 0 ? 0 : 1;
}
