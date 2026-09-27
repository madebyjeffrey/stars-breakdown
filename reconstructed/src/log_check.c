#include "core.h"

/* NE 10:CCCC-CDF5. Validate the log preamble and skip the variable-length
 * message and type-0x28 blocks until the first ordinary log record. The
 * stream open/read/close work has already been reduced to typed record spans,
 * retaining the original memory-log indices and error-3 rejection. */
StarsI16 stars_check_log_file(const StarsCheckLogFileInput *input,
    StarsCheckLogFileState *state, StarsI16 *error)
{
    StarsU16 index;
    StarsU16 offset;
    StarsU16 first_length;
    const StarsCheckLogRecord *record;
    if (!input || !state) return 0;
    state->memory_current = 0;
    state->memory_previous = -1;
    state->current_type = 0U;
    state->current_first_word = 0U;
    if (!input->opened) {
        if (input->open_error != 4 && error) *error = input->open_error;
        return 0;
    }
    if (!input->records || input->record_count < 2U) {
        if (error) *error = 3;
        return 0;
    }
    index = 0U;
    first_length = input->records[0].first_word;
    offset = 0U;
    while (offset < first_length) {
        ++index;
        if (index >= input->record_count) {
            if (error) *error = 3;
            return 0;
        }
        offset = (StarsU16)(offset + input->records[index].length + 2U);
    }
    ++index;
    if (index >= input->record_count) {
        if (error) *error = 3;
        return 0;
    }
    record = &input->records[index];
    while (record->type == 0x28U) {
        ++index;
        if (index >= input->record_count) {
            if (error) *error = 3;
            return 0;
        }
        record = &input->records[index];
    }
    if (record->type != 0U) {
        if (error) *error = 3;
        return 0;
    }
    state->current_type = record->type;
    state->current_first_word = first_length;
    return 1;
}
