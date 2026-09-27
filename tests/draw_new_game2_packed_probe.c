#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct DrawNewGame2PackedTrace {
    StarsI16 formats;
    StarsI16 strings;
    StarsI16 text_calls;
    StarsI16 saw_race;
    StarsI16 saw_file;
    StarsI16 saw_direct_file;
    StarsU16 last_format;
    StarsU8 last_first[20];
    StarsU8 last_second[20];
} DrawNewGame2PackedTrace;

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)context; (void)hwnd; return 3U;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)context; (void)hwnd; (void)dc;
}

static void select_font(void *context, StarsU16 dc, StarsU16 font)
{
    (void)context; (void)dc; (void)font;
}

static StarsU16 set_bk_mode(void *context, StarsU16 dc, StarsU16 mode)
{
    (void)context; (void)dc; (void)mode; return 1U;
}

static StarsI16 text_extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsI16 count)
{
    (void)context; (void)dc; (void)text; return count;
}

static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    DrawNewGame2PackedTrace *trace;
    const char *value;
    trace = (DrawNewGame2PackedTrace *)context;
    ++trace->strings;
    value = id == 0x30cU ? "EMPTY" : "STRING";
    if (capacity > 0U) {
        strncpy((char *)buffer, value, (size_t)capacity - 1U);
        buffer[capacity - 1U] = 0;
    }
    return (StarsI16)strlen((const char *)buffer);
}

static const StarsU8 *compressed_string(void *context, StarsU16 id)
{
    (void)context; (void)id; return (const StarsU8 *)"RACE";
}

static void copy_text(StarsU8 *destination, StarsU16 capacity,
    const StarsU8 *source)
{
    if (capacity == 0U)
        return;
    if (!source)
        source = (const StarsU8 *)"";
    strncpy((char *)destination, (const char *)source, (size_t)capacity - 1U);
    destination[capacity - 1U] = 0;
}

static StarsI16 format_text(void *context, StarsU8 *buffer, StarsU16 capacity,
    StarsU16 format_id, const StarsU8 *first, const StarsU8 *second)
{
    DrawNewGame2PackedTrace *trace;
    StarsU16 length;
    trace = (DrawNewGame2PackedTrace *)context;
    ++trace->formats;
    trace->last_format = format_id;
    copy_text(trace->last_first, (StarsU16)sizeof trace->last_first, first);
    copy_text(trace->last_second, (StarsU16)sizeof trace->last_second, second);
    if (format_id == 0x310U)
        ++trace->saw_race;
    if (format_id == 0x311U)
        ++trace->saw_file;
    if (format_id == 0x0aeeU)
        ++trace->saw_direct_file;
    if (capacity == 0U)
        return 0;
    copy_text(buffer, capacity, first);
    length = (StarsU16)strlen((const char *)buffer);
    if (second && second[0] != 0U && length + 1U < capacity) {
        buffer[length++] = '/';
        copy_text(buffer + length, (StarsU16)(capacity - length), second);
        length = (StarsU16)strlen((const char *)buffer);
    }
    return (StarsI16)length;
}

static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 count)
{
    DrawNewGame2PackedTrace *trace;
    (void)dc; (void)x; (void)y; (void)text; (void)count;
    trace = (DrawNewGame2PackedTrace *)context;
    ++trace->text_calls;
}

static void right_text(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 count, StarsI16 width)
{
    (void)context; (void)dc; (void)x; (void)y; (void)text;
    (void)count; (void)width;
}

static void set_text_color(void *context, StarsU16 dc, StarsU32 color)
{
    (void)context; (void)dc; (void)color;
}

static void draw_diamond(void *context, StarsU16 dc, const StarsRect *rect)
{
    (void)context; (void)dc; (void)rect;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsDrawNewGame2Player players[16];
    StarsDrawNewGame2Ops ops;
    StarsDrawNewGame2Input input;
    DrawNewGame2PackedTrace trace;
    StarsU8 packed_types[16];
    StarsU8 packed_files[6 * 13];
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 index;
    FILE *report;

    memset(players, 0, sizeof players);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    memset(packed_types, 0, sizeof packed_types);
    memset(packed_files, 0, sizeof packed_files);
    for (index = 0; index < 16; ++index)
        players[index].type = 0U;
    strcpy((char *)players[2].name, "Typed2");
    strcpy((char *)players[2].file_name, "Wrong2");
    strcpy((char *)players[3].name, "Typed3");
    strcpy((char *)players[3].file_name, "Wrong3");
    strcpy((char *)players[4].name, "Typed4");
    packed_types[1] = (StarsU8)(1U | (2U << 2));
    packed_types[2] = (StarsU8)(2U | (3U << 2));
    packed_types[3] = (StarsU8)(2U | (4U << 2));
    packed_types[4] = (StarsU8)(2U | (5U << 2));
    strcpy((char *)(packed_files + 3 * 13), "Packed3");
    strcpy((char *)(packed_files + 5 * 13), "Packed5");

    ops.get_dc = get_dc;
    ops.release_dc = release_dc;
    ops.select_font = select_font;
    ops.set_bk_mode = set_bk_mode;
    ops.text_extent = text_extent;
    ops.get_string = get_string;
    ops.compressed_string = compressed_string;
    ops.format_text = format_text;
    ops.text_out = text_out;
    ops.right_text = right_text;
    ops.set_text_color = set_text_color;
    ops.draw_diamond = draw_diamond;
    ops.context = &trace;
    input.hwnd = 4U;
    input.line_height = 10;
    input.text_color = 1U;
    input.dead_color = 2U;
    input.bold_font = 9U;
    input.players = players;
    input.packed_player_types = packed_types;
    input.packed_player_type_count = 16U;
    input.packed_race_files = packed_files;
    input.packed_race_file_count = 6U;
    input.packed_race_file_stride = 13U;
    input.ops = &ops;

    checks = 0;
    failures = 0;
    stars_draw_new_game2(&input, -1);
    failures += check(trace.text_calls == 16, &checks);
    failures += check(trace.strings == 12, &checks);
    failures += check(trace.formats == 4, &checks);
    failures += check(trace.saw_race == 2, &checks);
    failures += check(trace.saw_file == 2, &checks);
    failures += check(trace.saw_direct_file == 0, &checks);
    failures += check(trace.last_format == 0x311U, &checks);
    failures += check(strcmp((const char *)trace.last_first, "Typed4") == 0,
        &checks);
    failures += check(strcmp((const char *)trace.last_second, "Packed5") == 0,
        &checks);
    failures += check(packed_types[2] == 14U && packed_types[3] == 18U,
        &checks);
    failures += check(packed_files[3 * 13] == 'P' &&
        packed_files[5 * 13] == 'P', &checks);

    input.game_flags = 0x0040UL;
    memset(&trace, 0, sizeof trace);
    stars_draw_new_game2(&input, -1);
    failures += check(trace.saw_direct_file == 2, &checks);
    failures += check(strcmp((const char *)trace.last_first, "Packed5") == 0,
        &checks);

    report = fopen("C:\\DNG2P\\DNG2PE1.TXT", "w");
    if (report) {
        fprintf(report,
            "DRAW_NEW_GAME2_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("DRAW_NEW_GAME2_PACKED_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
