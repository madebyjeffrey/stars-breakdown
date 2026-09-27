#include "core.h"
#include <stdio.h>
#include <string.h>
#ifdef INTEGRATION_NATIVE
#include <windows.h>
#endif

#ifdef INTEGRATION_NATIVE
#define main popup_menu_e1_main
#include "BASE.C"
#undef main
#else
#define main popup_menu_e1_main
#include "popup_menu_integration_probe.c"
#undef main
#endif

typedef struct PopupMenuE2Trace {
    StarsU8 fleet[124];
    StarsU8 thing[64];
    StarsU8 planet[56];
    StarsI16 appends;
    StarsI16 separators;
    StarsI16 checked;
    StarsI16 menus;
    StarsI16 destroys;
    StarsI16 tracks;
    StarsI16 api_failures;
    StarsI16 labels_ok;
    StarsI16 payload_hits;
    StarsI16 resolved[8];
    StarsI16 resolved_count;
    StarsI16 peek_args_ok;
    StarsI16 track_flags;
    StarsI16 point_x;
    StarsI16 point_y;
    StarsU16 owner;
    StarsU32 command;
} PopupMenuE2Trace;

int popup_menu_e2_last_failures;
StarsU32 popup_menu_e2_last_command;

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

static StarsI16 e2_equal(const StarsU8 *left, const StarsU8 *right)
{
    if (!left || !right)
        return left == right;
    while (*left && *left == *right) {
        ++left;
        ++right;
    }
    return *left == *right;
}

static StarsI16 e2_check(StarsI16 condition, int *checks)
{
    if (!condition)
        popup_menu_e2_last_failures |= 1 << *checks;
    ++*checks;
    return condition ? 0 : 1;
}

static StarsI16 e2_client_to_screen(void *context, StarsU16 hwnd,
    StarsPoint *point)
{
    (void)context;
    (void)hwnd;
    point->x = (StarsI16)(point->x + 2);
    point->y = (StarsI16)(point->y + 3);
    return 1;
}

static StarsU16 e2_create_menu(void *context)
{
    PopupMenuE2Trace *trace = (PopupMenuE2Trace *)context;
    StarsU16 menu;

#ifdef INTEGRATION_NATIVE
    menu = (StarsU16)CreatePopupMenu();
    if (menu == 0)
        trace->api_failures = (StarsI16)(trace->api_failures | 1);
#else
    menu = (StarsU16)(300 + trace->menus);
#endif
    ++trace->menus;
    return menu;
}

static const StarsU8 *e2_resolve_text(void *context,
    const StarsPopupMenuText *text)
{
    PopupMenuE2Trace *trace = (PopupMenuE2Trace *)context;
    static const StarsU8 fleet[] = "Fleet record";
    static const StarsU8 thing[] = "Thing record";
    static const StarsU8 planet[] = "Planet record";
    static const StarsU8 deep[] = "Deep record";

    if (trace->resolved_count < 8)
        trace->resolved[trace->resolved_count++] = text->source_kind;
    if (text->source_kind == 1 &&
            text->id == (StarsI16)e2_get16(trace->fleet, 0)) {
        trace->payload_hits = (StarsI16)(trace->payload_hits | 1);
        return fleet;
    }
    if (text->source_kind == 2 &&
            text->id == (StarsI16)e2_get16(trace->thing, 0)) {
        trace->payload_hits = (StarsI16)(trace->payload_hits | 2);
        return thing;
    }
    if (text->source_kind == 3 &&
            text->id == (StarsI16)e2_get16(trace->planet, 0)) {
        trace->payload_hits = (StarsI16)(trace->payload_hits | 4);
        return planet;
    }
    if (text->source_kind == 4) {
        trace->payload_hits = (StarsI16)(trace->payload_hits | 8);
        return deep;
    }
    return (const StarsU8 *)0;
}

static void e2_append_menu(void *context, StarsU16 menu, StarsU16 flags,
    StarsU16 item, const StarsU8 *label)
{
    PopupMenuE2Trace *trace = (PopupMenuE2Trace *)context;

    ++trace->appends;
    if ((flags & 0x800U) != 0U)
        ++trace->separators;
    if ((flags & 8U) != 0U)
        ++trace->checked;
    if (label && e2_equal(label, (const StarsU8 *)"Fleet record"))
        ++trace->labels_ok;
#ifdef INTEGRATION_NATIVE
    if (!AppendMenu((HMENU)menu, (UINT)flags, (UINT)item,
            (LPCSTR)label))
        trace->api_failures = (StarsI16)(trace->api_failures | 2);
#else
    (void)menu;
    (void)item;
#endif
}

static void e2_track_popup(void *context, StarsU16 menu, StarsU16 flags,
    StarsI16 x, StarsI16 y, StarsU16 owner)
{
    PopupMenuE2Trace *trace = (PopupMenuE2Trace *)context;

    trace->track_flags = flags;
    trace->point_x = x;
    trace->point_y = y;
    trace->owner = owner;
#ifdef INTEGRATION_NATIVE
    if (!TrackPopupMenu((HMENU)menu, (UINT)flags, (int)x, (int)y, 0,
            (HWND)owner, (LPRECT)0))
        trace->api_failures = (StarsI16)(trace->api_failures | 8);
    else {
        MSG message;
        if (PeekMessage(&message, (HWND)owner, WM_COMMAND, WM_COMMAND,
                PM_NOREMOVE))
            trace->command = (StarsU32)(unsigned short)message.wParam;
    }
#else
    (void)menu;
    trace->command = 15000;
#endif
    ++trace->tracks;
}

