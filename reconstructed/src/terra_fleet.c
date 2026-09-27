#include "core.h"

/* NE 17:275C-2886. Sum terraforming components (type 0x80, subtype 7)
 * across the fleet's sixteen designs, preserving the byte-packed component
 * amount and multiplying each design's contribution by its ship count. */
StarsI32 stars_terra_from_fleet(const StarsTerraFleetInput *input)
{
    StarsI32 total = 0;
    StarsI32 design_total;
    StarsU16 design;
    StarsU16 component;
    if (!input) return 0;
    for (design = 0; design < 16U; ++design) {
        if (input->component_counts[design] > 0) {
            design_total = 0;
            for (component = 0;
                 component < input->designs[design].component_count;
                 ++component) {
                if (input->designs[design].components[component].type == 0x80U &&
                    (input->designs[design].components[component].encoded_amount &
                        0x00ffU) == 7U)
                    design_total += input->designs[design].components[component].encoded_amount >> 8;
            }
            total += design_total * input->component_counts[design];
        }
    }
    return total;
}
