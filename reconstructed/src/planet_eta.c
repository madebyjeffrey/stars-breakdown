#include "core.h"
#include <string.h>

/* NE 10:310C-3336. Select the localized production ETA form, preserving the
 * original sentinel (100), equal-range, zero, and negative branches. The
 * resource formatter is explicit because the executable obtains its format
 * strings from compressed DGROUP resources. */
StarsI16 stars_psz_production_eta(const StarsProductionEtaInput *input,
    StarsI16 *eta_first, StarsI16 *eta_last, StarsU8 *output,
    StarsU16 capacity, const StarsProductionEtaOps *ops, void *context)
{
    StarsU16 format_id;
    StarsU16 length;
    if (!input || !output || capacity == 0U || !ops) return 0;
    if (eta_first) *eta_first = input->eta_first;
    if (eta_last) *eta_last = input->eta_last;
    output[0] = 0U;
    if (input->eta_first == 100) {
        format_id = 0x332U;
        if (input->item_index >= 0 &&
                (StarsU16)input->item_index < input->production_count &&
                input->item_phase == 1U && input->item_stage <= 6U)
            format_id = 0x254U;
        if (!ops->get_string) return 0;
        ops->get_string(context, format_id, output, capacity);
        return 1;
    }
    if (input->eta_last == 100) {
        if (!ops->format) return 0;
        ops->format(context, 0x32fU, input->eta_first, 0,
            output, capacity);
        return 1;
    }
    if (input->eta_first == input->eta_last) {
        if (input->eta_first == 0 || input->eta_first == -1) {
            if (!ops->get_string) return 0;
            format_id = input->eta_first == 0 ? 0x333U : 0x334U;
            ops->get_string(context, format_id, output, capacity);
            return 1;
        }
        if (!ops->format) return 0;
        ops->format(context, 0x331U, input->eta_first, 0,
            output, capacity);
        length = (StarsU16)strlen((const char *)output);
        if (input->eta_first != 1 && (StarsU16)(length + 1U) < capacity) {
            output[length] = 's';
            output[length + 1U] = 0U;
        }
        return 1;
    }
    if (!ops->format) return 0;
    ops->format(context, 0x330U, input->eta_first, input->eta_last,
        output, capacity);
    return 1;
}
