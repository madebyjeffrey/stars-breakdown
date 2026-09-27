#include <stdio.h>
#include <string.h>
#include "core.h"

struct RecordProbe {
    StarsU16 clears;
    StarsU16 adds;
    StarsU16 names;
    StarsU16 estimates;
    StarsU16 formats;
    StarsU32 seen[2];
    StarsU8 last[64];
};

static void put16(StarsU8 *raw, StarsI16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static void put32(StarsU8 *raw, StarsI16 offset, StarsU32 value)
{
    put16(raw, offset, (StarsU16)(value & 0xffffUL));
    put16(raw, (StarsI16)(offset + 2), (StarsU16)(value >> 16));
}

static StarsU16 get16(const StarsU8 *raw, StarsI16 offset)
{
    return (StarsU16)(raw[offset] |
        ((StarsU16)raw[offset + 1] << 8));
}

static StarsU32 get32(const StarsU8 *raw, StarsI16 offset)
{
    return (StarsU32)get16(raw, offset) |
        ((StarsU32)get16(raw, (StarsI16)(offset + 2)) << 16);
}

static StarsU32 packed_item(StarsU16 kind, StarsU16 item, StarsU16 count)
{
    return ((StarsU32)kind << 17) | ((StarsU32)item << 10) | count;
}

static void get_string(void *context, StarsU16 id, StarsU8 *output,
    StarsU16 capacity)
{
    const char *text;
    (void)context;
    text = id == 0x33cU ? "No production" : "Production";
    strncpy((char *)output, text, capacity - 1U);
    output[capacity - 1U] = 0;
}

static void name_item(void *context, StarsU32 packed, StarsU8 *output,
    StarsU16 capacity)
{
    struct RecordProbe *probe = (struct RecordProbe *)context;
    const char *text = packed == packed_item(1U, 3U, 12U) ? "Scout" :
        "Destroyer";
    ++probe->names;
    strncpy((char *)output, text, capacity - 1U);
    output[capacity - 1U] = 0;
}

static void estimate(void *context, const void *planet, const StarsU32 *items,
    StarsU16 index, StarsI16 *first, StarsI16 *last)
{
    struct RecordProbe *probe = (struct RecordProbe *)context;
    (void)planet;
    probe->seen[index] = items[index];
    ++probe->estimates;
    *first = index == 0U ? 2 : 1;
    *last = *first;
}

static void format_line(void *context, StarsU8 marker, StarsU16 count,
    const StarsU8 *name, StarsU8 *output, StarsU16 capacity)
{
    struct RecordProbe *probe = (struct RecordProbe *)context;
    (void)count;
    ++probe->formats;
    if (capacity > 3U) {
        output[0] = marker;
        output[1] = ' ';
        strncpy((char *)output + 2, (const char *)name, capacity - 3U);
        output[capacity - 1U] = 0;
    }
}

static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    const StarsU8 *value)
{
    struct RecordProbe *probe = (struct RecordProbe *)context;
    (void)hwnd;
    if (message == 0x405U)
        ++probe->clears;
    if (message == 0x401U) {
        ++probe->adds;
        strncpy((char *)probe->last, (const char *)value,
            sizeof probe->last - 1U);
        probe->last[sizeof probe->last - 1U] = 0;
    }
}

int main(void)
{
    struct RecordProbe probe;
    StarsU8 planet[56];
    StarsU8 plprod[12];
    StarsU32 items[2];
    StarsFillPlanetProdQueue queue;
    StarsFillPlanetProdInput input;
    StarsU8 work[64];
    StarsU16 header;
    int checks;
    int failures;

    memset(&probe, 0, sizeof probe);
    memset(planet, 0, sizeof planet);
    memset(plprod, 0, sizeof plprod);
    memset(&input, 0, sizeof input);
    put16(planet, 0, 7U);
    put32(planet, 52, 0x00031220UL);
    header = (StarsU16)(4U | (1U << 8) | (2U << 9) | (4U << 12));
    put16(plprod, 0, header);
    plprod[2] = 8U;
    plprod[3] = 2U;
    put32(plprod, 4, packed_item(1U, 3U, 12U));
    put32(plprod, 8, packed_item(2U, 5U, 8U));
    items[0] = get32(plprod, 4);
    items[1] = get32(plprod, 8);
    queue.items = items;
    queue.item_count = 2U;
    input.default_hwnd = 7U;
    input.selected_queue = &queue;
    input.work = work;
    input.work_capacity = sizeof work;
    input.get_string = get_string;
    input.name_item = name_item;
    input.estimate_schedule = estimate;
    input.format_line = format_line;
    input.send_message = send_message;
    input.context = &probe;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof planet == 56U && get16(planet, 0) == 7U);
    CHECK(get32(planet, 52) == 0x00031220UL);
    CHECK((header & 0xffU) == 4U && ((header >> 8) & 1U) == 1U);
    CHECK(((header >> 9) & 7U) == 2U && ((header >> 12) & 0xfU) == 4U);
    CHECK(plprod[2] == 8U && plprod[3] == 2U);
    CHECK(items[0] == packed_item(1U, 3U, 12U));
    CHECK(items[1] == packed_item(2U, 5U, 8U));
    CHECK(((items[0] >> 17) & 7U) == 1U && ((items[0] >> 10) & 0x7fU) == 3U);
    CHECK((items[0] & 0x3ffU) == 12U && ((items[1] >> 17) & 7U) == 2U);
    CHECK(((items[1] >> 10) & 0x7fU) == 5U && (items[1] & 0x3ffU) == 8U);
    stars_fill_planet_production_list(&input);
    CHECK(probe.clears == 1U && probe.adds == 2U);
    CHECK(probe.names == 2U && probe.estimates == 2U && probe.formats == 2U);
    CHECK(probe.seen[0] == items[0] && probe.seen[1] == items[1]);
    CHECK(probe.last[0] == '*' && probe.last[2] == 'D');
    input.queue = &queue;
    input.selected_queue = 0;
    memset(work, 0, sizeof work);
    stars_fill_planet_production_list(&input);
    CHECK(work[0] == ' ' && work[2] == 'S');
    CHECK(checks == 16);
    printf("DRAW_PLANET_PRODUCTION_RECORD_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures ? 1 : 0;
#undef CHECK
}
