#include "core.h"
#include <stdio.h>
#include <string.h>

#ifdef INTEGRATION_NATIVE
#define main popup_packed_e1_main
#include "BASE.C"
#undef main
#else
#define main popup_packed_e1_main
#include "popup_packed_probe.c"
#undef main
#endif

typedef struct PopupPayload {
    StarsU8 popup[22];
    StarsU8 fleet[124];
    StarsU8 planet[56];
    StarsU8 hull[32];
    const StarsU8 *labels[6];
} PopupPayload;

typedef struct PopupE2Trace {
    PopupPayload *payload;
    StarsI16 measure_calls;
    StarsI16 slot_updates;
    StarsI16 created;
    StarsI16 width;
    StarsI16 height;
    StarsI16 screen_x;
    StarsI16 screen_y;
    StarsI16 last_kind;
    StarsI16 last_value1;
    StarsI32 content_checksum;
    StarsU16 message_window;
    StarsU16 capture_window;
} PopupE2Trace;

static StarsU16 e2_get16(const StarsU8 *raw, StarsI16 offset)
{
    return (StarsU16)(raw[offset] |
        ((StarsU16)raw[offset + 1] << 8));
}

static void e2_put16(StarsU8 *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static StarsI16 e2_label_length(const StarsU8 *label)
{
    StarsI16 length = 0;
    while (label[length] != 0)
        ++length;
    return length;
}

static StarsI32 e2_payload_checksum(const PopupPayload *payload,
    const StarsPopupMeasureCase *popup)
{
    StarsI32 sum;
    StarsI16 label_length;

    label_length = e2_label_length(payload->labels[0]);
    sum = (StarsI32)e2_get16(payload->popup, 4) +
        (StarsI32)e2_get16(payload->planet, 0) + label_length;
    if (popup->popup_kind == 2 || popup->popup_kind == 3)
        sum += (StarsI32)e2_get16(payload->fleet, 8) +
            (StarsI32)e2_get16(payload->fleet, 10) +
            (StarsI32)e2_get16(payload->fleet, 12) +
            (StarsI32)e2_get16(payload->fleet, 14) +
            (StarsI32)e2_get16(payload->hull, 0);
    if (popup->popup_kind == 5 || popup->popup_kind == 7)
        sum += (StarsI32)e2_get16(payload->planet, 15) +
            (StarsI32)e2_get16(payload->planet, 16) +
            (StarsI32)e2_get16(payload->planet, 20) +
            (StarsI32)e2_get16(payload->planet, 22);
    if (popup->popup_kind == 10)
        sum += popup->value1;
    if (popup->popup_kind == 12)
        sum += (StarsI32)e2_get16(payload->popup, 12) +
            (StarsI32)e2_get16(payload->popup, 14) +
            (StarsI32)e2_get16(payload->popup, 16);
    return sum;
}

static StarsPoint e2_oracle_measure(const PopupPayload *payload,
    const StarsPopupMeasureCase *popup)
{
    StarsPoint point;
    StarsI16 label_length;
    StarsI16 value;

    label_length = e2_label_length(payload->labels[popup->popup_kind % 6]);
    value = (StarsI16)(e2_get16(payload->popup, 4) & 7U);
    if (popup->popup_kind == 2 || popup->popup_kind == 3)
        value = (StarsI16)(value + (e2_get16(payload->fleet, 12) & 7U));
    if (popup->popup_kind == 5 || popup->popup_kind == 7)
        value = (StarsI16)(value + (e2_get16(payload->planet, 15) & 7U));
    point.x = (StarsI16)(20 + label_length * 3 + value);
    point.y = (StarsI16)(12 + label_length +
        (e2_get16(payload->hull, 2) & 7U));
    return point;
}

static StarsI16 e2_client_to_screen(void *context, StarsU16 hwnd,
    StarsPoint *point)
{
    (void)context;
    (void)hwnd;
    point->x = (StarsI16)(point->x + 5);
    point->y = (StarsI16)(point->y + 7);
    return 1;
}

static StarsU16 e2_get_dc(void *context, StarsU16 hwnd)
{
    (void)context;
    (void)hwnd;
    return 7;
}

static StarsU16 e2_select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    (void)context;
    (void)hdc;
    return (StarsU16)(font + 1);
}

