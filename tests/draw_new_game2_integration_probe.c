#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct DrawNewGame2IntegrationTrace {
    StarsI16 get_dc_calls;
    StarsI16 release_calls;
    StarsI16 mode_calls;
    StarsI16 font_calls;
    StarsI16 extent_calls;
    StarsI16 right_calls;
    StarsI16 diamond_calls;
    StarsI16 text_calls;
    StarsI16 color_calls;
    StarsU16 mode_values[4];
    StarsU32 color_values[40];
    StarsRect first_rect;
    StarsRect last_rect;
    StarsI16 first_right_x;
    StarsI16 first_text_x;
    StarsI16 first_text_y;
    StarsI16 second_text_y;
    StarsI16 last_text_y;
    StarsU8 first_text[96];
    StarsU8 last_label[8];
    StarsU16 string_ids[8];
    StarsI16 strings;
    StarsU16 compressed_ids[8];
    StarsI16 compressed;
} DrawNewGame2IntegrationTrace;

static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    DrawNewGame2IntegrationTrace *trace;
    (void)hwnd;
    trace = (DrawNewGame2IntegrationTrace *)context;
    ++trace->get_dc_calls;
    return 0x33U;
}

static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    DrawNewGame2IntegrationTrace *trace;
    (void)hwnd; (void)dc;
    trace = (DrawNewGame2IntegrationTrace *)context;
    ++trace->release_calls;
}

static void select_font(void *context, StarsU16 dc, StarsU16 font)
{
    DrawNewGame2IntegrationTrace *trace;
    (void)dc; (void)font;
    trace = (DrawNewGame2IntegrationTrace *)context;
    ++trace->font_calls;
}

static StarsU16 set_bk_mode(void *context, StarsU16 dc, StarsU16 mode)
{
    DrawNewGame2IntegrationTrace *trace;
    StarsI16 slot;
    (void)dc;
    trace = (DrawNewGame2IntegrationTrace *)context;
    slot = trace->mode_calls;
    if (slot < 4)
        trace->mode_values[slot] = mode;
    ++trace->mode_calls;
    return slot == 0 ? 7U : 7U;
}

static StarsI16 text_extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsI16 count)
{
    DrawNewGame2IntegrationTrace *trace;
    (void)dc; (void)text; (void)count;
    trace = (DrawNewGame2IntegrationTrace *)context;
    ++trace->extent_calls;
    return 37;
}

static StarsI16 get_string(void *context, StarsU16 id, StarsU8 *buffer,
    StarsU16 capacity)
{
    DrawNewGame2IntegrationTrace *trace;
    const char *value;
    StarsI16 slot;
    trace = (DrawNewGame2IntegrationTrace *)context;
    slot = trace->strings;
    if (slot < 8)
        trace->string_ids[slot] = id;
    ++trace->strings;
    value = id == 0x20cU ? "INACTIVE" : "EMPTY";
    if (capacity > 0U) {
        strncpy((char *)buffer, value, (size_t)capacity - 1U);
        buffer[capacity - 1U] = 0;
    }
    return (StarsI16)strlen((const char *)buffer);
}

static const StarsU8 *compressed_string(void *context, StarsU16 id)
{
    DrawNewGame2IntegrationTrace *trace;
    StarsI16 slot;
    trace = (DrawNewGame2IntegrationTrace *)context;
    slot = trace->compressed;
    if (slot < 8)
        trace->compressed_ids[slot] = id;
    ++trace->compressed;
    if (id == 800U)
        return (const StarsU8 *)"DEAD";
    return (const StarsU8 *)"PLAYERHEAD";
}

static StarsI16 format_text(void *context, StarsU8 *buffer, StarsU16 capacity,
    StarsU16 format_id, const StarsU8 *first, const StarsU8 *second)
{
    (void)context; (void)format_id; (void)second;
    if (capacity > 0U) {
        strncpy((char *)buffer, first ? (const char *)first : "",
            (size_t)capacity - 1U);
        buffer[capacity - 1U] = 0;
    }
    return (StarsI16)strlen((const char *)buffer);
}

static void text_out(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 count)
{
    DrawNewGame2IntegrationTrace *trace;
    (void)dc; (void)count;
    trace = (DrawNewGame2IntegrationTrace *)context;
    if (trace->text_calls == 0) {
        trace->first_text_x = x;
        trace->first_text_y = y;
        strncpy((char *)trace->first_text, (const char *)text,
            sizeof trace->first_text - 1U);
        trace->first_text[sizeof trace->first_text - 1U] = 0;
    } else if (trace->text_calls == 1) {
        trace->second_text_y = y;
    }
    trace->last_text_y = y;
    ++trace->text_calls;
}

static void right_text(void *context, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 count, StarsI16 width)
{
    DrawNewGame2IntegrationTrace *trace;
    (void)dc; (void)y; (void)width;
    trace = (DrawNewGame2IntegrationTrace *)context;
    if (trace->right_calls == 0)
        trace->first_right_x = x;
    if (count < (StarsI16)sizeof trace->last_label) {
        memcpy(trace->last_label, text, (size_t)count);
        trace->last_label[count] = 0;
    }
    ++trace->right_calls;
}

