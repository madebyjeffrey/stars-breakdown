/* Genuine C reconstruction of TossNonAutoBuildItems (NE 24:5aec-5c54). */
#include "core.h"

void stars_toss_non_auto_build_items(StarsTossBuildItemsInput *input)
{
    StarsU16 read;
    StarsU16 write;

    if (!input || !input->items) return;
    write = 0U;
    for (read = 0U; read < input->item_count; ++read) {
        StarsU32 packed = input->items[read].packed;
        if (((packed >> 17) & 7UL) == 1UL &&
            ((packed >> 10) & 0x7fUL) < 7UL) {
            if (write != read) input->items[write] = input->items[read];
            ++write;
        }
    }
    if (write == 0U) {
        if (input->free_items)
            input->free_items(input->context, input->items);
        input->items = 0;
    } else {
        input->item_count = write;
    }
}
