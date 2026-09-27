#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct FailureTrace {
    StarsI16 raw_calls;
    StarsI16 raw2_calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[8];
    StarsI16 payload2[3];
} FailureTrace;

static void raw_message(void *opaque, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    FailureTrace *trace;
    StarsI16 i;
    trace = (FailureTrace *)opaque;
    ++trace->raw_calls;
    trace->recipient = recipient;
    trace->code = code;
    for (i = 0; i < 8; ++i)
        trace->payload[i] = payload[i];
}

static void raw_message2(void *opaque, StarsI16 recipient, StarsI16 code,
    StarsI16 payload1, StarsI16 payload2, StarsI16 payload3)
{
    FailureTrace *trace;
    trace = (FailureTrace *)opaque;
    ++trace->raw2_calls;
    trace->recipient = recipient;
    trace->code = code;
    trace->payload2[0] = payload1;
    trace->payload2[1] = payload2;
    trace->payload2[2] = payload3;
}

static StarsI16 check_raw(StarsI16 variant, StarsI16 expected_code,
    const StarsI16 expected[8])
{
    StarsSatisfyFailureFrame frame;
    FailureTrace trace;
    StarsI16 i;

    memset(&frame, 0, sizeof frame);
    memset(&trace, 0, sizeof trace);
    frame.variant = variant;
    frame.recipient = 2;
    frame.fleet_id = 11;
    frame.resource = 3;
    frame.requested = 27;
    frame.target_word = 0x0123;
    frame.target_x = 14;
    stars_satisfy_raw_failure_message(&frame, raw_message, raw_message2,
        &trace);
    if (trace.raw_calls != 1 || trace.raw2_calls != 0 ||
            trace.recipient != 2 || trace.code != expected_code)
        return 0;
    for (i = 0; i < 8; ++i)
        if (trace.payload[i] != expected[i])
            return 0;
    return 1;
}

static StarsI16 check_raw2(StarsI16 variant, StarsI16 expected_code,
    StarsI16 expected1, StarsI16 expected2, StarsI16 expected3)
{
    StarsSatisfyFailureFrame frame;
    FailureTrace trace;

    memset(&frame, 0, sizeof frame);
    memset(&trace, 0, sizeof trace);
    frame.variant = variant;
    frame.recipient = 2;
    frame.fleet_id = 11;
    frame.thing_kind = 1;
    stars_satisfy_raw_failure_message(&frame, raw_message, raw_message2,
        &trace);
    return (StarsI16)(trace.raw_calls == 0 && trace.raw2_calls == 1 &&
        trace.recipient == 2 && trace.code == expected_code &&
        trace.payload2[0] == expected1 && trace.payload2[1] == expected2 &&
        trace.payload2[2] == expected3);
}

int main(void)
{
    static const StarsI16 amount[8] =
        {(StarsI16)0x800b, 11, 3, 27, 0, 14, 0x0123, 0};
    static const StarsI16 colonists[8] =
        {(StarsI16)0x800b, 11, 3, 27, 0, 14, 0x0123, 0};
    StarsI16 failures;

    failures = 0;
    if (!check_raw(STARS_SATISFY_FAILURE_SET_AMOUNT,
            STARS_SATISFY_MESSAGE_SET_AMOUNT, amount))
        failures = 1;
    if (!check_raw(STARS_SATISFY_FAILURE_SET_COLONISTS,
            STARS_SATISFY_MESSAGE_SET_COLONISTS, colonists))
        failures = 1;
    if (!check_raw2(STARS_SATISFY_FAILURE_COLONISTS_DEEP_SPACE,
            STARS_SATISFY_MESSAGE_COLONISTS_DEEP_SPACE,
            (StarsI16)0x800b, 11, 0))
        failures = 1;
    if (!check_raw2(STARS_SATISFY_FAILURE_TRANSFER_FUTILE,
            STARS_SATISFY_MESSAGE_TRANSFER_FUTILE,
            (StarsI16)0x800b, 11, 1))
        failures = 1;
#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYOFALL.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_FAILURE_CHECKS=44 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_FAILURE_CHECKS=44 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
