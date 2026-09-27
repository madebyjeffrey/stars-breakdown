#include "core.h"
#include <stdio.h>
#include <string.h>

/* CodeView identifies _hul as 123 bytes, with rghs at offset 58, four-byte
 * _hs entries, and chs at offset 122.  The adapter binds only those recovered
 * fields; hit testing and redraw/update behavior remain callbacks. */
#define RAW_HUL_BYTES 123
#define RAW_HS_OFFSET 58
#define RAW_HS_BYTES 4
#define RAW_CHS_OFFSET 122
#define SLOT_COUNT 3

static StarsU16 raw_u16(const StarsU8 *raw, StarsI16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1] << 8));
}

static void put_u16(StarsU8 *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static StarsI16 slot_offset(StarsI16 slot)
{
    return (StarsI16)(RAW_HS_OFFSET + slot * RAW_HS_BYTES);
}

static void put_hul(StarsU8 *raw, StarsI16 count)
{
    memset(raw, 0, RAW_HUL_BYTES);
    put_u16(raw, 0, 5);
    raw[RAW_CHS_OFFSET] = (StarsU8)count;
}

static void put_slot(StarsU8 *raw, StarsI16 slot, StarsU16 grhst,
    StarsU16 quantity)
{
    StarsI16 offset;

    offset = slot_offset(slot);
    put_u16(raw, offset, grhst);
    put_u16(raw, (StarsI16)(offset + 2), quantity);
}

static void load_slots(const StarsU8 *current, const StarsU8 *capacity,
    StarsDropPartSlot slots[SLOT_COUNT])
{
    StarsI16 i;
    StarsI16 offset;

    for (i = 0; i < SLOT_COUNT; ++i) {
        offset = slot_offset(i);
        slots[i].grhst = raw_u16(current, offset);
        slots[i].quantity = raw_u16(current, (StarsI16)(offset + 2));
        slots[i].capacity = raw_u16(capacity, (StarsI16)(offset + 2));
    }
}

static void store_slots(StarsU8 *current,
    const StarsDropPartSlot slots[SLOT_COUNT])
{
    StarsI16 i;
    StarsI16 offset;

    for (i = 0; i < SLOT_COUNT; ++i) {
        offset = slot_offset(i);
        put_u16(current, offset, slots[i].grhst);
        put_u16(current, (StarsI16)(offset + 2), slots[i].quantity);
    }
}

typedef struct Trace {
    StarsI16 empty;
    StarsI16 updates;
    StarsI16 invalidates;
    StarsI16 selections;
    StarsI16 selected;
    StarsI16 beeps;
    StarsI16 advances;
} Trace;

static StarsI16 hit(void *context, StarsI16 slot, const StarsPoint *point)
{
    (void)context;
    return slot == point->x;
}

static void empty_drop(void *context, StarsI16 source, StarsU16 *grhst,
    StarsU16 *quantity)
{
    Trace *trace;

    trace = (Trace *)context;
    if (source != 1)
        return;
    *grhst = 0;
    *quantity = 0;
    ++trace->empty;
}

static void update_cost(void *context, StarsI16 slot)
{
    Trace *trace;

    trace = (Trace *)context;
    if (slot >= 0)
        ++trace->updates;
}

static void invalidate(void *context)
{
    ++((Trace *)context)->invalidates;
}

static void set_selection(void *context, StarsI16 slot)
{
    Trace *trace;

    trace = (Trace *)context;
    trace->selected = slot;
    ++trace->selections;
}

static void beep(void *context)
{
    ++((Trace *)context)->beeps;
}

static void advance_tutor(void *context)
{
    ++((Trace *)context)->advances;
}

static void install_ops(StarsDropPartOps *ops, Trace *trace)
{
    memset(ops, 0, sizeof *ops);
    ops->hit_test = hit;
    ops->empty_drop = empty_drop;
    ops->update_cost = update_cost;
    ops->invalidate = invalidate;
    ops->set_selection = set_selection;
    ops->beep = beep;
    ops->advance_tutor = advance_tutor;
    ops->context = trace;
}

static void setup(StarsDropPartInput *input, StarsDropPartOps *ops,
    Trace *trace, StarsDropPartSlot slots[SLOT_COUNT])
{
    memset(input, 0, sizeof *input);
    install_ops(ops, trace);
    input->point.x = 3;
    input->point.y = 1;
    input->client_width = 100;
    input->source_grhst = 2;
    input->source_quantity = 0x0301;
    input->source_index = 1;
    input->selected_slot = -1;
    input->slots = slots;
    input->slot_count = SLOT_COUNT;
    input->ops = ops;
}

static StarsI16 run_empty_case(void)
{
    StarsU8 current[RAW_HUL_BYTES];
    StarsU8 capacity[RAW_HUL_BYTES];
    StarsDropPartSlot slots[SLOT_COUNT];
    StarsDropPartInput input;
    StarsDropPartOps ops;
    Trace trace;
    StarsI16 result;

    memset(&trace, 0, sizeof trace);
    put_hul(current, SLOT_COUNT);
    put_hul(capacity, SLOT_COUNT);
    put_slot(current, 1, 2, 0x0301);
    load_slots(current, capacity, slots);
    setup(&input, &ops, &trace, slots);
    result = stars_drop_part(&input);
    store_slots(current, slots);
    return result == 1 && trace.empty == 1 && trace.updates == 1 &&
        trace.invalidates == 1 && trace.advances == 1 &&
        raw_u16(current, (StarsI16)(slot_offset(1) + 2)) == 0x0001;
}

static StarsI16 run_merge_case(void)
{
    StarsU8 current[RAW_HUL_BYTES];
    StarsU8 capacity[RAW_HUL_BYTES];
    StarsDropPartSlot slots[SLOT_COUNT];
    StarsDropPartInput input;
    StarsDropPartOps ops;
    Trace trace;
    StarsI16 result;

    memset(&trace, 0, sizeof trace);
    put_hul(current, SLOT_COUNT);
    put_hul(capacity, SLOT_COUNT);
    put_slot(current, 0, 2, 0x0101);
    put_slot(current, 1, 2, 0x0301);
    put_slot(capacity, 0, 2, 0x0501);
    load_slots(current, capacity, slots);
    setup(&input, &ops, &trace, slots);
    input.point.x = 0;
    result = stars_drop_part(&input);
    store_slots(current, slots);
    return result == -1 && trace.selections == 1 && trace.selected == 0 &&
        trace.invalidates == 1 && trace.advances == 1 &&
        raw_u16(current, slot_offset(0)) == 2 &&
        raw_u16(current, (StarsI16)(slot_offset(0) + 2)) == 0x0401 &&
        raw_u16(current, (StarsI16)(slot_offset(1) + 2)) == 0x0001;
}

static StarsI16 run_reject_case(void)
{
    StarsU8 current[RAW_HUL_BYTES];
    StarsU8 capacity[RAW_HUL_BYTES];
    StarsDropPartSlot slots[SLOT_COUNT];
    StarsDropPartInput input;
    StarsDropPartOps ops;
    Trace trace;
    StarsI16 result;

    memset(&trace, 0, sizeof trace);
    put_hul(current, SLOT_COUNT);
    put_hul(capacity, SLOT_COUNT);
    put_slot(current, 0, 4, 0x0501);
    put_slot(current, 1, 2, 0x0301);
    put_slot(capacity, 0, 4, 0x0501);
    load_slots(current, capacity, slots);
    setup(&input, &ops, &trace, slots);
    input.point.x = 0;
    result = stars_drop_part(&input);
    store_slots(current, slots);
    return result == 3 && trace.beeps == 1 &&
        raw_u16(current, slot_offset(0)) == 4 &&
        raw_u16(current, (StarsI16)(slot_offset(0) + 2)) == 0x0501 &&
        raw_u16(current, (StarsI16)(slot_offset(1) + 2)) == 0x0301;
}

static StarsI16 run_no_modify_case(void)
{
    StarsU8 current[RAW_HUL_BYTES];
    StarsU8 capacity[RAW_HUL_BYTES];
    StarsDropPartSlot slots[SLOT_COUNT];
    StarsDropPartInput input;
    StarsDropPartOps ops;
    Trace trace;
    StarsI16 result;

    memset(&trace, 0, sizeof trace);
    put_hul(current, SLOT_COUNT);
    put_hul(capacity, SLOT_COUNT);
    put_slot(current, 0, 2, 0x0101);
    put_slot(current, 1, 2, 0x0301);
    put_slot(capacity, 0, 2, 0x0501);
    load_slots(current, capacity, slots);
    setup(&input, &ops, &trace, slots);
    input.point.x = 0;
    input.no_modify = 1;
    result = stars_drop_part(&input);
    store_slots(current, slots);
    return result == -1 && trace.selections == 0 && trace.invalidates == 0 &&
        raw_u16(current, (StarsI16)(slot_offset(0) + 2)) == 0x0101 &&
        raw_u16(current, (StarsI16)(slot_offset(1) + 2)) == 0x0301;
}

int main(void)
{
    StarsU8 raw[RAW_HUL_BYTES];
    StarsI16 failures;

    put_hul(raw, SLOT_COUNT);
    put_slot(raw, 0, 7, 0x0203);
    failures = raw[RAW_CHS_OFFSET] != SLOT_COUNT;
    failures += raw_u16(raw, slot_offset(0)) != 7;
    failures += raw_u16(raw, (StarsI16)(slot_offset(0) + 2)) != 0x0203;
    failures += !run_empty_case();
    failures += !run_merge_case();
    failures += !run_reject_case();
    failures += !run_no_modify_case();
    if (failures == 0)
        puts("I_DROP_PART_INTEGRATION_CHECKS=24 FAILURES=0 RESULT=PASS");
    else
        puts("I_DROP_PART_INTEGRATION_CHECKS=24 FAILURES=1 RESULT=FAIL");
    return failures == 0 ? 0 : 1;
}
