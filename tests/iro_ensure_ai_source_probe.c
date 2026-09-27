#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct IroProbe {
    int ensured;
    int validated;
    int writes;
    StarsI16 latest;
    StarsU8 current_written;
    StarsI16 percent_written;
} IroProbe;

static void ensure_designs(void *context)
{ ++((IroProbe *)context)->ensured; }
static StarsI16 latest_starbase(void *context)
{ return ((IroProbe *)context)->latest; }
static void validate_history(void *context)
{ ++((IroProbe *)context)->validated; }
static void write_research(void *context, StarsU8 current, StarsI16 percent)
{
    IroProbe *probe = (IroProbe *)context;
    ++probe->writes; probe->current_written = current;
    probe->percent_written = percent;
}

int main(void)
{
    StarsIroAiState state;
    StarsIroAiOps ops;
    IroProbe probe;
    StarsU8 requirements[2];
    StarsI16 latest;
    StarsI16 result;
    int failures = 0;

    state.tech[0] = 5; state.tech[1] = 6; state.tech[2] = 7;
    state.tech[3] = 8; state.tech[4] = 9; state.tech[5] = 10;
    state.current_tech = 0; state.percent_research = 0;
    requirements[0] = (StarsU8)((1U << 5) | 2U);
    requirements[1] = (StarsU8)((2U << 5) | 3U);
    memset(&probe, 0, sizeof(probe)); probe.latest = 11;
    ops.ensure_designs = ensure_designs; ops.latest_starbase = latest_starbase;
    ops.validate_history = validate_history; ops.write_research = write_research;
    result = stars_iro_ensure_ai(requirements, 2, &latest, 20, &state,
        &ops, &probe);
    if (result != 0x39e || latest != 11 || probe.ensured != 1 ||
        probe.validated != 1 || state.current_tech != 0 ||
        state.percent_research != 20 || probe.writes != 0)
        ++failures;

    requirements[0] = (StarsU8)((2U << 5) | 8U);
    requirements[1] = (StarsU8)((3U << 5) | 11U);
    state.current_tech = 0; probe.writes = 0;
    result = stars_iro_ensure_ai(requirements, 2, 0, 15, &state, &ops, &probe);
    if (result != 0 || state.current_tech != 0x32U || probe.writes != 1)
        ++failures;
    state.tech[0] = 1; state.tech[1] = 5;
    state.current_tech = 1; probe.writes = 0;
    result = stars_iro_ensure_ai(requirements, 0, 0, 0, &state,
        &ops, &probe);
    if (result != 0x39e || state.current_tech != 0 ||
        probe.writes != 1 || probe.current_written != 0 ||
        probe.percent_written != 0)
        ++failures;
    printf("IRO_ENSURE_AI_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
