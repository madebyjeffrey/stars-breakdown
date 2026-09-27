#include "core.h"
#include <stdio.h>

typedef struct ProxyLog {
    StarsU16 toolbar_messages;
    StarsU16 tooltip_calls;
    StarsU16 downstream_calls;
    StarsU32 last_proc;
    StarsU16 last_message;
} ProxyLog;

static void toolbar(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsI32 lparam)
{
    ProxyLog *log = (ProxyLog *)context;
    (void)hwnd; (void)message; (void)wparam; (void)lparam;
    ++log->toolbar_messages;
}

static void tooltip(void *context, StarsI16 which)
{
    ProxyLog *log = (ProxyLog *)context;
    if (which == -1) ++log->tooltip_calls;
}

static StarsI32 downstream(void *context, StarsU32 proc, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsI32 lparam)
{
    ProxyLog *log = (ProxyLog *)context;
    (void)hwnd; (void)wparam; (void)lparam;
    ++log->downstream_calls;
    log->last_proc = proc;
    log->last_message = message;
    return 0x12345678L;
}

int main(void)
{
    StarsFakeProcOps ops;
    ProxyLog log;
    int failures = 0;
    ops.toolbar = toolbar; ops.show_tooltip = tooltip;
    ops.call_window = downstream;
    log.toolbar_messages = 0U; log.tooltip_calls = 0U;
    log.downstream_calls = 0U; log.last_proc = 0U; log.last_message = 0U;
    if (stars_fake_combo_proc(0x11112222UL, 4U, 0x0200U, 6U, 7L,
        &ops, &log) != 0x12345678L || log.toolbar_messages != 1U ||
        log.tooltip_calls != 0U || log.downstream_calls != 1U ||
        log.last_proc != 0x11112222UL)
        ++failures;
    if (stars_fake_ce_proc(0x33334444UL, 4U, 0x0201U, 6U, 7L,
        &ops, &log) != 0x12345678L || log.toolbar_messages != 1U ||
        log.tooltip_calls != 1U || log.downstream_calls != 2U ||
        log.last_proc != 0x33334444UL)
        ++failures;
    if (stars_fake_ce_proc(0x33334444UL, 4U, 0x0001U, 6U, 7L,
        &ops, &log) != 0x12345678L || log.tooltip_calls != 1U ||
        log.downstream_calls != 3U)
        ++failures;
    printf("TB_PROXY_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
