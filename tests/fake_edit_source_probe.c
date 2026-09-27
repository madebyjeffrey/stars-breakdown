#include "core.h"
#include <stdio.h>

typedef struct EditProbe {
    int calls;
    StarsU32 proc;
    StarsU16 hwnd;
    StarsU16 message;
    StarsU16 character;
    StarsI32 lparam;
} EditProbe;

static StarsI32 call_window(void *context, StarsU32 proc, StarsU16 hwnd,
    StarsU16 message, StarsU16 character, StarsI32 lparam)
{
    EditProbe *probe = (EditProbe *)context;
    ++probe->calls;
    probe->proc = proc;
    probe->hwnd = hwnd;
    probe->message = message;
    probe->character = character;
    probe->lparam = lparam;
    return 1234;
}

int main(void)
{
    EditProbe probe;
    int failures = 0;
    probe.calls = 0;
    if (stars_fake_edit_proc(7, 0x0102U, '5', 99, 0x12345678UL,
        call_window, &probe) != 1234 || probe.calls != 1 ||
        probe.proc != 0x12345678UL || probe.hwnd != 7 ||
        probe.character != '5' || probe.lparam != 99)
        ++failures;
    if (stars_fake_edit_proc(7, 0x0102U, 'x', 99, 1, call_window, &probe) != 0 ||
        probe.calls != 1)
        ++failures;
    if (stars_fake_edit_proc(7, 0x0102U, 8, 99, 1, call_window, &probe) != 1234 ||
        probe.calls != 2)
        ++failures;
    if (stars_fake_edit_proc(7, 1, 'x', 99, 1, call_window, &probe) != 1234 ||
        probe.calls != 3)
        ++failures;
    printf("FAKE_EDIT_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
