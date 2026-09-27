#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 applied;
    StarsI16 removed;
    StarsI16 messages;
} Trace;

static StarsI16 intersect(void *p, const StarsMinefieldFleet *fleet,
    const StarsMinefield *field, StarsI32 *entry, StarsI32 *span)
{ (void)p; (void)fleet; (void)field; *entry = 4; *span = 2; return 1; }
static StarsI32 shield(void *p, StarsI16 owner, StarsI16 kind)
{ (void)p; (void)owner; (void)kind; return 0; }
static StarsI32 apply_damage(void *p, StarsMinefieldFleet *fleet,
    const StarsMinefield *field, StarsI32 damage)
{ Trace *t = (Trace *)p; (void)fleet; (void)field; ++t->applied; return damage; }
static void remove_mine(void *p, StarsMinefield *field)
{ Trace *t = (Trace *)p; (void)field; ++t->removed; }
static void message(void *p, StarsI16 owner, StarsI16 code, StarsI16 id,
    StarsI16 a, StarsI16 b, StarsI16 c)
{ Trace *t = (Trace *)p; (void)owner; (void)code; (void)id; (void)a; (void)b; (void)c; ++t->messages; }

int main(void)
{
    StarsMinefieldFleet fleet;
    StarsMinefield field;
    StarsMinefieldOps ops;
    StarsMinefieldInput input;
    Trace trace;
    StarsI32 travel;

    memset(&fleet, 0, sizeof fleet); memset(&field, 0, sizeof field);
    memset(&ops, 0, sizeof ops); memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    fleet.id = 3; fleet.owner = 1; fleet.engine_count = 1;
    fleet.engine_count_by_type[0] = 1;
    field.id = 9; field.owner = 2; field.mine_class = 1;
    field.strength = 2; field.active = 1;
    travel = 20;
    ops.context = &trace; ops.intersect = intersect;
    ops.shield_value = shield; ops.apply_damage = apply_damage;
    ops.remove_mine = remove_mine; ops.send_message = message;
    input.fleet = &fleet; input.fields = &field; input.field_count = 1;
    input.travel = &travel; input.ops = &ops;
    if (stars_travel_through_mine_fields(&input) != 0 || travel != 4 ||
            trace.applied != 1 || trace.removed != 1 || trace.messages != 2 ||
            field.active != 0) return 1;
    travel = 20; field.active = 0;
    if (stars_travel_through_mine_fields(&input) != 1 || travel != 20) return 2;
    puts("TRAVEL_MINE_FIELDS_SOURCE=PASS");
    return 0;
}
