#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 creates;
    StarsI16 deletes;
    StarsI16 bit_blits;
    StarsI16 pat_blits;
    StarsI16 message_id_calls;
    StarsI16 colors;
    StarsU16 last_width;
} Trace;

static StarsU16 create_dc(void *context, StarsU16 hdc)
{
    (void)hdc; ++((Trace *)context)->creates; return 7;
}
static StarsU16 select_object(void *context, StarsU16 dc, StarsU16 object)
{
    (void)context; (void)dc; (void)object; return 9;
}
static void delete_dc(void *context, StarsU16 dc)
{
    (void)dc; ++((Trace *)context)->deletes;
}
static StarsU32 set_color(void *context, StarsU16 hdc, StarsU32 color)
{
    (void)hdc; (void)color; ++((Trace *)context)->colors; return 0x1234UL;
}
static void bit_blt(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 dc, StarsI16 sx, StarsI16 sy,
    StarsU32 rop)
{
    Trace *trace = (Trace *)context;
    (void)hdc; (void)x; (void)y; (void)height; (void)dc; (void)sx;
    (void)sy; (void)rop; ++trace->bit_blits; trace->last_width = (StarsU16)width;
}
static void pat_blt(void *context, StarsU16 hdc, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU32 rop)
{
    (void)hdc; (void)x; (void)y; (void)width; (void)height; (void)rop;
    ++((Trace *)context)->pat_blits;
}
static StarsI16 message_id(void *context, StarsI16 current)
{
    (void)current; ++((Trace *)context)->message_id_calls; return 3;
}

static void initialize(StarsDecorateMsgTitleInput *input,
    StarsDecorateMsgTitleOps *ops, Trace *trace, StarsU8 *sent,
    StarsU8 *filtered)
{
    memset(input, 0, sizeof *input); memset(ops, 0, sizeof *ops);
    memset(trace, 0, sizeof *trace); memset(sent, 0, 49U);
    memset(filtered, 0, 49U);
    input->hdc = 1; input->rect.left = 0; input->rect.top = 0;
    input->rect.right = 100; input->rect.bottom = 20;
    input->current_message = 0; input->message_count = 1;
    input->sent_bits = sent; input->filtered_bits = filtered;
    input->message_bit_bytes = 49U; input->mono_bitmap = 2;
    input->message_bitmap = 3; input->shadow_brush = 4;
    input->highlight_brush = 5; input->ops = ops; input->context = trace;
    ops->create_compatible_dc = create_dc; ops->select_object = select_object;
    ops->delete_dc = delete_dc; ops->set_text_color = set_color;
    ops->set_background_color = set_color; ops->bit_blt = bit_blt;
    ops->pat_blt = pat_blt; ops->message_id = message_id;
}

int main(void)
{
    StarsDecorateMsgTitleInput input;
    StarsDecorateMsgTitleOps ops;
    Trace trace;
    StarsU8 sent[49], filtered[49];
    int failures = 0;

    initialize(&input, &ops, &trace, sent, filtered);
    sent[0] = 0x08; filtered[0] = 0x08;
    stars_decorate_msg_title_bar(&input);
    if (trace.creates != 1 || trace.deletes != 1 || trace.message_id_calls != 1 ||
            trace.bit_blits != 5 || trace.pat_blits != 3 || trace.colors != 4)
        ++failures;

    initialize(&input, &ops, &trace, sent, filtered);
    input.view_filtered = 1;
    input.game_flags = STARS_DECORATE_SOUND_DISABLED;
    stars_decorate_msg_title_bar(&input);
    if (input.view_filtered != 0 || trace.message_id_calls != 0 ||
            trace.bit_blits != 0 || trace.pat_blits != 0)
        ++failures;

#ifndef TARGET_NATIVE
    printf("DECORATE_MSG_TITLE_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
