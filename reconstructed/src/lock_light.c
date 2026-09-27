/* Genuine C reconstruction of DrawLockLight (NE 12:6b00-6c13). */
#include "core.h"

void stars_draw_lock_light(StarsDrawLockLightInput *input)
{
    StarsI16 width;
    StarsI16 height;
    StarsRect inner;

    if (!input) return;
    width = (StarsI16)(input->rect.right - input->rect.left);
    height = (StarsI16)(input->rect.bottom - input->rect.top);
    if (!input->full_redraw) {
        inner = input->rect;
        stars_expand_rect(&inner, -2, -2);
        if (input->fill_rect)
            input->fill_rect(input->context, input->hdc, &inner,
                input->button_face);
        return;
    }
    if (input->select_object)
        input->select_object(input->context, input->hdc,
            input->button_shadow);
    if (input->pat_blt) {
        input->pat_blt(input->context, input->hdc,
            (StarsI16)input->rect.left, (StarsI16)input->rect.top,
            1, height, 0xf00021UL);
        input->pat_blt(input->context, input->hdc,
            (StarsI16)input->rect.left, (StarsI16)input->rect.top,
            width, 1, 0xf00021UL);
    }
    if (input->select_object)
        input->select_object(input->context, input->hdc,
            input->button_hilite);
    if (input->pat_blt) {
        input->pat_blt(input->context, input->hdc,
            (StarsI16)input->rect.right, (StarsI16)input->rect.top,
            1, (StarsI16)(height + 1), 0xf00021UL);
        input->pat_blt(input->context, input->hdc,
            (StarsI16)input->rect.left, (StarsI16)input->rect.bottom,
            width, 1, 0xf00021UL);
    }
}
