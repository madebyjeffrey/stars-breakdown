#include "core.h"
#include <string.h>

/* NE 1B:3C92-3F1F. Format the display name for one production record.
 * The packed record selects either a ship design, a planetary item, or a
 * localized string.  Names and the resource lookup are supplied by the
 * caller so this source expresses the original data flow without embedding
 * the executable's DGROUP addresses. */
StarsU8 *stars_name_production_item(const StarsProductionNameInput *input)
{
    StarsU16 item;
    StarsU16 kind;
    StarsU16 index;
    StarsI16 difference;

    if (!input || !input->output)
        return 0;
    item = (StarsU16)((input->packed >> 10) & 0x7fU);
    kind = (StarsU16)((input->packed >> 17) & 7U);
    if (kind == 2U) {
        if (item < 0x10U) {
            if (input->base_ship_names && input->base_ship_flags &&
                    (input->base_ship_flags[item] & 0x200U) == 0U)
                strcpy((char *)input->output,
                    (const char *)input->base_ship_names[item]);
            else
                input->output[0] = 0;
        } else {
            index = (StarsU16)(item - 0x10U);
            if (index < 16U && input->player_ship_names &&
                    input->player_ship_flags &&
                    (input->player_ship_flags[index] & 0x200U) == 0U) {
                strcpy((char *)input->output,
                    (const char *)input->player_ship_names[index]);
                if ((input->selected_planet_flags & 0x200U) != 0U &&
                        input->player_ship_hulls) {
                    difference = (StarsI16)(input->selected_starbase_hull -
                        input->player_ship_hulls[index]);
                    if (difference > 0) {
                        if (input->starbase_suffix)
                            strcat((char *)input->output,
                                (const char *)input->starbase_suffix);
                    } else if (difference < 0 && input->ship_suffix) {
                        strcat((char *)input->output,
                            (const char *)input->ship_suffix);
                    }
                }
            } else {
                input->output[0] = 0;
            }
        }
    } else if (item >= 0x12U && item <= 0x1aU) {
        index = (StarsU16)(item - 0x12U);
        if (input->planetary_names)
            strcpy((char *)input->output,
                (const char *)input->planetary_names[index]);
        else
            input->output[0] = 0;
    } else if (input->get_string) {
        input->get_string(input->context,
            item == 0x1bU ? 0x51bU : (StarsU16)(item + 0x7eU),
            input->output);
    } else {
        input->output[0] = 0;
    }
    return input->output;
}
