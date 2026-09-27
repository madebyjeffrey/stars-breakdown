#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 appends;
    StarsI16 separators;
    StarsI16 checked;
    StarsI16 submenus;
    StarsI16 destroys;
    StarsI16 tracked_flags;
    StarsI16 escaped_seen;
    StarsI16 resolved[8];
    StarsI16 resolved_count;
    StarsU32 command;
} Trace;
static StarsI16 client_to_screen(void *context, StarsU16 hwnd,
    StarsPoint *point)
{
    (void)context; (void)hwnd; point->x += 2; point->y += 3; return 1;
}
static StarsU16 create_menu(void *context)
{
    Trace *trace = (Trace *)context;
    ++trace->submenus;
    return (StarsU16)(10 + trace->submenus);
}
static const StarsU8 *resolve_text(void *context,
    const StarsPopupMenuText *text)
{
    Trace *trace = (Trace *)context;
    static const StarsU8 planet[] = "P&lanet";
    static const StarsU8 fleet[] = "F&leet";
    static const StarsU8 thing[] = "T&hing";
    static const StarsU8 compressed[] = "C&ompressed";
    static const StarsU8 deep[] = "Deep Space";
    if (trace->resolved_count < 8)
        trace->resolved[trace->resolved_count++] = text->source_kind;
    if (text->source_kind == 1) return fleet;
    if (text->source_kind == 2) return thing;
    if (text->source_kind == 3) return compressed;
    if (text->source_kind == 4) return deep;
    return planet;
}
static void append_menu(void *context, StarsU16 menu, StarsU16 flags,
    StarsU16 item, const StarsU8 *label)
{
    Trace *trace = (Trace *)context;
    (void)menu; (void)item;
    ++trace->appends;
    if (flags & 0x800) ++trace->separators;
    if (flags & 8) ++trace->checked;
    if (label && label[0] == 'A' && label[1] == '&' && label[2] == '&')
        trace->escaped_seen = 1;
}
static void track_popup(void *context, StarsU16 menu, StarsU16 flags,
    StarsI16 x, StarsI16 y, StarsU16 owner)
{
    Trace *trace = (Trace *)context;
    (void)menu; (void)x; (void)y; (void)owner; trace->tracked_flags = flags;
}
static void destroy_menu(void *context, StarsU16 menu)
{
    (void)menu; ++((Trace *)context)->destroys;
}
static StarsI16 peek_command(void *context, StarsU16 hwnd, StarsU16 min,
    StarsU16 max, StarsU16 remove, StarsU32 *command)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)min; (void)max; (void)remove;
    *command = trace->command; return 1;
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
int main(void)
{
    static const StarsU8 direct0[] = "A&B";
    static const StarsU8 separator[] = { (StarsU8)-1, 0 };
    static const StarsU8 direct2[] = "Two";
    static const StarsU8 *direct[] = { direct0, separator, direct2 };
    static const StarsU32 flagged[] = {
        0x10000001UL, 0x20000002UL, 0x40000003UL, 0x80000004UL
    };
    StarsPopupMenuOps ops;
    Trace trace;
    StarsI16 selected;

    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.command = 15001;
    selected = stars_popup_menu(1, 4, 5, 3, (const StarsU32 *)0, direct, 2, 1,
        &ops);
    if (selected != 1 || trace.escaped_seen != 1 || trace.separators != 1 ||
            trace.checked != 1 || trace.tracked_flags != 2 ||
            trace.destroys != 1)
        return 1;
    memset(&trace, 0, sizeof trace);
    install(&ops, &trace);
    trace.command = 15003;
    selected = stars_popup_menu(1, 0, 0, 4, flagged, (const StarsU8 *const *)0,
        0, 0, &ops);
    if (selected != 3 || trace.resolved_count != 4 ||
            trace.resolved[0] != 4 || trace.resolved[1] != 2 ||
            trace.resolved[2] != 3 || trace.resolved[3] != 1)
        return 1;
    puts("POPUP_MENU_CHECKS=22 FAILURES=0 RESULT=PASS");
    return 0;
}
