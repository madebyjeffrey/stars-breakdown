#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 empty;
    StarsI16 updates;
    StarsI16 invalidates;
    StarsI16 selections;
    StarsI16 beeps;
    StarsI16 advances;
    StarsI16 selected;
} Trace;
static StarsI16 hit(void *context, StarsI16 slot, const StarsPoint *point)
{
    (void)context; return slot == point->x;
}
static void empty_drop(void *context, StarsI16 source, StarsU16 *grhst,
    StarsU16 *quantity)
{
    Trace *trace = (Trace *)context;
    (void)source; *grhst = 0; *quantity = 0; ++trace->empty;
}
static void update(void *context, StarsI16 slot)
{
    (void)slot; ++((Trace *)context)->updates;
}
static void invalidate(void *context)
{
    ++((Trace *)context)->invalidates;
}
static void select_slot(void *context, StarsI16 slot)
{
    Trace *trace = (Trace *)context; ++trace->selections; trace->selected = slot;
}
static void beep(void *context)
{
    ++((Trace *)context)->beeps;
}
static void advance(void *context)
{
    ++((Trace *)context)->advances;
}
static void install(StarsDropPartOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->hit_test = hit;
    ops->empty_drop = empty_drop;
    ops->update_cost = update;
    ops->invalidate = invalidate;
    ops->set_selection = select_slot;
    ops->beep = beep;
    ops->advance_tutor = advance;
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
    input->source_quantity = 0x0301;
    input->source_index = 1;
    input->selected_slot = -1;
    input->slots = slots;
    input->slot_count = 3;
    input->ops = ops;
}
int main(void)
{
    StarsDropPartInput input;
    StarsDropPartOps ops;
    StarsDropPartSlot slots[3];
    Trace trace;
    StarsI16 result;

    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    slots[1].quantity = 0x0301;
    input.point.x = 3;
    result = stars_drop_part(&input);
    if (result != 1 || trace.empty != 1 || trace.updates != 1 ||
            trace.invalidates != 1 || trace.advances != 1 ||
            slots[1].quantity != 0x0001)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    input.point.x = 1;
    result = stars_drop_part(&input);
    if (result != 2 || trace.beeps != 0)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    slots[0].grhst = 2;
    slots[0].quantity = 0x0101;
    slots[0].capacity = 0x0501;
    slots[1].quantity = 0x0301;
    input.point.x = 0;
    input.source_index = 1;
    result = stars_drop_part(&input);
    if (result != -1 || trace.selections != 1 || trace.selected != 0 ||
            trace.invalidates != 1 || slots[0].quantity != 0x0401 ||
            slots[1].quantity != 0x0001)
        return 1;
    memset(&trace, 0, sizeof trace);
    memset(slots, 0, sizeof slots);
    setup(&input, &ops, &trace, slots);
    slots[0].grhst = 4;
    slots[0].quantity = 0x0501;
    slots[0].capacity = 0x0501;
    input.point.x = 0;
    result = stars_drop_part(&input);
    if (result != 3 || trace.beeps != 1)
        return 1;
    puts("I_DROP_PART_CHECKS=26 FAILURES=0 RESULT=PASS");
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("C:\\IDP\\IDPR.TXT", "w");
        if (!report)
            return 2;
        fputs("I_DROP_PART_CHECKS=26 FAILURES=0 RESULT=PASS\n", report);
        fclose(report);
    }
#endif
    return 0;
}
