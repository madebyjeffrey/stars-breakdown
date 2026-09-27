#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 distance, fuel_calls, ideal_calls, gate;
} Trace;
static StarsI16 ideal(void *opaque, StarsI16 mode)
{ Trace *t = (Trace *)opaque; (void)mode; ++t->ideal_calls; return 5; }
static StarsI16 planet(void *opaque, StarsPoint point, StarsI16 *owner, StarsI16 *id)
{ (void)opaque; (void)point; *owner = -1; *id = -1; return 0; }
static StarsI32 fuel(void *opaque, StarsI16 warp)
{ Trace *t = (Trace *)opaque; ++t->fuel_calls; return warp > 6 ? 300L : 10L; }
static StarsI32 capacity(void *opaque, StarsI16 stat)
{ (void)opaque; (void)stat; return 100L; }
static StarsI16 distance(void *opaque, StarsPoint from, StarsPoint to)
{ (void)from; (void)to; return ((Trace *)opaque)->distance; }
static StarsI16 gate(void *opaque, StarsPoint from, StarsPoint to)
{ (void)from; (void)to; return ((Trace *)opaque)->gate; }

int main(void)
{
    StarsWarpBestState state;
    StarsWarpBestOps ops;
    Trace trace;
    int failures = 0;
    memset(&state, 0, sizeof state); memset(&ops, 0, sizeof ops);
    memset(&trace, 0, sizeof trace);
    ops.ideal_warp = ideal; ops.nearest_planet = planet;
    ops.fuel_to_waypoint = fuel; ops.fleet_stat = capacity;
    ops.distance_term = distance; ops.can_use_gate = gate;
    state.ops = &ops; state.context = &trace;
    state.ai_mode = 1; state.order_index = 1; state.order_warp = 5;
    state.order_count = 2; state.current_player = 1;
    state.fuel_on_board = 200L;
    trace.distance = 21;
    if (stars_iwarp_best_for_waypoint(&state) != 5 ||
            trace.fuel_calls != 4 || trace.ideal_calls != 2) ++failures;
    trace.distance = 16;
    if (stars_iwarp_best_for_waypoint(&state) != 4) ++failures;
    state.target_kind = 2;
    if (stars_iwarp_best_for_waypoint(&state) != 5) ++failures;
    state.target_kind = 1; state.order_kind = 2; trace.distance = 36;
    if (stars_iwarp_best_for_waypoint(&state) != 6) ++failures;
    state.order_kind = 0; state.ai_mode = 0; trace.distance = 21;
    trace.fuel_calls = 0; trace.ideal_calls = 0;
    if (stars_iwarp_best_for_waypoint(&state) != 5 ||
            trace.fuel_calls != 1 || trace.ideal_calls != 1) ++failures;
    state.order_index = 0; trace.fuel_calls = 0; trace.gate = 1;
    if (stars_iwarp_best_for_waypoint(&state) != 5 || trace.fuel_calls != 0)
        ++failures;
    state.order_index = 1;
    if (stars_iwarp_best_for_waypoint(&state) != 11) ++failures;
    state.ops = 0;
    if (stars_iwarp_best_for_waypoint(&state) != 0) ++failures;
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\IWARP\\IWR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "IWARP_BEST_CHECKS=8 FAILURES=%d RESULT=%s\n",
            failures, failures ? "FAIL" : "PASS");
        fclose(report);
    }
#else
    printf("IWARP_BEST_CHECKS=8 FAILURES=%d RESULT=%s\n",
        failures, failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