static void e2_destroy_menu(void *context, StarsU16 menu)
{
    PopupMenuE2Trace *trace = (PopupMenuE2Trace *)context;
#ifdef INTEGRATION_NATIVE
    if (!DestroyMenu((HMENU)menu))
        trace->api_failures = (StarsI16)(trace->api_failures | 4);
#else
    (void)menu;
#endif
    ++trace->destroys;
}

static StarsI16 e2_peek_command(void *context, StarsU16 hwnd,
    StarsU16 minimum, StarsU16 maximum, StarsU16 remove,
    StarsU32 *command)
{
    PopupMenuE2Trace *trace = (PopupMenuE2Trace *)context;

    trace->peek_args_ok = (StarsI16)(hwnd == trace->owner &&
        minimum == 0x111 && maximum == 0x111 && remove == 2);
#ifdef INTEGRATION_NATIVE
    {
        MSG message;
        if (!PeekMessage(&message, (HWND)hwnd, minimum, maximum, remove))
            return 0;
        *command = (StarsU32)(unsigned short)message.wParam;
        trace->command = *command;
        return 1;
    }
#else
    *command = trace->command;
    return trace->command != 0;
#endif
}

static void e2_install(StarsPopupMenuOps *ops, PopupMenuE2Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->client_to_screen = e2_client_to_screen;
    ops->create_menu = e2_create_menu;
    ops->resolve_text = e2_resolve_text;
    ops->append_menu = e2_append_menu;
    ops->track_popup = e2_track_popup;
    ops->destroy_menu = e2_destroy_menu;
    ops->peek_command = e2_peek_command;
    ops->context = trace;
}

static StarsI16 run_e2_case(StarsU16 owner, int *checks)
{
    static const StarsU32 ids[] = {
        0x80000011UL, 0x20000022UL, 0x40000033UL, 0x10000044UL,
        0xffffffffUL
    };
    PopupMenuE2Trace trace;
    StarsPopupMenuOps ops;
    StarsI16 selected;
    StarsI16 failures = 0;

    memset(&trace, 0, sizeof trace);
    e2_put16(trace.fleet, 0, 0x11);
    e2_put16(trace.thing, 0, 0x22);
    e2_put16(trace.planet, 0, 0x33);
    e2_install(&ops, &trace);
    selected = stars_popup_menu(owner, 10, 20, 5, ids, 0, 2, 1, &ops);
    popup_menu_e2_last_command = trace.command;
    failures += e2_check(trace.menus == 1, checks);
    failures += e2_check(trace.appends == 5, checks);
    failures += e2_check(trace.separators == 1, checks);
    failures += e2_check(trace.checked == 1, checks);
    failures += e2_check(trace.tracks == 1, checks);
    failures += e2_check(trace.track_flags == 2, checks);
    failures += e2_check(trace.point_x == 12 && trace.point_y == 23, checks);
    failures += e2_check(trace.owner == owner, checks);
    failures += e2_check(trace.destroys == 1, checks);
    failures += e2_check(trace.peek_args_ok == 1, checks);
    failures += e2_check(selected == 0, checks);
    failures += e2_check(trace.command == 15000UL, checks);
    failures += e2_check(trace.resolved_count == 4, checks);
    failures += e2_check(trace.resolved[0] == 1, checks);
    failures += e2_check(trace.resolved[1] == 2, checks);
    failures += e2_check(trace.resolved[2] == 3, checks);
    failures += e2_check(trace.resolved[3] == 4, checks);
    failures += e2_check(trace.payload_hits == 15, checks);
    failures += e2_check(trace.labels_ok == 1, checks);
    failures += e2_check(trace.api_failures == 0, checks);
    failures += e2_check(e2_get16(trace.fleet, 0) == 0x11, checks);
    failures += e2_check(e2_get16(trace.thing, 0) == 0x22, checks);
    failures += e2_check(e2_get16(trace.planet, 0) == 0x33, checks);
    failures += e2_check((ids[4] & 0xffffUL) == 0xffffUL, checks);
    failures += e2_check((ids[0] >> 16) == 0x8000U, checks);
    failures += e2_check((ids[1] >> 16) == 0x2000U, checks);
    return failures;
}

#ifdef INTEGRATION_NATIVE
#define E2_MAIN popup_menu_e2_probe
#else
#define E2_MAIN main
#endif

int popup_menu_e2_run(StarsU16 owner)
{
    int checks = 0;
    int failures;
    failures = run_e2_case(owner, &checks);
    failures += e2_check(sizeof(((PopupMenuE2Trace *)0)->fleet) == 124,
        &checks);
    failures += e2_check(sizeof(((PopupMenuE2Trace *)0)->thing) == 64,
        &checks);
    failures += e2_check(sizeof(((PopupMenuE2Trace *)0)->planet) == 56,
        &checks);
    if (failures == 0)
        puts("POPUP_MENU_E2_CHECKS=30 FAILURES=0 RESULT=PASS");
    else
        puts("POPUP_MENU_E2_CHECKS=30 FAILURES=1 RESULT=FAIL");
    return failures != 0;
}

int E2_MAIN(void)
{
    return popup_menu_e2_run(77);
}
