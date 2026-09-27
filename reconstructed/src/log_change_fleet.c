#include "core.h"
#include <string.h>

static void fleet_put_word(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static void fleet_put_long(StarsU8 *record, StarsU16 offset, StarsU32 value)
{
    fleet_put_word(record, offset, (StarsU16)value);
    fleet_put_word(record, (StarsU16)(offset + 2U),
        (StarsU16)(value >> 16));
}

static void fleet_write(const StarsLogChangeFleetOps *ops, void *context,
    StarsU16 type, StarsU16 length, const StarsU8 *record)
{
    if (ops && ops->write_memory)
        ops->write_memory(context, type, length, record);
}

/* Merge two successive five-item changes and choose the original compact
 * signed-width encoding: byte (type 1), word (type 2), or long (type 0x19).
 * Values are accumulated as unsigned 32-bit words because the 16-bit target
 * used the same two's-complement wrap when it added signed deltas. */
static void fleet_flush_xfer(const StarsLogXfer *first,
    const StarsLogXfer *second, const StarsLogChangeFleetOps *ops,
    void *context)
{
    StarsU8 record[26];
    StarsU32 total[5];
    StarsU32 magnitude;
    StarsU32 maximum = 0U;
    StarsU16 mask = 0U;
    StarsU16 type;
    StarsU16 length;
    StarsU16 i;

    if (!first || !second) return;
    for (i = 0; i < 5U; ++i) {
        total[i] = first->item[i] + second->item[i];
        if (total[i] != 0U)
            mask = (StarsU16)(mask | (StarsU16)(1U << i));
        magnitude = total[i];
        if ((StarsI32)total[i] < 0)
            magnitude = (StarsU32)(0U - magnitude);
        if (magnitude > maximum) maximum = magnitude;
    }
    if (mask == 0U) return;

    fleet_put_word(record, 0U, first->id);
    fleet_put_word(record, 2U, second->id);
    record[4] = (StarsU8)((first->grobj & 0x0fU) |
        ((second->grobj & 0x0fU) << 4));
    record[5] = (StarsU8)mask;
    if (maximum < 0x80U) {
        type = 1U;
        length = 6U;
        for (i = 0; i < 5U; ++i)
            if (total[i] != 0U) record[length++] = (StarsU8)total[i];
    } else if (maximum < 0x8000U) {
        type = 2U;
        length = 6U;
        for (i = 0; i < 5U; ++i) {
            if (total[i] != 0U) {
                fleet_put_word(record, length, (StarsU16)total[i]);
                length = (StarsU16)(length + 2U);
            }
        }
    } else {
        type = 0x19U;
        length = 6U;
        for (i = 0; i < 5U; ++i) {
            if (total[i] != 0U) {
                fleet_put_long(record, length, total[i]);
                length = (StarsU16)(length + 4U);
            }
        }
    }
    fleet_write(ops, context, type, length, record);
}

static void fleet_flush_xferf(const StarsLogXferf *first,
    const StarsLogXferf *second, const StarsLogChangeFleetOps *ops,
    void *context)
{
    StarsU8 record[39];
    StarsU16 mask = 0U;
    StarsU16 length = 7U;
    StarsU16 i;

    if (!first || !second) return;
    for (i = 0; i < 16U; ++i)
        if (first->item[i] != 0U)
            mask = (StarsU16)(mask | (StarsU16)(1U << i));
    if (mask == 0U) return;
    fleet_put_word(record, 0U, first->id);
    fleet_put_word(record, 2U, second->id);
    record[4] = (StarsU8)((first->grobj & 0x0fU) |
        ((second->grobj & 0x0fU) << 4));
    fleet_put_word(record, 5U, mask);
    for (i = 0; i < 16U; ++i) {
        if (first->item[i] != 0U) {
            fleet_put_word(record, length, first->item[i]);
            length = (StarsU16)(length + 2U);
        }
    }
    fleet_write(ops, context, 0x17U, length, record);
}

static void fleet_write_order(const StarsLogFleet *new_fleet,
    StarsU16 type, StarsU16 order_index, StarsU16 special_delete,
    const StarsLogChangeFleetOps *ops, void *context)
{
    StarsU8 record[22];
    StarsU16 length = 0x16U;

    fleet_put_word(record, 0U, new_fleet->object_id);
    fleet_put_word(record, 2U, (StarsU16)(order_index | special_delete));
    if (new_fleet->orders)
        memcpy(record + 4, new_fleet->orders + order_index * 18U, 18U);
    else
        memset(record + 4, 0, 18U);
    while (length > 4U && record[(StarsU16)(length - 1U)] == 0U)
        --length;
    fleet_write(ops, context, type, length, record);
}

/* NE 10:8EBE-9340. Log the observable changes between two fleet snapshots.
 * Ship counts and minerals are coalesced in the original pending transfer
 * records; plan, cloak/detail bit, field 0x60, and order-array changes are
 * emitted immediately. The callbacks cover only WriteMemRt and the previous
 * type-5 order lookup, leaving the fleet model itself in ordinary C data. */
void stars_log_change_fleet(const StarsLogFleet *old_fleet,
    const StarsLogFleet *new_fleet, StarsU16 log_flags,
    StarsLogChangeFleetState *state, const StarsLogChangeFleetOps *ops,
    void *context)
{
    StarsLogXfer current;
    StarsLogXferf current_flex;
    StarsU8 record[4];
    StarsU16 i;
    StarsU16 common;
    StarsI16 order_delta;
    int changed;

    if (!old_fleet || !new_fleet || !state || !ops) return;
    if ((log_flags & 0x0002U) != 0U) return;

    memset(&current_flex, 0, sizeof current_flex);
    current_flex.id = old_fleet->object_id;
    current_flex.grobj = 2U;
    changed = 0;
    for (i = 0; i < 16U; ++i) {
        current_flex.item[i] = (StarsU16)(new_fleet->ship_counts[i] -
            old_fleet->ship_counts[i]);
        if (current_flex.item[i] != 0U) changed = 1;
    }
    if (changed) {
        if (!state->valid_xferf) {
            state->xferf = current_flex;
            state->valid_xferf = 1U;
        } else {
            fleet_flush_xferf(&state->xferf, &current_flex, ops, context);
            state->valid_xferf = 0U;
        }
        return;
    }

    memset(&current, 0, sizeof current);
    current.id = old_fleet->object_id;
    current.grobj = 2U;
    changed = 0;
    for (i = 0; i < 5U; ++i) {
        current.item[i] = new_fleet->minerals[i] - old_fleet->minerals[i];
        if (current.item[i] != 0U) changed = 1;
    }
    if (changed) {
        if (!state->valid_xfer) {
            state->xfer = current;
            state->valid_xfer = 1U;
        } else {
            fleet_flush_xfer(&state->xfer, &current, ops, context);
            state->valid_xfer = 0U;
        }
        return;
    }

    if (old_fleet->plan != new_fleet->plan) {
        fleet_put_word(record, 0U, new_fleet->object_id);
        fleet_put_word(record, 2U, new_fleet->plan);
        fleet_write(ops, context, 0x2aU, 4U, record);
    }
    if (((old_fleet->flags ^ new_fleet->flags) & 0x0200U) != 0U) {
        fleet_put_word(record, 0U, new_fleet->object_id);
        fleet_put_word(record, 2U, (StarsU16)((new_fleet->flags >> 9) & 1U));
        fleet_write(ops, context, 0x0aU, 4U, record);
    }
    if ((StarsU8)old_fleet->field_60 != (StarsU8)new_fleet->field_60) {
        fleet_put_word(record, 0U, new_fleet->object_id);
        fleet_put_word(record, 2U, (StarsU16)(new_fleet->field_60 & 0xffU));
        fleet_write(ops, context, 0x2aU, 4U, record);
    }

    order_delta = (StarsI16)(new_fleet->order_count - old_fleet->order_count);
    common = 0U;
    while (common < old_fleet->order_count &&
            common < new_fleet->order_count && old_fleet->orders &&
            new_fleet->orders &&
            memcmp(old_fleet->orders + common * 18U,
                new_fleet->orders + common * 18U, 18U) == 0)
        ++common;
    if (common == old_fleet->order_count && order_delta == 0)
        return;
    if (order_delta < 0) {
        fleet_put_word(record, 0U, new_fleet->object_id);
        fleet_put_word(record, 2U, (StarsU16)(common |
            (order_delta == -2 ? 0x8000U : 0U)));
        fleet_write(ops, context, 3U, 4U, record);
    } else if (order_delta == 0) {
        if (ops->previous_order_replace &&
                ops->previous_order_replace(context, new_fleet->object_id,
                    common) != 0)
            return;
        fleet_write_order(new_fleet, 5U, common, 0U, ops, context);
    } else {
        fleet_write_order(new_fleet, 4U, common, 0U, ops, context);
    }
}
