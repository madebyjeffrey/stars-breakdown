#include <stdio.h>
#include <string.h>
#include "core.h"

#define RAW_SHDEF_SIZE 147
#define RAW_NAME_OFFSET 8
#define RAW_FLAGS_OFFSET 123
#define RAW_CEXIST_OFFSET 131

typedef struct Trace {
    StarsI16 resets;
    StarsI16 adds;
    StarsI16 ends;
    StarsI16 enables;
    StarsI16 edit_enabled;
    StarsI16 delete_enabled;
    StarsI16 list_count;
    StarsU8 names[4][40];
} Trace;

static void put_u16(StarsU8 *record, StarsI16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)(value & 0xffU);
    record[offset + 1] = (StarsU8)(value >> 8);
}

static void put_u32(StarsU8 *record, StarsI16 offset, StarsU32 value)
{
    record[offset] = (StarsU8)(value & 0xffUL);
    record[offset + 1] = (StarsU8)((value >> 8) & 0xffUL);
    record[offset + 2] = (StarsU8)((value >> 16) & 0xffUL);
    record[offset + 3] = (StarsU8)((value >> 24) & 0xffUL);
}

static void put_record(StarsU8 *records, StarsI16 index, StarsU16 flags,
    StarsU32 existing, const char *name)
{
    StarsU8 *record;
    StarsI16 length;

    record = records + (long)index * RAW_SHDEF_SIZE;
    memset(record, 0, RAW_SHDEF_SIZE);
    put_u16(record, RAW_FLAGS_OFFSET, flags);
    put_u32(record, RAW_CEXIST_OFFSET, existing);
    length = (StarsI16)strlen(name);
    if (length > 31)
        length = 31;
    memcpy(record + RAW_NAME_OFFSET, name, (size_t)length);
}

static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, const StarsU8 *text)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    (void)wparam;
    if (message == 0x40b)
        ++trace->resets;
    else if (message == 0x403) {
        if (trace->adds < 4 && text)
            strcpy((char *)trace->names[trace->adds], (const char *)text);
        ++trace->adds;
    } else if (message == 0x40e)
        ++trace->ends;
}

static void enable_window(void *context, StarsU16 item, StarsU16 enable)
{
    Trace *trace;

    trace = (Trace *)context;
    ++trace->enables;
    if (item == 0x822 && enable)
        trace->edit_enabled = 1;
    if (item == 0x821 && enable)
        trace->delete_enabled = 1;
}

static StarsU16 get_dialog_item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context;
    (void)hwnd;
    return (StarsU16)(id + 10);
}

static StarsI16 queued(void *context, StarsI16 index, StarsI16 *value)
{
    (void)context;
    (void)index;
    *value = 0;
    return 1;
}

static StarsI16 list_count(void *context, StarsU16 hwnd)
{
    Trace *trace;

    trace = (Trace *)context;
    (void)hwnd;
    trace->list_count = trace->adds;
    return trace->list_count;
}

static void set_window_rect(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)context;
    (void)hwnd;
    rect->left = 4;
    rect->top = 0;
    rect->right = 100;
    rect->bottom = 20;
}

static void set_window_pos(void *context, StarsU16 hwnd, StarsU16 insert,
    StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height, StarsU16 flags)
{
    (void)context;
    (void)hwnd;
    (void)insert;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)flags;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsU8 records[16 * RAW_SHDEF_SIZE];
    StarsFillBuildDesign designs[16];
    StarsFillBuildInput input;
    Trace trace;
    StarsI16 i;
    StarsI16 checks;
    StarsI16 failures;
    FILE *report;

    memset(records, 0, sizeof records);
    memset(designs, 0, sizeof designs);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    for (i = 0; i < 16; ++i)
        put_record(records, i, 0x0200, 0UL, "Hidden");
    put_record(records, 0, 0, 1UL, "Packed0");
    put_record(records, 1, 0, 0UL, "Packed1");
    put_record(records, 2, 0x0200, 0UL, "Hidden");
    designs[0].visible = 0;
    strcpy((char *)designs[0].name, "Shadow0");
    designs[1].visible = 1;
    strcpy((char *)designs[1].name, "Shadow1");
    designs[1].has_hull = 1;
    input.dropdown = 3;
    input.slot_dialog = 4;
    input.mode = 0;
    input.line_height = 15;
    input.parent_width = 200;
    input.designs = designs;
    input.packed_designs = records;
    input.packed_design_stride = RAW_SHDEF_SIZE;
    input.send_message = send_message;
    input.enable_window = enable_window;
    input.get_dialog_item = get_dialog_item;
    input.queued = queued;
    input.list_count = list_count;
    input.get_window_rect = set_window_rect;
    input.set_window_pos = set_window_pos;
    input.context = &trace;
    stars_fill_build_dropdown(&input);
    checks = 0;
    failures = 0;
    failures += check(trace.resets == 1 && trace.ends == 1, &checks);
    failures += check(trace.adds == 2 && trace.list_count == 2, &checks);
    failures += check(strcmp((char *)trace.names[0], "Packed0") == 0 &&
        strcmp((char *)trace.names[1], "Packed1") == 0, &checks);
    failures += check(strcmp((char *)trace.names[0], "Shadow0") != 0 &&
        strcmp((char *)trace.names[1], "Shadow1") != 0, &checks);
    failures += check(trace.delete_enabled && !trace.edit_enabled, &checks);
    failures += check(trace.enables == 3, &checks);
    failures += check(records[0] == 0 && records[RAW_NAME_OFFSET] == 'P' &&
        records[RAW_FLAGS_OFFSET + 1] == 0, &checks);
    failures += check(records[2 * RAW_SHDEF_SIZE + RAW_NAME_OFFSET] == 'H',
        &checks);
    report = fopen("C:\\FBDP\\FBDPE1.TXT", "w");
    if (report) {
        fprintf(report,
            "FILL_BUILD_DD_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("FILL_BUILD_DD_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
