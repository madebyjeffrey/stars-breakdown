/* Test-only catcher for the host adapter's nonreturning abort callback. */
#include "core.h"
#include <setjmp.h>
static jmp_buf stream_test_environment;
StarsU16 stream_test_abort_environment;
StarsI16 stream_test_abort_value;
static void stream_test_abort(void *context, StarsU16 environment, StarsI16 value)
{
    (void)context;
    stream_test_abort_environment = environment;
    stream_test_abort_value = value;
    longjmp(stream_test_environment, 1);
}
int stream_test_run(StarsU8 *data, int operation, StarsU16 offset, StarsU16 count,
    const StarsStreamApi *input, void *context)
{
    StarsStreamApi api = *input;
    api.abort = stream_test_abort;
    stream_test_abort_environment = 0;
    stream_test_abort_value = 0;
    if (setjmp(stream_test_environment)) return 1;
    if (operation == 0) stars_stream_open(data, offset,
        count < 32768U ? (StarsI16)count : (StarsI16)((StarsI32)count - 65536L), &api, context);
    else if (operation == 1) stars_stream_close(data, &api, context);
    else if (operation == 2) stars_stream_read(data, data + offset, count, &api, context);
    else stars_stream_write(data, data + offset, count, &api, context);
    return 0;
}
