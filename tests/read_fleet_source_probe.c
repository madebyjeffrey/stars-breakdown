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

typedef struct Trace { StarsI16 alerts; } Trace;
static void alert(void *context, StarsI16 code)
{ (void)code; ((Trace *)context)->alerts++; }
static StarsI16 decode(void *context, const StarsU8 *input, StarsU16 length,
    StarsU8 *output, StarsU16 *output_length)
{
    StarsU16 n = length < *output_length ? length : *output_length;
    (void)context;
    memcpy(output, input, n);
    *output_length = n;
    return 1;
}

int main(void)
{
    static const StarsU16 ship_words[] = { 2U, 5U };
    static const StarsU8 mineral_payload[] = {
        7U, 0x34U, 0x12U, 0x04U, 0x03U, 0x02U, 0x01U, 9U
    };
    static const StarsU8 tail[] = {
        5U, 0U, 100U, 0U, 200U, 0U, 3U, 0U, 0U, 0U
    };
    static const StarsU8 name[] = { 0U, 'F', 'l', 'e', 'e', 't', 0U };
    StarsReadFleetInput input;
    StarsReadFleetState state;
    StarsReadFleetOps ops;
    Trace trace;

    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    input.fleet_flags = 0x0008U;
    input.ship_bitmap = 5U;
    input.ship_words = ship_words;
    input.mineral_bitmap = 0x0139U;
    input.mineral_payload = mineral_payload;
    input.mineral_payload_size = (StarsU16)sizeof mineral_payload;
    input.header_type = 0x10U;
    input.tail_payload = tail;
    input.tail_payload_size = (StarsU16)sizeof tail;
    input.planet = 2;
    input.x = 10; input.y = 20; input.game_turn = 0;
    ops.alert = alert;
    if (!stars_read_fleet(&input, &state, &ops, &trace)) return 1;
    if (state.ship_counts[0] != 2U || state.ship_counts[2] != 5U ||
            state.minerals[0] != 7U || state.minerals[1] != 0x1234U ||
            state.minerals[2] != 0x01020304UL || state.minerals[4] != 9U ||
            state.design_plan != 3U || state.order_count != 0U)
        return 2;

    memset(&input, 0, sizeof input);
    memset(&state, 0, sizeof state);
    input.fleet_flags = 0x0008U;
    input.header_type = 0x15U;
    input.name_payload = name;
    input.name_payload_size = (StarsU16)sizeof name;
    ops.decompress_name = decode;
    if (!stars_read_fleet(&input, &state, &ops, &trace) ||
            strcmp(state.name, "Fleet") != 0)
        return 3;
    puts("READ_FLEET_SOURCE=PASS");
    return 0;
}
