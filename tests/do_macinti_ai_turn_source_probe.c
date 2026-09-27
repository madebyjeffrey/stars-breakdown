#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 merge_count;
    StarsI16 merge_masks[4];
    StarsI16 status_count;
    StarsI16 status_budgets[6];
    StarsI16 split_count;
    StarsU8 split_masks[5][16];
    StarsI16 ensured;
    StarsI16 starbase;
    StarsI16 colonizer;
    StarsI16 planets;
    StarsI16 fleets;
} Trace;

static StarsI16 create_starter(void *context)
{
    (void)context;
    return 1;
}

static StarsI16 merge(void *context, StarsI16 mask)
{
    Trace *trace;
    trace = (Trace *)context;
    trace->merge_masks[trace->merge_count] = mask;
    ++trace->merge_count;
    return 1;
}

static StarsI16 status(void *context, StarsI16 first, StarsI16 last,
    StarsI16 budget, StarsI16 *selected, StarsU8 mask[16])
{
    Trace *trace;
    (void)first;
    (void)last;
    (void)budget;
    (void)selected;
    trace = (Trace *)context;
    if (first == 12 && last == 13) {
        mask[3] = 2;
        mask[12] = 3;
        mask[13] = 4;
    }
    if (trace->status_count < 6)
        trace->status_budgets[trace->status_count] = budget;
    ++trace->status_count;
    return 1;
}
static StarsI16 design_status_info(void *context, StarsI16 index,
    StarsI16 *active, StarsI16 *hull_kind)
{
    (void)context;
    if (index == 12) {
        *active = 0;
        *hull_kind = 0x1d;
    } else if (index == 13) {
        *active = 0;
        *hull_kind = 0x1e;
    } else {
        return 0;
    }
    return 1;
}

static StarsI16 split(void *context, const StarsU8 mask[16])
{
    Trace *trace;
    trace = (Trace *)context;
    if (trace->split_count < 5)
        memcpy(trace->split_masks[trace->split_count], mask, 16);
    ++trace->split_count;
    return 1;
}

static StarsI16 ensure(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->ensured;
    return 1;
}

static StarsI16 ensure_starbase(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->starbase;
    return 1;
}

static StarsI16 colonizer(void *context, StarsI16 *count)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->colonizer;
    *count = 3;
    return 1;
}

static StarsI16 visit_planets(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->planets;
    return 1;
}

static StarsI16 visit_fleets(void *context)
{
    Trace *trace;
    trace = (Trace *)context;
    ++trace->fleets;
    return 1;
}

int main(void)
{
    StarsMacintiAiTurnOps ops;
    StarsMacintiAiTurnInput input;
    Trace trace;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    ops.create_starter = create_starter;
    ops.merge_shdefs = merge;
    ops.check_design_status = status;
    ops.design_status_info = design_status_info;
    ops.split_shdefs = split;
    ops.ensure_shdefs = ensure;
    ops.ensure_starbase_designs = ensure_starbase;
    ops.colonizer_decision = colonizer;
    ops.visit_planets = visit_planets;
    ops.visit_fleets = visit_fleets;
    ops.context = &trace;
    input.turn = 150;
    input.advanced_player = 1;
    input.starter_missing = 1;
    input.ops = &ops;
    if (!stars_do_macinti_ai_turn(&input) || trace.merge_count != 4 ||
            trace.merge_masks[0] != 0x03fc || trace.merge_masks[1] != 1 ||
            trace.merge_masks[2] != -0x4000 || trace.merge_masks[3] != 0x3000 ||
            trace.status_count != 6 || trace.split_count != 5 ||
            trace.status_budgets[0] != 70 ||
            trace.status_budgets[1] != 5000 ||
            trace.status_budgets[2] != 70 ||
            trace.status_budgets[5] != 70 ||
            trace.split_masks[0][3] != 2 ||
            trace.split_masks[0][12] != 0 ||
            trace.split_masks[0][13] != 4 ||
            trace.split_masks[1][0] != 2 ||
            trace.split_masks[2][1] != 2 ||
            trace.split_masks[3][14] != 2 ||
            trace.split_masks[3][15] != 2 ||
            trace.split_masks[4][10] != 2 ||
            trace.split_masks[4][11] != 2 ||
            trace.ensured != 1 || trace.starbase != 1 || trace.colonizer != 1 ||
            trace.planets != 1 || trace.fleets != 1 ||
            input.armada_potency[0] != 7 || input.armada_potency[1] != 3 ||
            input.armada_potency[2] != 8 || input.armada_potency[3] != 3 ||
            input.starter_order != 7 || input.colonizer_count != 3)
        return 1;
    memset(&trace, 0, sizeof trace);
    input.turn = 30;
    input.starter_missing = 0;
    input.armada_potency[0] = 0;
    if (!stars_do_macinti_ai_turn(&input) || trace.merge_count != 4 ||
            trace.merge_masks[0] != 0x03fc || trace.split_count != 0 ||
            input.armada_potency[0] != 6 || input.armada_potency[2] != 6)
        return 2;
    puts("MACINTI_AI_TURN_CHECKS=29 FAILURES=0 RESULT=PASS");
    return 0;
}
