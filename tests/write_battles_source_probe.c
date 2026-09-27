#ifndef TARGET_NATIVE
#include <stdio.h>
#endif
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 failures;
    StarsU16 records;
    StarsU16 primary;
    StarsU16 continuation;
    StarsU16 record_sizes[4];
    StarsU16 players;
    StarsU16 designs;
    StarsU16 fleets;
    StarsU16 planets;
} Trace;

static void mark_player(void *opaque, StarsU16 player)
{
    Trace *t = (Trace *)opaque;
    (void)player;
    ++t->players;
}

static void mark_design(void *opaque, StarsU16 player, StarsU16 slot)
{
    Trace *t = (Trace *)opaque;
    if (player != 2U || slot != 4U) ++t->failures;
    ++t->designs;
}

static void mark_fleet(void *opaque, StarsU16 player, StarsU16 id)
{
    Trace *t = (Trace *)opaque;
    if (player != 3U || id != 9U) ++t->failures;
    ++t->fleets;
}

static void mark_planet(void *opaque, StarsU16 id, StarsI16 viewer)
{
    Trace *t = (Trace *)opaque;
    if (id != 0x44U || viewer != 1) ++t->failures;
    ++t->planets;
}

static void write_record(void *opaque, StarsU16 type, StarsU16 size,
    const StarsU8 *data)
{
    Trace *t = (Trace *)opaque;
    if (!data || size >= STARS_BATTLE_RECORD_LIMIT ||
            (type != STARS_BATTLE_PRIMARY &&
             type != STARS_BATTLE_CONTINUATION)) ++t->failures;
    if (t->records < 4U) t->record_sizes[t->records] = size;
    if (type == STARS_BATTLE_PRIMARY) ++t->primary;
    if (type == STARS_BATTLE_CONTINUATION) ++t->continuation;
    ++t->records;
}

int main(void)
{
    Trace t;
    StarsWriteBattlesOps ops;
    StarsWriteBattlesInput input;
    StarsWriteBattleEvent events[2];
    StarsWriteBattleBlock block;
    StarsU8 small[80];
    StarsU8 large[14U + 40U * STARS_BATTLE_EVENT_BYTES];
    StarsWriteBattleEvent many[40];
    StarsU16 i;
    memset(&t, 0, sizeof t);
    memset(&ops, 0, sizeof ops);
    ops.mark_player = mark_player;
    ops.mark_design = mark_design;
    ops.mark_fleet = mark_fleet;
    ops.mark_planet = mark_planet;
    ops.write_record = write_record;
    memset(small, 0, sizeof small);
    events[0].player = 2U; events[0].is_design = 1U;
    events[0].design_slot = 4U; events[0].object_id = 0U;
    events[1].player = 3U; events[1].is_design = 0U;
    events[1].object_id = 9U;
    block.visibility = 2UL; block.planet_id = 0x44U;
    block.data = small; block.size = 72U;
    block.events = events; block.event_count = 2U;
    input.player = 1; input.log_is_current = 0;
    input.blocks = &block; input.block_count = 1U;
    if (!stars_write_battles(&input, &ops, &t) || t.records != 1U ||
            t.primary != 1U || t.continuation != 0U ||
            t.record_sizes[0] != 72U || t.players != 2U || t.designs != 1U ||
            t.fleets != 1U || t.planets != 1U || t.failures != 0U) return 1;

    memset(&t, 0, sizeof t);
    memset(large, 0x5a, sizeof large);
    for (i = 0U; i < 40U; ++i) {
        many[i].data = large + 14U + i * STARS_BATTLE_EVENT_BYTES;
        many[i].size = STARS_BATTLE_EVENT_BYTES;
        many[i].player = 3U;
        many[i].object_id = 9U;
        many[i].design_slot = 0U;
        many[i].is_design = 0U;
    }
    block.visibility = 2UL; block.planet_id = 0xffffU;
    block.data = large; block.size = sizeof large;
    block.events = many; block.event_count = 40U;
    if (!stars_write_battles(&input, &ops, &t) || t.records != 2U ||
            t.primary != 1U || t.continuation != 1U ||
            t.record_sizes[0] != 1000U || t.record_sizes[1] != 174U ||
            t.failures != 0U) return 1;

    memset(&t, 0, sizeof t);
    input.log_is_current = 1;
    if (!stars_write_battles(&input, &ops, &t) || t.records != 0U ||
            t.failures != 0U) return 1;
#ifndef TARGET_NATIVE
    printf("WRITE_BATTLES_CHECKS=3 FAILURES=0 RESULT=PASS\n");
#endif
    return 0;
}
