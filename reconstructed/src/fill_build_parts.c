#include "core.h"

/* NE 26:63E8-6528. Enumerate each selected component group, convert the
 * component and bitmap indexes to the dialog's four-character code, and
 * append each eligible entry to the list box. */
void stars_fill_build_parts_lb(StarsFillBuildPartsInput *input)
{
    StarsU16 group_mask;
    StarsU16 item;
    StarsU16 bitmap;
    StarsU16 group_letter;
    StarsI16 found;
    StarsU8 text[5];

    if (!input || !input->send_message || !input->lookup_part) return;
    input->send_message(input->context, input->hwnd_list, 0x0405U,
        (const StarsU8 *)0);
    group_mask = 1U;
    group_letter = (StarsU16)'A';
    while (group_mask != 0U) {
        if ((group_mask & input->component_groups) != 0U) {
            item = 0U;
            for (;;) {
                found = input->lookup_part(input->context, group_mask, item,
                    &bitmap);
                if (found == 0) break;
                if (input->starbase_mode && group_mask == 0x0800U &&
                    (item == 0x000fU || item == 0x0010U))
                    found = (StarsI16)-1;
                if (found == 1) {
                    text[0] = (StarsU8)group_letter;
                    text[1] = (StarsU8)(item + (StarsU16)'A');
                    text[2] = (StarsU8)(bitmap % 26U + (StarsU16)'A');
                    text[3] = (StarsU8)(bitmap / 26U + (StarsU16)'A');
                    text[4] = 0;
                    input->send_message(input->context, input->hwnd_list,
                        0x0401U, text);
                }
                ++item;
            }
        }
        group_mask = (StarsU16)(group_mask << 1);
        ++group_letter;
    }
}
