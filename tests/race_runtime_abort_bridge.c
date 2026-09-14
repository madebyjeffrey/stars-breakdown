/* Test-only outer event-loop recovery. No longjmp crosses a Python callback. */
#include "runtime.h"
#include <setjmp.h>
static jmp_buf race_test_environment;
StarsU16 race_test_abort_token;
StarsI16 race_test_abort_value;
static void race_test_abort(void *context, StarsU16 token, StarsI16 value)
{
    (void)context;
    race_test_abort_token = token; race_test_abort_value = value;
    longjmp(race_test_environment, 1);
}
int race_test_save(StarsRuntime *r, StarsU16 file, StarsU16 player)
{
    const StarsPlatform *saved = r->platform;
    StarsPlatform platform = *saved;
    StarsU16 top = r->scratch_top;
    platform.abort = race_test_abort; r->platform = &platform;
    race_test_abort_token = 0; race_test_abort_value = 0;
    if (setjmp(race_test_environment)) {
        r->platform = saved; r->scratch_top = top; return -2;
    }
    stars_save_race(r, file, player);
    r->platform = saved; return 0;
}
