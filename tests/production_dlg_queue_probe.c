#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 menu_calls;
    StarsI16 menu_count;
    StarsI16 selected_queue;
    StarsI16 edit_calls;
    StarsI16 edit_accept;
    StarsI16 names_ok;
    StarsI16 editor_ok;
    StarsU8 editor_text[32];
} Trace;

static StarsI16 point_in_rect(void *context, const StarsRect *rect,
    StarsPoint point)
{
    (void)context;
    return point.x >= rect->left && point.x < rect->right &&
        point.y >= rect->top && point.y < rect->bottom;
}

static const StarsU8 *compressed_string(void *context, StarsU16 id)
{
    (void)context;
    if (id == 0x04c0U)
        return (const StarsU8 *)"<Customize>";
    return (const StarsU8 *)0;
}

static StarsI16 popup_menu_items(void *context, StarsU16 window,
    StarsI16 x, StarsI16 y, const StarsU8 *const *names, StarsU16 count,
    const StarsU8 *editor_text)
{
    Trace *trace = (Trace *)context;
    (void)window;
    (void)x;
    (void)y;
    ++trace->menu_calls;
    trace->menu_count = (StarsI16)count;
    trace->names_ok = count == 2U && names[0] && names[1] &&
        strcmp((const char *)names[0], "Alpha") == 0 &&
        strcmp((const char *)names[1], "Gamma") == 0;
    trace->editor_ok = editor_text &&
        strcmp((const char *)editor_text, "<Customize>") == 0;
    return 1;
}

static StarsI16 popup_menu_editor(void *context, StarsU16 window,
    StarsI16 x, StarsI16 y, const StarsU8 *const *names, StarsU16 count,
    const StarsU8 *editor_text)
{
    Trace *trace = (Trace *)context;
    (void)window;
    (void)x;
    (void)y;
    ++trace->menu_calls;
    trace->menu_count = (StarsI16)count;
    trace->names_ok = count == 2U && names[0] && names[1];
    trace->editor_ok = editor_text &&
        strcmp((const char *)editor_text, "<Customize>") == 0;
    return 3;
}

static void select_zip(void *context, StarsU16 window, StarsI16 index)
{
    Trace *trace = (Trace *)context;
    (void)window;
    trace->selected_queue = index;
}

static StarsI16 edit_cancel(void *context, StarsU16 window,
    StarsZipProductionQueue *queues, StarsU16 count)
{
    Trace *trace = (Trace *)context;
    (void)window;
    ++trace->edit_calls;
    queues[0].name[0] = (StarsU8)'X';
    queues[2].name[0] = (StarsU8)'Y';
    trace->edit_accept = (StarsI16)(count == 4U);
    return 0;
}

static StarsI16 edit_accept(void *context, StarsU16 window,
    StarsZipProductionQueue *queues, StarsU16 count)
{
    Trace *trace = (Trace *)context;
    (void)window;
    ++trace->edit_calls;
    queues[2].name[0] = (StarsU8)'Z';
    trace->edit_accept = (StarsI16)(count == 4U);
    return 1;
}

static void set_state(StarsProductionDlgState *state,
    StarsZipProductionQueue *queues)
{
    memset(state, 0, sizeof *state);
    memset(queues, 0, sizeof(StarsZipProductionQueue) * 4U);
    strcpy((char *)queues[0].name, "Alpha");
    strcpy((char *)queues[2].name, "Gamma");
    queues[0].valid = 1;
    queues[2].valid = 1;
    state->zip_queues = queues;
    state->zip_queue_capacity = 4;
    state->diamond.right = 20;
    state->diamond.bottom = 20;
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
    StarsZipProductionQueue queues[4];
    StarsProductionDlgOps ops;
    StarsProductionDlgInput input;
    Trace trace;
    StarsU8 original_alpha;
    StarsU8 original_gamma;
    int checks;
    int failures;

    memset(&trace, 0, sizeof trace);
    set_state(&state, queues);
    memset(&ops, 0, sizeof ops);
    ops.point_in_rect = point_in_rect;
    ops.get_compressed_string = compressed_string;
    ops.popup_menu_items = popup_menu_items;
    ops.select_zip = select_zip;
    ops.context = &trace;
    input.state = &state;
    input.ops = &ops;
    checks = 0;
    failures = 0;

    stars_production_dlg(&input, 10, 516, 0, 0x00050005UL);
    check(trace.menu_calls == 1 && trace.menu_count == 2,
        &checks, &failures);
    check(trace.names_ok && trace.editor_ok, &checks, &failures);
    check(trace.selected_queue == 2, &checks, &failures);

    original_alpha = queues[0].name[0];
    original_gamma = queues[2].name[0];
    trace.menu_calls = 0;
    trace.names_ok = 0;
    trace.editor_ok = 0;
    ops.popup_menu_items = popup_menu_editor;
    ops.edit_zip_dialog = edit_cancel;
    stars_production_dlg(&input, 10, 516, 0, 0x00050005UL);
    check(trace.menu_calls == 1 && trace.menu_count == 2,
        &checks, &failures);
    check(trace.names_ok && trace.editor_ok, &checks, &failures);
    check(trace.edit_calls == 1 && trace.edit_accept == 1,
        &checks, &failures);
    check(queues[0].name[0] == original_alpha &&
        queues[2].name[0] == original_gamma, &checks, &failures);

    trace.menu_calls = 0;
    trace.edit_calls = 0;
    trace.edit_accept = 0;
    ops.edit_zip_dialog = edit_accept;
    stars_production_dlg(&input, 10, 516, 0, 0x00050005UL);
    check(trace.menu_calls == 1 && trace.edit_calls == 1 &&
        trace.edit_accept == 1, &checks, &failures);
    check(queues[2].name[0] == (StarsU8)'Z', &checks, &failures);

    /* The real table has four entries.  An unsigned capacity must still
       clamp safely before walking that table when a packed/global value is
       unexpectedly oversized. */
    state.zip_queue_capacity = 0xffffU;
    strcpy((char *)queues[2].name, "Gamma");
    trace.menu_calls = 0;
    trace.edit_calls = 0;
    ops.popup_menu_items = popup_menu_items;
    ops.edit_zip_dialog = (StarsProductionDlgEditZipDialog)0;
    stars_production_dlg(&input, 10, 516, 0, 0x00050005UL);
    check(trace.menu_calls == 1 && trace.menu_count == 2 &&
        trace.names_ok && trace.editor_ok, &checks, &failures);

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("PDGQRES.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "PRODUCTION_DLG_QUEUE_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    if (failures)
        puts("PRODUCTION_DLG_QUEUE_CHECKS=10 FAILURES=1 RESULT=FAIL");
    else
        puts("PRODUCTION_DLG_QUEUE_CHECKS=10 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
}
