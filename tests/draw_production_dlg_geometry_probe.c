#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 dc_count;
    StarsI16 release_count;
    StarsI16 item_count;
    StarsU16 item_ids[4];
    StarsI16 window_rects;
    StarsI16 client_rects;
    StarsI16 screen_rects;
    StarsI16 headings;
    StarsI16 mineral_labels;
    StarsI16 separators;
    StarsI16 right_count;
    StarsI16 first_heading_x;
    StarsI16 first_heading_y;
    StarsI16 first_label_x;
    StarsI16 first_label_y;
    StarsI16 first_separator_x;
    StarsI16 first_separator_y;
    StarsI16 first_right_x;
    StarsI16 first_right_y;
    StarsI16 eta_count;
    StarsI16 diamond_count;
    StarsRect diamond;
} Trace;

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)hwnd;
    ++((Trace *)context)->dc_count;
    return 7;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)hwnd;
    (void)dc;
    ++((Trace *)context)->release_count;
}

static void select_font(void *context, StarsU16 dc, StarsU16 font)
{
    (void)context;
    (void)dc;
    (void)font;
}

static void set_color(void *context, StarsU16 dc, StarsU32 color)
{
    (void)context;
    (void)dc;
    (void)color;
}

static StarsI16 extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsI16 length)
{
    (void)context;
    (void)dc;
    (void)text;
    return length;
}

static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    Trace *trace = (Trace *)context;
    (void)dc;
    if (length == 18 && strncmp((const char *)text,
        "Required Minerals:", 18) == 0) {
        if (trace->headings == 0) {
            trace->first_heading_x = x;
            trace->first_heading_y = y;
        }
        ++trace->headings;
    } else if (length == 2 && strncmp((const char *)text, "kT", 2) == 0) {
        if (trace->separators == 0) {
            trace->first_separator_x = x;
            trace->first_separator_y = y;
        }
        ++trace->separators;
    } else if (length == 7 && strncmp((const char *)text, "Ironium", 7) == 0) {
        if (trace->mineral_labels == 0) {
            trace->first_label_x = x;
            trace->first_label_y = y;
        }
        ++trace->mineral_labels;
    }
}

static void right_text(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length, StarsI16 width)
{
    Trace *trace = (Trace *)context;
    (void)dc;
    (void)text;
    (void)length;
    (void)width;
    if (trace->right_count == 0) {
        trace->first_right_x = x;
        trace->first_right_y = y;
    }
    ++trace->right_count;
}

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    (void)context;
    return id == 0x2feU ? (const StarsU8 *)"kT" : (const StarsU8 *)0;
}

static const StarsU8 *label(void *context, StarsU16 id)
{
    (void)context;
    if (id == 0x33fU) return (const StarsU8 *)"Required Minerals:";
    if (id == 0x0d2U)
        return (const StarsU8 *)"Apply or define a production template";
    return (const StarsU8 *)0;
}

static void get_window_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    ++trace->window_rects;
    rect->left = 10;
    rect->top = 20;
    rect->right = 150;
    rect->bottom = 80;
}

static void get_client_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    ++trace->client_rects;
    rect->left = 0;
    rect->top = 0;
    rect->right = 200;
    rect->bottom = 120;
}

static void screen_to_client(void *context, StarsU16 hwnd, StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    (void)rect;
    ++trace->screen_rects;
}

static StarsU16 get_item(void *context, StarsU16 hwnd, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    if (trace->item_count < 4)
        trace->item_ids[trace->item_count] = id;
    ++trace->item_count;
    return id;
}

static StarsI16 selection(void *context, StarsU16 hwnd)
{
    (void)context;
    return hwnd == 0x416U ? 1 : 1;
}

static StarsI16 costs(void *context, StarsI16 selected, StarsI32 values[4])
{
    (void)context;
    (void)selected;
    values[0] = 1;
    values[1] = 20;
    values[2] = 300;
    values[3] = 4000;
    return 1;
}

static StarsI16 format_number(void *context, StarsI32 value, StarsU8 *out,
    StarsU16 capacity)
{
    const char *text;
    (void)context;
    (void)capacity;
    if (value == 1) text = "1";
    else if (value == 20) text = "20";
    else if (value == 300) text = "300";
    else text = "4000";
    strcpy((char *)out, text);
    return (StarsI16)strlen(text);
}

static StarsU32 mineral_color(void *context, StarsU16 row)
{
    (void)context;
    return (StarsU32)row;
}

static StarsI16 eta(void *context, StarsI16 selected, StarsI16 y)
{
    (void)selected;
    (void)y;
    ++((Trace *)context)->eta_count;
    return 1;
}

static const StarsU8 *eta_format(void *context, StarsU16 id)
{
    (void)context;
    return id == 0x340U ? (const StarsU8 *)"%d%% Done,   Completion " :
        (const StarsU8 *)0;
}

static void diamond(void *context, StarsU16 dc, const StarsRect *rect)
{
    Trace *trace = (Trace *)context;
    (void)dc;
    trace->diamond = *rect;
    ++trace->diamond_count;
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
    ops.get_dc = get_dc;
    ops.release_dc = release_dc;
    ops.select_font = select_font;
    ops.set_text_color = set_color;
    ops.text_extent = extent;
    ops.text_out = text_out;
    ops.right_text = right_text;
    ops.compressed_string = compressed;
    ops.get_label = label;
    ops.get_client_rect = get_client_rect;
    ops.get_window_rect = get_window_rect;
    ops.screen_to_client = screen_to_client;
    ops.get_dialog_item = get_item;
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
    input.text_color = 9;
    input.diamond_rect = &diamond_rect;
    input.mineral_labels = minerals;
    input.mineral_label_count = 6;
    input.ops = &ops;
    checks = 0;
    failures = 0;

    stars_draw_production_dlg(&input, -1);
    check(trace.dc_count == 1 && trace.release_count == 1, &checks, &failures);
    check(trace.item_count == 2 && trace.item_ids[0] == 0x416U &&
        trace.item_ids[1] == 0x417U, &checks, &failures);
    check(trace.window_rects == 2 && trace.screen_rects == 2 &&
        trace.client_rects == 1, &checks, &failures);
    check(trace.headings == 2 && trace.first_heading_x == 10 &&
        trace.first_heading_y == 84, &checks, &failures);
    check(trace.mineral_labels == 2 && trace.first_label_x == 30 &&
        trace.first_label_y == 94, &checks, &failures);
    check(trace.separators == 6 && trace.first_separator_x == 128 &&
        trace.first_separator_y == 94, &checks, &failures);
    check(trace.right_count == 8 && trace.first_right_x == 126 &&
        trace.first_right_y == 94, &checks, &failures);
    check(trace.eta_count == 1, &checks, &failures);
    check(trace.diamond_count == 1 && trace.diamond.left == 6 &&
        trace.diamond.top == 83 && trace.diamond.right == 16 &&
        trace.diamond.bottom == 94, &checks, &failures);
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("DPDGRES.TXT", "w");
        if (!report)
            return 2;
        fprintf(report,
            "DRAW_PRODUCTION_DLG_GEOMETRY_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#endif
    printf("DRAW_PRODUCTION_DLG_GEOMETRY_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
