#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct DrawNewGame2LocalizationTrace {
    StarsI16 strings;
    StarsI16 formats;
    StarsI16 texts;
    StarsI16 group_index;
    StarsI16 level_index;
    StarsU16 format_ids[8];
    StarsU16 string_ids[8];
    StarsU8 first[8][20];
    StarsU8 second[8][20];
} DrawNewGame2LocalizationTrace;

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

static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    DrawNewGame2LocalizationTrace *trace;
    StarsI16 slot;
    const char *value;

    trace = (DrawNewGame2LocalizationTrace *)context;
    slot = trace->strings;
    if (slot < 8)
        trace->string_ids[slot] = id;
    ++trace->strings;
    value = id == 0x30cU ? "EMPTY" : id == 0x20bU ? "SPECIAL" :
        id == 0x20dU ? "GENERIC" : "OTHER";
    copy_text(buffer, capacity, (const StarsU8 *)value);
    return (StarsI16)strlen((const char *)buffer);
}

static const StarsU8 *compressed_string(void *context, StarsU16 id)
{
    (void)context;
    if (id == 0x567U + 2U)
        return (const StarsU8 *)"RACE2";
    return id == 0x319U ? (const StarsU8 *)"PLAYER" :
        (const StarsU8 *)"OTHER";
}

static const StarsU8 *ai_group_label(void *context, StarsU16 index)
{
    DrawNewGame2LocalizationTrace *trace;
    trace = (DrawNewGame2LocalizationTrace *)context;
    trace->group_index = (StarsI16)index;
    return (const StarsU8 *)"GROUP3";
}

static const StarsU8 *ai_level_label(void *context, StarsU16 index)
{
    DrawNewGame2LocalizationTrace *trace;
    trace = (DrawNewGame2LocalizationTrace *)context;
    trace->level_index = (StarsI16)index;
    return (const StarsU8 *)"LEVEL2";
}

static StarsI16 format_text(void *context, StarsU8 *buffer, StarsU16 capacity,
    StarsU16 format_id, const StarsU8 *first, const StarsU8 *second)
{
    DrawNewGame2LocalizationTrace *trace;
    StarsI16 slot;
    StarsU16 length;

    trace = (DrawNewGame2LocalizationTrace *)context;
    slot = trace->formats;
    if (slot < 8) {
        trace->format_ids[slot] = format_id;
        copy_text(trace->first[slot], 20U, first);
        copy_text(trace->second[slot], 20U, second);
    }
    ++trace->formats;
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
    DrawNewGame2LocalizationTrace *trace;
    (void)dc; (void)x; (void)y; (void)text; (void)count;
    trace = (DrawNewGame2LocalizationTrace *)context;
    ++trace->texts;
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
    DrawNewGame2LocalizationTrace trace;
    StarsI16 checks;
    StarsI16 failures;
    FILE *report;

    memset(players, 0, sizeof players);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    players[1].type = (StarsU8)(1U | (2U << 2));
    players[2].type = (StarsU8)(1U | (6U << 2));
    players[3].type = (StarsU8)(1U | (7U << 2));
    players[4].type = (StarsU8)(2U | (3U << 2));
    strcpy((char *)players[4].name, "NAME4");
    strcpy((char *)players[4].file_name, "FILE4");
    players[5].type = (StarsU8)(2U | (4U << 2));
    strcpy((char *)players[5].name, "NAME5");
    players[6].type = (StarsU8)(2U | (5U << 2));
    strcpy((char *)players[6].name, "NAME6");
    strcpy((char *)players[6].file_name, "FILE6");
    players[7].type = (StarsU8)(3U | (3U << 2) | (2U << 5));
    players[8].type = 4U;

    ops.get_dc = get_dc;
    ops.release_dc = release_dc;
    ops.select_font = select_font;
    ops.set_bk_mode = set_bk_mode;
    ops.text_extent = text_extent;
    ops.get_string = get_string;
    ops.compressed_string = compressed_string;
    ops.ai_group_label = ai_group_label;
    ops.ai_level_label = ai_level_label;
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
    input.ops = &ops;

    checks = 0;
    failures = 0;
    stars_draw_new_game2(&input, -1);
    failures += check(trace.texts == 16, &checks);
    failures += check(trace.strings == 11, &checks);
    failures += check(trace.formats == 5, &checks);
    failures += check(trace.string_ids[0] == 0x30cU, &checks);
    failures += check(trace.string_ids[1] == 0x20bU, &checks);
    failures += check(trace.string_ids[2] == 0x20dU, &checks);
    failures += check(trace.group_index == 3 && trace.level_index == 2,
        &checks);
    failures += check(trace.format_ids[0] == 0x310U &&
        strcmp((const char *)trace.first[0], "RACE2") == 0, &checks);
    failures += check(trace.format_ids[1] == 0x311U &&
        strcmp((const char *)trace.first[1], "NAME4") == 0 &&
        strcmp((const char *)trace.second[1], "FILE4") == 0, &checks);
    failures += check(trace.format_ids[2] == 0x310U &&
        strcmp((const char *)trace.first[2], "NAME5") == 0, &checks);
    failures += check(trace.format_ids[3] == 0x311U &&
        strcmp((const char *)trace.first[3], "NAME6") == 0 &&
        strcmp((const char *)trace.second[3], "FILE6") == 0, &checks);
    failures += check(trace.format_ids[4] == 0x312U &&
        strcmp((const char *)trace.first[4], "GROUP3") == 0 &&
        strcmp((const char *)trace.second[4], "LEVEL2") == 0, &checks);

    input.game_flags = 0x0040UL;
    memset(&trace, 0, sizeof trace);
    stars_draw_new_game2(&input, -1);
    failures += check(trace.formats == 5 && trace.format_ids[0] == 0x310U,
        &checks);
    failures += check(trace.format_ids[1] == 0x0aeeU &&
        trace.format_ids[2] == 0x310U && trace.format_ids[3] == 0x0aeeU,
        &checks);
    failures += check(strcmp((const char *)trace.first[3], "FILE6") == 0 &&
        trace.second[3][0] == 0, &checks);

    report = fopen("C:\\DNG2L\\DNG2LE2.TXT", "w");
    if (report) {
        fprintf(report,
            "DRAW_NEW_GAME2_LOCALIZATION_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("DRAW_NEW_GAME2_LOCALIZATION_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
