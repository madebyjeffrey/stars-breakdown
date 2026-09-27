#include "core.h"

/* NE 11:2886-2B25. Sum mine-laying component power for the selected designs. */
StarsI32 stars_c_lay_mines_from_lpfl(const StarsCLayMinesInput *input,
        StarsI16 i_type, StarsI16 i_shdef)
{
    StarsU16 first;
    StarsU16 last;
    StarsU16 design_index;
    StarsU16 component_index;
    StarsI32 total;

    if (!input)
        return 0;
    if (i_type == 0) {
        first = 0;
        last = 3;
    }
    else if (i_type == 1) {
        first = 4;
        last = 6;
    }
    else if (i_type == 2) {
        first = 7;
        last = 9;
    }
    else {
        first = 0;
        last = 9;
    }
    total = 0;
    for (design_index = 0; design_index < 16U; ++design_index) {
        const StarsLayMinesDesign *design;
        StarsI32 design_power;
        StarsU16 count;
        if (input->component_counts[design_index] == 0U ||
                (i_shdef != -1 && design_index != (StarsU16)i_shdef))
            continue;
        if (design_index >= input->design_count)
            continue;
        design = &input->designs[design_index];
        design_power = 0;
        for (component_index = 0; component_index < design->component_count;
                ++component_index) {
            const StarsLayMinesComponent *component;
            StarsU16 subtype;
            StarsU16 quantity;
            StarsI32 power;
            component = &design->components[component_index];
            subtype = (StarsU16)(component->packed_count_type & 0xffU);
            quantity = (StarsU16)(component->packed_count_type >> 8);
            if (component->kind == 0x100U && subtype >= first &&
                    subtype <= last && input->part_power) {
                power = input->part_power(input->context, component->kind,
                    subtype);
                design_power += (StarsI32)quantity * power;
            }
            else if (i_type < 1 && component->kind == 0x10U &&
                    subtype == 0x12U)
                design_power += (StarsI32)quantity << 2;
        }
        if (design->hull_kind == 0x1bU || design->hull_kind == 0x1cU)
            design_power *= 2;
        count = input->component_counts[design_index];
        total += design_power * count;
    }
    if (total >= 0 && total < 0x989681L)
        total *= 10;
    else
        total = 100000000L;
    return total;
}
