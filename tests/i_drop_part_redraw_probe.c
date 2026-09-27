#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    char order[32];
    StarsI16 length;
    StarsRect client;
    StarsRect invalidated;
    StarsI16 erase;
    StarsI16 draw_mode;
    StarsI16 draw_slot;
    StarsI16 component_mode;
    StarsI16 component_slot;
    StarsI16 hull_mode;
    StarsI16 hull_slot;
    StarsI16 hull_was_null;
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
    (void)source;
    (void)grhst;
    (void)quantity;
    append((Trace *)context, 'E');
}

static void update_cost(void *context, StarsI16 slot)
{
    (void)slot;
    append((Trace *)context, 'U');
}

static void get_client_rect(void *context, StarsRect *rect)
{
    Trace *trace;

    trace = (Trace *)context;
    *rect = trace->client;
    append(trace, 'G');
}

static void draw_slot(void *context, StarsI16 mode, const StarsRect *rect,
    StarsI16 slot)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->draw_mode = mode;
    trace->draw_slot = slot;
    if (rect->left != trace->client.left || rect->top != trace->client.top ||
            rect->right != trace->client.right ||
            rect->bottom != trace->client.bottom)
        append(trace, 'x');
    append(trace, 'D');
}

static void draw_selected_component(void *context, StarsI16 mode,
    StarsI16 slot)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->component_mode = mode;
    trace->component_slot = slot;
    append(trace, 'C');
}

static void draw_selected_hull(void *context, StarsI16 mode, StarsI16 slot,
    const StarsRect *rect)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->hull_mode = mode;
    trace->hull_slot = slot;
    trace->hull_was_null = rect == (const StarsRect *)0;
    append(trace, 'H');
}

static void invalidate_rect(void *context, const StarsRect *rect,
    StarsI16 erase)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->invalidated = *rect;
    trace->erase = erase;
    append(trace, 'I');
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
    ops->get_client_rect = get_client_rect;
    ops->draw_slot = draw_slot;
    ops->draw_selected_component = draw_selected_component;
    ops->draw_selected_hull = draw_selected_hull;
    ops->invalidate_rect = invalidate_rect;
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
    input->point.y = 1;
    input->client_width = 100;
    input->y_build_info_sum = 77;
    input->source_grhst = 2;
    input->source_quantity = 0x0301;
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
    trace.client.left = 4;
    trace.client.top = 5;
    trace.client.right = 104;
    trace.client.bottom = 105;
    memset(slots, 0, sizeof slots);
    slots[1].quantity = 0x0301;
    setup(&input, &ops, &trace, slots);
    input.point.x = 3;
    result = stars_drop_part(&input);
    failures += check(result == 1, &checks);
    failures += check(strcmp(trace.order, "EUGDICHA") == 0, &checks);
    failures += check(trace.draw_mode == 0 && trace.draw_slot == 1,
        &checks);
    failures += check(trace.invalidated.left == 4 &&
        trace.invalidated.top == 77 && trace.invalidated.right == 104 &&
        trace.invalidated.bottom == 105 && trace.erase == 1, &checks);
    failures += check(trace.component_mode == 0 &&
        trace.component_slot == -1 && trace.hull_mode == 0 &&
        trace.hull_slot == -1 && trace.hull_was_null, &checks);

    memset(&trace, 0, sizeof trace);
    trace.client.left = 1;
    trace.client.top = 2;
    trace.client.right = 101;
    trace.client.bottom = 102;
    memset(slots, 0, sizeof slots);
    slots[0].grhst = 2;
    slots[0].quantity = 0x0101;
    slots[0].capacity = 0x0501;
    slots[1].quantity = 0x0301;
    setup(&input, &ops, &trace, slots);
    input.point.x = 0;
    result = stars_drop_part(&input);
    failures += check(result == -1, &checks);
    failures += check(strcmp(trace.order, "USGIA") == 0, &checks);
    failures += check(trace.invalidated.left == 1 &&
        trace.invalidated.top == 77 && trace.invalidated.right == 101 &&
        trace.invalidated.bottom == 102 && trace.erase == 1, &checks);

    report = fopen("C:\\IDPR\\IDPRE3.TXT", "w");
    if (report) {
        fprintf(report,
            "I_DROP_PART_REDRAW_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("I_DROP_PART_REDRAW_E3_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures,
        failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