static StarsPoint measure_content(void *context,
    const StarsPopupMeasureCase *popup)
{
    PopupE2Trace *trace = (PopupE2Trace *)context;
    StarsPoint point;
    StarsI16 label_length;
    StarsI16 value;

    ++trace->measure_calls;
    trace->last_kind = popup->popup_kind;
    trace->last_value1 = popup->value1;
    trace->content_checksum = e2_payload_checksum(trace->payload, popup);
    label_length = e2_label_length(
        trace->payload->labels[popup->popup_kind % 6]);
    value = (StarsI16)(e2_get16(trace->payload->popup, 4) & 7U);
    if (popup->popup_kind == 2 || popup->popup_kind == 3)
        value = (StarsI16)(value +
            (e2_get16(trace->payload->fleet, 12) & 7U));
    if (popup->popup_kind == 5 || popup->popup_kind == 7)
        value = (StarsI16)(value +
            (e2_get16(trace->payload->planet, 15) & 7U));
    point.x = (StarsI16)(20 + label_length * 3 + value);
    point.y = (StarsI16)(12 + label_length +
        (e2_get16(trace->payload->hull, 2) & 7U));
    return point;
}

static void e2_update_slots(void *context)
{
    ++((PopupE2Trace *)context)->slot_updates;
}

static StarsI16 e2_metric(void *context, StarsI16 index)
{
    (void)context;
    (void)index;
    return 120;
}

static StarsU16 e2_create_window(void *context, StarsU16 parent, StarsU32 style,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height)
{
    PopupE2Trace *trace = (PopupE2Trace *)context;
    (void)parent;
    (void)style;
    trace->created = 1;
    trace->screen_x = x;
    trace->screen_y = y;
    trace->width = width;
    trace->height = height;
    return 99;
}

static void e2_release_dc(void *context, StarsU16 hwnd, StarsU16 hdc)
{
    (void)context;
    (void)hwnd;
    (void)hdc;
}

static void e2_send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 font)
{
    PopupE2Trace *trace = (PopupE2Trace *)context;
    (void)message;
    (void)font;
    trace->message_window = hwnd;
}

static void e2_set_capture(void *context, StarsU16 hwnd)
{
    ((PopupE2Trace *)context)->capture_window = hwnd;
}

static void e2_install(StarsPopupOps *ops, PopupE2Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->client_to_screen = e2_client_to_screen;
    ops->get_dc = e2_get_dc;
    ops->select_font = e2_select_font;
    ops->measure = measure_content;
    ops->update_slots = e2_update_slots;
    ops->get_system_metric = e2_metric;
    ops->create_window = e2_create_window;
    ops->release_dc = e2_release_dc;
    ops->send_message = e2_send_message;
    ops->set_capture = e2_set_capture;
    ops->context = trace;
}

