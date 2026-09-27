#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 first_argument;
    StarsI16 select_calls;
} Trace;

static StarsU16 parameter_count(void *context, StarsI16 message_id)
{
    (void)context;
    return message_id == 3 ? 1U : 0U;
}

static StarsI16 select_first(void *context, StarsI16 argument)
{
    Trace *trace = (Trace *)context;
    trace->first_argument = argument;
    ++trace->select_calls;
    return 3;
}

static void initialize(StarsReadPlayerMessagesInput *input,
    StarsReadMessageStreamRecord *message_records,
    StarsReadMessageStreamRecord *incoming_records, StarsU8 *buffer,
    StarsU8 *sent_bits, StarsReadIncomingPlayerMessage *incoming,
    Trace *trace)
{
    memset(input, 0, sizeof *input);
    memset(message_records, 0, sizeof *message_records * 2U);
    memset(incoming_records, 0, sizeof *incoming_records * 2U);
    memset(buffer, 0, 32U); memset(sent_bits, 0, 64U);
    memset(incoming, 0, sizeof *incoming * 2U); memset(trace, 0, sizeof *trace);
    input->message_records = message_records;
    input->incoming_records = incoming_records;
    input->message_record_count = 2U;
    input->incoming_record_count = 2U;
    input->message_buffer = buffer; input->message_capacity = 32U;
    input->sent_bits = sent_bits; input->sent_bit_bytes = 64U;
    input->incoming = incoming; input->incoming_capacity = 2U;
    input->parameter_count = parameter_count;
    input->select_first = select_first; input->context = trace;
}

int main(void)
{
    StarsReadPlayerMessagesInput input;
    StarsReadMessageStreamRecord messages[2], incoming_records[2];
    StarsReadIncomingPlayerMessage incoming[2];
    StarsU8 buffer[32], sent_bits[64];
    Trace trace;
    static const StarsU8 message_one[] = { 3, 2, 0xff, 0xff, 0x34, 0x12 };
    static const StarsU8 message_two[] = { 4, 0, 10, 0 };
    static const StarsU8 player_one[] = { 9, 8, 7 };
    static const StarsU8 player_two[] = { 6, 5 };
    static const StarsU8 ignored[] = { 0xaa };
    int failures = 0;

    initialize(&input, messages, incoming_records, buffer, sent_bits,
        incoming, &trace);
    messages[0].kind = STARS_READ_MESSAGE_BUFFER_KIND;
    messages[0].bytes = 6U; messages[0].data = message_one;
    messages[1].kind = STARS_READ_MESSAGE_BUFFER_KIND;
    messages[1].bytes = 4U; messages[1].data = message_two;
    incoming_records[0].kind = STARS_READ_PLAYER_MESSAGE_KIND;
    incoming_records[0].bytes = 3U; incoming_records[0].data = player_one;
    incoming_records[1].kind = STARS_READ_PLAYER_MESSAGE_KIND;
    incoming_records[1].bytes = 2U; incoming_records[1].data = player_two;
    if (!stars_read_player_messages(&input) || input.message_bytes != 10U ||
            input.message_count != 2U || !(sent_bits[0] & 0x18U) ||
            input.incoming_count != 2U || incoming[0].payload[1] != 8U ||
            input.current_message != 3 || trace.first_argument != 0)
        ++failures;

    initialize(&input, messages, incoming_records, buffer, sent_bits,
        incoming, &trace);
    messages[0].kind = STARS_READ_MESSAGE_BUFFER_KIND;
    messages[0].bytes = 1U; messages[0].data = ignored;
    input.message_capacity = 0U;
    if (!stars_read_player_messages(&input) || input.message_bytes != 0U)
        ++failures;

#ifndef TARGET_NATIVE
    printf("READ_PLAYER_MSG_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
