#include <stdio.h>
#include "core.h"

typedef struct Trace {
    StarsI16 calls;
    StarsMoveOrder order;
    StarsI16 mode;
} Trace;

static StarsI16 move_fleet(void *value, const StarsPoint *point,
    const StarsMoveOrder *order, StarsI16 mode)
{
    Trace *trace = (Trace *)value;
    trace->calls++;
    trace->order = *order;
    trace->mode = mode;
    if (point->x != 0 || point->y != 0) return -1;
    return 7;
}

int main(void)
{
    StarsRetargetMinerCandidate candidates[3];
    StarsRetargetMinerInput input;
    Trace trace;
    StarsI16 result;

    candidates[0].planet_id = 2;
    candidates[0].point.x = 10;
    candidates[0].point.y = 10;
    candidates[0].weight_a = 1;
    candidates[0].weight_b = 1;
    candidates[0].weight_c = 1;
    candidates[1].planet_id = 3;
    candidates[1].point.x = 20;
    candidates[1].point.y = 0;
    candidates[1].weight_a = 2;
    candidates[1].weight_b = 2;
    candidates[1].weight_c = 2;
    candidates[2].planet_id = 4;
    candidates[2].point.x = 100;
    candidates[2].point.y = 100;
    candidates[2].weight_a = 9;
    candidates[2].weight_b = 9;
    candidates[2].weight_c = 9;
    input.current_planet = 2;
    input.fleet_point.x = 0;
    input.fleet_point.y = 0;
    input.candidates = candidates;
    input.candidate_count = 3;
    trace.calls = 0;
    result = stars_f_retarget_miner(&input, move_fleet, &trace);
    if (result != 7 || trace.calls != 1 || trace.order.id != 3 ||
        trace.order.point.x != 20 || trace.order.flags != 0x1160U ||
        trace.mode != 0)
        return 1;
    candidates[0].weight_a = 9;
    candidates[0].weight_b = 9;
    candidates[0].weight_c = 9;
    input.candidate_count = 1;
    result = stars_f_retarget_miner(&input, move_fleet, &trace);
    if (result != 0)
        return 2;
    puts("F_RETARGET_MINER_SOURCE=PASS");
    return 0;
}
