#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeState {
    int queued;
    int item;
    int count;
    int selected;
    StarsU16 target;
} ProbeState;

static StarsI16 probe_target(void *context,
    const StarsCyberPacketPlanet *source, StarsU16 *target)
{
    ProbeState *state;
    state = (ProbeState *)context;
    *target = (StarsU16)(source->id + 1);
    state->target = *target;
    return 1;
}

static StarsI16 probe_queue(void *context, StarsI16 item, StarsI16 count,
    StarsI16 object, StarsI16 mode)
{
    ProbeState *state;
    (void)object;
    (void)mode;
    state = (ProbeState *)context;
    state->queued += 1;
    state->item = item;
    state->count = count;
    return 1;
}

static void probe_select(void *context, StarsU16 source, StarsU16 target)
{
    ProbeState *state;
    (void)source;
    (void)target;
    state = (ProbeState *)context;
    state->selected += 1;
}

int main(void)
{
    StarsCyberPacketPlanet planets[2];
    StarsDoCyberPacketsOps ops;
    StarsDoCyberPacketsInput input;
    ProbeState state;

    memset(planets, 0, sizeof(planets));
    memset(&state, 0, sizeof(state));
    planets[0].id = 4;
    planets[0].owner = 2;
    planets[0].packet_flags = 0x0080;
    planets[0].packet_strength = 15;
    planets[0].available[0] = 1000;
    planets[0].available[1] = 800;
    planets[0].available[2] = 600;
    planets[0].cost[0] = 900;
    planets[0].cost[1] = 100;
    planets[0].cost[2] = 500;
    planets[1].owner = 3;
    ops.find_target = probe_target;
    ops.queue_item = probe_queue;
    ops.select_planet = probe_select;
    ops.context = &state;
    input.planets = planets;
    input.planet_count = 2;
    input.player = 2;
    input.minimum_strength = 5;
    input.maximum_packets = 0;
    input.ops = &ops;
    stars_do_cyber_packets(&input);
    if (state.queued != 1 || state.item != 15 || state.count != 3 ||
            state.selected != 1 || state.target != 5 ||
            planets[0].packet_strength != 0)
        return 1;
    puts("DO_CYBER_PACKETS_SOURCE=PASS");
    return 0;
}
