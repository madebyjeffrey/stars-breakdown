#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 compressed_ids[16];
    StarsU16 compressed_count;
    StarsU16 label_ids[8];
    StarsU16 label_count;
    StarsI16 costs;
    StarsI16 formats;
    StarsI16 rights;
    StarsI16 texts;
    StarsI16 colors;
    StarsI16 etas;
    StarsI16 eta_formats;
    StarsI16 ironium;
    StarsI16 boranium;
    StarsI16 germanium;
    StarsI16 resources;
    StarsI16 separators;
    StarsI16 headings;
    StarsI16 diamonds;
    StarsI32 values[8];
} Trace;

static StarsU16 getdc(void *context, StarsU16 hwnd)
{
    (void)hwnd;
    ++((Trace *)context)->texts;
    return 3;
}

static void release(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)context; (void)hwnd; (void)dc;
}

static void font(void *context, StarsU16 dc, StarsU16 value)
{
    (void)context; (void)dc; (void)value;
}

static void background(void *context, StarsU16 dc, StarsU32 value)
{
    (void)context; (void)dc; (void)value;
}

static void color(void *context, StarsU16 dc, StarsU32 value)
{
    (void)dc; (void)value;
    ++((Trace *)context)->colors;
}

static StarsI16 extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsI16 length)
{
    (void)context; (void)dc; (void)text;
    return length;
}

static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    Trace *trace = (Trace *)context;
    (void)dc; (void)x; (void)y;
    ++trace->texts;
    if (length == 18 && strncmp((const char *)text,
        "Required Minerals:", 18) == 0)
        ++trace->headings;
    if (length == 2 && strncmp((const char *)text, "kT", 2) == 0)
        ++trace->separators;
    if (length == 7 && strncmp((const char *)text, "Ironium", 7) == 0)
        ++trace->ironium;
    if (length == 8 && strncmp((const char *)text, "Boranium", 8) == 0)
        ++trace->boranium;
    if (length == 9 && strncmp((const char *)text, "Germanium", 9) == 0)
        ++trace->germanium;
    if (length == 9 && strncmp((const char *)text, "Resources", 9) == 0)
        ++trace->resources;
}

static void right_text(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length, StarsI16 width)
{
    (void)dc; (void)x; (void)y; (void)text; (void)length; (void)width;
    ++((Trace *)context)->rights;
}

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    if (trace->compressed_count < 16)
        trace->compressed_ids[trace->compressed_count++] = id;
    return id == 0x2feU ? (const StarsU8 *)"kT" : (const StarsU8 *)0;
}

static const StarsU8 *label(void *context, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    if (trace->label_count < 8)
        trace->label_ids[trace->label_count++] = id;
    if (id == 0x33fU) return (const StarsU8 *)"Required Minerals:";
    if (id == 0x0d2U)
        return (const StarsU8 *)"Apply or define a production template";
    return (const StarsU8 *)0;
}

static void rect(void *context, StarsU16 hwnd, StarsRect *value)
{
    (void)context; (void)hwnd;
    value->left = 10; value->top = 10; value->right = 150; value->bottom = 80;
}

static StarsU16 item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd;
    return (StarsU16)(id + 10U);
}

static StarsI16 selection(void *context, StarsU16 hwnd)
{
    (void)context; (void)hwnd;
    return 1;
}

static StarsI16 costs(void *context, StarsI16 selected, StarsI32 values[4])
{
    Trace *trace = (Trace *)context;
    (void)selected;
    values[0] = 1; values[1] = 20; values[2] = 300; values[3] = 4000;
    ++trace->costs;
    return 1;
}

static StarsI16 format_number(void *context, StarsI32 value, StarsU8 *out,
    StarsU16 capacity)
{
    Trace *trace = (Trace *)context;
    const char *text;
    (void)capacity;
    ++trace->formats;
    if (value == 1) text = "1";
    else if (value == 20) text = "20";
    else if (value == 300) text = "300";
    else text = "4000";
    strcpy((char *)out, text);
    trace->values[trace->formats - 1] = value;
    return (StarsI16)strlen(text);
}

