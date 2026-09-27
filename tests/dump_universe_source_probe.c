#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct DumpState {
    StarsU16 opens;
    StarsU16 closes;
    StarsU16 writes;
    StarsU16 bytes;
    StarsU16 alert_id;
    StarsI16 alert_style;
    StarsU8 name_seen;
    StarsU8 output[256];
} DumpState;

static StarsI16 open_stream(void *context, const StarsU8 *name, StarsU16 mode)
{
    DumpState *state = (DumpState *)context;
    ++state->opens;
    if (mode != 0x1012U || strcmp((const char *)name, "BASE.map") != 0)
        return 0;
    return 1;
}

static void write_stream(void *context, const StarsU8 *data, StarsU16 count)
{
    DumpState *state = (DumpState *)context;
    if ((StarsU16)(state->bytes + count) <= (StarsU16)sizeof(state->output))
        memcpy(state->output + state->bytes, data, count);
    state->bytes = (StarsU16)(state->bytes + count);
    ++state->writes;
}

static void close_stream(void *context)
{
    DumpState *state = (DumpState *)context;
    ++state->closes;
}

static const StarsU8 *planet_name(void *context, StarsI16 id)
{
    DumpState *state = (DumpState *)context;
    state->name_seen = (StarsU8)(state->name_seen + (id == 7 ? 1 : 0));
    return (const StarsU8 *)(id == 7 ? "Alpha" : "Beta");
}

static void alert(void *context, StarsU16 id, StarsI16 style)
{
    DumpState *state = (DumpState *)context;
    state->alert_id = id;
    state->alert_style = style;
}

int main(void)
{
    static const StarsDumpUniversePlanet planets[2] = {
        { 7, -12, 34 }, { 8, 56, -78 }
    };
    StarsDumpUniverseInput input;
    DumpState state;
    StarsU16 silent;

    memset(&input, 0, sizeof(input));
    memset(&state, 0, sizeof(state));
    silent = 9U;
    input.game_id = 1UL;
    input.player = 0;
    input.base_name = (const StarsU8 *)"BASE";
    input.planets = planets;
    input.planet_count = 2U;
    input.planet_name = planet_name;
    input.open_stream = open_stream;
    input.write_stream = write_stream;
    input.close_stream = close_stream;
    input.alert = alert;
    input.file_error_silent = &silent;
    input.context = &state;
    stars_dump_universe(&input);
    if (state.opens != 1U || state.closes != 1U || state.writes != 3U ||
            state.name_seen != 1U || state.alert_id != 0x4d6U ||
            state.alert_style != 0x40 || silent != 9U ||
            strcmp((const char *)state.output,
                "#\tX\tY\tName\r\n1\t-12\t34\tAlpha\r\n2\t56\t-78\tBeta\r\n") != 0)
        return 1;

    memset(&state, 0, sizeof(state));
    input.game_id = 0UL;
    stars_dump_universe(&input);
    if (state.opens != 0U || state.alert_id != 0x4d7U ||
            state.alert_style != 0x10 || silent != 9U)
        return 2;
    puts("DUMP_UNIVERSE_SOURCE=PASS");
    return 0;
}