static StarsI16 run_case(PopupPayload *payload, StarsI16 kind,
    StarsI16 extra_rows, StarsI16 expected_measure_value, int *checks)
{
    PopupE2Trace trace;
    StarsPopupInput input;
    StarsPopupOps ops;
    StarsPopupMeasureCase measure_case;
    StarsPoint expected_size;
    StarsI16 expected_width;
    StarsI16 expected_height;
    StarsI16 expected_x;
    StarsI16 expected_y;
    StarsI16 expected_measure_calls;
    StarsI16 failures = 0;

    memset(&trace, 0, sizeof trace);
    trace.payload = payload;
    e2_install(&ops, &trace);
    memset(&input, 0, sizeof input);
    input.hwnd = 1;
    input.x = 90;
    input.y = 90;
    input.popup_kind = kind;
    input.planet_id = (StarsI16)e2_get16(payload->popup, 2);
    input.font_normal = 4;
    input.line_height = 8;
    input.line_height_large = 10;
    input.slot_extra_rows = extra_rows;
    input.ops = &ops;
    memset(&measure_case, 0, sizeof measure_case);
    measure_case.popup_kind = kind;
    measure_case.value1 = expected_measure_value;
    expected_size = e2_oracle_measure(payload, &measure_case);
    expected_width = expected_size.x;
    expected_height = expected_size.y;
    expected_measure_calls = 1;
    if (kind >= 5 && kind <= 7) {
        expected_width = (StarsI16)(expected_width + 2);
        expected_height = (StarsI16)(expected_height + 2);
    } else if (kind == 8 || kind == 10 || kind == 12) {
        expected_width = (StarsI16)(expected_width + 2);
        expected_height = (StarsI16)(expected_height + 2);
    } else if (kind == 9) {
        expected_width = 0x158;
        expected_height = (StarsI16)(10 + 8 * 12 + 0x4e);
        expected_measure_calls = 0;
    } else if (kind == 11 || kind == 14) {
        expected_width = 0x154;
        expected_height = (StarsI16)(8 * 7 + 0x13a);
        if (kind == 11)
            expected_height = (StarsI16)(expected_height + extra_rows * 8);
        expected_measure_calls = 0;
    } else if (kind == 13) {
        expected_width = 0x78;
        expected_height = 0x50;
        expected_measure_calls = 0;
    }
    expected_x = (StarsI16)(95 - expected_width);
    expected_y = (StarsI16)(97 - expected_height);
    if (expected_x > (StarsI16)(120 - expected_width))
        expected_x = (StarsI16)(120 - expected_width);
    if (expected_x < 0)
        expected_x = 0;
    if (expected_y > (StarsI16)(120 - expected_height))
        expected_y = (StarsI16)(120 - expected_height);
    if (expected_y < 0)
        expected_y = 0;
    stars_popup(&input);
    ++*checks;
    if (trace.created != 1)
        ++failures;
    ++*checks;
    if (trace.measure_calls != expected_measure_calls)
        ++failures;
    ++*checks;
    if (trace.width != expected_width || trace.height != expected_height)
        ++failures;
    ++*checks;
    if (trace.message_window != 99 || trace.capture_window != 99)
        ++failures;
    ++*checks;
    if (trace.screen_x != expected_x || trace.screen_y != expected_y)
        ++failures;
    ++*checks;
    if (expected_measure_calls && trace.last_kind != kind)
        ++failures;
    ++*checks;
    if (expected_measure_calls && trace.content_checksum !=
            e2_payload_checksum(payload, &measure_case))
        ++failures;
    ++*checks;
    if (kind == 10 && trace.last_value1 != input.planet_id)
        ++failures;
    ++*checks;
    if ((kind == 11 || kind == 14) && trace.slot_updates != 1)
        ++failures;
#ifndef INTEGRATION_NATIVE
    if (failures != 0)
        printf("POPUP_E2_CASE kind=%d failures=%d actual=%d,%d expected=%d,%d measures=%d expected_measures=%d checksum=%d expected_checksum=%d\\n",
            kind, failures, trace.width, trace.height, expected_width,
            expected_height, trace.measure_calls, expected_measure_calls,
            (int)trace.content_checksum,
            (int)e2_payload_checksum(payload, &measure_case));
#endif
    return failures;
}

#ifdef INTEGRATION_NATIVE
#define E2_MAIN popup_e2_probe
#else
#define E2_MAIN main
#endif

