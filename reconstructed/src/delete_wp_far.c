#include <string.h>
#include "core.h"

/* NE 0B:9E28-9FE4. Remove one 18-byte waypoint order, optionally recycling
 * the removed order into the former tail slot. */
void stars_delete_wp_far(StarsDeleteWpFarInput *input, StarsU16 i_del,
        StarsU16 f_recycle)
{
    StarsU8 saved[18];
    StarsU8 *removed;
    StarsU8 *last;
    StarsU16 move_count;

    if (!input || !input->records || !input->iord_mac)
        return;
    removed = input->records + (StarsU32)i_del * 18U;
    last = input->records + (StarsU32)(input->cord - 1U) * 18U;
    if (f_recycle != 0U && (i_del == 0x56U || input->cord == 2U))
        f_recycle = 0U;
    if (f_recycle != 0U && i_del != 0x56U && input->cord != 2U) {
        if (memcmp(last, removed, 4U) != 0) {
            memcpy(saved, removed, sizeof(saved));
            move_count = (StarsU16)(input->cord - i_del - 1U);
            memmove(removed, removed + 18, (size_t)move_count * 18U);
            memcpy(last, saved, sizeof(saved));
            return;
        }
        f_recycle = 0U;
    }
    move_count = (StarsU16)(input->cord - i_del - 1U);
    memmove(removed, removed + 18, (size_t)move_count * 18U);
    if (f_recycle == 0U) {
        --input->cord;
        --*input->iord_mac;
    }
    else {
        memcpy(last, saved, sizeof(saved));
    }
}
