#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ProbeTrace { StarsI16 ellipses, modes, intersections; } ProbeTrace;
static StarsI16 probe_intersect(void *opaque, const StarsRect *a, const StarsRect *b)
{ ProbeTrace *t = (ProbeTrace *)opaque; ++t->intersections;
    return !((StarsI16)a->right < (StarsI16)b->left ||
        (StarsI16)a->left > (StarsI16)b->right); }
static void probe_mode(void *opaque, StarsU16 dc, StarsI16 mode)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)mode; ++t->modes; }
static StarsU32 probe_color(void *opaque, StarsU16 dc, StarsU32 color)
{ (void)opaque; (void)dc; (void)color; return 17UL; }
static void probe_ellipse(void *opaque, StarsU16 dc, const StarsRect *rect)
{ ProbeTrace *t = (ProbeTrace *)opaque; (void)dc; (void)rect; ++t->ellipses; }

int main(void)
{
    StarsRadarCircleOps ops;
    StarsRadarCircleState state;
    ProbeTrace trace;
    StarsRect rect;
    StarsI16 failures;
    memset(&ops, 0, sizeof ops); memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    ops.intersect = probe_intersect; ops.set_mode = probe_mode;
    ops.set_background = probe_color; ops.ellipse = probe_ellipse;
    state.hdc = 4; state.capacity = 4; state.clip.left = 0;
    state.clip.top = 0; state.clip.right = 100; state.clip.bottom = 100;
    state.hollow = 1; state.ops = &ops; state.context = &trace;
    rect.left = 0; rect.top = 0; rect.right = 20; rect.bottom = 20;
    stars_draw_radar_circle(&state, &rect);
    rect.left = 80; rect.top = 80; rect.right = 100; rect.bottom = 100;
    stars_draw_radar_circle(&state, &rect);
    stars_draw_radar_circle(&state, (StarsRect *)0);
    state.clip.left = 40; state.clip.top = 40; state.clip.right = 60; state.clip.bottom = 60;
    rect.left = 0; rect.top = 0; rect.right = 100; rect.bottom = 100;
    stars_draw_radar_circle(&state, &rect);
    failures = state.covered != 1 || state.current != 0;
    state.capacity = 0; state.covered = 0;
    rect.left = 40; rect.top = 40; rect.right = 50; rect.bottom = 50;
    stars_draw_radar_circle(&state, &rect);
    failures += trace.ellipses < 3;
    failures += trace.intersections < 3;
    {
        FILE *report;
        report = fopen("C:\\RADAR\\RDR.TXT", "w");
        if (!report) return 3;
        fprintf(report, "RADAR_CIRCLE_CHECKS=3 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("RADAR_CIRCLE_CHECKS=3 FAILURES=%d RESULT=%s\n", (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
