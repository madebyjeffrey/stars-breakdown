#include <stdio.h>
#include "core.h"

int main(void)
{
    static const StarsCheckLogRecord records[] = {
        { 8U, 2U, 4U }, { 1U, 2U, 0U }, { 0x28U, 3U, 0U }, { 0U, 0U, 0U }
    };
    StarsCheckLogFileInput input;
    StarsCheckLogFileState state;
    StarsI16 error = 0;
    input.opened = 1U; input.open_error = 0; input.records = records;
    input.record_count = 4U;
    if (!stars_check_log_file(&input, &state, &error) ||
            state.memory_current != 0 || state.memory_previous != -1 ||
            state.current_type != 0U || state.current_first_word != 4U) return 1;
    input.records = 0; input.record_count = 0U; error = 0;
    if (stars_check_log_file(&input, &state, &error) || error != 3) return 2;
    input.opened = 0U; input.open_error = 4; error = 9;
    if (stars_check_log_file(&input, &state, &error) || error != 9) return 3;
    puts("LOG_CHECK_SOURCE=PASS");
    return 0;
}
