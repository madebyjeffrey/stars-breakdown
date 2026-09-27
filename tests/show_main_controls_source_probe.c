#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 ids[10];
    StarsI16 shows[10];
    StarsU16 count;
    StarsI16 string_id;
    StarsU16 text_parent;
    StarsU16 text_id;
    const StarsU8 *text;
} Trace;

static StarsU16 get_item(void *context, StarsU16 hwnd, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    trace->ids[trace->count] = id;
    return (StarsU16)(0x400U + id);
}

static void show_window(void *context, StarsU16 control, StarsI16 show)
{
    Trace *trace = (Trace *)context;
    trace->shows[trace->count] = show;
    trace->count++;
    (void)control;
}

static const StarsU8 *get_string(void *context, StarsI16 id)
{
    Trace *trace = (Trace *)context;
    trace->string_id = id;
    return id == 0x02e2 ? (const StarsU8 *)"S2" : (const StarsU8 *)"S3";
}

static void set_text(void *context, StarsU16 parent, StarsU16 id,
    const StarsU8 *text)
{
    Trace *trace = (Trace *)context;
    trace->text_parent = parent;
    trace->text_id = id;
    trace->text = text;
}

static int check_trace(Trace *trace, StarsI16 show, StarsI16 button_show,
    StarsI16 string_id, const char *text)
{
    static const StarsU16 ids[10] = {
        0x0816U, 0x0818U, 0x0817U, 0x0810U, 0x0811U,
        0x0812U, 0x0813U, 0x0814U, 0x0815U, 1U
    };
    StarsU16 i;
    if (trace->count != 10U || trace->string_id != string_id ||
        trace->text_parent != 77U || trace->text_id != 2U ||
        strcmp((const char *)trace->text, text) != 0)
        return 0;
    for (i = 0; i < 9U; ++i)
        if (trace->ids[i] != ids[i] || trace->shows[i] != show) return 0;
    return trace->ids[9] == ids[9] && trace->shows[9] == button_show;
}

int main(void)
{
    StarsShowMainControlsInput input;
    Trace trace;
    memset(&trace, 0, sizeof(trace));
    input.hwnd = 77U;
    input.show = 5;
    input.get_dialog_item = get_item;
    input.show_window = show_window;
    input.get_string = get_string;
    input.set_dialog_item_text = set_text;
    input.context = &trace;
    stars_show_main_controls(&input);
    if (!check_trace(&trace, 5, 0, 0x02e2, "S2")) return 1;
    memset(&trace, 0, sizeof(trace));
    input.show = 0;
    stars_show_main_controls(&input);
    if (!check_trace(&trace, 0, 5, 0x02e3, "S3")) return 2;
    puts("SHOW_MAIN_CONTROLS_SOURCE=PASS");
    return 0;
}
