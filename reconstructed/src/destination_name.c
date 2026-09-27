#include "core.h"
#include <string.h>

/* NE 34:4F60-51A8. Return a fleet destination label, distinguishing orders
 * that are unresolved or in transit from a resolved location. */
StarsI16 stars_get_destination_name(const StarsDestinationNameInput *input,
    StarsU8 *output, StarsU16 capacity)
{
    const StarsU16 *orders;
    StarsU16 kind;
    StarsU16 i;
    if (!input || !output || capacity == 0U || !input->compressed) return 0;
    orders = input->orders;
    if (input->cord < 2U) goto dash;
    kind = orders[3] & 0x000fU;
    if ((orders[3] & 0x1000U) != 0U) {
        if (kind == 1U) {
            for (i = 0; i < 5U; ++i)
                if ((orders[4U + i] >> 12) == 6U) goto transit;
        } else if (kind == 2U) {
            if (((orders[3] >> 8) & 0x000fU) == 1U) goto dash;
        } else if (kind == 4U || kind == 5U) {
            goto dash;
        } else if (kind == 6U) {
transit:
            if (input->hdc && input->set_color)
                input->set_color(input->context, input->hdc, 0x007fU);
            return input->compressed(input->context, 0x049dU, output, capacity);
        }
    }
    orders = input->next_orders;
    if (input->location)
        return input->location(input->context, (orders[3] >> 8) & 0x000fU,
            (StarsI16)orders[2], (StarsI16)orders[0], (StarsI16)orders[1],
            output, capacity);
    return 0;
dash:
    if (!input->dash_text || strlen((const char *)input->dash_text) + 1U > capacity)
        return 0;
    strcpy((char *)output, (const char *)input->dash_text);
    return 1;
}