static void set_text_color(void *context, StarsU16 dc, StarsU32 color)
{
    DrawNewGame2IntegrationTrace *trace;
    (void)dc;
    trace = (DrawNewGame2IntegrationTrace *)context;
    if (trace->color_calls < (StarsI16)(sizeof trace->color_values /
            sizeof trace->color_values[0]))
        trace->color_values[trace->color_calls] = color;
    ++trace->color_calls;
}

static void draw_diamond(void *context, StarsU16 dc, const StarsRect *rect)
{
    DrawNewGame2IntegrationTrace *trace;
    (void)dc;
    trace = (DrawNewGame2IntegrationTrace *)context;
    if (trace->diamond_calls == 0)
        trace->first_rect = *rect;
    trace->last_rect = *rect;
    ++trace->diamond_calls;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

static void set_ops(StarsDrawNewGame2Ops *ops,
    DrawNewGame2IntegrationTrace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->get_dc = get_dc;
    ops->release_dc = release_dc;
    ops->select_font = select_font;
    ops->set_bk_mode = set_bk_mode;
    ops->text_extent = text_extent;
    ops->get_string = get_string;
    ops->compressed_string = compressed_string;
    ops->format_text = format_text;
    ops->text_out = text_out;
    ops->right_text = right_text;
    ops->set_text_color = set_text_color;
    ops->draw_diamond = draw_diamond;
    ops->context = trace;
}

int main(void)
{
    StarsDrawNewGame2Player players[16];
    StarsDrawNewGame2Ops ops;
    StarsDrawNewGame2Input input;
    DrawNewGame2IntegrationTrace trace;
    StarsI16 checks;
    StarsI16 failures;
    FILE *report;

    memset(players, 0, sizeof players);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    players[0].type = 0U;
    players[1].type = (StarsU8)(1U | (6U << 2));
    set_ops(&ops, &trace);
    input.hwnd = 4U;
    input.line_height = 10;
    input.text_color = 0x11U;
    input.dead_color = 0x22U;
    input.bold_font = 9U;
    input.players = players;
    input.ops = &ops;
    checks = 0;
    failures = 0;

    stars_draw_new_game2(&input, -1);
    failures += check(trace.get_dc_calls == 1 && trace.release_calls == 1,
        &checks);
    failures += check(trace.mode_calls == 2 && trace.mode_values[0] == 1U &&
        trace.mode_values[1] == 7U, &checks);
    failures += check(trace.font_calls == 1 && trace.extent_calls == 1,
        &checks);
    failures += check(trace.diamond_calls == 16 && trace.text_calls == 16 &&
        trace.right_calls == 16, &checks);
    failures += check(trace.first_rect.left == 49U &&
        trace.first_rect.top == 6U && trace.first_rect.right == 60U &&
        trace.first_rect.bottom == 17U, &checks);
    failures += check(trace.last_rect.top == 216U &&
        trace.last_rect.bottom == 227U, &checks);
    failures += check(trace.first_right_x == 43 && trace.first_text_x == 66 &&
        trace.first_text_y == 6 && trace.second_text_y == 20 &&
        trace.last_text_y == 216, &checks);
    failures += check(strcmp((const char *)trace.last_label, "16:") == 0,
        &checks);
    failures += check(trace.color_calls == 16 && trace.color_values[0] ==
        0x11UL && trace.color_values[15] == 0x11UL, &checks);
    failures += check(trace.compressed == 1 &&
        trace.compressed_ids[0] == 0x319U, &checks);

    memset(&trace, 0, sizeof trace);
    set_ops(&ops, &trace);
    input.hdc = 0x44U;
    input.read_only = 1;
    input.player_count = 2;
    players[0].active = 1;
    players[0].dead = 1;
    strcpy((char *)players[0].name, "ALPHA");
    players[1].active = 0;
    stars_draw_new_game2(&input, -1);
    failures += check(trace.get_dc_calls == 0 && trace.release_calls == 0,
        &checks);
    failures += check(trace.diamond_calls == 2 && trace.text_calls == 2 &&
        trace.right_calls == 2, &checks);
    failures += check(trace.strings == 1 && trace.string_ids[0] == 0x20cU,
        &checks);
    failures += check(trace.compressed == 2 &&
        trace.compressed_ids[0] == 0x319U &&
        trace.compressed_ids[1] == 800U, &checks);
    failures += check(trace.color_calls == 3 && trace.color_values[0] ==
        0x22UL && trace.color_values[1] == 0x11UL &&
        trace.color_values[2] == 0x11UL, &checks);
    failures += check(strcmp((const char *)trace.first_text, "ALPHA (DEAD)") == 0,
        &checks);

    report = fopen("C:\\DNG2I\\DNG2IE3.TXT", "w");
    if (report) {
        fprintf(report,
            "DRAW_NEW_GAME2_INTEGRATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("DRAW_NEW_GAME2_INTEGRATION_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
