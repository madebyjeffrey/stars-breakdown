#include "core.h"
#include <string.h>

enum {
    STARS_LOAD_LOG_RECORD_END = 0,
    STARS_LOAD_LOG_RECORD_SERIAL = 9,
    STARS_LOAD_LOG_RECORD_MESSAGE = 0x28,
    STARS_LOAD_LOG_HEADER_BYTES = 16,
    STARS_LOAD_LOG_ERROR_BAD_VERSION = 0x1d,
    STARS_LOAD_LOG_ERROR_BAD_TURN = 0x1e,
    STARS_LOAD_LOG_ERROR_BAD_RECORD = 3
};

static StarsU16 load_log_word(const StarsU8 *data)
{
    return (StarsU16)(data[0] | ((StarsU16)data[1] << 8));
}

static StarsU32 load_log_long(const StarsU8 *data)
{
    return (StarsU32)load_log_word(data) |
        ((StarsU32)load_log_word(data + 2U) << 16);
}

static void load_log_error(const StarsLoadLogFileInput *input,
    StarsI16 error)
{
    if (input->file_error)
        input->file_error(input->context, error);
}

static StarsI16 load_log_store(const StarsLoadLogFileInput *input,
    StarsU16 offset, const StarsLoadGameRecord *record)
{
    StarsU16 header;
    StarsU16 end;

    if (!input->log || record->length > 0x03ffU ||
            (record->length && !record->data))
        return 0;
    end = (StarsU16)(offset + record->length + 2U);
    if (end < offset || end > input->log_capacity)
        return 0;
    header = (StarsU16)((record->type << 10) | record->length);
    input->log[offset] = (StarsU8)header;
    input->log[(StarsU16)(offset + 1U)] = (StarsU8)(header >> 8);
    if (record->length && record->data)
        memcpy(input->log + (StarsU16)(offset + 2U), record->data,
            record->length);
    return 1;
}

/* NE 10:C7A2-CCCC.  The loader validates the companion file's game/turn
 * preamble, copies its length-prefixed memory log, preserves the optional
 * serial configuration record, and appends the linked-message records.  The
 * original has two physical sources (disk and a Windows resource); source
 * selection is intentionally outside this routine, while the record order and
 * success/failure rules are kept here as ordinary C. */
StarsI16 stars_load_log_file(const StarsLoadLogFileInput *input,
    StarsLoadLogFileState *state)
{
    const StarsLoadGameRecord *header;
    const StarsLoadGameRecord *record;
    StarsU16 index;
    StarsU16 offset;
    StarsU16 target_length;
    StarsU16 file_turn_flags;

    if (!input || !state)
        return 0;
    memset(state, 0, sizeof *state);
    state->memory_previous = -1;
    if (!input->records || input->record_count == 0U ||
            !input->records[0].data ||
            input->records[0].length < STARS_LOAD_LOG_HEADER_BYTES) {
        load_log_error(input, STARS_LOAD_LOG_ERROR_BAD_RECORD);
        return 0;
    }
    header = &input->records[0];
    state->version = load_log_word(header->data + 8U);
    state->file_turn = load_log_word(header->data + 10U);
    state->file_flags = load_log_word(header->data + 14U);
    if (load_log_long(header->data + 4U) != input->game_id ||
            state->file_turn < input->game_turn)
        return 1;
    if (state->file_turn != input->game_turn) {
        load_log_error(input, STARS_LOAD_LOG_ERROR_BAD_TURN);
        return 1;
    }
    file_turn_flags = (StarsU16)((state->file_flags >> 13) & 7U);
    if (file_turn_flags != (StarsU16)((input->game_turn_flags >> 9) & 7U)) {
        load_log_error(input, STARS_LOAD_LOG_ERROR_BAD_VERSION);
        return 1;
    }
    if (input->accept_header && !input->accept_header(input->context,
            state->version, state->file_flags))
        return 0;
    /* RTLOGHDR follows the BOF: cbLog at 0, serial at 2, config at 6.
     * 0x11 is its byte length, not its record type. */
    index = 1U;
    if (index >= input->record_count ||
            input->records[index].type != STARS_LOAD_LOG_RECORD_SERIAL ||
            input->records[index].length < 2U || !input->records[index].data) {
        load_log_error(input, STARS_LOAD_LOG_ERROR_BAD_RECORD);
        return 0;
    }
    record = &input->records[index++];
    target_length = load_log_word(record->data);
    if (target_length > input->log_capacity || !input->log) {
        load_log_error(input, STARS_LOAD_LOG_ERROR_BAD_RECORD);
        return 0;
    }
    if (record->length == 17U) {
        if (input->serial && !input->serial(input->context,
                record))
            return 0;
    }
    offset = 0U;
    while (offset < target_length) {
        if (index >= input->record_count) {
            load_log_error(input, STARS_LOAD_LOG_ERROR_BAD_RECORD);
            return 0;
        }
        record = &input->records[index++];
        if ((StarsU16)(record->length + 2U) >
                (StarsU16)(target_length - offset)) {
            load_log_error(input, STARS_LOAD_LOG_ERROR_BAD_RECORD);
            return 0;
        }
        if (!load_log_store(input, offset, record)) {
            load_log_error(input, STARS_LOAD_LOG_ERROR_BAD_RECORD);
            return 0;
        }
        offset = (StarsU16)(offset + record->length + 2U);
    }
    while (index < input->record_count &&
            input->records[index].type == STARS_LOAD_LOG_RECORD_MESSAGE) {
        record = &input->records[index++];
        if (input->message && !input->message(input->context, record))
            return 0;
        ++state->messages;
    }
    if (index >= input->record_count ||
            input->records[index].type != STARS_LOAD_LOG_RECORD_END) {
        load_log_error(input, STARS_LOAD_LOG_ERROR_BAD_RECORD);
        return 0;
    }
    state->memory_current = target_length;
    state->log_length = target_length;
    if (input->dirty_game)
        input->dirty_game(input->context);
    return 1;
}
