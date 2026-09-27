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
    StarsU16 messages;
    StarsI16 message;
    StarsI16 value;
} Trace;

static void send_message(void *context, StarsI16 player, StarsI16 message,
    StarsI16 id, StarsI16 object, StarsI16 value)
{
    Trace *trace = (Trace *)context;
    (void)player; (void)id; (void)object;
    trace->messages = (StarsU16)(trace->messages + 1U);
    trace->message = message;
    trace->value = value;
}
static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 which)
{ (void)context; (void)player; (void)which; return 3; }
static StarsI16 optimal(void *context, const StarsReadPlanetState *state,
    StarsI16 player)
{ (void)context; (void)state; (void)player; return 42; }

int main(void)
{
    static const StarsU8 retained_planet[] = {
        0x0a, 0x00, 0x87, 0x2b, 0x00, 0x44, 0x57, 0x44, 0x32, 0x32,
        0x32, 0x3e, 0x00, 0x6a, 0xd5, 0x01, 0x7d, 0x03, 0x78, 0x01,
        0xfa, 0x00, 0x0a, 0xa0, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00
    };
    static const StarsU8 unowned_planet[] = {
        0x00, 0xf8, 0x07, 0x11, 0x00, 0x44, 0x57, 0x44, 0x29, 0x0e,
        0x40
    };
    static const StarsU8 payload[] = {
        0x55, 10, 20, 30, 1, 2, 3, 40, 50, 60,
        0x34, 0x12, 0x39, 7, 0x34, 0x12, 0x04, 0x03, 0x02, 0x01,
        0x78, 0x56, 0x34, 0x12, 0xbc, 0x9a
    };
    static const StarsU8 short_payload[] = { 0 };
    StarsReadPlanetInput input;
    StarsReadPlanetState state;
    StarsReadPlanetOps ops;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    input.id_player_word = 0x0007U;
    input.flags_word = 0x6204U;
    input.header_flags = 0;
    input.game_turn = 37;
    input.payload = payload;
    input.payload_size = (StarsU16)sizeof payload;
    ops.send_message = send_message;
    if (!stars_read_planet(&input, &state, 0, &ops, &trace)) return 1;
    if (state.id != 7 || state.player != 0 || state.turn != 37 ||
            state.mineral_levels[1] != 20U || state.environment[2] != 60U ||
            state.minerals[0] != 7U || state.minerals[1] != 0x1234U ||
            state.minerals[2] != 0x01020304UL ||
            state.opaque_44 != 0x12345678UL || state.opaque_48 != 0x9abcU)
        return 2;

    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    input.id_player_word = 0x0001U;
    input.flags_word = 0;
    input.game_turn = 12;
    input.payload = short_payload;
    input.payload_size = 1U;
    ops.race_stat = race_stat;
    ops.optimal_value = optimal;
    if (!stars_read_planet(&input, &state, 0, &ops, &trace) ||
            trace.message != 0xaa || state.turn != 12U ||
            (state.flags & 0x0800U) == 0U)
        return 3;

    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    input.id_player_word = (StarsU16)(retained_planet[0] |
        ((StarsU16)retained_planet[1] << 8));
    input.flags_word = (StarsU16)(retained_planet[2] |
        ((StarsU16)retained_planet[3] << 8));
    input.header_flags = 0x3421U;
    input.payload = retained_planet + 4U;
    input.payload_size = (StarsU16)(sizeof retained_planet - 4U);
    if (!stars_read_planet(&input, &state, 0, 0, 0))
        return 4;
    if (state.id != 10 || state.player != 0 || state.routing_word != 62U ||
            state.minerals[0] != 469UL || state.minerals[1] != 893UL ||
            state.minerals[2] != 376UL || state.minerals[3] != 250UL)
        return 5;

    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    input.id_player_word = (StarsU16)(unowned_planet[0] |
        ((StarsU16)unowned_planet[1] << 8));
    input.flags_word = (StarsU16)(unowned_planet[2] |
        ((StarsU16)unowned_planet[3] << 8));
    input.header_flags = 0x340bU;
    input.payload = unowned_planet + 4U;
    input.payload_size = (StarsU16)(sizeof unowned_planet - 4U);
    if (!stars_read_planet(&input, &state, 0, 0, 0) ||
            state.id != 0 || state.player != -1 ||
            state.opaque_20[0] != 0UL ||
            state.opaque_20[1] != 0x0041f000UL)
        return 6;
    puts("READ_PLANET_SOURCE=PASS");
    return 0;
}
