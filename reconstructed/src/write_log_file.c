#include "core.h"
#include <string.h>

static StarsU16 write_log_word(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] |
        ((StarsU16)data[(StarsU16)(offset + 1U)] << 8));
}

/* NE 10:CDF6-D057. Persist the current memory-log header and records, then
 * append outgoing player messages and the type-zero terminator. */
StarsI16 stars_write_log_file(const StarsWriteLogFileInput *input)
{
    StarsU8 header[17];
    StarsU16 cursor;
    StarsU16 record_header;
    StarsU16 length;
    StarsU16 i;
    StarsI16 message_length;
    StarsU16 queue_length;

    if (!input || !input->create_file || !input->write_record ||
            !input->close_stream)
        return 0;
    if (input->current_player == input->player &&
            (input->player_flags & 0x0200U) == 0U &&
            input->previous_log_type != 0x2eU && input->current_queue &&
            input->previous_queue) {
        queue_length = (StarsU16)(0x1aU -
            (StarsU16)((12U - input->queue_count_byte) * 2U));
        if (memcmp(input->current_queue, input->previous_queue,
                queue_length) != 0U && input->write_memory)
            input->write_memory(input->context, 0x2eU, queue_length,
                input->current_queue);
    }
    if (!input->create_file(input->context, input->player,
            input->file_base)) {
        if (input->alert) input->alert(input->context, 0x11U, 0x10);
        return 0;
    }
    memset(header, 0, sizeof(header));
    header[0] = (StarsU8)input->log_size;
    header[1] = (StarsU8)(input->log_size >> 8);
    header[2] = (StarsU8)input->serial;
    header[3] = (StarsU8)(input->serial >> 8);
    header[4] = (StarsU8)(input->serial >> 16);
    header[5] = (StarsU8)(input->serial >> 24);
    if (input->environment)
        memcpy(header + 6, input->environment, 11U);
    input->write_record(input->context, 9U, 0x11U, header);
    cursor = 0;
    while (cursor < input->log_size) {
        if (!input->log || (StarsU16)(cursor + 2U) > input->log_size)
            break;
        record_header = write_log_word(input->log, cursor);
        length = (StarsU16)(record_header & 0x03ffU);
        if ((StarsU16)(cursor + length + 2U) > input->log_size)
            break;
        input->write_record(input->context,
            (StarsU16)(record_header >> 10), length,
            input->log + (StarsU16)(cursor + 2U));
        cursor = (StarsU16)(cursor + length + 2U);
    }
    for (i = 0; i < input->message_count; ++i) {
        message_length = input->messages[i].compressed_length;
        if (message_length < 0) message_length = (StarsI16)-message_length;
        input->write_record(input->context, 0x28U,
            (StarsU16)(message_length + 0x0cU), &input->messages[i]);
    }
    input->write_record(input->context, 0U, 0U, 0);
    input->close_stream(input->context);
    if (input->dirty_game) input->dirty_game(input->context);
    if (input->game_flags)
        *input->game_flags = (StarsU16)((*input->game_flags & 0xfeffU) |
            0x0100U);
    return 1;
}
