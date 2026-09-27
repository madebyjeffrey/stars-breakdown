#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    char events[16];
    StarsI16 event_count;
    StarsI16 cases;
    StarsI16 last_kind;
    StarsRect last_slot;
    StarsI16 last_starbase_mode;
} Trace;

static void event(Trace *trace, char value)
{
    if (trace->event_count < 16)
        trace->events[trace->event_count] = value;
    ++trace->event_count;
}

static StarsI16 get_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context;
    (void)hwnd;
    rect->left = 4;
    rect->top = 5;
    rect->right = 120;
    rect->bottom = 70;
    return 1;
}

static StarsU32 background(void *context, StarsU16 hdc, StarsU32 color)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    event(trace, color == 0xffffffUL ? 'B' : 'b');
    return color == 0xffffffUL ? 0x11UL : 0x11UL;
}

static StarsU32 text_color(void *context, StarsU16 hdc, StarsU32 color)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    event(trace, color == 0 ? 'T' : 't');
    return color == 0 ? 0x22UL : 0x22UL;
}

static StarsU16 background_mode(void *context, StarsU16 hdc, StarsU16 mode)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    if (mode == 2) event(trace, 'M');
    else if (mode == 1) event(trace, 'm');
    else event(trace, 'R');
    return 3;
}

static StarsU16 font(void *context, StarsU16 hdc, StarsU16 selected)
{
    Trace *trace = (Trace *)context;
    (void)hdc;
    event(trace, selected == 4 ? 'F' : 'f');
    return 3;
}

static void draw_case(void *context, const StarsDrawPopupCase *popup)
{
    Trace *trace = (Trace *)context;
    ++trace->cases;
    trace->last_kind = popup->popup_kind;
    trace->last_slot = popup->slot_rect;
    trace->last_starbase_mode = popup->starbase_mode;
    event(trace, 'C');
}

static void install(StarsDrawPopupOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->get_client_rect = get_rect;
    ops->set_background_color = background;
    ops->set_text_color = text_color;
    ops->set_background_mode = background_mode;
    ops->select_font = font;
    ops->draw_fleet = draw_case;
    ops->draw_slot_hull = draw_case;
    ops->context = trace;
}

static void setup(StarsDrawPopupInput *input, StarsDrawPopupOps *ops,
    Trace *trace, StarsI16 kind)
{
    memset(input, 0, sizeof *input);
    memset(trace, 0, sizeof *trace);
    install(ops, trace);
    input->hwnd = 8;
    input->hdc = 9;
    input->popup_kind = kind;
    input->font_normal = 3;
    input->font_bold = 4;
    input->arial8_height = 9;
    input->slot_starbase_mode = 1;
    input->ops = ops;
}

static int events_match(const Trace *trace, const char *expected)
{
    StarsI16 length = (StarsI16)strlen(expected);
    return trace->event_count == length &&
        memcmp(trace->events, expected, (size_t)length) == 0;
}

static void check(int condition, int *checks, int *failures)
{
    ++*checks;
    if (!condition) ++*failures;
}

int main(void)
{
    StarsDrawPopupInput input;
    StarsDrawPopupOps ops;
    Trace trace;
    int checks;
    int failures;

    checks = 0;
    failures = 0;
    setup(&input, &ops, &trace, 3);
    stars_draw_popup(&input);
    check(trace.cases == 1 && trace.last_kind == 3, &checks, &failures);
    check(events_match(&trace, "BTMFCfRtb"), &checks, &failures);
    check(trace.last_slot.left == 4 && trace.last_slot.top == 5 &&
        trace.last_slot.right == 120 && trace.last_slot.bottom == 70,
        &checks, &failures);

    setup(&input, &ops, &trace, 14);
    stars_draw_popup(&input);
    check(trace.cases == 1 && trace.last_kind == 14 &&
        trace.last_starbase_mode == 1, &checks, &failures);
    check(events_match(&trace, "BTMFFmCfRtb"), &checks, &failures);
    check(trace.last_slot.left == 10 && trace.last_slot.top == 315 &&
        trace.last_slot.right == 120 && trace.last_slot.bottom == 70,
        &checks, &failures);
    check(trace.last_slot.right > trace.last_slot.left &&
        trace.last_slot.bottom > 0, &checks, &failures);

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("DRPSRES.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "DRAW_POPUP_STATE_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    printf("DRAW_POPUP_STATE_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
