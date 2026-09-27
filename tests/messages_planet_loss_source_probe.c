#include "core.h"
#include <stdio.h>

typedef struct LossLog {
    StarsU16 ids[3];
    StarsI16 players[3];
    StarsI16 details[3];
    StarsU16 calls;
} LossLog;

static void *lookup(void *context, StarsU16 id)
{
    LossLog *log = (LossLog *)context;
    log->ids[log->calls] = id;
    return id == 0x1234U || id == 0x5678U ? log : 0;
}

static void mark(void *context, void *planet, StarsI16 player, StarsI16 detail)
{
    LossLog *log = (LossLog *)context;
    (void)planet;
    log->players[log->calls] = player;
    log->details[log->calls] = detail;
    ++log->calls;
}

int main(void)
{
    StarsU8 messages[16];
    StarsPlanetLossOps ops;
    LossLog log;
    int failures = 0;
    messages[0] = 0x02U; messages[1] = 7U; messages[2] = 0U;
    messages[3] = 0x34U; messages[4] = 0x12U;
    messages[5] = 0x42U; messages[6] = 0x8fU; messages[7] = 0x06U;
    messages[8] = 0; messages[9] = 0;
    messages[10] = 0; messages[11] = 0;
    messages[12] = 0x78U; messages[13] = 0x56U;
    ops.lookup = lookup; ops.mark = mark;
    log.calls = 0U;
    stars_mark_planets_player_lost(messages, 14U, 2, &ops, &log);
    if (log.calls != 2U || log.ids[0] != 0x1234U ||
        log.ids[1] != 0x5678U || log.players[0] != 2 ||
        log.players[1] != 2 || log.details[0] != 3 || log.details[1] != 3)
        ++failures;
    log.calls = 0U;
    stars_mark_planets_player_lost(messages, 14U, 3, &ops, &log);
    if (log.calls != 0U) ++failures;
    /* Ordinary production/scrap messages carry planet gotos too. They must
     * not mark a planet as lost or even invoke the planet lookup. */
    messages[1] = 0x3fU; messages[2] = 0U;
    messages[6] = 0x41U; messages[7] = 0x07U;
    log.calls = 0U;
    stars_mark_planets_player_lost(messages, 14U, 2, &ops, &log);
    if (log.calls != 0U) ++failures;
    messages[1] = 0x23U;
    stars_mark_planets_player_lost(messages, 5U, 2, &ops, &log);
    if (log.calls != 1U || log.ids[0] != 0x1234U) ++failures;
    messages[1] = 0x40U;
    log.calls = 0U;
    stars_mark_planets_player_lost(messages, 5U, 2, &ops, &log);
    if (log.calls != 1U || log.ids[0] != 0x1234U) ++failures;
    printf("MESSAGES_PLANET_LOSS_CHECKS=5 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
