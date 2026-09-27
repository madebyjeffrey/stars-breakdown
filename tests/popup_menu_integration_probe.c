#include "core.h"
#include <stdio.h>
#include <string.h>
#ifdef TARGET_NATIVE
#include <windows.h>
#endif

typedef struct Trace {
    StarsI16 appends;
    StarsI16 separators;
    StarsI16 checked;
    StarsI16 menus;
    StarsI16 destroys;
    StarsI16 tracks;
    StarsI16 resolved[8];
    StarsI16 resolved_count;
    StarsI16 submenu_label_ok;
    StarsI16 api_failures;
    StarsI16 track_flags;
    StarsI16 real_track;
    StarsU16 observed_command;
    StarsU32 command;
    StarsI16 localized_count;
    StarsI16 localized_first;
    StarsI16 localized_last;
    StarsU32 localized_sum;
} Trace;

static StarsI16 text_equal(const StarsU8 *left, const StarsU8 *right)
{
    if (!left || !right)
        return left == right;
    while (*left && *left == *right) {
        ++left;
        ++right;
    }
    return *left == *right;
}

static StarsI16 client_to_screen(void *context, StarsU16 hwnd,
    StarsPoint *point)
{
    (void)context;
    (void)hwnd;
    point->x = (StarsI16)(point->x + 2);
    point->y = (StarsI16)(point->y + 3);
    return 1;
}

static StarsU16 create_menu(void *context)
{
    Trace *trace;
    StarsU16 menu;

    trace = (Trace *)context;
#ifdef TARGET_NATIVE
    menu = (StarsU16)CreatePopupMenu();
    if (menu == 0)
        trace->api_failures = (StarsI16)(trace->api_failures | 1);
#else
    menu = (StarsU16)(100 + trace->menus);
#endif
    ++trace->menus;
    return menu;
}

static const StarsU8 *resolve_text(void *context,
    const StarsPopupMenuText *text)
{
    Trace *trace;
    static const StarsU8 fleet[] = "F&leet";
    static const StarsU8 thing[] = "T&hing";
    static const StarsU8 compressed[] = "C&ompressed";
    static const StarsU8 deep[] = "Deep Space";
    static const StarsU8 *const compressed_strings[] = {
        (const StarsU8 *)"All", (const StarsU8 *)"Armor",
        (const StarsU8 *)"Beam Weapons", (const StarsU8 *)"Bombs",
        (const StarsU8 *)"Electrical", (const StarsU8 *)"Engines",
        (const StarsU8 *)"Mechanical", (const StarsU8 *)"Mine Layers",
        (const StarsU8 *)"Mining Robots", (const StarsU8 *)"Orbital",
        (const StarsU8 *)"Planetary", (const StarsU8 *)"Scanners",
        (const StarsU8 *)"Shields", (const StarsU8 *)"Ship Hulls",
        (const StarsU8 *)"Starbase Hulls", (const StarsU8 *)"Terraforming",
        (const StarsU8 *)"Torpedoes"
    };
    const StarsU8 *value;
    StarsI16 i;

    trace = (Trace *)context;
    if (trace->resolved_count < 8)
        trace->resolved[trace->resolved_count++] = text->source_kind;
    if (text->source_kind == 1)
        return fleet;
    if (text->source_kind == 2)
        return thing;
    if (text->source_kind == 3) {
        if (text->id >= 0x43f && text->id <= 0x44f) {
            value = compressed_strings[text->id - 0x43f];
            if (trace->localized_count == 0)
                trace->localized_first = text->id;
            trace->localized_last = text->id;
            ++trace->localized_count;
            for (i = 0; value[i] != 0; ++i)
                trace->localized_sum += (StarsU8)value[i];
            return value;
        }
        return compressed;
    }
    if (text->source_kind == 4)
        return deep;
    return 0;
}

static void append_menu(void *context, StarsU16 menu, StarsU16 flags,
    StarsU16 item, const StarsU8 *label)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->appends;
    if (flags & 0x800U)
        ++trace->separators;
    if (flags & 8U)
        ++trace->checked;
    if ((flags & 0x10U) != 0U && label &&
            text_equal(label, (const StarsU8 *)"Group"))
        trace->submenu_label_ok = 1;
