#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    char order[16];
    StarsI16 length;
} Trace;

static void append(Trace *trace, char value)
{
    if (trace->length < (StarsI16)sizeof trace->order - 1)
        trace->order[trace->length++] = value;
    trace->order[trace->length] = 0;
}

static StarsI16 hit(void *context, StarsI16 slot, const StarsPoint *point)
{
    (void)context;
    return slot == point->x;
}

static void empty_drop(void *context, StarsI16 source, StarsU16 *grhst,
    StarsU16 *quantity)
{
    (void)source; (void)grhst; (void)quantity;
    append((Trace *)context, 'E');
}

static void update_cost(void *context, StarsI16 slot)
{
    (void)slot;
    append((Trace *)context, 'U');
}

static void invalidate(void *context)
{
    append((Trace *)context, 'I');
}

static void select_slot(void *context, StarsI16 slot)
{
    (void)slot;
    append((Trace *)context, 'S');
}

static void beep(void *context)
{
    append((Trace *)context, 'B');
}

static void advance_tutor(void *context)
{
    append((Trace *)context, 'A');
}

static void install(StarsDropPartOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->hit_test = hit;
    ops->empty_drop = empty_drop;
    ops->update_cost = update_cost;
    ops->invalidate = invalidate;
    ops->set_selection = select_slot;
    ops->beep = beep;
    ops->advance_tutor = advance_tutor;
    ops->context = trace;
}

static void setup(StarsDropPartInput *input, StarsDropPartOps *ops,
    Trace *trace, StarsDropPartSlot *slots)
{
    memset(input, 0, sizeof *input);
    install(ops, trace);
    input->point.x = 0;
    input->point.y = 1;
    input->client_width = 100;
    input->source_grhst = 2;
    input->source_quantity = 0x0701;
    input->source_index = 1;
    input->slots = slots;
    input->slot_count = 2;
    input->ops = ops;
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsDropPartInput input;
    StarsDropPartOps ops;
    StarsDropPartSlot slots[2];
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 result;
    FILE *report;

    checks = 0;
    failures = 0;

    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    slots[0].grhst = 2; slots[0].quantity = 0x0101;
    slots[0].capacity = 0x0301; slots[1].quantity = 0x0701;
    result = stars_drop_part(&input);
    failures += check(result == -1 && slots[0].quantity == 0x0301 &&
        slots[1].quantity == 0x0501, &checks);
    failures += check(strcmp(trace.order, "USIA") == 0, &checks);

    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    slots[0].grhst = 2; slots[0].quantity = 0x0101;
    slots[0].capacity = 0x0a01; slots[1].quantity = 0x0201;
    result = stars_drop_part(&input);
    failures += check(result == -1 && slots[0].quantity == 0x0801 &&
        slots[1].quantity == 0x0001, &checks);
    failures += check(strcmp(trace.order, "USIA") == 0, &checks);

    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    slots[0].grhst = 2; slots[0].quantity = 0x0101;
    slots[0].capacity = 0x0501; slots[1].quantity = 0x0701;
    input.no_modify = 1;
    result = stars_drop_part(&input);
    failures += check(result == -1 && slots[0].quantity == 0x0101 &&
        slots[1].quantity == 0x0701, &checks);
    failures += check(trace.length == 0, &checks);

    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    input.source_index = -1; input.source_quantity = 0x0201;
    slots[0].grhst = 2; slots[0].quantity = 0x0101;
    slots[0].capacity = 0x0501;
    result = stars_drop_part(&input);
    failures += check(result == -1 && slots[0].quantity == 0x0301,
        &checks);
    failures += check(strcmp(trace.order, "SIA") == 0, &checks);

    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    slots[0].grhst = 2; slots[0].quantity = 0x0101;
    slots[0].capacity = 0; slots[1].quantity = 0x0701;
    result = stars_drop_part(&input);
    failures += check(result == -1 && slots[0].quantity == 0x0101 &&
        slots[1].quantity == 0x0701, &checks);
    failures += check(strcmp(trace.order, "USIA") == 0, &checks);

    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    slots[0].grhst = 4; slots[0].quantity = 0x0501;
    slots[0].capacity = 0x0501; slots[1].quantity = 0x0701;
    result = stars_drop_part(&input);
    failures += check(result == 3 && strcmp(trace.order, "B") == 0,
        &checks);

    report = fopen("C:\\IDPM\\IDPME2.TXT", "w");
    if (report) {
        fprintf(report,
            "I_DROP_PART_MUTATION_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("I_DROP_PART_MUTATION_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
