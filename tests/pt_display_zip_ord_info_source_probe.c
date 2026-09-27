#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct ZipInfoTrace {
    StarsI16 font_calls;
    StarsU16 fonts[16];
    StarsI16 string_calls;
    StarsI16 ids[16];
    StarsI16 stream_calls;
    StarsI16 stream_ids[16];
    StarsI16 stream_x[16];
    StarsI16 stream_y[16];
    StarsI16 stream_print[16];
    StarsI16 center_calls;
    StarsI16 center_x;
    StarsI16 center_y;
    StarsI16 center_length;
} ZipInfoTrace;

static void select_font(void *opaque, StarsU16 dc, StarsU16 font)
{
    ZipInfoTrace *trace = (ZipInfoTrace *)opaque;
    (void)dc;
    trace->fonts[trace->font_calls++] = font;
}

static StarsI16 get_string(void *opaque, StarsI16 id, StarsU8 *text,
    StarsU16 capacity)
{
    ZipInfoTrace *trace = (ZipInfoTrace *)opaque;
    if (trace->string_calls < 16)
        trace->ids[trace->string_calls] = id;
    ++trace->string_calls;
    if (capacity >= 4U) {
        text[0] = 'T'; text[1] = 'X'; text[2] = 'T'; text[3] = 0;
    }
    return id == 0x0267 ? 3 : 3;
}

static void center_text(void *opaque, StarsU16 dc, StarsI16 x, StarsI16 y,
    const StarsU8 *text, StarsI16 length)
{
    ZipInfoTrace *trace = (ZipInfoTrace *)opaque;
    ++trace->center_calls; trace->center_x = x; trace->center_y = y;
    trace->center_length = length;
    (void)dc; (void)text;
}

static void stream_text(void *opaque, StarsU16 dc, StarsI16 *x, StarsI16 y,
    const StarsU8 *text, StarsI16 length, StarsI16 print)
{
    ZipInfoTrace *trace = (ZipInfoTrace *)opaque;
    StarsI16 slot = trace->stream_calls;
    if (slot < 16) {
        trace->stream_x[slot] = *x;
        trace->stream_y[slot] = y;
        trace->stream_print[slot] = print;
        trace->stream_ids[slot] = length;
    }
    ++trace->stream_calls;
    *x = (StarsI16)(*x + 10);
    (void)dc; (void)text;
}

static void setup(StarsZipOrderInfoInput *input, StarsZipOrderInfoOps *ops,
    ZipInfoTrace *trace)
{
    (void)memset(input, 0, sizeof(*input));
    (void)memset(ops, 0, sizeof(*ops));
    (void)memset(trace, 0, sizeof(*trace));
    input->font_normal = 1U; input->font_bold = 2U;
    input->arial8_height = 8; input->ops = ops; input->context = trace;
    ops->select_font = select_font; ops->get_string = get_string;
    ops->center_text = center_text; ops->stream_text = stream_text;
}

int main(void)
{
    StarsZipOrderInfoInput input;
    StarsZipOrderInfoOps ops;
    ZipInfoTrace trace;
    StarsPoint point;
    StarsI16 expected[13] = { 0x0267, 0x023f, 0x023e, 0x023d, 0x023c,
        0x0238, 0x0239, 0x023a, 0x023b, 0x0236, 0x0237,
        0x0234, 0x0235 };
    StarsI16 i;
    int failures = 0;

    setup(&input, &ops, &trace);
    point = stars_pt_display_zip_ord_info(9U, 100, 1, &input);
    if (point.x != 28 || point.y != 100 || trace.center_calls != 1 ||
            trace.center_x != 100 || trace.center_y != 4 ||
            trace.center_length != 3 || trace.string_calls != 13 ||
            trace.stream_calls != 12 || trace.font_calls != 10)
        ++failures;
    for (i = 0; i < 13; ++i)
        if (trace.ids[i] != expected[i]) ++failures;
    if (trace.stream_y[0] != 12 || trace.stream_y[3] != 36 ||
            trace.stream_y[4] != 52 || trace.stream_y[6] != 64 ||
            trace.stream_y[8] != 76 || trace.stream_y[10] != 88 ||
            trace.stream_x[4] != 4 || trace.stream_x[5] != 14 ||
            trace.stream_print[11] != 1)
        ++failures;
    setup(&input, &ops, &trace);
    point = stars_pt_display_zip_ord_info(9U, 100, 0, &input);
    if (point.x != 28 || point.y != 100 || trace.center_calls != 0 ||
            trace.string_calls != 12 || trace.stream_calls != 12 ||
            trace.stream_print[0] != 0)
        ++failures;
    if (failures == 0)
        puts("PT_DISPLAY_ZIP_ORD_INFO_SOURCE=PASS");
    else
        puts("PT_DISPLAY_ZIP_ORD_INFO_SOURCE=FAIL");
    return failures;
}