int E2_MAIN(void)
{
    PopupPayload payload;
    StarsI16 i;
    StarsI16 offsets[11];
    StarsU16 expected_fields[11];
    int checks = 0;
    int failures = 0;
    static const StarsU8 mineral[] = "Minerals";
    static const StarsU8 fleet[] = "Fleet";
    static const StarsU8 planet[] = "Planet";
    static const StarsU8 population[] = "Population";
    static const StarsU8 resource[] = "Resource";
    static const StarsU8 component[] = "Component";

    memset(&payload, 0, sizeof payload);
    payload.labels[0] = mineral;
    payload.labels[1] = fleet;
    payload.labels[2] = planet;
    payload.labels[3] = population;
    payload.labels[4] = resource;
    payload.labels[5] = component;
    e2_put16(payload.popup, 0, 3);
    e2_put16(payload.popup, 2, 37);
    e2_put16(payload.popup, 4, 12);
    e2_put16(payload.popup, 6, 3);
    e2_put16(payload.popup, 8, 21);
    e2_put16(payload.popup, 10, 44);
    e2_put16(payload.popup, 12, 7);
    e2_put16(payload.popup, 14, 2);
    e2_put16(payload.popup, 16, 9);
    e2_put16(payload.popup, 18, 6);
    e2_put16(payload.popup, 20, 5);
    e2_put16(payload.fleet, 8, 101);
    e2_put16(payload.fleet, 10, 202);
    e2_put16(payload.fleet, 12, 4);
    e2_put16(payload.fleet, 14, 2);
    e2_put16(payload.fleet, 16, 3);
    e2_put16(payload.hull, 0, 7);
    e2_put16(payload.hull, 2, 9);
    e2_put16(payload.planet, 0, 0x1234);
    e2_put16(payload.planet, 15, 1);
    e2_put16(payload.planet, 16, 2);
    e2_put16(payload.planet, 20, 9);
    e2_put16(payload.planet, 22, 4);

    offsets[0] = 0; offsets[1] = 2; offsets[2] = 4; offsets[3] = 6;
    offsets[4] = 8; offsets[5] = 10; offsets[6] = 12; offsets[7] = 14;
    offsets[8] = 16; offsets[9] = 18; offsets[10] = 20;
    expected_fields[0] = 3; expected_fields[1] = 37;
    expected_fields[2] = 12; expected_fields[3] = 3;
    expected_fields[4] = 21; expected_fields[5] = 44;
    expected_fields[6] = 7; expected_fields[7] = 2;
    expected_fields[8] = 9; expected_fields[9] = 6;
    expected_fields[10] = 5;
    for (i = 0; i < 11; ++i) {
        ++checks;
        if (e2_get16(payload.popup, offsets[i]) != expected_fields[i])
            ++failures;
        ++checks;
        if (payload.popup[offsets[i]] != (StarsU8)expected_fields[i] ||
                payload.popup[offsets[i] + 1] !=
                    (StarsU8)(expected_fields[i] >> 8))
            ++failures;
    }
    failures += run_case(&payload, 1, 0, 0, &checks);
    failures += run_case(&payload, 2, 0, 0, &checks);
    failures += run_case(&payload, 3, 0, 0, &checks);
    failures += run_case(&payload, 4, 0, 0, &checks);
    failures += run_case(&payload, 5, 0, 0, &checks);
    failures += run_case(&payload, 6, 0, 0, &checks);
    failures += run_case(&payload, 7, 0, 0, &checks);
    failures += run_case(&payload, 8, 0, 0, &checks);
    failures += run_case(&payload, 9, 0, 0, &checks);
    failures += run_case(&payload, 10, 0, 37, &checks);
    failures += run_case(&payload, 11, 3, 0, &checks);
    failures += run_case(&payload, 12, 0, 0, &checks);
    failures += run_case(&payload, 13, 0, 0, &checks);
    failures += run_case(&payload, 14, 0, 0, &checks);
    if (failures == 0)
        puts("POPUP_E2_CHECKS=148 FAILURES=0 RESULT=PASS");
    else
        puts("POPUP_E2_CHECKS=148 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}
