#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 events[32];
    StarsI16 players[32];
    StarsI16 values[32][5];
    StarsI16 count;
    StarsI16 learned[8];
    StarsI16 learn_count;
    StarsI16 uninhabited;
} Trace;

static StarsBattleMessagePlanet planet;
static StarsBattleMessagePlanet *find_planet(void *context, StarsI16 id)
{
    (void)context;
    return id == planet.id ? &planet : (StarsBattleMessagePlanet *)0;
}
static StarsI16 race_stat(void *context, StarsI16 player, StarsI16 stat)
{
    (void)context; (void)player; (void)stat; return 8;
}
static void uninhabit(void *context, StarsBattleMessagePlanet *value)
{
    Trace *trace = (Trace *)context; (void)value; ++trace->uninhabited;
}
static void learn(void *context, StarsI16 player, StarsI16 x, StarsI16 y,
    StarsI16 event)
{
    Trace *trace = (Trace *)context; (void)x; (void)y;
    if (trace->learn_count < 8) trace->learned[trace->learn_count++] = event;
    (void)player;
}
static void send_message(void *context, StarsI16 player, StarsI16 event,
    StarsI16 object, StarsI16 x, StarsI16 y, StarsI16 a, StarsI16 b,
    StarsI16 c, StarsI16 d, StarsI16 e)
{
    Trace *trace = (Trace *)context; StarsI16 i;
    (void)object; (void)x; (void)y;
    if (trace->count >= 32) return;
    i = trace->count++;
    trace->players[i] = player; trace->events[i] = event;
    trace->values[i][0] = a; trace->values[i][1] = b;
    trace->values[i][2] = c; trace->values[i][3] = d; trace->values[i][4] = e;
}

int main(void)
{
    StarsSendBattleMessagesInput input;
    StarsBattleMessageFleet fleets[2];
    StarsU16 losses[16 * 16];
    Trace trace;
    memset(&input, 0, sizeof input); memset(fleets, 0, sizeof fleets);
    memset(losses, 0, sizeof losses); memset(&trace, 0, sizeof trace);
    fleets[0].id = 7; fleets[0].player = 0; fleets[0].planet_id = 22;
    fleets[0].ship_counts[2] = 3; fleets[0].missed_flags = 0x80;
    fleets[1].id = 8; fleets[1].player = 1; fleets[1].planet_id = 22;
    fleets[1].ship_counts[3] = 2;
    planet.id = 22; planet.owner = 0; planet.flags = 0x0200;
    planet.starbase_design = 4; planet.mineral_value = 1200; planet.population = 4;
    input.fleets = fleets; input.fleet_count = 2; input.player_count = 3;
    input.battle_player_mask = 2; input.battle_player_count = 2;
    input.battle_id = 11; input.player_losses = losses;
    input.spectator_players = 4; input.missed_players = 1;
    input.find_planet = find_planet; input.send_message = send_message;
    input.learn_tech = learn; input.uninhabit = uninhabit; input.context = &trace;
    stars_send_battle_messages(&input);
    if (!(losses[2] & 0x4000U) || !(losses[19] & 0x4000U) ||
            trace.count != 3 || trace.events[0] != 0x00f9 ||
            trace.events[1] != 0x0180 || trace.events[2] != 0x009f) return 1;
    memset(&trace, 0, sizeof trace); memset(losses, 0, sizeof losses);
    input.battle_player_mask = 3; input.battle_player_count = 2;
    input.ships_involved = 2; input.spectator_players = 0;
    input.missed_players = 0; losses[2] = 3; losses[19] = 2;
    stars_send_battle_messages(&input);
    if (trace.count != 2 || trace.events[0] != 0x0091 ||
            trace.events[1] != 0x0091) return 2;
    memset(&trace, 0, sizeof trace); memset(losses, 0, sizeof losses);
    input.battle_player_mask = 1; input.battle_player_count = 1;
    input.ships_involved = 0; input.starbase_died = 1;
    input.starbase_damaged = 0; input.race_stat = race_stat;
    stars_send_battle_messages(&input);
    if (trace.count != 1 || trace.events[0] != 0x008d ||
            trace.uninhabited != 1) return 3;
    puts("SEND_BATTLE_MESSAGES_SOURCE=PASS");
    return 0;
}
