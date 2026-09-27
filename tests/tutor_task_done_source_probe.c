#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace {
    StarsI16 checks;
    StarsU16 fail_kind;
    StarsI16 fail_first;
} ProbeTrace;

static StarsI16 probe_check(void *opaque, StarsU16 kind,
    const StarsI16 args[6])
{
    ProbeTrace *trace = (ProbeTrace *)opaque;
    ++trace->checks;
    if (kind == trace->fail_kind &&
        (trace->fail_first < 0 || args[0] == trace->fail_first))
        return 0;
    return 1;
}

static StarsI16 probe_value(void *opaque, StarsU16 kind, StarsI16 a,
    StarsI16 b)
{ (void)opaque; (void)kind; (void)a; (void)b; return 0; }

int main(void)
{
    StarsTutorTaskState state;
    StarsTutorTaskOps ops;
    ProbeTrace trace;
    StarsI16 failures;
    StarsI16 result;

    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.check = probe_check;
    ops.value = probe_value;
    ops.context = &trace;

    state.turn = 0;
    state.idt = 0;
    result = stars_tutor_task_done(&state, &ops);
    failures = result != 1 || state.idt_bold != 5;

    memset(&state, 0, sizeof state);
    trace.fail_kind = STARS_TUTOR_CHECK_SELECTION;
    trace.fail_first = 2;
    state.turn = 0;
    state.idt = 0x10;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 0 || state.idh != 0x1771 || state.idt_bold != 0x12;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    state.flags = 0x0200;
    state.turn = 0;
    state.idt = 0x20;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 1 || state.idt_bold != 0x27 ||
        (state.flags & 8U) == 0U;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.fail_kind = STARS_TUTOR_CHECK_QUEUE;
    trace.fail_first = 0x0d;
    state.turn = 1;
    state.idt = 0x28;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 0 || state.idt_bold != 0x2a;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.fail_kind = STARS_TUTOR_CHECK_FLEET_WP;
    trace.fail_first = 1;
    state.turn = 2;
    state.idt = 0x30;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 1 || state.idt_bold != 0x37 || trace.checks < 7;

    memset(&state, 0, sizeof state);
    state.turn = 3;
    state.idt = 0x60;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 1;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.fail_kind = STARS_TUTOR_CHECK_XFER_WP;
    trace.fail_first = 3;
    state.turn = 6;
    state.idt = 0xa0;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 0 || state.idt_bold != 0xa4;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.fail_kind = STARS_TUTOR_CHECK_MESSAGES;
    trace.fail_first = -1;
    state.turn = 8;
    state.idt = 0xd8;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 0 || state.idt_bold != 0xd8;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.fail_kind = STARS_TUTOR_CHECK_TEMPLATE;
    trace.fail_first = -1;
    state.turn = 10;
    state.idt = 0x100;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 0 || state.idt_bold != 0x100;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    state.turn = 11;
    state.idt = 0x108;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 1;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.fail_kind = STARS_TUTOR_CHECK_QUEUE;
    trace.fail_first = 0x0d;
    state.turn = 18;
    state.idt = 0x188;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 0 || state.idt_bold != 0x188;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.fail_kind = STARS_TUTOR_CHECK_MESSAGES;
    trace.fail_first = 4;
    state.turn = 26;
    state.idt = 0x200;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 0 || state.idt_bold != 0x200;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    trace.fail_kind = STARS_TUTOR_CHECK_MESSAGES;
    trace.fail_first = -1;
    state.turn = 36;
    state.idt = 0x278;
    result = stars_tutor_task_done(&state, &ops);
    failures += result != 0 || state.idt_bold != 0x27c;

    {
        FILE *report;
        report = fopen("C:\\TUTOR\\TTR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "TUTOR_TASK_CHECKS=13 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("TUTOR_TASK_CHECKS=13 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
