#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 hit_calls;
} Trace;

static StarsI16 conflicting_hit(void *context, StarsI16 slot,
    const StarsPoint *point)
{
    Trace *trace;
    trace = (Trace *)context;
    (void)slot; (void)point;
    ++trace->hit_calls;
    return 0;
}

static void install(StarsDropPartOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->hit_test = conflicting_hit;
    ops->context = trace;
}

static void setup(StarsDropPartInput *input, StarsDropPartOps *ops,
    Trace *trace, StarsDropPartSlot *slots, const StarsRect *rects)
{
    memset(input, 0, sizeof *input);
    install(ops, trace);
    input->client_width = 100;
    input->source_grhst = 2;
    input->source_quantity = 0x0301;
    input->source_index = 1;
    input->no_modify = 1;
    input->slots = slots;
    input->slot_count = 3;
    input->packed_slot_rects = rects;
    input->packed_slot_rect_count = 3;
    input->ops = ops;
}

static StarsI16 run_case(StarsDropPartInput *input, StarsI16 x, StarsI16 y)
{
    input->point.x = x;
    input->point.y = y;
    return stars_drop_part(input);
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    static const StarsRect rects[3] = {
        {10, 20, 30, 40}, {30, 20, 50, 40}, {10, 40, 30, 60}
    };
    StarsDropPartInput input;
    StarsDropPartOps ops;
    StarsDropPartSlot slots[3];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    FILE *report;

    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    slots[0].grhst = 2;
    slots[0].quantity = 0x0101;
    slots[0].capacity = 0x0501;
    slots[2].grhst = 2;
    setup(&input, &ops, &trace, slots, rects);
    checks = 0;
    failures = 0;
    failures += check(run_case(&input, 10, 20) == -1, &checks);
    failures += check(run_case(&input, 29, 39) == -1, &checks);
    failures += check(run_case(&input, 30, 20) == 2, &checks);
    failures += check(run_case(&input, 50, 25) == 0, &checks);
    failures += check(run_case(&input, 9, 25) == 1, &checks);
    failures += check(run_case(&input, 10, 40) == -1, &checks);
    failures += check(run_case(&input, 29, 40) == -1, &checks);
    failures += check(run_case(&input, 30, 40) == 1, &checks);
    failures += check(run_case(&input, 10, 60) == 1, &checks);
    failures += check(trace.hit_calls == 0, &checks);
    failures += check(input.packed_slot_rect_count == 3, &checks);
    failures += check(rects[0].left == 10 && rects[1].right == 50 &&
        rects[2].top == 40, &checks);
    report = fopen("C:\\IDPH\\IDPHE1.TXT", "w");
    if (report) {
        fprintf(report,
            "I_DROP_PART_HIT_TABLE_E1_CHECKS=12 FAILURES=%d RESULT=%s\n",
            (int)failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    if (failures == 0)
        puts("I_DROP_PART_HIT_TABLE_E1_CHECKS=12 FAILURES=0 RESULT=PASS");
    else
        puts("I_DROP_PART_HIT_TABLE_E1_CHECKS=12 FAILURES=1 RESULT=FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
