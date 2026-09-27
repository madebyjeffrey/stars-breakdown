#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct FuelTrace {
    StarsI16 calls;
    StarsI16 recipient;
    StarsI16 code;
    StarsI16 payload[8];
} FuelTrace;

static void raw_message(void *opaque, StarsI16 recipient, StarsI16 code,
    const StarsI16 payload[8])
{
    FuelTrace *trace;
    StarsI16 i;
    trace = (FuelTrace *)opaque;
    ++trace->calls;
    trace->recipient = recipient;
    trace->code = code;
    for (i = 0; i < 8; ++i)
        trace->payload[i] = payload[i];
}

static StarsI16 check_frame(StarsI16 variant, StarsI32 optimal_delta,
    StarsI16 expected_code, const StarsI16 expected[8])
{
    StarsSatisfyFuelFrame frame;
    FuelTrace trace;
    StarsI16 i;

    memset(&frame, 0, sizeof frame);
    memset(&trace, 0, sizeof trace);
    frame.variant = variant;
    frame.recipient = 2;
    frame.fleet_id = 11;
    frame.target_word = 0x0123;
    frame.target_x = 14;
    frame.target_id = 7;
    frame.fleet_fuel = 30;
    frame.fuel_stat = 40;
    frame.required_fuel = 55;
    frame.optimal_delta = optimal_delta;
    stars_satisfy_raw_fuel_message(&frame, raw_message, &trace);
    if (trace.calls != 1 || trace.recipient != 2 ||
            trace.code != expected_code)
        return 0;
    for (i = 0; i < 8; ++i)
        if (trace.payload[i] != expected[i])
            return 0;
    return 1;
}

int main(void)
{
    static const StarsI16 optimal_down[8] =
        {(StarsI16)0x800b, 11, 6, 0x0123, 4, 14, 7, 0};
    static const StarsI16 optimal_up[8] =
        {(StarsI16)0x800b, 11, 9, 0x0123, 4, 14, 7, 0};
    static const StarsI16 failed[8] =
        {(StarsI16)0x800b, 11, 14, 0x0123, 0, 0, 0, 0};
    static const StarsI16 capacity[8] =
        {(StarsI16)0x800b, 11, 40, 55, 55, 0, 0, 0};
    static const StarsI16 available[8] =
        {(StarsI16)0x800b, 14, 0x0123, 11, 25, 0, 0, 0};
    StarsI16 failures;

    failures = 0;
    if (!check_frame(STARS_SATISFY_FUEL_MESSAGE_NONE, -6,
            STARS_SATISFY_MESSAGE_TRANSFER, optimal_down))
        failures = 1;
    if (!check_frame(STARS_SATISFY_FUEL_MESSAGE_NONE, 9,
            STARS_SATISFY_MESSAGE_FLEET_TRANSFER, optimal_up))
        failures = 1;
    if (!check_frame(STARS_SATISFY_FUEL_MESSAGE_FAILED_LOAD, 0,
            STARS_SATISFY_MESSAGE_FUEL_FAILED_LOAD, failed))
        failures = 1;
    if (!check_frame(STARS_SATISFY_FUEL_MESSAGE_CAPACITY, 0,
            STARS_SATISFY_MESSAGE_FUEL_CAPACITY, capacity))
        failures = 1;
    if (!check_frame(STARS_SATISFY_FUEL_MESSAGE_AVAILABLE, 0,
            STARS_SATISFY_MESSAGE_FUEL_AVAILABLE, available))
        failures = 1;

#ifdef TARGET_NATIVE
    {
        FILE *report;
        report = fopen("SYOFUEL.TXT", "w");
        if (report) {
            fprintf(report,
                "SATISFY_ORDERS_RAW_FUEL_CHECKS=40 FAILURES=%d RESULT=%s\n",
                failures ? 1 : 0, failures ? "FAIL" : "PASS");
            fclose(report);
        }
    }
#else
    printf("SATISFY_ORDERS_RAW_FUEL_CHECKS=40 FAILURES=%d RESULT=%s\n",
        failures ? 1 : 0, failures ? "FAIL" : "PASS");
#endif
    return failures ? 1 : 0;
}