#ifdef TARGET_NATIVE
    if (!AppendMenu((HMENU)menu, (UINT)flags, (UINT)item,
            (LPCSTR)label))
        trace->api_failures = (StarsI16)(trace->api_failures | 2);
#else
    (void)menu;
    (void)item;
#endif
}

static void track_popup(void *context, StarsU16 menu, StarsU16 flags,
    StarsI16 x, StarsI16 y, StarsU16 owner)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)menu;
    (void)x;
    (void)y;
    if (trace->real_track) {
#ifdef TARGET_NATIVE
        if (!TrackPopupMenu((HMENU)menu, (UINT)flags, (int)x, (int)y,
                0, (HWND)owner, (LPRECT)0))
            trace->api_failures = (StarsI16)(trace->api_failures | 8);
        else {
            trace->command = 15000;
            PostMessage((HWND)owner, WM_COMMAND, 15000, 0L);
        }
#else
        (void)menu;
        (void)owner;
#endif
    }
    trace->track_flags = flags;
    ++trace->tracks;
}

static void destroy_menu(void *context, StarsU16 menu)
{
    Trace *trace;

    trace = (Trace *)context;
#ifdef TARGET_NATIVE
    if (!DestroyMenu((HMENU)menu) && trace->destroys != 1)
        trace->api_failures = (StarsI16)(trace->api_failures | 4);
#else
    (void)menu;
#endif
    ++trace->destroys;
}

static StarsI16 peek_command(void *context, StarsU16 hwnd, StarsU16 min,
    StarsU16 max, StarsU16 remove, StarsU32 *command)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    (void)min;
    (void)max;
    (void)remove;
#ifdef TARGET_NATIVE
    if (trace->real_track) {
        MSG message;
        if (!PeekMessage(&message, (HWND)hwnd, WM_COMMAND, WM_COMMAND,
                PM_REMOVE)) {
            if (trace->command == 0)
                return 0;
            *command = trace->command;
            trace->observed_command = (StarsU16)trace->command;
            return 1;
        }
        *command = message.wParam ?
            (StarsU32)(unsigned short)message.wParam : trace->command;
        trace->observed_command = (StarsU16)*command;
        return 1;
    }
#else
    (void)hwnd;
#endif
    *command = trace->command;
    return 1;
}

static void install(StarsPopupMenuOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->client_to_screen = client_to_screen;
    ops->create_menu = create_menu;
    ops->resolve_text = resolve_text;
    ops->append_menu = append_menu;
    ops->track_popup = track_popup;
    ops->destroy_menu = destroy_menu;
    ops->peek_command = peek_command;
    ops->context = trace;
}

static StarsI16 run_direct_case(void)
{
    static const StarsU8 first[] = "A&B";
    static const StarsU8 separator[] = { (StarsU8)-1, 0 };
    static const StarsU8 third[] = "Two";
    static const StarsU8 *labels[] = { first, separator, third };
    StarsPopupMenuOps ops;
    Trace trace;
    StarsI16 selected;
    StarsI16 actual_selected;
    StarsI16 mask;

    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.command = 15001;
    actual_selected = stars_popup_menu(0, 4, 5, 3, 0, labels, 2, 1, &ops);
    mask = actual_selected == 1 ? 0 : 1;
    mask = (StarsI16)(mask | (trace.appends == 3 ? 0 : 2));
    mask = (StarsI16)(mask | (trace.separators == 1 ? 0 : 4));
    mask = (StarsI16)(mask | (trace.checked == 1 ? 0 : 8));
    mask = (StarsI16)(mask | (trace.tracks == 1 ? 0 : 16));
    mask = (StarsI16)(mask | (trace.track_flags == 2 ? 0 : 32));
    mask = (StarsI16)(mask | (trace.destroys == 1 ? 0 : 64));
    mask = (StarsI16)(mask | (trace.api_failures == 0 ? 0 :
        128 + trace.api_failures));
    if (mask == 0)
        return 0;
    selected = (StarsI16)(actual_selected < 0 ? 15 : actual_selected);
    return (StarsI16)(16 * selected + mask);
}

