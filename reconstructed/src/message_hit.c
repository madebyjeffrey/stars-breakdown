/* Genuine C reconstruction of HtMsgBox (NE 7:7d8c-7ea9). */
#include "core.h"

StarsI16 stars_message_hit_test(const StarsMessageHitTestInput *input)
{
    StarsI16 height;
    StarsU16 i;

    if (!input) return 0;
    if (input->point.x < input->title_rect.left ||
        input->point.x >= input->title_rect.right ||
        input->point.y < input->title_rect.top ||
        input->point.y >= input->title_rect.bottom) return 0;
    height = (StarsI16)(input->title_rect.bottom - input->title_rect.top);
    if (input->point.x < input->title_rect.left + height &&
        input->current_message >= 0 &&
        input->current_message < (StarsI16)input->message_count &&
        (input->message_flags & 0x0100U) == 0U) return 1;
    if (input->point.x < input->title_rect.right - height ||
        (input->message_flags & 0x0100U) != 0U) {
        if ((input->game_flags & 4U) == 0U &&
            input->point.x >= input->title_rect.right - height - 0x18)
            return 3;
    } else if (input->sent && input->filtered) {
        for (i = 0U; i < 0x31U; ++i)
            if ((input->sent[i] & input->filtered[i]) != 0U) return 2;
    }
    return 0;
}
