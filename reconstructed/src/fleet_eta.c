#include "core.h"
#include <string.h>

/* NE 34:51A8-53B8. Return the report ETA text, suppressing routes whose
 * leading order is unresolved or still in transit. Fuel shortage is exposed
 * as the original red-text callback, and the leading decimal ETA is returned
 * through years (with the original zero-to-32000 sentinel). */
StarsI16 stars_get_eta(const StarsGetEtaInput *input, StarsU8 *output,
    StarsU16 capacity, StarsI16 *years, const StarsGetEtaOps *ops,
    void *context)
{
    StarsU16 i;
    StarsU16 kind;
    StarsU16 subkind;
    StarsI16 value = 0;
    StarsI16 parsed = 0;
    const StarsU8 *text;
    if (!input || !output || capacity == 0U || !ops) return 0;
    if (input->cord < 2U) goto no_eta;
    if ((input->order_flags & 0x1000U) != 0U) {
        kind = (StarsU16)(input->order_flags & 0x000fU);
        if (kind == 1U) {
            for (i = 0; i < 5U; ++i)
                if ((input->order_words[i] >> 12) == 6U) goto no_eta;
        } else if (kind == 2U) {
            subkind = (StarsU16)((input->order_flags >> 8) & 0x000fU);
            if (subkind == 1U) goto no_eta;
        } else if (kind == 4U || kind == 5U || kind == 6U) {
            goto no_eta;
        }
    }
    if (!ops->get_text) return 0;
    ops->get_text(context, input->hdc, output, capacity);
    if (input->hdc != 0U && input->fuel_use > input->fuel_available &&
            ops->set_color)
        ops->set_color(context, 0x00ffU);
    text = output;
    while (*text >= '0' && *text <= '9') {
        value = (StarsI16)(value * 10 + *text - '0');
        ++text;
    }
    parsed = value == 0 ? 32000 : value;
    if (years) *years = parsed;
    return 1;
no_eta:
    if (!input->dash_text) return 0;
    if ((StarsU16)(strlen((const char *)input->dash_text) + 1U) > capacity)
        return 0;
    strcpy((char *)output, (const char *)input->dash_text);
    if (years) *years = 0;
    return 1;
}