static StarsI16 run_flagged_case(void)
{
    static const StarsU32 ids[] = {
        0x10000001UL, 0x20000002UL, 0x40000003UL, 0x80000004UL,
        0xffffffffUL
    };
    StarsPopupMenuOps ops;
    Trace trace;
    StarsI16 selected;

    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.command = 15003;
    selected = stars_popup_menu(0, 0, 0, 5, ids, 0, 0, 0, &ops);
    return selected == 3 && trace.appends == 5 && trace.separators == 1 &&
        trace.checked == 1 && trace.resolved_count == 4 &&
        trace.resolved[0] == 4 && trace.resolved[1] == 2 &&
        trace.resolved[2] == 3 && trace.resolved[3] == 1 &&
        trace.destroys == 1 && trace.api_failures == 0;
}

static StarsI16 run_submenu_case(void)
{
    static const StarsU8 group[] = "Group";
    static const StarsU8 first[] = "A&B";
    static const StarsU8 separator[] = { (StarsU8)-1, 0 };
    static const StarsU8 second[] = "B";
    static const StarsU8 *labels[] = { 0, group, first, separator, second };
    static const StarsU32 ids[] = { 0, 1, 0, 0, 1 };
    StarsPopupMenuOps ops;
    Trace trace;
    StarsI16 selected;

    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.command = 15002;
    selected = stars_popup_menu(0, 0, 0, 5, ids, labels, -2, 0, &ops);
    return selected == 2 && trace.menus == 2 && trace.appends == 4 &&
        trace.separators == 1 && trace.checked == 2 &&
        trace.resolved_count == 0 && trace.submenu_label_ok == 1 &&
        trace.destroys == 2 && trace.api_failures == 0;
}

static StarsI16 run_localized_case(void)
{
    static const StarsU32 ids[] = {
        0x4000043fUL, 0x40000440UL, 0x40000441UL, 0x40000442UL,
        0x40000443UL, 0x40000444UL, 0x40000445UL, 0x40000446UL,
        0x40000447UL, 0x40000448UL, 0x40000449UL, 0x4000044aUL,
        0x4000044bUL, 0x4000044cUL, 0x4000044dUL, 0x4000044eUL,
        0x4000044fUL
    };
    StarsPopupMenuOps ops;
    Trace trace;
    StarsI16 selected;

    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.command = 15016;
    selected = stars_popup_menu(0, 0, 0, 17, ids, 0, 16, 0, &ops);
    return selected == 16 && trace.appends == 17 &&
        trace.checked == 1 && trace.resolved_count == 8 &&
        trace.localized_count == 17 &&
        trace.localized_first == 0x43f && trace.localized_last == 0x44f &&
        trace.localized_sum == 15229UL && trace.destroys == 1 &&
        trace.api_failures == 0;
}

#ifdef TARGET_NATIVE
static StarsI16 run_real_track_case(StarsU16 owner)
{
    static const StarsU8 first[] = "First";
    static const StarsU8 second[] = "Second";
    static const StarsU8 *labels[] = { first, second };
    StarsPopupMenuOps ops;
    Trace trace;
    StarsI16 selected;

    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.real_track = 1;
    selected = stars_popup_menu(owner, 80, 80, 2, 0, labels, -1, 0,
        &ops);
    return selected == 0 && trace.appends == 2 && trace.tracks == 1 &&
        trace.track_flags == 0 && trace.destroys == 1 &&
        trace.observed_command == 15000 && trace.api_failures == 0;
}
#endif

int main(void)
{
    StarsI16 failures;

    failures = run_direct_case();
    if (!run_flagged_case())
        failures += 32;
    if (!run_submenu_case())
        failures += 64;
    if (!run_localized_case())
        failures += 128;
    if (failures == 0)
        puts("POPUP_MENU_INTEGRATION_CHECKS=31 FAILURES=0 RESULT=PASS");
    else
        puts("POPUP_MENU_INTEGRATION_CHECKS=30 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
