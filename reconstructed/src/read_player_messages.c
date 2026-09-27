#include "core.h"

static StarsU16 read_message_word(const StarsU8 *p)
{
    return (StarsU16)(p[0] | ((StarsU16)p[1] << 8));
}

static void read_message_copy(StarsU8 *destination, const StarsU8 *source,
    StarsU16 count)
{
    StarsU16 i;
    for (i = 0; i < count; ++i)
        destination[i] = source[i];
}

static void read_message_mark_sent(StarsReadPlayerMessagesInput *input,
    StarsU16 message_id)
{
    StarsU16 byte_index;
    StarsU8 bit;
    byte_index = (StarsU16)(message_id >> 3);
    bit = (StarsU8)(1U << (message_id & 7U));
    if (input->sent_bits && byte_index < input->sent_bit_bytes)
        input->sent_bits[byte_index] = (StarsU8)(
            input->sent_bits[byte_index] | bit);
}

static StarsI16 read_message_append(StarsReadPlayerMessagesInput *input,
    const StarsReadMessageStreamRecord *record)
{
    if (record->kind != STARS_READ_MESSAGE_BUFFER_KIND || !record->data)
        return 1;
    if (record->bytes == 0U)
        return 1;
    if (record->bytes > input->message_capacity ||
            input->message_bytes > (StarsU16)(input->message_capacity -
            record->bytes))
        return 0;
    read_message_copy(input->message_buffer + input->message_bytes,
        record->data, record->bytes);
    input->message_bytes = (StarsU16)(input->message_bytes + record->bytes);
    return 1;
}

/* Decode only complete records from the newly appended message span.  The
 * low nine bits identify the text, and the following seven bits say whether
 * each parameter occupies one or two bytes in the wire stream. */
static void read_message_decode_new(StarsReadPlayerMessagesInput *input,
    StarsU16 old_bytes)
{
    StarsU16 offset = old_bytes;
    StarsU16 word;
    StarsU16 mask;
    StarsU16 count;
    StarsU16 i;
    StarsU16 width;
    StarsI16 message_id;

    while (input->message_buffer && offset < input->message_bytes) {
        if ((StarsU16)(input->message_bytes - offset) < 4U)
            break;
        word = read_message_word(input->message_buffer + offset);
        message_id = (StarsI16)(word & STARS_READ_MESSAGE_ID_MASK);
        mask = (StarsU16)(word >> STARS_READ_MESSAGE_MASK_SHIFT);
        count = input->parameter_count ? input->parameter_count(
            input->context, message_id) : 0U;
        if (count > STARS_READ_MESSAGE_MASK_BITS)
            count = STARS_READ_MESSAGE_MASK_BITS;
        /* MSGHDR also carries a two-byte goto word after message/mask. */
        offset = (StarsU16)(offset + 4U);
        for (i = 0U; i < count; ++i) {
            width = (mask & 1U) != 0U ? 2U : 1U;
            if ((StarsU16)(input->message_bytes - offset) < width)
                return;
            offset = (StarsU16)(offset + width);
            mask = (StarsU16)(mask >> 1);
        }
        read_message_mark_sent(input, (StarsU16)message_id);
        ++input->message_count;
    }
}

static void read_message_append_incoming(
    StarsReadPlayerMessagesInput *input,
    const StarsReadMessageStreamRecord *record)
{
    if (record->kind != STARS_READ_PLAYER_MESSAGE_KIND || !record->data ||
            input->incoming_count >= input->incoming_capacity ||
            record->bytes > STARS_READ_PLAYER_MESSAGE_PAYLOAD_CAPACITY)
        return;
    input->incoming[input->incoming_count].bytes = record->bytes;
    read_message_copy(input->incoming[input->incoming_count].payload,
        record->data, record->bytes);
    ++input->incoming_count;
}

/* Genuine semantic reconstruction of ReadPlayerMessages, NE 7:994A-9BD5.
 * ReadRt is represented by already-framed stream records: type 0x0c carries
 * the packed message table and type 0x28 carries one incoming player-message
 * payload.  The original far allocations and longjmp memory guard become
 * caller-owned bounded buffers here, while the decode order and first-message
 * selection remain explicit. */
StarsI16 stars_read_player_messages(StarsReadPlayerMessagesInput *input)
{
    StarsU16 i;
    StarsU16 old_bytes;

    if (!input || !input->message_buffer || input->message_capacity <
            input->message_bytes || (input->message_record_count != 0U &&
            !input->message_records) || (input->incoming_record_count != 0U &&
            !input->incoming_records) || (input->incoming_capacity != 0U &&
            !input->incoming))
        return 0;
    old_bytes = input->message_bytes;
    for (i = 0U; i < input->message_record_count; ++i)
        if (!read_message_append(input, &input->message_records[i]))
            break;
    read_message_decode_new(input, old_bytes);
    for (i = 0U; i < input->incoming_record_count; ++i)
        read_message_append_incoming(input, &input->incoming_records[i]);
    input->current_message = -1;
    if (input->select_first)
        input->current_message = input->select_first(input->context, 0);
    return 1;
}
