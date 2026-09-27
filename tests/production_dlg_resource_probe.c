#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 events[8];
    StarsU16 event_count;
    StarsU16 copied_id;
    StarsU16 compressed_id;
    StarsI16 popup_calls;
    StarsI16 menu_calls;
} Trace;

static void record_event(Trace *trace, StarsU16 value)
{
    if (trace->event_count < 8)
        trace->events[trace->event_count++] = value;
}

static StarsI16 copy_string(void *context, StarsU16 id, StarsU8 *text,
    StarsU16 capacity)
{
    Trace *trace = (Trace *)context;
    const char *value = "Production template help";
    StarsU16 length = (StarsU16)strlen(value);

    trace->copied_id = id;
    record_event(trace, 1);
    if (capacity <= length)
        return 0;
    memcpy(text, value, length + 1U);
    return (StarsI16)length;
}

static const StarsU8 *compressed_string(void *context, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    trace->compressed_id = id;
    record_event(trace, 3);
    return (const StarsU8 *)"<Customize>";
}

static void popup(void *context, StarsU16 hwnd, StarsI16 x, StarsI16 y)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    (void)x;
    (void)y;
    ++trace->popup_calls;
    record_event(trace, 2);
}

static StarsI16 popup_menu(void *context, StarsU16 hwnd, StarsI16 x,
    StarsI16 y)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    (void)x;
    (void)y;
    ++trace->menu_calls;
    record_event(trace, 4);
    return -1;
}

static StarsI16 point_in_rect(void *context, const StarsRect *rect,
    StarsPoint point)
{
    (void)context;
    return point.x >= rect->left && point.x < rect->right &&
        point.y >= rect->top && point.y < rect->bottom;
}

static void install(StarsProductionDlgOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->copy_popup_string = copy_string;
    ops->get_compressed_string = compressed_string;
    ops->popup = popup;
    ops->popup_menu = popup_menu;
    ops->point_in_rect = point_in_rect;
    ops->context = trace;
}

static void check(int condition, int *checks, int *failures)
{
    ++*checks;
    if (!condition)
        ++*failures;
}

int main(void)
{
    StarsProductionDlgState state;
    StarsProductionDlgOps ops;
    StarsProductionDlgInput input;
    Trace trace;
    StarsU16 before_invalid;
    int checks;
    int failures;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    state.diamond.left = 0;
    state.diamond.top = 0;
    state.diamond.right = 20;
    state.diamond.bottom = 20;
    install(&ops, &trace);
    input.state = &state;
    input.ops = &ops;
    checks = 0;
    failures = 0;

    stars_production_dlg(&input, 10, 513, 0, 0x00050005UL);
    check(trace.copied_id == 0x00d3U, &checks, &failures);
    check(trace.popup_calls == 1, &checks, &failures);
    check(trace.event_count == 2 && trace.events[0] == 1 &&
        trace.events[1] == 2, &checks, &failures);
    check(strcmp((const char *)state.popup_text,
        "Production template help") == 0,
        &checks, &failures);

    trace.event_count = 0;
    stars_production_dlg(&input, 10, 516, 0, 0x00050005UL);
    check(trace.compressed_id == 0x04c0U, &checks, &failures);
    check(trace.menu_calls == 1, &checks, &failures);
    check(trace.event_count == 2 && trace.events[0] == 3 &&
        trace.events[1] == 4, &checks, &failures);

    before_invalid = trace.event_count;
    stars_production_dlg(&input, 10, 513, 0, 0x00320032UL);
    check(trace.event_count == before_invalid && trace.popup_calls == 1 &&
        trace.menu_calls == 1, &checks, &failures);

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("PDGERES.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "PRODUCTION_DLG_RESOURCE_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    if (failures)
        puts("PRODUCTION_DLG_RESOURCE_CHECKS=8 FAILURES=1 RESULT=FAIL");
    else
        puts("PRODUCTION_DLG_RESOURCE_CHECKS=8 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
}
