#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 menu_calls;
    StarsI16 selected;
    StarsI16 names_ok;
} Trace;

static StarsI16 point_in_rect(void *context, const StarsRect *rect,
    StarsPoint point)
{
    (void)context;
    return point.x >= rect->left && point.x < rect->right &&
        point.y >= rect->top && point.y < rect->bottom;
}

static void put16(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[offset + 1U] = (StarsU8)(value >> 8);
}

static void seed_record(StarsU8 *record, const char *name, StarsU8 valid,
    StarsU8 no_research, StarsU8 item_count, StarsU16 first_item)
{
    memset(record, 0, 40U);
    strcpy((char *)record, name);
    record[13] = valid;
    record[14] = no_research;
    record[15] = item_count;
    put16(record, 16U, first_item);
    put16(record, 38U, (StarsU16)(first_item + 11U));
}

static StarsI16 popup_menu_items(void *context, StarsU16 window,
    StarsI16 x, StarsI16 y, const StarsU8 *const *names, StarsU16 count,
    const StarsU8 *editor_text)
{
    Trace *trace = (Trace *)context;
    (void)window;
    (void)x;
    (void)y;
    trace->menu_calls++;
    trace->names_ok = count == 2U && names[0] && names[1] &&
        strcmp((const char *)names[0], "Alpha") == 0 &&
        strcmp((const char *)names[1], "Gamma") == 0 &&
        editor_text != 0;
    return 1;
}

static void select_zip(void *context, StarsU16 window, StarsI16 index)
{
    Trace *trace = (Trace *)context;
    (void)window;
    trace->selected = index;
}

static const StarsU8 *compressed_string(void *context, StarsU16 id)
{
    (void)context;
    if (id == 0x04c0U)
        return (const StarsU8 *)"Edit production queue";
    return (const StarsU8 *)0;
}

static void check(int condition, int *checks, int *failures)
{
    ++*checks;
    if (!condition)
        ++*failures;
}

int main(void)
{
    StarsU8 raw[200];
    StarsZipProductionQueue queues[6];
    StarsProductionDlgState state;
    StarsProductionDlgOps ops;
    StarsProductionDlgInput input;
    Trace trace;
    int checks;
    int failures;

    memset(raw, 0, sizeof raw);
    seed_record(raw, "Alpha", 1U, 0U, 12U, 0x1200U);
    seed_record(raw + 40U, "Beta", 0U, 1U, 2U, 0x2200U);
    seed_record(raw + 80U, "Gamma", 1U, 1U, 3U, 0x3200U);
    seed_record(raw + 120U, "Delta", 0U, 0U, 4U, 0x4200U);
    seed_record(raw + 160U, "Omega", 1U, 0U, 5U, 0x5200U);
    memset(queues, 0, sizeof queues);
    queues[5].name[0] = (StarsU8)'Q';
    stars_production_dlg_decode_zip_queues(queues, 9U, raw, 5U);

    checks = 0;
    failures = 0;
    check(sizeof(StarsZipProductionQueue) == 40U, &checks, &failures);
    check(queues[0].valid == 1U && queues[0].no_research == 0U &&
        queues[0].item_count == 12U, &checks, &failures);
    check(strcmp((const char *)queues[0].name, "Alpha") == 0,
        &checks, &failures);
    check(queues[0].items[0] == 0x1200U &&
        queues[0].items[11] == 0x120bU, &checks, &failures);
    check(queues[2].valid == 1U && queues[2].no_research == 1U &&
        strcmp((const char *)queues[2].name, "Gamma") == 0,
        &checks, &failures);
    check(queues[4].valid == 1U &&
        strcmp((const char *)queues[4].name, "Omega") == 0,
        &checks, &failures);
    check(queues[5].name[0] == (StarsU8)'Q', &checks, &failures);
    check(raw[13] == 1U && raw[14] == 0U && raw[15] == 12U &&
        raw[16] == 0U && raw[17] == 0x12U, &checks, &failures);
    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    state.zip_queues = queues;
    state.zip_queue_capacity = 5U;
    state.diamond.right = 20;
    state.diamond.bottom = 20;
    ops.popup_menu_items = popup_menu_items;
    ops.select_zip = select_zip;
    ops.point_in_rect = point_in_rect;
    ops.get_compressed_string = compressed_string;
    ops.context = &trace;
    input.state = &state;
    input.ops = &ops;
    stars_production_dlg(&input, 10U, 516U, 0U, 0x00050005UL);
    check(trace.menu_calls == 1, &checks, &failures);
    check(trace.names_ok != 0, &checks, &failures);
    check(trace.selected == 2, &checks, &failures);

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("PDGPE1.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "PRODUCTION_DLG_PACKED_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    if (failures)
        puts("PRODUCTION_DLG_PACKED_CHECKS=11 FAILURES=1 RESULT=FAIL");
    else
        puts("PRODUCTION_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
}
