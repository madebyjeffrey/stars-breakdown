#include <stdio.h>
#include <string.h>
#include "core.h"

void stars_stream_open(StarsU8 *data, StarsU16 name, StarsI16 mode,
    const StarsStreamApi *api, void *context)
{ (void)data; (void)name; (void)mode; (void)api; (void)context; }
void stars_stream_write(StarsU8 *data, const StarsU8 *input, StarsU16 count,
    const StarsStreamApi *api, void *context)
{ (void)data; (void)input; (void)count; (void)api; (void)context; }
void stars_write_bof(StarsU8 *data, StarsI16 player, StarsI16 type,
    StarsI16 multi, StarsTickCount ticks, StarsWriteTransfer write,
    void *context)
{
    (void)data; (void)player; (void)type; (void)multi; (void)ticks;
    (void)write; (void)context;
}

typedef struct Trace {
    StarsU16 type;
    StarsU16 length;
    StarsU16 calls;
    StarsU8 record[128];
} Trace;

static void write_record(void *context, StarsU16 type, StarsU16 length,
    const StarsU8 *record)
{
    Trace *trace = (Trace *)context;
    trace->type = type;
    trace->length = length;
    ++trace->calls;
    if (length > sizeof trace->record) length = (StarsU16)sizeof trace->record;
    memcpy(trace->record, record, length);
}

static StarsU16 get_word(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] | ((StarsU16)data[offset + 1U] << 8));
}

static int write_owned_planet(StarsReadPlanetState *state, Trace *trace,
    StarsU8 *record, StarsU16 *length)
{
    memset(record, 0, 128U);
    memset(trace, 0, sizeof *trace);
    return stars_write_planet(state, 0x0dU, 0U, record, 128U, length,
        write_record, trace);
}

int main(void)
{
    static const StarsU8 retained_planet[] = {
        0x0a, 0x00, 0x87, 0x2b, 0x00, 0x44, 0x57, 0x44, 0x32, 0x32,
        0x32, 0x3e, 0x00, 0x6a, 0xd5, 0x01, 0x7d, 0x03, 0x78, 0x01,
        0xfa, 0x00, 0x0a, 0xa0, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00
    };
    StarsReadPlanetState state;
    StarsReadPlanetInput input;
    StarsU8 record[128];
    StarsU16 length;
    Trace trace;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    state.id = 5;
    state.player = -1;
    state.flags = 2U;
    state.mineral_levels[0] = 3U;
    state.turn = 9U;
    if (!stars_write_planet(&state, 0x0eU, 1U, record,
            (StarsU16)sizeof record, &length, write_record, &trace) ||
            trace.type != 0x0eU || trace.length != 14U ||
            get_word(trace.record, 0U) != 0xf805U ||
            get_word(trace.record, 2U) != 2U ||
            trace.record[4] != 1U || trace.record[5] != 3U ||
            get_word(trace.record, 12U) != 9U)
        return 1;

    memset(&state, 0, sizeof state);
    state.id = 8;
    state.player = 2;
    state.flags = 0x0208U;
    state.routing_word = 0x1234U;
    state.minerals[0] = 7UL;
    state.minerals[1] = 0x1234UL;
    state.minerals[2] = 0x00010000UL;
    state.opaque_20[1] = 0x0001f000UL;
    state.opaque_44 = 0x11223344UL;
    state.opaque_48 = 1U;
    if (!write_owned_planet(&state, &trace, record, &length) ||
            trace.calls != 1U || trace.type != 0x0dU ||
            trace.length != 27U || length != trace.length ||
            get_word(trace.record, 0U) != 0x1008U ||
            get_word(trace.record, 2U) != 0x6208U ||
            get_word(trace.record, 11U) != 0x1234U ||
            trace.record[13] != 0x39U ||
            memcmp(trace.record + 14U,
                "\x07\x34\x12\x00\x00\x01\x00"
                "\x44\x33\x22\x11\x01\x00", 13U) != 0)
    {
        StarsU16 index;
        fprintf(stderr, "writer type=%u length=%u callback=%u\n",
            (unsigned)trace.type, (unsigned)trace.length,
            (unsigned)trace.calls);
        for (index = 0; index < trace.length; ++index)
            fprintf(stderr, "%02x", (unsigned)trace.record[index]);
        fputc('\n', stderr);
        return 2;
    }

    state.opaque_44 = 0UL;
    state.opaque_48 = 0U;
    state.opaque_20[0] = 1UL;
    state.flags = 0x0008U;
    if (!write_owned_planet(&state, &trace, record, &length) ||
            trace.length != 29U || get_word(trace.record, 2U) != 0x2808U ||
            memcmp(trace.record + 21U,
                "\x01\x00\x00\x00\x00\xf0\x01\x00", 8U) != 0)
    {
        StarsU16 index;
        fprintf(stderr, "writer type=%u length=%u flags=%04x\n",
            (unsigned)trace.type, (unsigned)trace.length,
            (unsigned)get_word(trace.record, 2U));
        for (index = 0; index < trace.length; ++index)
            fprintf(stderr, "%02x", (unsigned)trace.record[index]);
        fputc('\n', stderr);
        return 3;
    }

    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    input.id_player_word = get_word(retained_planet, 0U);
    input.flags_word = get_word(retained_planet, 2U);
    input.header_flags = (StarsU16)((0x0dU << 10) | sizeof retained_planet);
    input.game_turn = 0U;
    input.payload = retained_planet + 4U;
    input.payload_size = (StarsU16)(sizeof retained_planet - 4U);
    if (!stars_read_planet(&input, &state, 0, 0, 0)) return 4;
    state.flags = (StarsU16)(state.flags & 0xf7ffU);
    if (!write_owned_planet(&state, &trace, record, &length) ||
            trace.length != sizeof retained_planet ||
            memcmp(trace.record, retained_planet,
                sizeof retained_planet) != 0)
    {
        StarsU16 index;
        fprintf(stderr, "retained planet round trip length=%u expected=%lu\n",
            (unsigned)trace.length, (unsigned long)sizeof retained_planet);
        for (index = 0; index < trace.length; ++index)
            fprintf(stderr, "%02x", (unsigned)trace.record[index]);
        fputc('\n', stderr);
        return 5;
    }

    puts("WRITE_PLANET_SOURCE=PASS");
    return 0;
}
