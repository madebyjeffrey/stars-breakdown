#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsU16 destination_counts[16];
    StarsU16 deleted_counts[16];
    StarsI16 delete_id;
    StarsI16 selected_id;
    StarsI16 invalidated_kind;
    StarsI16 invalidated_id;
    StarsU16 lookups;
} Trace;

static void cargo(void *context, StarsU8 *destination, StarsU8 *deleted)
{
    (void)context; (void)destination; (void)deleted;
}
static void lookup(void *context, StarsI16 id, StarsU8 *fleet)
{
    Trace *trace = (Trace *)context;
    (void)id; (void)fleet; ++trace->lookups;
}
static void remove_fleet(void *context, StarsI16 id, StarsI16 kind,
    StarsI16 selected)
{
    Trace *trace = (Trace *)context;
    trace->delete_id = id; trace->invalidated_kind = kind;
    trace->selected_id = selected;
}
static void invalidate(void *context, StarsI16 kind, StarsI16 id)
{
    Trace *trace = (Trace *)context;
    trace->invalidated_kind = kind; trace->invalidated_id = id;
}

static StarsU16 word_at(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU16)(bytes[offset] | ((StarsU16)bytes[offset + 1U] << 8));
}
static void put_word(StarsU8 *bytes, StarsU16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)value; bytes[offset + 1U] = (StarsU8)(value >> 8);
}

int main(void)
{
    StarsU8 destination[124]; StarsU8 deleted[124];
    StarsMergeFleetInput input; Trace trace; StarsU16 i;
    memset(destination, 0, sizeof destination); memset(deleted, 0, sizeof deleted);
    memset(&trace, 0, sizeof trace);
    put_word(destination, 0, 12U); put_word(deleted, 0, 7U);
    put_word(destination, 4, 0x0400U); put_word(deleted, 4, 0x0200U);
    for (i = 0; i < 16U; ++i) { put_word(destination, (StarsU16)(12U + i * 2U), i); put_word(deleted, (StarsU16)(12U + i * 2U), 2U); }
    input.destination = destination; input.deleted = deleted; input.no_delete = 0;
    input.cargo_balance = cargo; input.lookup = lookup; input.delete_fleet = remove_fleet;
    input.invalidate_report = invalidate; input.context = &trace;
    stars_merge_two_fleets(&input);
    if (word_at(destination, 12) != 2U || word_at(destination, 14) != 3U ||
        word_at(deleted, 12) != 0U || trace.lookups != 2U ||
        trace.delete_id != 7 || trace.selected_id != 12 ||
        trace.invalidated_kind != 1 || trace.invalidated_id != 2) return 1;
    input.no_delete = 1; input.delete_fleet = 0; input.invalidate_report = 0;
    stars_merge_two_fleets(&input);
    if ((word_at(deleted, 4) & 0x0400U) == 0U) return 2;
    puts("MERGE_FLEETS_SOURCE=PASS"); return 0;
}
