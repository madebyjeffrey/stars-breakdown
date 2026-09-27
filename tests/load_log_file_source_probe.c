#ifndef TARGET_NATIVE
#include <stdio.h>
#endif
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU8 header[16];
    StarsU8 serial_data[17];
    StarsU8 log_data[2];
    StarsU8 message_data[3];
    StarsU8 log[8];
    StarsLoadGameRecord records[5];
    StarsU16 headers;
    StarsU16 serials;
    StarsU16 messages;
    StarsU16 dirty;
    StarsU16 errors;
    StarsI16 error_id;
} Trace;

static StarsU16 word(const StarsU8 *data)
{
    return (StarsU16)(data[0] | ((StarsU16)data[1] << 8));
}

static void put_word(StarsU8 *data, StarsU16 value)
{
    data[0] = (StarsU8)value;
    data[1] = (StarsU8)(value >> 8);
}

static StarsU32 long_value(const StarsU8 *data)
{
    return (StarsU32)word(data) | ((StarsU32)word(data + 2U) << 16);
}

static StarsI16 accept_header(void *opaque, StarsU16 version,
    StarsU16 flags)
{
    Trace *t = (Trace *)opaque;
    if (version != 0x1234U || flags != 0x6000U) ++t->errors;
    ++t->headers;
    return 1;
}

static StarsI16 serial(void *opaque, const StarsLoadGameRecord *record)
{
    Trace *t = (Trace *)opaque;
    if (!record || record->length != 17U || !record->data ||
            record->data[2] != 0xa1U) ++t->errors;
    ++t->serials;
    return 1;
}

static StarsI16 message(void *opaque, const StarsLoadGameRecord *record)
{
    Trace *t = (Trace *)opaque;
    if (!record || record->length != 3U || !record->data ||
            record->data[0] != 0xb2U) ++t->errors;
    ++t->messages;
    return 1;
}

static void file_error(void *opaque, StarsI16 error)
{
    Trace *t = (Trace *)opaque;
    ++t->errors;
    t->error_id = error;
}

static void dirty_game(void *opaque)
{
    ++((Trace *)opaque)->dirty;
}

static void initialize(Trace *t)
{
    StarsU32 game_id = 0x11223344UL;
    memset(t, 0, sizeof *t);
    t->header[4] = (StarsU8)game_id;
    t->header[5] = (StarsU8)(game_id >> 8);
    t->header[6] = (StarsU8)(game_id >> 16);
    t->header[7] = (StarsU8)(game_id >> 24);
    put_word(t->header + 8U, 0x1234U);
    put_word(t->header + 10U, 5U);
    put_word(t->header + 14U, 0x6000U);
    put_word(t->header, 0x4a33U);
    put_word(t->serial_data, 4U);
    t->serial_data[2] = 0xa1;
    t->log_data[0] = 0xc3;
    t->message_data[0] = 0xb2;
    t->records[0].type = 8; t->records[0].length = 16;
    t->records[0].data = t->header;
    t->records[1].type = 9; t->records[1].length = 17;
    t->records[1].data = t->serial_data;
    t->records[2].type = 1; t->records[2].length = 2;
    t->records[2].data = t->log_data;
    t->records[3].type = 0x28; t->records[3].length = 3;
    t->records[3].data = t->message_data;
    t->records[4].type = 0; t->records[4].length = 0;
}

int main(void)
{
    Trace t;
    StarsLoadLogFileInput input;
    StarsLoadLogFileState state;
    StarsU32 game_id = 0x11223344UL;
    initialize(&t);
    memset(&input, 0, sizeof input);
    input.records = t.records;
    input.record_count = 5;
    input.game_id = game_id;
    input.game_turn = 5;
    input.game_turn_flags = 0x0600;
    input.log = t.log;
    input.log_capacity = sizeof t.log;
    input.accept_header = accept_header;
    input.serial = serial;
    input.message = message;
    input.file_error = file_error;
    input.dirty_game = dirty_game;
    input.context = &t;
    if (!stars_load_log_file(&input, &state)) return 1;
    if (state.memory_current != 4U || state.memory_previous != -1 ||
            state.version != 0x1234U || state.file_turn != 5U ||
            state.messages != 1U || t.headers != 1U || t.serials != 1U ||
            t.messages != 1U || t.dirty != 1U || t.errors != 0U ||
            long_value(t.log) == 0U) return 1;

    /* A different game or an older turn is a non-applicable log, not a
     * corrupt one; the original returns success without mutating the log. */
    t.header[4] ^= 1U;
    t.dirty = 0;
    if (!stars_load_log_file(&input, &state) || t.dirty != 0U ||
            t.errors != 0U) return 1;
    t.header[4] ^= 1U;

    /* A matching game with a future log turn reaches the raw loader's
     * explicit wrong-turn diagnostic and still returns the non-applicable
     * result used by its caller. */
    put_word(t.header + 10U, 6U);
    if (!stars_load_log_file(&input, &state) || t.errors != 1U ||
            t.error_id != 0x1e) return 1;
    put_word(t.header + 10U, 5U);
    t.errors = 0;
    t.error_id = 0;

    /* A matching header with no type-0 terminator is a malformed log. */
    t.records[4].type = 0x29;
    if (stars_load_log_file(&input, &state) || t.errors != 1U ||
            t.error_id != 3) return 1;
#ifndef TARGET_NATIVE
    printf("LOAD_LOG_CHECKS=6 FAILURES=0 RESULT=PASS\n");
#endif
    return 0;
}
