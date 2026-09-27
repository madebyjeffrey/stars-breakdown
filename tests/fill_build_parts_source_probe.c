#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 reset;
    StarsU16 count;
    StarsU16 groups[8];
    StarsU16 items[8];
    char text[8][5];
} Trace;

static StarsI16 lookup(void *context, StarsU16 group, StarsU16 item,
    StarsU16 *bitmap)
{
    Trace *trace = (Trace *)context;
    if (group == 1U) {
        if (item == 0U) { *bitmap = 0U; return 1; }
        if (item == 1U) { *bitmap = 26U; return 1; }
        return 0;
    }
    if (group == 4U) {
        if (item == 0U) { *bitmap = 51U; return 1; }
        return 0;
    }
    if (group == 0x0800U) {
        if (item == 15U || item == 16U) { *bitmap = item; return 1; }
        return 0;
    }
    (void)trace;
    return 0;
}

static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    const StarsU8 *text)
{
    Trace *trace = (Trace *)context;
    (void)hwnd;
    if (message == 0x0405U) { trace->reset++; return; }
    trace->groups[trace->count] = text[0];
    trace->items[trace->count] = text[1];
    strcpy(trace->text[trace->count], (const char *)text);
    trace->count++;
}

int main(void)
{
    StarsFillBuildPartsInput input;
    Trace trace;
    memset(&input, 0, sizeof(input));
    memset(&trace, 0, sizeof(trace));
    input.hwnd_list = 77U;
    input.component_groups = 5U;
    input.lookup_part = lookup;
    input.send_message = send_message;
    input.context = &trace;
    stars_fill_build_parts_lb(&input);
    if (trace.reset != 1U || trace.count != 3U ||
        strcmp(trace.text[0], "AAAA") != 0 ||
        strcmp(trace.text[1], "ABAB") != 0 ||
        strcmp(trace.text[2], "CAZB") != 0)
        return 1;
    memset(&trace, 0, sizeof(trace));
    input.component_groups = 0x0800U;
    input.starbase_mode = 1;
    stars_fill_build_parts_lb(&input);
    if (trace.reset != 1U || trace.count != 0U) return 2;
    puts("FILL_BUILD_PARTS_SOURCE=PASS");
    return 0;
}