static StarsU32 mineral_color(void *context, StarsU16 row)
{
    (void)row;
    ++((Trace *)context)->colors;
    return 0x100UL + row;
}

static const StarsU8 *eta_format(void *context, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    ++trace->eta_formats;
    return id == 0x340U ? (const StarsU8 *)"%d%% Done,   Completion " :
        (const StarsU8 *)0;
}

static StarsI16 eta(void *context, StarsI16 selected, StarsI16 y)
{
    (void)selected; (void)y;
    ++((Trace *)context)->etas;
    return 1;
}

static void diamond(void *context, StarsU16 dc, const StarsRect *rect_value)
{
    (void)dc; (void)rect_value;
    ++((Trace *)context)->diamonds;
}

static void check(int condition, int *checks, int *failures)
{
    ++*checks;
    if (!condition) ++*failures;
}

int main(void)
{
    static const StarsU8 ironium[] = "Ironium";
    static const StarsU8 boranium[] = "Boranium";
    static const StarsU8 germanium[] = "Germanium";
    static const StarsU8 colonists[] = "Colonists";
    static const StarsU8 fuel[] = "Fuel";
    static const StarsU8 resources[] = "Resources";
    static const StarsU8 *const minerals[6] = {
        ironium, boranium, germanium, colonists, fuel, resources
    };
    StarsDrawProductionDlgOps ops;
    StarsDrawProductionDlgInput input;
    StarsRect diamond_rect;
    Trace trace;
    int checks;
    int failures;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    ops.get_dc = getdc;
    ops.release_dc = release;
    ops.select_font = font;
    ops.set_background_color = background;
    ops.set_text_color = color;
    ops.text_extent = extent;
    ops.text_out = text_out;
    ops.right_text = right_text;
    ops.compressed_string = compressed;
    ops.get_label = label;
    ops.get_window_rect = rect;
    ops.screen_to_client = rect;
    ops.get_client_rect = rect;
    ops.get_dialog_item = item;
    ops.selection = selection;
    ops.get_costs = costs;
    ops.get_eta = eta;
    ops.get_eta_format = eta_format;
    ops.format_number = format_number;
    ops.get_mineral_color = mineral_color;
    ops.draw_diamond = diamond;
    ops.context = &trace;
    input.hwnd = 4;
    input.line_height = 10;
    input.max_mineral_width = 12;
    input.normal_font = 1;
    input.bold_font = 2;
    input.face_color = 7;
    input.diamond_rect = &diamond_rect;
    input.mineral_labels = minerals;
    input.mineral_label_count = 6;
    input.ops = &ops;
    checks = 0;
    failures = 0;

    stars_draw_production_dlg(&input, -1);
    check(trace.costs == 2, &checks, &failures);
    check(trace.formats == 8 && trace.values[0] == 1 &&
        trace.values[7] == 4000, &checks, &failures);
    check(trace.rights == 8, &checks, &failures);
    check(trace.ironium == 2 && trace.boranium == 2 &&
        trace.germanium == 2 && trace.resources == 2, &checks, &failures);
    check(trace.separators == 6, &checks, &failures);
    check(trace.headings == 2, &checks, &failures);
    check(trace.label_count == 3 && trace.label_ids[0] == 0x33fU &&
        trace.label_ids[1] == 0x33fU && trace.label_ids[2] == 0x0d2U,
        &checks, &failures);
    check(trace.compressed_count == 7 && trace.compressed_ids[0] == 0x2feU &&
        trace.compressed_ids[6] == 0x2feU, &checks, &failures);
    check(trace.eta_formats == 1 && trace.etas == 1, &checks, &failures);
    check(trace.colors == 24 && trace.diamonds == 1, &checks, &failures);

#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("DPDRE.TXT", "w");
        if (!report) return 2;
        fprintf(report,
            "DRAW_PRODUCTION_DLG_RESOURCE_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    if (failures)
        puts("DRAW_PRODUCTION_DLG_RESOURCE_CHECKS=10 FAILURES=1 RESULT=FAIL");
    else
        puts("DRAW_PRODUCTION_DLG_RESOURCE_CHECKS=10 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
}
