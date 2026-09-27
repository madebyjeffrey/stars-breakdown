#include "core.h"

static void startup_put_word(StarsU8 *data, StarsU16 offset, StarsU16 value)
{
    data[offset] = (StarsU8)value;
    data[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static void startup_put_long(StarsU8 *data, StarsU16 offset, StarsI32 value)
{
    startup_put_word(data, offset, (StarsU16)value);
    startup_put_word(data, (StarsU16)(offset + 2U),
        (StarsU16)((StarsU32)value >> 16));
}

static StarsI32 startup_long(const StarsU8 *data, StarsU16 offset)
{
    return (StarsI32)((StarsU32)data[offset] |
        ((StarsU32)data[(StarsU16)(offset + 1U)] << 8) |
        ((StarsU32)data[(StarsU16)(offset + 2U)] << 16) |
        ((StarsU32)data[(StarsU16)(offset + 3U)] << 24));
}

/* NE 10:4690-4812. Optionally materialize the selected technology chain as
 * a player design, increment its built/existing counters, then create the
 * one-ship fleet and cache its weight in rgwtMin[4]. */
StarsI16 stars_create_startup_ship(const StarsCreateStartupShipInput *input)
{
    StarsI16 design;
    StarsU16 i;
    StarsU16 base;
    StarsStartupFleetState *fleet;
    StarsI32 weight;

    if (!input || input->player < 0 || input->player >= 16 ||
            !input->design_count || !input->design_table ||
            !input->create_fleet || !input->fleet_stat)
        return 0;
    design = input->design_index;
    if (input->add_design) {
        design = (StarsI16)*input->design_count;
        *input->design_count = (StarsU8)(*input->design_count + 1U);
        if (design < 0 || (StarsU16)(design + 1) > input->design_capacity ||
                !input->design_words || input->design_word_count < 73U)
            return 0;
        base = (StarsU16)design * 147U;
        for (i = 0; i < 73U; ++i)
            startup_put_word(input->design_table,
                (StarsU16)(base + i * 2U), input->design_words[i]);
        input->design_table[(StarsU16)(base + 146U)] =
            input->design_hull_id;
    }
    if (design < 0 || design >= 16)
        return 0;
    base = (StarsU16)design * 147U;
    startup_put_long(input->design_table, (StarsU16)(base + 0x7fU),
        startup_long(input->design_table, (StarsU16)(base + 0x7fU)) + 1L);
    startup_put_long(input->design_table, (StarsU16)(base + 0x83U),
        startup_long(input->design_table, (StarsU16)(base + 0x83U)) + 1L);
    fleet = input->create_fleet(input->context, input->player, input->planet);
    if (!fleet)
        return 0;
    fleet->bytes[(StarsU16)(12U + (StarsU16)design)] = 1U;
    weight = input->fleet_stat(input->context, fleet, 1U);
    startup_put_long(fleet->bytes, 92U, weight);
    fleet->bytes[96] = 0U;
    return design;
}
