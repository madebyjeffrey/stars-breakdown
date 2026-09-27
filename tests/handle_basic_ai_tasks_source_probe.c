#include "core.h"
#include <stdio.h>

typedef struct BasicProbe { int fleet; int queues; int upgrades; int finishes; int mines; } BasicProbe;
static void keep(void *c) { ((BasicProbe *)c)->fleet = 1; }
static void queue(void *c, StarsI16 d) { (void)d; ((BasicProbe *)c)->queues = 1; }
static StarsI16 upgrade(void *c, StarsU16 id) { (void)id; ++((BasicProbe *)c)->upgrades; return 1; }
static void finish(void *c, StarsU16 id, StarsI16 did) { (void)id; (void)did; ++((BasicProbe *)c)->finishes; }
static void mines(void *c) { ((BasicProbe *)c)->mines = 1; }
int main(void)
{
    StarsAiBasicPlanet planet;
    StarsHandleBasicOps ops;
    BasicProbe probe;
    int failures = 0;
    planet.id = 2; planet.owner = 1; planet.research_level = 1;
    planet.mineral_total = 0; planet.ai_marker = 0;
    probe.fleet = 0; probe.queues = 0; probe.upgrades = 0;
    probe.finishes = 0; probe.mines = 0;
    ops.keep_fleets_moving = keep; ops.queue_starbases = queue;
    ops.upgrade_starbase = upgrade; ops.fling = 0; ops.scanner = 0;
    ops.defenses = 0; ops.terraforming = 0; ops.finish_production = finish;
    ops.fix_planets_under_attack = 0; ops.add_mines_blocked = mines;
    stars_handle_basic_ai_tasks(&planet, 1, 1, 4, &ops, &probe);
    if (!probe.fleet || !probe.queues || probe.upgrades != 1 ||
            probe.finishes != 1 || !probe.mines)
        ++failures;
    printf("HANDLE_BASIC_AI_CHECKS=1 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
