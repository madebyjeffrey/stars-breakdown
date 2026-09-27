#include "core.h"
#include <string.h>

/* NE 1B:3B00-3C91. Refill the production-source list and decorate each
 * entry with its ship/item marker.  Window messages are callbacks because
 * the original routine's only UI operation is the two SENDMESSAGE calls. */
void stars_fill_production_source_list(const StarsFillProdSourceInput *input)
{
    StarsU8 marker[6];
    StarsU8 name[76];
    StarsU16 i;
    StarsU16 item;
    StarsU16 kind;

    if (!input || !input->items || !input->name ||
            !input->send_message)
        return;
    for (i = 0; i < 6U; ++i)
        marker[i] = ' ';
    input->send_message(input->context, input->hwnd, 0x405U, 0);
    for (i = 0; i < input->item_count; ++i) {
        if ((input->items[i].packed & 0x3ffUL) == 0UL)
            continue;
        input->name(input->context, input->items[i].packed, name);
        kind = (StarsU16)((input->items[i].packed >> 17) & 7U);
        item = (StarsU16)((input->items[i].packed >> 10) & 0x7fU);
        if (kind == 2U)
            marker[0] = item < 0x10U ? '*' : '#';
        else if (item < 7U) {
            marker[0] = 'I';
            if (input->append_item_suffix)
                input->append_item_suffix(input->context, name);
        } else
            marker[0] = ' ';
        input->send_message(input->context, input->hwnd, 0x401U, marker);
        marker[0] = ' ';
    }
}
