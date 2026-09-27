#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 dirty;
    StarsI16 recipient;
} Trace;

static void mark_dirty(void *context)
{
    ++((Trace *)context)->dirty;
}

static StarsI16 recipient(void *context)
{
    return ((Trace *)context)->recipient;
}

static StarsI16 compress_short(void *context, const StarsU8 *text,
    StarsU16 text_length, StarsU8 *out, StarsU16 *out_length)
{
    (void)context;
    if (text_length == 3U && text[0] == 'a' && text[1] == 'b' &&
            text[2] == 'c') {
        out[0] = 'Q';
        *out_length = 1U;
        return 1;
    }
    *out_length = 0U;
    return 0;
}

static void initialize(StarsFinishPlrMsgEntryInput *input,
    StarsFinishPlayerMessage *entries, Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(entries, 0, sizeof *entries * 3U);
    memset(trace, 0, sizeof *trace);
    input->entries = entries;
    input->capacity = 3U;
    input->recipient = recipient;
    input->mark_dirty = mark_dirty;
    input->context = trace;
}

int main(void)
{
    StarsFinishPlrMsgEntryInput input;
    StarsFinishPlayerMessage entries[3];
    Trace trace;
    static const StarsU8 hello[] = "hello";
    static const StarsU8 abc[] = "abc";
    static const StarsU8 empty[] = "";
    int failures = 0;

    initialize(&input, entries, &trace);
    trace.recipient = 5;
    input.sender = 2; input.message_id = 42; input.direction = 1;
    input.editor_text = hello; input.editor_length = 5U;
    if (!stars_finish_plr_msg_entry(&input) || input.count != 1U ||
            input.current != 1 || entries[0].stored_length != -6 ||
            strcmp((const char *)entries[0].payload, "hello") != 0 ||
            trace.dirty != 1)
        ++failures;

    initialize(&input, entries, &trace);
    input.count = 1U; input.current = 0; input.direction = 0;
    entries[0].stored_length = 1; entries[0].recipient = 7;
    entries[0].payload[0] = 'Q'; trace.recipient = 7;
    input.editor_text = abc; input.editor_length = 3U;
    input.compress = compress_short;
    if (!stars_finish_plr_msg_entry(&input) || entries[0].stored_length != 1 ||
            entries[0].payload[0] != 'Q' || trace.dirty != 0)
        ++failures;

    initialize(&input, entries, &trace);
    input.count = 2U; input.current = 1; input.direction = -1;
    input.editor_text = empty; input.editor_length = 0U;
    if (stars_finish_plr_msg_entry(&input) || input.count != 1U ||
            input.current != 0 || trace.dirty != 1)
        ++failures;

    initialize(&input, entries, &trace);
    input.count = 1U; input.current = 0; input.direction = 1000;
    input.editor_text = hello; input.editor_length = 5U;
    if (stars_finish_plr_msg_entry(&input) || input.count != 0U ||
            trace.dirty != 1)
        ++failures;

#ifndef TARGET_NATIVE
    printf("F_FINISH_MSG_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
